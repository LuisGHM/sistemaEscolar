#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Course {
    int id;
    char name[50];
    char description[100];
};

void createCourse() {
    struct Course newCourse;
    int lastId = 0;

    // Open file in read mode to find the last ID
    FILE *file = fopen("database/courses.csv", "r");
    if (file == NULL) {
        // If the file doesn't exist, create it and add the header
        file = fopen("database/courses.csv", "w");
        if (file == NULL) {
            printf("Error creating the file.\n");
            return;
        }
        fprintf(file, "ID,Name,Description\n");
        newCourse.id = 1;
        fclose(file);
    } else {
        // Read the last existing ID
        char line[255];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d", &lastId);
        }
        newCourse.id = lastId + 1;  // Increment the ID
        fclose(file);  // Close the file after reading
    }

    // Reopen the file in append mode to add the new course
    file = fopen("database/courses.csv", "a");
    if (file == NULL) {
        printf("Error opening the file for writing.\n");
        return;
    }

    // Read course data
    printf("Enter the course name: ");
    fgets(newCourse.name, sizeof(newCourse.name), stdin);
    printf("Enter the course description: ");
    fgets(newCourse.description, sizeof(newCourse.description), stdin);

    // Remove possible newline characters read by fgets
    newCourse.name[strcspn(newCourse.name, "\n")] = 0;
    newCourse.description[strcspn(newCourse.description, "\n")] = 0;

    // Write to the file
    fprintf(file, "%d,%s,%s\n", newCourse.id, newCourse.name, newCourse.description);

    // Close the file
    fclose(file);

    printf("Course created successfully!\n");
}

void readAllCourses() {
    FILE *file = fopen("database/courses.csv", "r");
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

void readCourseById() {
    int courseId;
    printf("Enter the course ID: ");
    scanf("%d", &courseId);
    getchar(); // Consume the newline character left by scanf

    FILE *file = fopen("database/courses.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[255];
    int id;
    char *token;
    char name[50], description[100];

    // Ignora o cabeçalho do arquivo
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Extrai o ID da linha
        token = strtok(line, ",");
        id = atoi(token);

        // Separa os demais dados do curso
        token = strtok(NULL, ",");
        strcpy(name, token);
        token = strtok(NULL, ",");
        strcpy(description, token);

        // Compara o ID extraído com o ID procurado
        if (id == courseId) {
            printf("ID: %d\n", id);
            printf("Name: %s\n", name);
            printf("Description: %s\n", description);
            fclose(file);
            return;
        }
    }

    printf("Course not found.\n");
    fclose(file);
}

void updateCourse() {
    int courseId;
    printf("Enter the ID of the course you want to update: ");
    scanf("%d", &courseId);
    getchar(); // Consume the newline character left by scanf

    // Open file in read mode
    FILE *file = fopen("database/courses.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Create a temporary file to store the updated courses
    FILE *tempFile = fopen("database/tempCourse.csv", "w");
    if (tempFile == NULL) {
        printf("Error creating the temporary file.\n");
        fclose(file);
        return;
    }

    char line[255];
    int courseFound = 0;

    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == courseId) {
            struct Course updateCourse;
            updateCourse.id = id;

            // Copy current data to updateCourse
            sscanf(line, "%d,%49[^,],%99[^\n]", &updateCourse.id, updateCourse.name, updateCourse.description);

            // Read the new course data
            printf("What do you want to update?\n");
            printf("1. Name\n");
            printf("2. Description\n");
            printf("Option: ");
            int option;
            scanf("%d", &option);
            getchar(); // Consume the newline character left by scanf

            switch (option) {
                case 1:
                    printf("Enter the new course name: ");
                    fgets(updateCourse.name, sizeof(updateCourse.name), stdin);
                    updateCourse.name[strcspn(updateCourse.name, "\n")] = 0;
                    break;
                case 2:
                    printf("Enter the new course description: ");
                    fgets(updateCourse.description, sizeof(updateCourse.description), stdin);
                    updateCourse.description[strcspn(updateCourse.description, "\n")] = 0;
                    break;
                default:
                    printf("Invalid option. No changes will be made.\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("database/tempCourse.csv"); // Remove the temporary file
                    return;
            }

            // Write the updated data to the temporary file
            fprintf(tempFile, "%d,%s,%s\n", updateCourse.id, updateCourse.name, updateCourse.description);
            courseFound = 1;
        } else {
            // Write the non-updated courses to the temporary file
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (!courseFound) {
        printf("Course not found.\n");
        remove("database/tempCourse.csv"); // Remove the temporary file
        return;
    }

    // Remove the original file
    if (remove("database/courses.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Rename the temporary file to the original name
    if (rename("database/tempCourse.csv", "database/courses.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Course updated successfully!\n");
}

void deleteCourse() {
    int courseId;
    printf("Enter the ID of the course you want to delete: ");
    scanf("%d", &courseId);
    getchar(); // Consume the newline character left by scanf

    // Open file in read mode
    FILE *file = fopen("database/courses.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Create a temporary file to store the non-deleted courses
    FILE *tempFile = fopen("database/temp.csv", "w");
    if (tempFile == NULL) {
        printf("Error creating the temporary file.\n");
        fclose(file);
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id != courseId) {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file
    if (remove("database/courses.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Rename the temporary file to the original name
    if (rename("database/temp.csv", "database/courses.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Course deleted successfully!\n");
}

void crudCourse() {
    int choice;

    do {
        printf("Choose an option:\n");
        printf("1. Create course\n");
        printf("2. Read all courses\n");
        printf("3. Find a course by ID\n");
        printf("4. Update a course\n");
        printf("5. Delete course\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        switch (choice) {
            case 1:
                createCourse();
                break;
            case 2:
                readAllCourses();
                break;
            case 3:
                readCourseById();
                break;
            case 4:
                updateCourse();
                break;
            case 5:
                deleteCourse();
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
