#include <stdio.h>
#include "crud_courses.c"
#include "crud_finan.c"
#include "crud_student.c"

void regitration(){

}

void finan(){

}

int main() {
    int choice;

    do {
        printf("================:\n");
        printf("Olá Fulano \n");
        printf("================:\n");
        printf("Choose an option:\n");
        printf("1. Minha matrícula\n");
        printf("2. Financeiro\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                regitration();
                break;
            case 2:
                finan();
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