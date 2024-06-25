#include <fcntl.h> // for : open
#include <unistd.h>// for : close
#include <stdio.h> // for : fprintf
#include <errno.h> // for : errno
#include <string.h>// for : strerror
		  
int main(int argc, char *argv[]){

	// We can test an infinite number of file with a single command
	for(int i = 1; i < argc; i++){
		
		// Handle open file error
		int file_descriptor = open(argv[i], O_RDONLY);
		if(file_descriptor == -1){

			// write on the stderr
			printf("Error : %s '%s'.\n", strerror(errno), argv[i]);
		}else{

			// write on the stdout
			printf("File '%s' exists.\n", argv[i]);

			// Handle close Error
			if(close(file_descriptor) == -1){
				printf("Error : %s, by closing %s.\n", strerror(errno), argv[i]);
			}
		}
	}
	// Exit successfully
	return 0;
}




