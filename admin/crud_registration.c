#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char fullName[101];
    char email[50];
    char birthday[11];
} Student;

// Function to search for a student by full name and return the student's complete data
int findStudentByNameComplete(const char *nameComplete, Student *student) {
    FILE *file = fopen("database/student.csv", "r");
    if (file == NULL) {
        printf("Error opening the student file.\n");
        return 0;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%100[^,],%49[^,],%10[^\n]", &student->id, student->fullName, student->email, student->birthday);
        if (strcmp(student->fullName, nameComplete) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Function to search for a course by name
int findCourseByName(const char *name) {
    FILE *file = fopen("database/courses.csv", "r");
    if (file == NULL) {
        printf("Error opening the course file.\n");
        return 0;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        char courseName[100];
        sscanf(line, "%*d,%99[^,\n]", courseName);
        if (strcmp(courseName, name) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

// Function to create a registration
void createRegistration() {
    char studentName[101];
    char courseName[101];
    Student student;

    // Generate a random registration number
    srand(time(NULL));
    int registration = rand() % 10000 + 1; // Random number between 1 and 10000

    // Open the registration file to check the last ID
    FILE *regFile = fopen("database/registration.csv", "r");
    int lastId = 0;
    if (regFile != NULL) {
        char line[255];
        while (fgets(line, sizeof(line), regFile)) {
            sscanf(line, "%*d,%*d,%*[^,],%*[^,],%*[^,],%*[^,\n]");
            lastId++;
        }
        fclose(regFile);
    }

    // Set the registration ID as the next available
    int id = lastId + 1;

    // Request the student's full name
    printf("Enter the full name of the student to register: ");
    fgets(studentName, sizeof(studentName), stdin);
    studentName[strcspn(studentName, "\n")] = 0;

    // Check if the student exists and get their data
    if (!findStudentByNameComplete(studentName, &student)) {
        printf("Student not found. Please try again.\n");
        return;
    }

    // Request the course name
    printf("Enter the name of the course to register the student: ");
    fgets(courseName, sizeof(courseName), stdin);
    courseName[strcspn(courseName, "\n")] = 0;

    // Check if the course exists
    if (!findCourseByName(courseName)) {
        printf("Course not found. Please try again.\n");
        return;
    }

    // Open the registration file to add a new registration
    FILE *file = fopen("database/registration.csv", "a");
    if (file == NULL) {
        printf("Error opening the registration file.\n");
        return;
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END); // Go to the end of the file
    if (ftell(file) == 0) {   // If the file size is 0, it's empty
        // Write the header with the field names
        fprintf(file, "ID,Registration,StudentID,StudentName,Email,Birthday,CourseName\n");
    }

    fprintf(file, "%d,%d,%d,%s,%s,%s,%s\n", id, registration, student.id, student.fullName, student.email, student.birthday, courseName);

    fclose(file);

    printf("Registration created successfully!\n");
}

// Function to list all registrations
void readAllRegistrations() {
    FILE *file = fopen("database/registration.csv", "r");
    if (file == NULL) {
        printf("Error opening the registration file.\n");
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

// Function to find a registration by ID
void findRegistrationById() {
    int registrationId;
    printf("Enter the Registration ID: ");
    scanf("%d", &registrationId);
    getchar(); // Consumir o caractere de nova linha deixado pelo scanf

    FILE *file = fopen("database/registration.csv", "r");
    if (file == NULL) {
        printf("Error opening the registration file.\n");
        return;
    }

    char line[255];
    int found = 0; // Variável para indicar se o registro foi encontrado

    // Algoritmo de pesquisa sequencial
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d,%*d,%*d,%*[^,],%*[^,],%*[^,],%*[^,\n]", &id);
        if (id == registrationId) { // Comparar o ID atual com o ID desejado
            printf("%s", line);
            found = 1; // Indicar que o registro foi encontrado
            break;
        }
    }

    if (!found) {
        printf("Registration not found.\n");
    }

    fclose(file);
}

// Function to update a registration
void updateRegistration() {
    int registrationId;
    printf("Enter the Registration ID to update: ");
    scanf("%d", &registrationId);
    getchar();

    char studentName[101];
    char courseName[101];
    Student student;

    // Open the registration file for reading and writing
    FILE *file = fopen("database/registration.csv", "r");
    FILE *tempFile = fopen("database/temp_registration.csv", "w");
    if (file == NULL || tempFile == NULL) {
        printf("Error opening the registration file.\n");
        return;
    }

    // Copy data to the temporary file, updating the corresponding registration
    char line[255];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        int id, reg, studentId;
        char oldStudentName[101], oldEmail[50], oldBirthday[11], oldCourseName[101];
        sscanf(line, "%d,%d,%d,%100[^,],%49[^,],%10[^,],%100[^\n]", &id, &reg, &studentId, oldStudentName, oldEmail, oldBirthday, oldCourseName);
        if (id == registrationId) {
            // Request the student's full name
            printf("Enter the full name of the student to register: ");
            fgets(studentName, sizeof(studentName), stdin);
            studentName[strcspn(studentName, "\n")] = 0;

            // Check if the student exists and get their data
            if (!findStudentByNameComplete(studentName, &student)) {
                printf("Student not found. Please try again.\n");
                fclose(file);
                fclose(tempFile);
                remove("database/temp_registration.csv");
                return;
            }

            // Request the course name
            printf("Enter the name of the course to register the student: ");
            fgets(courseName, sizeof(courseName), stdin);
            courseName[strcspn(courseName, "\n")] = 0;

            // Check if the course exists
            if (!findCourseByName(courseName)) {
                printf("Course not found. Please try again.\n");
                fclose(file);
                fclose(tempFile);
                remove("database/temp_registration.csv");
                return;
            }

            fprintf(tempFile, "%d,%d,%d,%s,%s,%s,%s\n", id, reg, student.id, student.fullName, student.email, student.birthday, courseName);
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    if (!found) {
        printf("Registration not found.\n");
    } else {
        printf("Registration updated successfully.\n");
    }

    fclose(file);
    fclose(tempFile);

    remove("database/registration.csv");
    rename("database/temp_registration.csv", "database/registration.csv");
}

// Function to delete a registration
void deleteRegistration() {
    int registrationId;
    printf("Enter the Registration ID to delete: ");
    scanf("%d", &registrationId);
    getchar();

    FILE *file = fopen("database/registration.csv", "r");
    if (file == NULL) {
        printf("Error opening the registration file.\n");
        return;
    }

    FILE *tempFile = fopen("database/temp_registration.csv", "w");
    if (tempFile == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    char line[255];
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == registrationId) {
            found = 1;
        } else {
            fprintf(tempFile, "%s", line);
        }
    }

    if (!found) {
        printf("Registration not found.\n");
    } else {
        printf("Registration deleted successfully.\n");
    }

    fclose(file);
    fclose(tempFile);

    remove("database/registration.csv");
    rename("database/temp_registration.csv", "database/registration.csv");
}

void crudRegistration() {
    int choice;

    do {
        printf("================\n");
        printf("Choose an option:\n");
        printf("1. Create Registration\n");
        printf("2. Read all Registrations\n");
        printf("3. Find a Registration\n");
        printf("4. Update a Registration\n");
        printf("5. Delete Registration\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                createRegistration();
                break;
            case 2:
                readAllRegistrations();
                break;
            case 3:
                findRegistrationById();
                break;
            case 4:
                updateRegistration();
                break;
            case 5:
                deleteRegistration();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (choice != 0);
}
