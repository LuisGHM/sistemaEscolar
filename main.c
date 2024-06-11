#include <stdio.h>
#include <string.h>
#include "home.c"
#include "homeStudent.c"

#define MAX_LINE_LENGTH 255

int adminLogin() {
    char username[50];
    char password[50];

    printf("Admin Login\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0) {
        printf("Admin login successful!\n");
        return 1;
    } else {
        printf("Invalid username or password.\n");
        return 0;
    }
}

int studentLogin(char *registration) {
    char birthday[11]; 

    printf("Student Login\n");
    printf("Registration: ");
    scanf("%s", registration); 
    printf("Birthday (DDMMAAAA): ");
    scanf("%s", birthday); 

    FILE *file = fopen("database/registration.csv", "r");
    if (file == NULL) {
        printf("Error opening registration file.\n");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int loginSuccessful = 0;

    while (fgets(line, sizeof(line), file)) {
        char currentRegistration[50], currentBirthday[11];
        sscanf(line, "%*d,%49[^,],%*d,%*[^,],%*[^,],%49[^,],%*[^,]", currentRegistration, currentBirthday); 
        if (strcmp(registration, currentRegistration) == 0 && strcmp(birthday, currentBirthday) == 0) {
            loginSuccessful = 1;
            break;
        }
    }

    fclose(file);

    if (loginSuccessful) {
        printf("Student login successful!\n");
        return 1;
    } else {
        printf("Invalid registration or birthday.\n");
        return 0;
    }
}

int main() {
    int loginChoice;
    char registration[50];

    do {
        printf("Login Options:\n");
        printf("1. Admin Login\n");
        printf("2. Student Login\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &loginChoice);

        switch (loginChoice) {
            case 1:
                if (adminLogin()) {
                    home(1);
                }
                break;
            case 2:
                if (studentLogin(registration)) {
                    homeStudent(1, registration);
                }
                break;
            case 0:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (loginChoice != 0);
    return 0;
}
