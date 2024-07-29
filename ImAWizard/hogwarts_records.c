#include <string.h> // strcpy - strcmp
#include <unistd.h> // read - close - write
#include <stdio.h>  // fprintf - printf - fgets
#include <stdlib.h> // itoa
#include <fcntl.h>  // open
#include <stdlib.h> // exit - fopen - atoi
#include <errno.h>  // errno
					// arr[i][j] <=> *( arr[i] + j) *( *(arr + i) + j)	
#define MAX_STUDENTS 200 
#define STR_LEN 50
#define INPUT_LEN (STR_LEN * 5)
#define FILE_MODE 0644
#define FILENAME "student_record.txt"
#define DATASPLITTER ' '
typedef struct student{
	char name[50];
	char house[50];
	int year;
	double gpa;
} student;

student build_student(char *str_student);
student add_info_student(int el, char* buff, student stud);

void student_list(student stud[], size_t size);
void strsplit(char* str, int count_line, int count_column, char arr[MAX_STUDENTS][INPUT_LEN]);
void display_information();
void delete_student(size_t *index, student *students, size_t index_delete);
void strjoin(int count_line, int count_column, char arr[MAX_STUDENTS-1][INPUT_LEN], char join, char res[INPUT_LEN]);

int save_record(student storage[], size_t count);
int load_record(student storage[], size_t count);
int parser_input(char *input, size_t *index, student *students);


int main(void){
	
	student students[MAX_STUDENTS];
	// load save (nb_student == index)
	size_t nb_student = load_record(students, MAX_STUDENTS);
	int end = 0;
	
	do{
		char input[INPUT_LEN];
		// basic informations
		display_information();
		// get the input
		char *input_res = fgets(input, 250, stdin);
		if(input_res == NULL){
			fprintf(stderr, "Error: unvalid input");
			exit(EXIT_FAILURE);
		}
		// parsing of the input
		end = parser_input(input, &nb_student, students);
	}while(end != 1);
	save_record(students, nb_student);
	return 0;
}

int parser_input(char *input, size_t *index, student *students){
/* parse the input and take action according to the result */
	char buff[MAX_STUDENTS][INPUT_LEN];
	strsplit(input, 5, STR_LEN, buff);
	// action 
	if (strcmp(buff[0], "add") == 0){
		char str_student[INPUT_LEN];
		strjoin(4, STR_LEN, buff + 1, ' ', str_student);
		students[*index] = build_student(str_student);
		printf("Record added: Name: %s, House: %s, Year: %d, GPA: %.1f \n", students[*index].name, students[*index].house, students[*index].year, students[*index].gpa);
		(*index)++;
	}else if(strcmp(buff[0], "delete") == 0){
		// index points on the next possible index but we want the last student index then -1
		size_t index_delete = (size_t)atoi(buff[1]);
		// Handle error with atoi
		if(index_delete == 0){
			fprintf(stderr, "Error: Invalid Index, we need an integer");
			exit(EXIT_FAILURE);
		}
		// delete the index
		delete_student(index, students, index_delete - 1);
	}else if(strcmp(buff[0], "list") == 0){
		student_list(students, *index);	
	}else if(strcmp(buff[0], "exit") == 0){
		return 1;				
	}else{
		fprintf(stderr, "Error: Wrong INPUT command");
		exit(EXIT_FAILURE);
	}
		
	return 0;
}

void delete_student(size_t *index, student *students, size_t index_delete){
/* we don't delete shift all existing student */

	// if the user want to delete an impossible index
	if(*index > 0 && index_delete <= *index){
		
		(*index)--;	
		for(size_t i = index_delete; i < *index; i++){
			strcpy(students[i].name, students[i+1].name);
			strcpy(students[i].house, students[i+1].house);
			students[i].year = students[i+1].year;
			students[i].gpa = students[i+1].gpa;
		}
	}
}

void student_list(student stud[], size_t size){
	for(size_t i = 0; i < size; i++){
		printf("%ld. Name: %s, House: %s, Year: %d, GPA: %.1f \n", i+1, stud[i].name, stud[i].house, stud[i].year, stud[i].gpa);
	}
	printf("\n");
}

void display_information(){
/* display help information */
	printf("Commands: \n");
	printf("1. add <name> <house> <year> <gpa>\n");	
	printf("2. list\n");
	printf("3. delete <index>\n");
	printf("4. exit\n\n");
}

int save_record(student storage[], size_t count){
/* save students in a predef file */
	// open
	int fd = open(FILENAME, O_TRUNC | O_WRONLY);
	if(fd == -1){
		fprintf(stderr, "Error: Can not open the file for saving");
		exit(EXIT_FAILURE);
	}
	
	char student_lines[MAX_STUDENTS][INPUT_LEN];
	char ultimate_buffer[MAX_STUDENTS *(INPUT_LEN + 1)];
	
	// we save the number of student
	sprintf(student_lines[0], "%ld", count);	
	// we save student into an array with a string format
	for(size_t i = 0; i < count ; i++){
		sprintf(student_lines[i+1], "%s %s %d %.1f", storage[i].name, storage[i].house, storage[i].year, storage[i].gpa);
	}
	// we join the student to write all in one function in the file	
	strjoin(count+1, INPUT_LEN, student_lines, '\n', ultimate_buffer);

	int res = write(fd, ultimate_buffer, count * (INPUT_LEN + 1));
	if(res == -1){
		fprintf(stderr, "Error: Can not write into the file for saving");
		exit(EXIT_FAILURE);
	}
	
	res = close(fd);
	if(res == -1){
		fprintf(stderr, "Error: Can not close the file for saving");
		exit(EXIT_FAILURE);
	}

	return 0;
}

int load_record(student storage[], size_t count){
/* Load the save student save file */	
	FILE* fp = fopen(FILENAME, "a+");
	char buff[100];
	size_t nb_student = 0;
	if(fp == NULL){
		fprintf(stderr, "Error : issue with open or creat the file %s", FILENAME);
		exit(EXIT_FAILURE);
	}
	
	// we get the number of student
	char size_str[50];
	// the file is empty
	if(fgets(size_str, 50, fp) == NULL){
		return 0;	
	}
	size_t size = 0;
	size = (size_t)atoi(size_str);
    // get every line of the file and convert them to a student 
	while(fgets(buff, 100, fp) != NULL && nb_student < size && nb_student < count){
		storage[nb_student] = build_student(buff);
		nb_student++;
	}
		
	// handle error with max_students
	if(nb_student == MAX_STUDENTS){
		fprintf(stderr, "Error : too many student in the file");
		exit(EXIT_FAILURE);
	}

	// handle error with fget
	if(errno != 0){
		perror("Error : issue with reading");
		exit(EXIT_FAILURE);
	}

	// handle close error
	int res = fclose(fp);
	if(res != 0){
		fprintf(stderr, "Error : issue with closing the file %s", FILENAME);
		exit(EXIT_FAILURE);
	}
	return nb_student;
}
		
student add_info_student(int el, char* buff, student stud){
/* fill student in information */ 
	switch (el){
		case 0:
			strcpy(stud.name, buff);
			break;
		case 1:
			strcpy(stud.house, buff);
			break;
		case 2:
			stud.year = atoi(buff);
			if(stud.year == 0){
				fprintf(stderr, "can't convert student's to integer");
				exit(EXIT_FAILURE);
			}
			break;
		case 3:
			stud.gpa = atof(buff);
			break;
		default:
			fprintf(stderr, "too many information for student, wrong format");
			exit(EXIT_FAILURE);
			break;
	}
	return stud;
}

void strjoin(int count_line, int count_column, char arr[MAX_STUDENTS-1][INPUT_LEN], char join, char res[INPUT_LEN]){
/* Join string of an array (of string) with the join character */
	size_t i_res = 0;
	for(size_t i = 0; i < (size_t)count_line; i++){
		size_t j = 0;
		// browse the array 
		while(arr[i][j] != '\0' && j < (size_t)count_column){
			// handle out of range, because too long information
			if(i_res >= INPUT_LEN){
				fprintf(stderr, "Error: too long student, can not add him");
				exit(EXIT_FAILURE);
			}
			// add char to the string	
			res[i_res] = arr[i][j];
			i_res++;
			j++;
		}
		// prevent join character at the end of the array
		if(i < (size_t)count_line - 1){
			res[i_res] = join;
			i_res++;
		}else{
			res[i_res] = '\0';
		}
	}
}

void strsplit(char* str, int count_line, int count_column, char arr[MAX_STUDENTS][INPUT_LEN]){
/* string split function */
	int i = 0; // index of str
	int i_arr = 0; // index of first arr arr[i_arr]
	int ii_arr = 0; // index of second arr arr[i_arr][ii_arr]
	int useless_space = 1; // prevent useless space
						   
	while(str[i] != '\0'){	
		// \n is a useless char
		if(str[i] == '\n' || (str[i] == ' ' && useless_space == 1)){
			i++;
			continue;
		}
		// we go to the next index, we finish to copy the information
		if(str[i] == DATASPLITTER){
			arr[i_arr][ii_arr] = '\0'; // specify that the string is finished
			i_arr ++;
			ii_arr = 0;
			useless_space = 1; // if the next input input is a space then we skip it
		// handle out of range
		}else if(ii_arr >= count_column - 1 || i_arr >= count_line){
			fprintf(stderr, "Error : Index out of range pls make good student");
			exit(EXIT_FAILURE);
		// put the character in the right place in the array
		}else{
			arr[i_arr][ii_arr] = str[i];
			ii_arr++;
			useless_space = 0; // we can have a space after a char
		}
		// browse the string
		i++;
	}
	arr[i_arr][ii_arr] = '\0';
}

student build_student(char *str_student){
/* string student to student type */

	// storage a student info in an array
	char arr[MAX_STUDENTS][INPUT_LEN];
	student stud;
	strsplit(str_student, 4, STR_LEN, arr);
	// convert info string info to student
	for(size_t i = 0; i < 4; i++){
		stud = add_info_student(i, arr[i], stud);
    }
	return stud;
}

