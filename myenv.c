//P1-SSOO-22/23

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFSIZE 1 
#define PERMISSIONS 0644 // Everybody is allowed to read but only the owner is allowed to write

int main(int argc, char *argv[])
{
    // If there are less than two arguments (argv[0] -> program, argv[1] -> file to save environment) then print an error y return -1 
    if(argc < 3)
    {
    	printf("Too few arguments\n");
    	return -1;

    }else{
        int infile, outfile, nread;
        char *variable = argv[1], buffer[BUFFSIZE];

        if ((infile = open("./env.txt", O_RDONLY)) < 0){
            // Cannot open the environment file
            return -1;
        }

        else{
            /* O_TRUNC for emptying the file,
            O_WRONLY for giving only read permissions on the file and
            O_CREAT for creating a file if the given one doesn't exists;
            The rest is to give all permissions of the file to the user and the group.*/
            if ((outfile = open(argv[2], O_TRUNC|O_WRONLY|O_CREAT, PERMISSIONS)) < 0){
                // Cannot open the output file
                return -1;
            }

            else{
                int ls = 0; // initialize the line's size

                while ((nread = read(infile,buffer,BUFFSIZE)) > 0){
                    if(buffer[0] != '\n'){
                        ls++;

                    }else{
                        ls++; 
                        char line[ls]; // the buffer of the line
                        lseek(infile,-(ls),SEEK_CUR);
                        read(infile, line, ls);
                        int equal=1, i = 0; // equal is for comparing the line and the variable given 

                        while (equal == 1 && variable[i]!= '\0' ){
                            //If the variable is exactly equal to the beginning of the line then continue
                            if(variable[i] != line[i]){
                                equal = 0;
                            }

                            i++; // i is for going through the positions
                        }
                        
                        // Verify that it is "equal" and that the next character is '='
                        if (equal == 1 && line[i]== '='){
                            // Write it into the output file
                            write(outfile, line, ls);
                        }

                        // Reset the line size
                        ls = 0;                        
                    }
                }

                if(nread == -1){
                    // There has been an error while reading the env.txt
                    return -1;
                }

            }
            // Close files
            close(infile);
            close(outfile);       
        }
    }
    // Everything has worked fine
    return 0;
}
