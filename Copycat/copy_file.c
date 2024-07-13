#include <stdio.h>  // fprinf
#include <stdlib.h> // exit
#include <string.h> // strcmp
#include <fcntl.h>  // open
#include <unistd.h> // write - read - close

int close_file(int fd, char* file);
int open_file(char* file, int flags, int mode);
int check_format(int argc, char *argv[]);
int main(int argc, char *argv[]){
	// args checking
	check_format(argc, argv);
	char *src = argv[1];
	char *dst = argv[2];
	char *arg = argv[3];
	char buff[1024];
	int dst_flags = O_WRONLY | O_CREAT;
	char* action;

	if(strcmp(arg, "-r") == 0){
		dst_flags |= O_TRUNC;
		action = "copied";
	}
	else{
		dst_flags |= O_APPEND;
		action = "appended";
	}
	
	int fd_src = open_file(src, O_RDONLY, 0);
	int fd_dst = open_file(dst, dst_flags, 0644);

	ssize_t read_res = read(fd_src, buff, 1024);
	while(read_res != 0){
		if(write(fd_dst, buff, read_res) != -1){
			read_res = read(fd_src, buff, 1024);			
		}
		else{
			fprintf(stderr, "error : can not write more on the file %s \n",dst);
			close_file(fd_src, src);
			close_file(fd_dst, dst);
			exit(EXIT_FAILURE);
		}		
	}
	
	close_file(fd_src, src);
	close_file(fd_dst, dst);
	
	printf("Content of %s %s to %s \n", src, action, dst);	
}

int open_file(char* file, int flags, int mode){
	int fd = 0;
	
	// dest need to be created if doesn't exist but src need to throw an error
	if (mode > 0){
		fd = open(file, flags, mode);
	}else{
		fd = open(file, flags);
	}
	
	if(fd == -1){
		fprintf(stderr, "Error : Unable to open source file %s \n", file);
		exit(EXIT_FAILURE);	
	}
	return fd;
}

int close_file(int fd, char* file){
	int res = close(fd);
	
	if(res == -1){
		fprintf(stderr, "%s can not be close \n", file);
		exit(EXIT_FAILURE);
	}
	return 0;
}

int check_format(int argc, char *argv[]){
	if(argc == 4 && (strcmp(argv[3], "-r") == 0 || strcmp(argv[3], "-a") == 0))
		return 0;
	
	fprintf(stderr, "Usage %s [source.txt] [destination.txt] [-a|-r]", argv[0]);
	exit(EXIT_FAILURE);
}
