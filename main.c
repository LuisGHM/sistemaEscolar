#include <stdio.h>
#include "crud_courses.c"
#include "crud_finan.c"

int main() {
    int choice;

    do {
        printf("Choose an option:\n");
        printf("1. CRUD Courses\n");
        printf("2. CRUD Finan\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        switch (choice) {
            case 1:
                crudCourse();
                break;
            case 2:
                crudFinan();
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