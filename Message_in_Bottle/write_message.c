#include <stdio.h>  // fprintf - perror
#include <stdlib.h> // exit
#include <fcntl.h>  // open
#include <unistd.h> // close
#include <errno.h>  // errno - perror
#include <string.h> // strlen- strcmp - strcat


/* USAGE : ./exe [args] [filename] [message]
 * args :
 * 	-r (replace = replace the message)
 * 	-a (append  = add at the end of the file) 
 * 	no args = -r
 */
// VALID ARGS
char *REPLACE = "-r";
char *APPEND  = "-a";

int main(int argc, char* argv[]){
	
	// restricts use 
	if(argc != 3 && argc != 4){
		fprintf(stderr, "%s : usage : %s [args=optional] [filename] \"[message]\"\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}
	
	int index_arg = 1;
	char *args = argv[index_arg];
	
	// if there is no arg then basic position are shifted
	if(argc == 3){
		index_arg --;
		args = "-r";
	}
	
	const char *path = argv[1+index_arg];
	const char *message = strcat(argv[2+index_arg], "\n");
	int fd = 0;
	
	/* WRITING ACTIONS */
	if(strcmp(args, REPLACE) == 0)
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if(strcmp(args, APPEND) == 0)
		fd = open(path, O_WRONLY | O_APPEND| O_CREAT, 0644);
	else{
		fprintf(stderr, "ERROR : invalid arg");
		exit(EXIT_FAILURE);
	}
	
	// Handle open error
	if(fd == -1){
		perror("ERROR : opening file ");
		exit(EXIT_FAILURE);
	}

	// Handle write error
	if(write(fd, message, strlen(message)) == -1){
		perror("ERROR : writing file ");
		exit(EXIT_FAILURE);
	}
	
	// Handle close error
	if(close(fd) == -1){
		perror("ERROR : closing file ");
		exit(EXIT_FAILURE);
	}

	return 0;
}
