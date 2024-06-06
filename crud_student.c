#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int id;
    char name[50];
    char lastName[50];
    char email[50];
    char birthday[11]; // Formato "DD/MM/AAAA"
};

void createStudent() {
    struct Student newStudent;
    int lastId = 0;

    // Open file in read mode to find the last ID
    FILE *file = fopen("database/student.csv", "r");
    if (file == NULL) {
        // If the file doesn't exist, create it and add the header
        file = fopen("database/student.csv", "w");
        if (file == NULL) {
            printf("Error creating the file.\n");
            return;
        }
        fprintf(file, "ID,Name,LastName,Email,Birthday\n");
        newStudent.id = 1;
        fclose(file);
    } else {
        // Read the last existing ID
        char line[255];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d", &lastId);
        }
        newStudent.id = lastId + 1;  // Increment the ID
        fclose(file);  // Close the file after reading
    }

    // Reopen the file in append mode to add the new course
    file = fopen("database/student.csv", "a");
    if (file == NULL) {
        printf("Error opening the file for writing.\n");
        return;
    }

    // Read course data
    printf("Enter the Student Name: ");
    fgets(newStudent.name, sizeof(newStudent.name), stdin);
    printf("Enter the Student LastName: ");
    fgets(newStudent.lastName, sizeof(newStudent.lastName), stdin);
    printf("Enter the Student Email: ");
    fgets(newStudent.email, sizeof(newStudent.email), stdin);
    printf("Enter the Student Birthday (DD/MM/AAAA): ");
    fgets(newStudent.birthday, sizeof(newStudent.birthday), stdin);

    // Remove possible newline characters read by fgets
    newStudent.name[strcspn(newStudent.name, "\n")] = 0;
    newStudent.lastName[strcspn(newStudent.lastName, "\n")] = 0;
    newStudent.email[strcspn(newStudent.email, "\n")] = 0;
    newStudent.birthday[strcspn(newStudent.birthday, "\n")] = 0;

    // Write to the file
    fprintf(file, "%d,%s,%s,%s,%s\n", newStudent.id, newStudent.name, newStudent.lastName, newStudent.email, newStudent.birthday);

    // Close the file
    fclose(file);

    printf("Student created successfully!\n");
}


// Ler Students
void readAllStudent() {
    FILE *file = fopen("database/student.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    char line[255];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}



void readStudentById() {
    int studentId;
    printf("Enter the Student ID: ");
    scanf("%d", &studentId);
    getchar(); // Consume the newline character left by scanf

    FILE *file = fopen("database/student.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == studentId) {
            printf("%s", line);
            fclose(file);
            return;
        }
    }

    printf("Student not found.\n");
    fclose(file);
}

void updateStudent() {
    int studentId;
    printf("Enter the ID of the Student you want to update: ");
    scanf("%d", &studentId);
    getchar(); // Consume the newline character left by scanf

    // Open file in read mode
    FILE *file = fopen("database/student.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Create a temporary file to store the updated courses
    FILE *tempFile = fopen("database/tempStudent.csv", "w");
    if (tempFile == NULL) {
        printf("Error creating the temporary file.\n");
        fclose(file);
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == studentId) {
            struct Student updateStudent;
            updateStudent.id = id;

            // Copy current data to updateStudent
            sscanf(line, "%d,%49[^,],%49[^,],%49[^,],%10[^\n]", &updateStudent.id, updateStudent.name, updateStudent.lastName, updateStudent.email, updateStudent.birthday);

            // Read the new course data
            printf("What do you want to update?\n");
            printf("1. Name\n");
            printf("2. LastName\n");
            printf("3. Email\n");
            printf("4. Birthday\n");
            printf("Option: ");
            int option;
            scanf("%d", &option);
            getchar(); // Consume the newline character left by scanf

            switch (option) {
                case 1:
                    printf("Enter the new name Student: ");
                    fgets(updateStudent.name, sizeof(updateStudent.name), stdin);
                    updateStudent.name[strcspn(updateStudent.name, "\n")] = 0;
                    break;
                case 2:
                    printf("Enter the new LastName Student: ");
                    fgets(updateStudent.lastName, sizeof(updateStudent.lastName), stdin);
                    updateStudent.lastName[strcspn(updateStudent.lastName, "\n")] = 0;
                    break;
                case 3:
                    printf("Enter the new Email students: ");
                    fgets(updateStudent.email, sizeof(updateStudent.email), stdin);
                    updateStudent.email[strcspn(updateStudent.email, "\n")] = 0;
                    break;
                case 4:
                    printf("Enter the new Birthday (DD/MM/AAAA): ");
                    fgets(updateStudent.birthday, sizeof(updateStudent.birthday), stdin);
                    updateStudent.birthday[strcspn(updateStudent.birthday, "\n")] = 0;
                    break;
                default:
                    printf("Invalid option. No changes will be made.\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("database/tempStudent.csv"); // Remove the temporary file
                    return;
            }

            // Write the updated data to the temporary file
            fprintf(tempFile, "%d,%s,%s,%s,%s\n", updateStudent.id, updateStudent.name, updateStudent.lastName, updateStudent.email, updateStudent.birthday);
        } else {
            // Write the non-updated courses to the temporary file
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file
    if (remove("database/student.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Rename the temporary file to the original name
    if (rename("database/tempStudent.csv", "database/student.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Student updated successfully!\n");
}

void deleteStudent() {
    int studentId;
    printf("Enter the ID of the Student you want to delete: ");
    scanf("%d", &studentId);
    getchar(); // Consume the newline character left by scanf

    // Open file in read mode
    FILE *file = fopen("database/student.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Create a temporary file to store the non-deleted students
    FILE *tempFile = fopen("database/tempStudent.csv", "w");
    if (tempFile == NULL) {
        printf("Error creating the temporary file.\n");
        fclose(file);
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id != studentId) {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file
    if (remove("database/student.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Rename the temporary file to the original name
    if (rename("database/tempStudent.csv", "database/student.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Student deleted successfully!\n");
}

void crudStudent() {
    int choice;

    do {
        printf("================\n");
        printf("Choose an option:\n");
        printf("1. Create Student\n");
        printf("2. Read all Students\n");
        printf("3. Find a Student by ID\n");
        printf("4. Update a Student\n");
        printf("5. Delete Student\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        switch (choice) {
            case 1:
                createStudent();
                break;
            case 2:
                readAllStudent();
                break;
            case 3:
                readStudentById();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    } while (choice != 0);
    return;
}

int main() {
    crudStudent();
    return 0;
}
