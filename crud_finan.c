#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Finan {
    int id;
    char studentName[50];
    char dueDate[20];
    int paid; // New field
};

void createFinan() {
    struct Finan newFinan;
    int lastId = 0;

    // Open file in read mode to find the last ID
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        // If the file doesn't exist, create it and add the header
        file = fopen("database/finan.csv", "w");
        if (file == NULL) {
            printf("Error creating the file.\n");
            return;
        }
        fprintf(file, "ID,Student Name,Due Date,Paid\n");
        newFinan.id = 1;
        fclose(file);
    } else {
        // Read the last existing ID
        char line[255];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d", &lastId);
        }
        newFinan.id = lastId + 1;  // Increment the ID
        fclose(file);  // Close the file after reading
    }

    // Reopen the file in append mode to add the new finan
    file = fopen("database/finan.csv", "a");
    if (file == NULL) {
        printf("Error opening the file for writing.\n");
        return;
    }

    // Read finan data
    printf("Enter the student name: ");
    fgets(newFinan.studentName, sizeof(newFinan.studentName), stdin);
    
    // Read and validate the due date
    while (1) {
        printf("Enter the due date (DD/MM/YYYY): ");
        fgets(newFinan.dueDate, sizeof(newFinan.dueDate), stdin);
        
        // Validate the due date
        if (isValidDate(newFinan.dueDate)) {
            break;
        } else {
            printf("Invalid due date. Please try again.\n");
        }
    }
    
    printf("Enter 1 if the finan is paid, or 0 if it's not paid: ");
    scanf("%d", &newFinan.paid);
    getchar(); // Consume the newline character left by scanf

    // Remove possible newline characters read by fgets
    newFinan.studentName[strcspn(newFinan.studentName, "\n")] = 0;
    newFinan.dueDate[strcspn(newFinan.dueDate, "\n")] = 0;

    // Write to the file
    fprintf(file, "%d,%s,%s,%d\n", newFinan.id, newFinan.studentName, newFinan.dueDate, newFinan.paid);

    // Close the file
    fclose(file);

    printf("Finan created successfully!\n");
}

int isValidDate(const char *date) {
    int day, month, year;
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0;
    }
    if (year < 1900 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return 0;
    }
    if (month == 2) {
        if (day > 29) {
            return 0;
        }
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0 && day == 29)) {
            return 1;
        }
        return 0;
    }
    return 1;
}

void readAllFinan() {
    FILE *file = fopen("database/finan.csv", "r");
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

void readFinanById() {
    int finanId;
    printf("Enter the finan ID: ");
    scanf("%d", &finanId);
    getchar(); // Consume the newline character left by scanf

    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == finanId) {
            printf("%s", line);
            fclose(file);
            return;
        }
    }

    printf("Finan not found.\n");
    fclose(file);
}

void updateFinan() {
    int finanId;
    printf("Enter the ID of the finan you want to update: ");
    scanf("%d", &finanId);
    getchar(); // Consume the newline character left by scanf

    // Open file in read mode
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Create a temporary file to store the updated finans
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
        if (id == finanId) {
            struct Finan updatedFinan;
            updatedFinan.id = id;

            // Read the new finan data
            printf("What do you want to update?\n");
            printf("1. Student Name\n");
            printf("2. Due Date\n");
            printf("3. Paid\n");
            printf("Option: ");
            int option;
            scanf("%d", &option);
            getchar(); // Consume the newline character left by scanf

            switch (option) {
                case 1:
                    printf("Enter the new student name: ");
                    fgets(updatedFinan.studentName, sizeof(updatedFinan.studentName), stdin);
                    updatedFinan.dueDate[0] = '\0'; // Clear the due date
                    updatedFinan.paid = 0; // Clear the paid status
                    break;
                case 2:
                    printf("Enter the new due date: ");
                    fgets(updatedFinan.dueDate, sizeof(updatedFinan.dueDate), stdin);
                    updatedFinan.studentName[0] = '\0'; // Clear the student name
                    updatedFinan.paid = 0; // Clear the paid status
                    break;
                case 3:
                    printf("Enter 1 if the finan is paid, or 0 if it's not paid: ");
                    scanf("%d", &updatedFinan.paid);
                    getchar(); // Consume the newline character left by scanf
                    updatedFinan.studentName[0] = '\0'; // Clear the student name
                    updatedFinan.dueDate[0] = '\0'; // Clear the due date
                    break;
                default:
                    printf("Invalid option. No changes will be made.\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("database/temp.csv"); // Remove the temporary file
                    return;
            }

            // Remove possible newline characters read by fgets
            updatedFinan.studentName[strcspn(updatedFinan.studentName, "\n")] = 0;
            updatedFinan.dueDate[strcspn(updatedFinan.dueDate, "\n")] = 0;

            // Write the updated data to the temporary file
            fprintf(tempFile, "%d,%s,%s,%d\n", updatedFinan.id, updatedFinan.studentName, updatedFinan.dueDate, updatedFinan.paid);
        } else {
            // Write the non-updated finans to the temporary file
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file
    if (remove("database/finan.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Rename the temporary file to the original name
    if (rename("database/temp.csv", "database/finan.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Finan updated successfully!\n");
}

void deleteFinan() {
    int finanId;
    printf("Enter the ID of the finan you want to delete: ");
    scanf("%d", &finanId);
    getchar(); // Consume the newline character left by scanf

    // Open file in read mode
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Create a temporary file to store the non-deleted finans
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
        if (id != finanId) {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remove the original file
    if (remove("database/finan.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Rename the temporary file to the original name
    if (rename("database/temp.csv", "database/finan.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Finan deleted successfully!\n");
}

    void sortUnpaidFinanByDueDate() {
        // Open the file in read mode
        FILE *file = fopen("database/finan.csv", "r");
        if (file == NULL) {
            printf("Error opening the file.\n");
            return;
        }

        // Read the file line by line and store the unpaid finans in an array
        struct Finan unpaidFinans[100]; // Assuming a maximum of 100 unpaid finans
        int unpaidFinanCount = 0;
        char line[255];
        while (fgets(line, sizeof(line), file)) {
            struct Finan finan;
            sscanf(line, "%d,%[^,],%[^,],%d", &finan.id, finan.studentName, finan.dueDate, &finan.paid);
            if (finan.paid == 0) {
                unpaidFinans[unpaidFinanCount] = finan;
                unpaidFinanCount++;
            }
        }

        fclose(file);

        // Sort the unpaid finans by due date using bubble sort algorithm
        for (int i = 0; i < unpaidFinanCount - 1; i++) {
            for (int j = 0; j < unpaidFinanCount - i - 1; j++) {
                if (compareDates(unpaidFinans[j].dueDate, unpaidFinans[j + 1].dueDate) < 0) {
                    struct Finan temp = unpaidFinans[j];
                    unpaidFinans[j] = unpaidFinans[j + 1];
                    unpaidFinans[j + 1] = temp;
                }
            }
        }

        // Print the sorted unpaid finans
        printf("Unpaid Finans (Most recent/atrasada to furthest due date):\n");
        for (int i = 0; i < unpaidFinanCount; i++) {
            printf("ID: %d\n", unpaidFinans[i].id);
            printf("Student Name: %s\n", unpaidFinans[i].studentName);
            printf("Due Date: %s\n", unpaidFinans[i].dueDate);
            printf("Paid: %d\n", unpaidFinans[i].paid);
            printf("------------------------\n");
        }
    }

    int compareDates(const char *date1, const char *date2) {
        int day1, month1, year1;
        int day2, month2, year2;
        sscanf(date1, "%d/%d/%d", &day1, &month1, &year1);
        sscanf(date2, "%d/%d/%d", &day2, &month2, &year2);

        if (year1 < year2) {
            return -1;
        } else if (year1 > year2) {
            return 1;
        } else {
            if (month1 < month2) {
                return -1;
            } else if (month1 > month2) {
                return 1;
            } else {
                if (day1 < day2) {
                    return -1;
                } else if (day1 > day2) {
                    return 1;
                } else {
                    return 0;
                }
            }
        }
    }

void crudFinan() {
    int choice;

    do {
        printf("Choose an option:\n");
        printf("1. Create finan\n");
        printf("2. Read all finans\n");
        printf("3. Sort fina by date\n");
        printf("4. Find a finan by ID\n");
        printf("5. Update a finan\n");
        printf("6. Delete finan\n");
        printf("0. Exit\n");
        printf("Option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character left by scanf

        switch (choice) {
            case 1:
                createFinan();
                break;
            case 2:
                readAllFinan();
                break;
            case 3:
                sortUnpaidFinanByDueDate();
                readFinanById();
                break;
            case 4:
                readFinanById();
                break;
            case 5:
                updateFinan();
                break;
            case 6:
                deleteFinan();
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