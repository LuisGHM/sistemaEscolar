#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FinanStudent {
    int id;
    char studentName[50];
    char dueDate[20];
    int paid;
};

void readFinan() {
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

void finan() {
    printf("================\n");
    printf("Finan:\n");
    readFinan();
}

