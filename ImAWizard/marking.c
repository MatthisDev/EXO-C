#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define NAME_LENGTH 50
#define HOUSE_LENGTH 20
#define FILENAME "students.csv"

typedef struct {
    char name[NAME_LENGTH];
    char house[HOUSE_LENGTH];
    int year;
    float gpa;
} Student;

Student records[MAX_RECORDS];
int record_count = 0;

void add_student(const char *name, const char *house, int year, float gpa) {
    if (record_count >= MAX_RECORDS) {
        fprintf(stderr, "Error: Maximum number of records reached.\n");
        return;
    }
    strncpy(records[record_count].name, name, NAME_LENGTH);
    strncpy(records[record_count].house, house, HOUSE_LENGTH);
    records[record_count].year = year;
    records[record_count].gpa = gpa;
    record_count++;
    printf("Record added: %s, House: %s, Year: %d, GPA: %.2f\n", name, house, year, gpa);
}

void list_students() {
    if (record_count == 0) {
        printf("No records found.\n");
        return;
    }
    for (int i = 0; i < record_count; ++i) {
        printf("%d. Name: %s, House: %s, Year: %d, GPA: %.2f\n", i + 1, records[i].name, records[i].house, records[i].year, records[i].gpa);
    }
}

void delete_student(int index) {
    if (index < 1 || index > record_count) {
        fprintf(stderr, "Error: Invalid index.\n");
        return;
    }
    printf("Record deleted: %s\n", records[index - 1].name);
    for (int i = index - 1; i < record_count - 1; ++i) {
        records[i] = records[i + 1];
    }
    record_count--;
}

void save_records() {
    FILE *file = fopen(FILENAME, "w");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file for writing.\n");
        return;
    }
    for (int i = 0; i < record_count; ++i) {
        fprintf(file, "%s,%s,%d,%.2f\n", records[i].name, records[i].house, records[i].year, records[i].gpa);
    }
    fclose(file);
    printf("Records saved to %s\n", FILENAME);
}

void load_records() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file for reading.\n");
        return;
    }
    record_count = 0;
    while (fscanf(file, "%49[^,],%19[^,],%d,%f\n", records[record_count].name, records[record_count].house, &records[record_count].year, &records[record_count].gpa) == 4) {
        record_count++;
    }
    fclose(file);
    printf("Records loaded from %s\n", FILENAME);
}

int main() {
    char command[100];
    char name[NAME_LENGTH];
    char house[HOUSE_LENGTH];
    int year, index;
    float gpa;

    load_records(); // Load records on startup

    while (1) {
        printf("Commands:\n1. add <name> <house> <year> <gpa>\n2. list\n3. delete <index>\n4. save\n5. load\n6. exit\n\n");
        printf("> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (sscanf(command, "add %49s %19s %d %f", name, house, &year, &gpa) == 4) {
            add_student(name, house, year, gpa);
        } else if (strcmp(command, "list") == 0) {
            list_students();
        } else if (sscanf(command, "delete %d", &index) == 1) {
            delete_student(index);
        } else if (strcmp(command, "save") == 0) {
            save_records();
        } else if (strcmp(command, "load") == 0) {
            load_records();
        } else if (strcmp(command, "exit") == 0) {
            save_records(); // Save records on exit
            break;
        } else {
            fprintf(stderr, "Error: Unknown command.\n");
        }
    }

    return 0;
}
