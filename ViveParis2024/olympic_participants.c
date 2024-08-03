#include <string.h> // strcspn - strcmp
#include <stdio.h>  // fgets - sscanf - printf - fprintf
#include <stdlib.h> // malloc - exit

#define NAME_LENGTH 50
#define SPORT_LENGTH 100
struct participant{
	char name[NAME_LENGTH];
	char sport[SPORT_LENGTH];
};

int main(void){
	size_t participants_count = 0;
	struct participant *participants = (struct participant*)malloc(sizeof(struct participant) * participants_count);
	printf("Commands:\n- add <name>, <sport>\n- list\n- exit\n");

	while(1){
		char buff[NAME_LENGTH + SPORT_LENGTH + 6];
		printf(":");
		// get the input
		fgets(buff, sizeof(buff), stdin);
		
		// replace the last char by the end of string char
		buff[strcspn(buff, "\n")] = '\0';
		
		struct participant new_participant;
		if(sscanf(buff, "add %[^,], %s", new_participant.name, new_participant.sport) == 2){
			// increment the size
			participants_count++;
			participants = realloc(participants, sizeof(struct participant)*participants_count);
			// add to global list
			strncpy(participants[participants_count - 1].name, new_participant.name, NAME_LENGTH);
			strncpy(participants[participants_count - 1].sport, new_participant.sport, SPORT_LENGTH);	
		}else if(strcmp("list", buff) == 0){
			printf("Olympic Games Participants:\n");
			for(size_t i = 0; i < participants_count; i++){
				printf("%ld. Name: %s, Sport: %s\n", i+1, participants[i].name, participants[i].sport);
			}
		}else if(strcmp("exit", buff) == 0){
			break;
		}else{
			fprintf(stderr, "Error: Wrong Input Format");
			exit(EXIT_FAILURE);
		}
	}
	free(participants);
}
