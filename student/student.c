#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 255

void readStudent(const char *registration) {
    FILE *file = fopen("database/registration.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int found = 0;
    
    while (fgets(line, sizeof(line), file)) {
        int id, studentID;
        char studentRegistration[50], studentName[100], email[50], birthday[11], courseName[100];
        sscanf(line, "%d,%49[^,],%d,%99[^,],%49[^,],%10[^,],%99[^,\n]", &id, studentRegistration, &studentID, studentName, email, birthday, courseName);

        if (strcmp(registration, studentRegistration) == 0) {
            printf("Registration: %s\n", studentRegistration);
            printf("Student Name: %s\n", studentName);
            printf("Email: %s\n", email);
            printf("Birthday: %s\n", birthday);
            printf("Course Name: %s\n", courseName);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Student with registration %s not found.\n", registration);
    }

    fclose(file);
}

void student(const char *registration) {
    printf("================\n");
    printf("Student Information:\n");
    
    readStudent(registration);
}