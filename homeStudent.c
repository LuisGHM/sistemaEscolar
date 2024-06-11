#include <stdio.h>
#include <string.h>
#include "student/finan.c" 
#include "student/student.c"

int homeStudent(int isAdmin, char *registration) {
    int choice;
    do {
        printf("Choose an option:\n");
        printf("1. Finan\n");
        printf("2. Student\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                finan(registration);
                break;
            case 2:
                student(registration);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (choice != 0);
    return 0;
}
