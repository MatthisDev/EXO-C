#include <string.h> // strcspn - strcmp
#include <stdio.h>  // fgets - sscanf - printf - fprintf
#include <stdlib.h> // malloc - exit

struct participant{
	char *name ;
	char *sport;
};

int main(void){
	size_t count = 0;
	struct participant *participants = (struct participant*)malloc(sizeof(struct participant) * count);
	printf("Commands:\n- add <name>, <sport>\n- list\n- exit\n");

	while(1){
		char buff[250];
		printf(":");
		
		// get the input
		fgets(buff, sizeof(buff), stdin);
		
		// replace the last char by the end of string char
		buff[strcspn(buff, "\n")] = '\0';
		
		char name[100];
		char sport[100];
		if(sscanf(buff, "add %[^,], %s", name, sport) == 2){
			// increment the size
			count++;
			size_t name_length = strcspn(name, "\n");
			size_t sport_length = strcspn(sport, "\n");
			
			// dynamically allocate memory
			participants = realloc(participants, sizeof(struct participant)*count);
			participants[count - 1].name = (char *)malloc(sizeof(char)*name_length);
			participants[count - 1].sport = (char *)malloc(sizeof(char)*sport_length);
			
			// add to global list
			strncpy(participants[count - 1].name, name, name_length);
			strncpy(participants[count - 1].sport, sport, sport_length);	

		}else if(strcmp("list", buff) == 0){
			printf("Olympic Games Participants:\n");
			for(size_t i = 0; i < count; i++){
				printf("%ld. Name: %s, Sport: %s\n", i+1, participants[i].name, participants[i].sport);
			}
		}else if(strcmp("exit", buff) == 0){
			// free all allocate space
			for(size_t i; i < count; i++){
				free(participants[i].name);
				free(participants[i].sport);
			}
			free(participants);
			break;
		}else{
			fprintf(stderr, "Error: Wrong Input Format");
			exit(EXIT_FAILURE);
		}
	}
}
