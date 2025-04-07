//P1-SSOO-22/23

#include <stdio.h>		// Header file for system call printf
#include <unistd.h>		// Header file for system call gtcwd
#include <sys/types.h>	// Header file for system calls opendir, readdir and closedir
#include <dirent.h>
#include <string.h>


int main(int argc, char *argv[])
{
	// Declaration of the directory that to be opened
	DIR *directory;

	// Current directory
	if(argc==1){
		// Get current directory's path
		char actual_dir[PATH_MAX];

        if (getcwd(actual_dir, PATH_MAX) == NULL){
			return -1;
		}

		// Open the directory
		directory=opendir(actual_dir);
	}

	else if(argc >= 2){
		// Open the directory 
		directory = opendir(argv[1]);
	}

	if (directory ==NULL){
		// Cannot open the directory
		return -1;
		
	}else{
		// Creates the structure of type dirent where each element of the directory will be saved
		struct dirent* element;

		// The loop will be carried out until we finish reading the number of elements inside the directory
		while ((element = readdir(directory))!=NULL){
            printf("%s\n", element->d_name);  
        }

        // Close the directory
        closedir(directory);
	}
	// Everything has worked fine
	return 0;
}

