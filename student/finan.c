#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 255

struct FinanStudent {
    int id;
    char studentName[50];
    char dueDate[20];
    int paid;
};

void readFinan(const char *registration) {
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int id;
        char finanRegistration[50], studentName[100], dueDate[20];
        int paid;
        
        sscanf(line, "%d,%49[^,],%99[^,],%19[^,],%d", &id, finanRegistration, studentName, dueDate, &paid);

        if (strcmp(registration, finanRegistration) == 0) {
            printf("Registration: %s\n", finanRegistration);
            printf("Student Name: %s\n", studentName);
            printf("Due Date: %s\n", dueDate);
            printf("Paid: %s\n", paid ? "Yes" : "No");
            found = 1;
        }
    }
    
    if (!found) {
        printf("No financial records found for student with registration %s.\n", registration);
    }

    fclose(file);
}

void finan(const char *registration) {
    printf("================\n");
    printf("Financial Information for Registration %s:\n", registration);
    
    readFinan(registration);
}


