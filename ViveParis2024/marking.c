#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 50
#define SPORT_LENGTH 50

// Define the participant struct
struct participant {
    char* name;
    char* sport;
};

struct participant** add_participant(struct participant** participants, int* count, const char* name, const char* sport) {
    // Reallocate memory for the new participant pointer
    participants = realloc(participants, (*count + 1) * sizeof(struct participant*));
    if (!participants) {
        fprintf(stderr, "Error: Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for the new participant struct
    participants[*count] = malloc(sizeof(struct participant));
    if (!participants[*count]) {
        fprintf(stderr, "Error: Unable to allocate memory for participant.\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for name and sport, and copy the provided values
    participants[*count]->name = malloc((strlen(name) + 1) * sizeof(char));
    participants[*count]->sport = malloc((strlen(sport) + 1) * sizeof(char));
    if (!participants[*count]->name || !participants[*count]->sport) {
        fprintf(stderr, "Error: Unable to allocate memory for name or sport.\n");
        exit(EXIT_FAILURE);
    }
    
    strcpy(participants[*count]->name, name);
    strcpy(participants[*count]->sport, sport);

    (*count)++;
    return participants;
}

void list_participants(struct participant** participants, int count) {
    printf("Olympic Games Participants:\n");
    for (int i = 0; i < count; ++i) {
        printf("%d. Name: %s, Sport: %s\n", i + 1, participants[i]->name, participants[i]->sport);
    }
}

void free_participants(struct participant** participants, int count) {
    for (int i = 0; i < count; ++i) {
        free(participants[i]->name);
        free(participants[i]->sport);
        free(participants[i]);
    }
    free(participants);
}

int main(void) {
    struct participant** participants = NULL; // Pointer to the array of participant pointers
    int participant_count = 0; // Number of participants

    // Add some participants
    participants = add_participant(participants, &participant_count, "Usain Bolt", "100m Sprint");
    participants = add_participant(participants, &participant_count, "Michael Phelps", "200m Butterfly");
    participants = add_participant(participants, &participant_count, "Simone Biles", "Gymnastics");
    participants = add_participant(participants, &participant_count, "Katie Ledecky", "800m Freestyle");

    // List all participants
    list_participants(participants, participant_count);

    // Free allocated memory
    free_participants(participants, participant_count);

    return 0;
}
