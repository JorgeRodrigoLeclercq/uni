//P1-SSOO-22/23

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE 1 


int main(int argc, char *argv[])
{
	// If there are less than two arguments (argv[0] -> program, argv[1] -> file to process) then print an error and return -1
	if(argc < 2)
	{
		printf("Too few arguments\n");
		return -1;

	}else{
		// Initialize variables for the file, the buffer and the result of reading
		int infile, nread;
		char buffer[BUFFSIZE];

		// Cannot open the file
		if ((infile = open(argv[1], O_RDONLY)) < 0){
			return -1;
		}

		else{
			int lines = 0, words = 0, bytes = 0; 
			int in_word = 0; // 0 means currently reading a word; 1 otherwise

			// The byte is not a space, a tab or a line jump
			while ((nread = read(infile,buffer,BUFFSIZE)) > 0){  

				// Word
				if (buffer[0] != ' ' && buffer[0] != '\t' && buffer[0] != '\n'){
					in_word = 1; 

				// Space, tab or line jump
				}else{
					if (in_word == 1){
						in_word = 0;
						words++;
					}

					// Line jump
					if (buffer[0] == '\n'){
						lines++;
					}
				}
				bytes++;
			}

			if(nread == -1){
				return -1;
			}

			// If it finishes inside a word then add one
			if(in_word){
				words ++;
			}

			close(infile);
			printf("%d %d %d %s\n",lines,words,bytes,argv[1]);
		}
	}
	return 0;
}

