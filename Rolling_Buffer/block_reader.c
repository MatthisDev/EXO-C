#include <stdio.h>  // fprintf
#include <stdlib.h> // exit
#include <errno.h>  // errno
#include <unistd.h> // read - write - close
#include <fcntl.h>  // open
#include <string.h> // sterror

int convert_to_int(char* str_number);
int get_content(const int fd, char* buff, size_t size);
int open_file(const char* filename);
int close_file(const int fd);
void display_block(char* buff, size_t begining, size_t size);
void handle_display(const int fd, char* buff, size_t size_block, size_t size_buff);

int main(int argc, char* argv[]){
	if(argc !=3){
		fprintf(stderr, "Usage : %s [filename] [blocksize]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// default args
	size_t size_block = convert_to_int(argv[2]);
	const char* filename = argv[1];
	char buff[1024];
	
	// content
	const int fd = open_file(filename);
	handle_display(fd, buff, size_block, 1024);
	close_file(fd);
	return 0;	
}

/* Handle SYST CALL and error */ 
int get_content(const int fd, char* buff, size_t size){

	ssize_t res = read(fd, buff, size);
	if(res == -1){
		fprintf(stderr, "error : cannot read the file");
		exit(EXIT_FAILURE);
	}
	return res;
		
}
int open_file(const char* filename){
	const int fd = open(filename, O_RDONLY);
	if(fd == -1){
		perror("error : can not open the file");
		exit(EXIT_FAILURE);
	}
	return fd;
}
int close_file(int fd){
	int res = close(fd);
	
	if(res == -1){
		fprintf(stderr, "error : can not close the file");
		exit(EXIT_FAILURE);
	}
	return 0;
}
/* ---------------------------- */
void handle_display(const int fd, char* buff, size_t size_block, size_t size_buff){
	
	int begining = 0; // current index were we can fill the buffer
	int res = get_content(fd, buff, size_block);
	
	if((size_t)res < size_block)
		size_block = res;

	while(res != 0){
						
		display_block(buff, begining, size_block);
		/*
			if we haven't the space to store one block from the current index of
			the buffer to the end we going to the end 
		*/
		
		if(size_buff - begining < size_block)
			begining = 0;
		else
			begining += size_block;
		
		// fill the buffer with the next block
		res = get_content(fd, &buff[begining], size_block);
		
			
		/*
			we adjust the size of the content to print 
			(the number of character in the file may not be dividible by 
			the block_size) 	
		*/

		if((size_t)res < size_block)
			size_block = res;
		else
			printf("\n");		

	}
}

void display_block(char* buff, size_t begining, size_t size){
    for(size_t i = begining; i < (begining+size); i++){
		printf("%c", buff[i]);
	}	 
}
/* made for fun but there is : atoi() from stdlib.h  */
int convert_to_int(char* str_number){
	int int_number = 0;
	size_t i = 0; // unknow length
	
	//browse the string	
	while(str_number[i] != '\0'){
		int number_equiv = str_number[i] - '0';

		// verify if it is number and handle error case
		if(number_equiv < 0 || number_equiv > 9){
			perror("error : blocksize must be an integer");
			exit(EXIT_FAILURE);
		}
		
		// make the conversion
		int_number = (int_number * 10) + number_equiv;
		i++;
	}
	return int_number;
}
