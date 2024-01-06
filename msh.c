// P2-SSOO-22/23

// MSH main file
// Write your msh source code here

// #include "parser.h"
#include <stddef.h> /* NULL */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>


#define MAX_COMMANDS 8

// Files in case of redirection
char filev[3][64];

// Store the execvp's second parameter
char *argv_execvp[8];

void siginthandler(int param)
{
	printf("****  Exiting MSH **** \n");
	exit(0);
}

/* Timer */
pthread_t timer_thread;
unsigned long mytime = 0;

void *timer_run()
{
	while (1)
	{
		usleep(1000);
		mytime++;
	}
}

/**
 * Get the command with its parameters for execvp
 * Execute this instruction before run an execvp to obtain the complete command
 * @param argvv
 * @param num_command
 * @return
 */
void getCompleteCommand(char ***argvv, int num_command)
{
	// Reset first
	for (int j = 0; j < 8; j++)
		argv_execvp[j] = NULL;

	int i = 0;
	for (i = 0; argvv[num_command][i] != NULL; i++)
		argv_execvp[i] = argvv[num_command][i];
}

/**
 * Main sheell  Loop
 */
int main(int argc, char *argv[])
{
	/**** Do not delete this code.****/
	int end = 0;
	int executed_cmd_lines = -1;
	char *cmd_line = NULL;
	char *cmd_lines[10];

	if (!isatty(STDIN_FILENO))
	{
		cmd_line = (char *)malloc(100);
		while (scanf(" %[^\n]", cmd_line) != EOF)
		{
			if (strlen(cmd_line) <= 0)
				return 0;
			cmd_lines[end] = (char *)malloc(strlen(cmd_line) + 1);
			strcpy(cmd_lines[end], cmd_line);
			end++;
			fflush(stdin);
			fflush(stdout);
		}
	}

	pthread_create(&timer_thread, NULL, timer_run, NULL);

	/*********************************/

	char ***argvv = NULL;
	int num_commands;

	while (1)
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		// Prompt
		write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

		// Get command
		executed_cmd_lines++;
		if (end != 0 && executed_cmd_lines < end)
		{
			command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
		}
		else if (end != 0 && executed_cmd_lines == end)
		{
			return 0;
		}
		else
		{
			command_counter = read_command(&argvv, filev, &in_background); // NORMAL MODE
		}

		//************************************************************************************************

		if (command_counter > 0)
		{
			if (strcmp(argvv[0][0], "mycalc") == 0 && argvv[0][1] != NULL && argvv[0][2] != NULL && argvv[0][3] != NULL)
			{
				// Take the command mycalc if the arguments are valid
				int op1 = atoi(argvv[0][1]), op2 = atoi(argvv[0][3]); // convert the operators into integers
				char sentence[100]; // variable for saving the message to write

				if (strcmp(argvv[0][2], "add") == 0)
				{
					// Case where the argument in mycalc is add
					char aux[128];  // string for the Acc environment variable
					char *p = aux; // pointer of aux

					if (p == NULL)
					{
						p = "0";
					}

					sprintf(aux, "%d", (atoi(aux) + op1 + op2)); // save in aux as a string the future value of Acc
					setenv("Acc", p, 1); // // create the environment variable Acc with value p
					snprintf(sentence, 100, "[OK] %d + %d = %d; Acc %s\n", op1, op2, op1 + op2, getenv("Acc")); // save the message in the variable sentence
				}

				else if (strcmp(argvv[0][2], "mul") == 0)
				{
					// Case where the argument in mycalc is mul
					// Save the message in the variable sentence
					snprintf(sentence, 100, "[OK] %d * %d = %d\n", op1, op2, op1 * op2);
				}

				else if (strcmp(argvv[0][2], "div") == 0)
				{
					// Case where the argument in mycalc is div
					if (op2 == 0)
					{
						// Check if the division is /0
						snprintf(sentence, 100, "[ERROR]You cannot divide by 0\n");
					}

					else{
						// Save the message in the variable sentence
						snprintf(sentence, 100, "[OK] %d / %d = %d; Remainder %d\n", op1, op2, op1 / op2, op1 % op2);
					}
					
				}

				else
				{
					// There has been an error with the structure of the command
					// Save the message in the variable sentence
					snprintf(sentence, 100, "[ERROR] The structure of the command is mycalc <operand_1> <add/mul/div> <operand_2>\n");
				}

				if (sentence[1] == 'O')
				{
					// If it is a success we write it on the std_error
					write(STDERR_FILENO, sentence, strlen(sentence));
				}

				else
				{
					// If it is a success we write it on the std_output
					write(STDOUT_FILENO, sentence, strlen(sentence));
				}

			}

			else if (strcmp(argvv[0][0], "mytime") == 0)
			{
				char sentence[100];
				unsigned long hours, minutes, seconds, timeleft;
				hours = mytime / (3600000); // convert original miliseconds to hours

				if (hours == 0)
				{
					minutes = mytime / 60000; // convert original miliseconds to minutes

					if (minutes == 0)
					{
						seconds = mytime / 1000; // convert  original miliseconds to seconds
					}

					else
					{
						timeleft = mytime - minutes * (60000);
						seconds = timeleft / 1000; // convert remaining miliseconds to seconds
					}
				}

				else
				{
					timeleft = mytime - hours * (3600000);
					minutes = timeleft / 60000; // convert remaining miliseconds to minutes
					if (minutes == 0)
					{
						seconds = mytime / 1000; // convert "original" miliseconds to seconds
					}

					else
					{
						timeleft = timeleft - minutes * (60000);
						seconds = timeleft / 1000; // convert remaining miliseconds to seconds
					}
				}

				// Save the message in the variable sentence
				snprintf(sentence, 100, "%02lu:%02lu:%02lu\n", hours, minutes, seconds);
				// We write it on the std_error
				write(STDERR_FILENO, sentence, strlen(sentence));
			}

			else
			{
				// There are commands different from mycalc and mytime
				pid_t pid;		  // to save the pid
				int fd, fd2, fd3; // file descriptors for the input, output and error files

				//  an Create array of pipes of size commands - 1
				int fdp[command_counter - 1][2];

				// Create n-1 pipes
				for (int i = 0; i < command_counter - 1; i++)
				{
					if (pipe(fdp[i]) == -1)
					{
						perror("Cannot create the pipe");
					}
				}

				// Loop for all the commands
				for (int i = 0; i < command_counter; i++)
				{
					// Create a child
					pid = fork();

					if (pid == -1)
					{
						// If there is an error
						perror("There is an error creating a child");
						return (-1);
					}

					else if (pid == 0)
					{
						// CHILD
						// Prepares the commmand to be executed
						getCompleteCommand(argvv, i);
						if (i == 0)
						{
							// In the first command
							if (strcmp(filev[0], "0") != 0)
							{
								// Check if there is an input file
								if ((fd = open(filev[0], O_RDONLY)) < 0)
								{
									// Print an error if it cannot be read
									perror("Cannot read the input file");
								}
								// Change the standard input to the input file
								dup2(fd, STDIN_FILENO);
								close(fd);
							}

							if (command_counter == 1)
							{
								// If we have only one command we have to check the output and error file
								if (strcmp(filev[1], "0") != 0)
								{
									// Output file
									if ((fd2 = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0)
									{
										// Print an error if it cannot be read
										perror("Cannot read the output file");
									}

									// Change the standard output to the output file
									dup2(fd2, STDOUT_FILENO);
									close(fd2);
								}

								if (strcmp(filev[2], "0") != 0)
								{
									// Error file
									if ((fd3 = open(filev[2], O_WRONLY | O_CREAT | O_APPEND, 0664)) < 0)
									{
										// Prints an error if it cannot be read
										perror("Cannot read the error file");
									}

									// Change the Standard error to the error file
									dup2(fd3, STDERR_FILENO);
									close(fd3);
								}
							}

							else
							{
								// Change the output to the first pipe
								dup2(fdp[0][1], STDOUT_FILENO);
							}
						}

						else if (i < command_counter - 1)
						{
							// The command is between the first and the final one

							// We change the input of the command to the previous pipe output
							dup2(fdp[i - 1][0], STDIN_FILENO);
							// Change the output of the command to the following pipe input
							dup2(fdp[i][1], STDOUT_FILENO);
						}

						else
						{
							// The command is the last one of the sequence

							// Change the input of the last command to the previous pipe output
							dup2(fdp[i - 1][0], STDIN_FILENO);

							// The standard output and error depends on if there is a file or not
							if (strcmp(filev[1], "0") != 0)
							{
								// Output file
								if ((fd2 = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0)
								{
									// Prints an error if it cannot be read
									perror("Cannot read the output file");
								}

								// Change the Standard output to the output file
								dup2(fd2, STDOUT_FILENO);
								close(fd2);
							}

							if (strcmp(filev[2], "0") != 0)
							{
								// Error file
								if ((fd3 = open(filev[2], O_WRONLY | O_CREAT | O_APPEND, 0664)) < 0)
								{
									// Print an error if it cannot be read
									perror("Cannot read the error file");
								}

								// Change the Standard error to the error file
								dup2(fd3, STDERR_FILENO);
								close(fd3);
							}
						}

						for (int j = 0; j < command_counter - 1; j++)
						{
							// It will close all the pipes even if they are already closed
							close(fdp[j][0]); // Output
							close(fdp[j][1]); // Input
						}

						// Executes the command
						execvp(argv_execvp[0], argv_execvp);
					}

					else
					{
						// PARENT
						if (i == 0 && command_counter > 1)
						{
							// For the first command it only closes the input of the first pipe, it won't be used again
							close(fdp[0][1]);
						}

						if (i > 0)
						{
							// Close all the used pipes as they aren't needed anymore
							close(fdp[i - 1][1]);
							close(fdp[i - 1][0]);
						}

						if (in_background == 0)
						{
							// Only waits if background is False
							wait(NULL);
						}

						else
						{
							if (i == command_counter - 1)
							{
								// Print the pid of the last command; for simple ones will be also the last one
								printf("[%d]\n", pid);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
