#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareDates(const char *date1, const char *date2);
int isValidDate(const char *date);
#define MAX_LINE_LENGTH 255

struct Finan {
    int id;
    char registration[50];
    char studentName[50];
    char dueDate[20];
    int paid; // New field
};

// Definição da estrutura do nó da lista encadeada
struct Node {
    struct Finan data;
    struct Node* next;
};

// Função para comparar datas (retorne -1 se date1 < date2, 0 se iguais, 1 se date1 > date2)
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

// Função para trocar dados entre dois nós
void swap(struct Finan* a, struct Finan* b) {
    struct Finan temp = *a;
    *a = *b;
    *b = temp;
}

// Função para adicionar um novo nó ao final da lista
void append(struct Node** head_ref, struct Finan new_data) {
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    struct Node* last = *head_ref;
    new_node->data = new_data;
    new_node->next = NULL;
    
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL)
        last = last->next;

    last->next = new_node;
}

// Função para imprimir a lista
void printList(struct Node* node) {
    while (node != NULL) {
        printf("ID: %d\n", node->data.id);
        printf("Registration: %s\n", node->data.registration);
        printf("Student Name: %s\n", node->data.studentName);
        printf("Due Date: %s\n", node->data.dueDate);
        printf("Paid: %d\n", node->data.paid);
        printf("------------------------\n");
        node = node->next;
    }
}

// Função Bubble Sort para lista encadeada
void bubbleSort(struct Node* head) {
    int swapped;
    struct Node* ptr1;
    struct Node* lptr = NULL;

    // Verifique se a lista está vazia
    if (head == NULL)
        return;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (compareDates(ptr1->data.dueDate, ptr1->next->data.dueDate) > 0) {
                swap(&(ptr1->data), &(ptr1->next->data));
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
}

// Função para ordenar os finans não pagos pela data de vencimento usando lista encadeada
void sortUnpaidFinanByDueDate() {
    // Abrir o arquivo em modo de leitura
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Ler o arquivo linha por linha e armazenar os finans não pagos em uma lista encadeada
    struct Node* unpaidFinans = NULL;
    char line[255];
    while (fgets(line, sizeof(line), file)) {
        struct Finan finan;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%d", &finan.id, finan.registration, finan.studentName, finan.dueDate, &finan.paid);
        if (finan.paid == 0) {
            append(&unpaidFinans, finan);
        }
    }
    fclose(file);

    // Ordenar os finans não pagos pela data de vencimento usando Bubble Sort
    bubbleSort(unpaidFinans);

    // Imprimir os finans não pagos ordenados
    printf("Unpaid Finans (Most recent/atrasada to furthest due date):\n");
    printList(unpaidFinans);
}

// Função para criar um novo registro financeiro
void createFinan() {
    struct Finan newFinan;
    int lastId = 0;

    // Abrir o arquivo em modo de leitura para encontrar o último ID
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        // Se o arquivo não existir, crie-o e adicione o cabeçalho
        file = fopen("database/finan.csv", "w");
        if (file == NULL) {
            printf("Error creating the file.\n");
            return;
        }
        fprintf(file, "ID,Registration,Student Name,Due Date,Paid\n");
        newFinan.id = 1;
        fclose(file);
    } else {
        // Ler o último ID existente
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d", &lastId);
        }
        newFinan.id = lastId + 1; // Incrementar o ID
        fclose(file); // Fechar o arquivo após a leitura
    }

    // Reabrir o arquivo em modo de adição para adicionar o novo registro financeiro
    file = fopen("database/finan.csv", "a");
    if (file == NULL) {
        printf("Error opening the file for writing.\n");
        return;
    }

    // Solicitar o registro do aluno
    printf("Enter the student's registration: ");
    fgets(newFinan.registration, sizeof(newFinan.registration), stdin);
    newFinan.registration[strcspn(newFinan.registration, "\n")] = '\0'; // Remover o caractere de nova linha

    // Verificar se o registro do aluno existe no arquivo registration.csv
    FILE *regFile = fopen("database/registration.csv", "r");
    if (regFile == NULL) {
        printf("Error opening the registration file.\n");
        fclose(file);
        return;
    }

    int registrationFound = 0;
    char regLine[MAX_LINE_LENGTH];
    while (fgets(regLine, sizeof(regLine), regFile)) {
        char reg[50], name[100];
        sscanf(regLine, "%*d,%49[^,],%*d,%99[^,]", reg, name);
        if (strcmp(newFinan.registration, reg) == 0) {
            registrationFound = 1;
            strcpy(newFinan.studentName, name);
            break;
        }
    }
    fclose(regFile);

    if (!registrationFound) {
        printf("Student registration not found.\n");
        fclose(file);
        return;
    }

    // Solicitar o nome do aluno
    printf("Enter the student's name: ");
    fgets(newFinan.studentName, sizeof(newFinan.studentName), stdin);
    newFinan.studentName[strcspn(newFinan.studentName, "\n")] = '\0'; // Remover o caractere de nova linha

    // Solicitar a data de vencimento
    printf("Enter the due date (DD/MM/YYYY): ");
    fgets(newFinan.dueDate, sizeof(newFinan.dueDate), stdin);
    newFinan.dueDate[strcspn(newFinan.dueDate, "\n")] = '\0'; // Remover o caractere de nova linha

    // Solicitar o status do pagamento
    printf("Enter 1 if the payment has been made, or 0 otherwise: ");
    scanf("%d", &newFinan.paid);
    getchar(); // Consumir o caractere de nova linha deixado pelo scanf

    // Escrever no arquivo
    fprintf(file, "%d,%s,%s,%s,%d\n", newFinan.id, newFinan.registration, newFinan.studentName, newFinan.dueDate, newFinan.paid);

    // Fechar o arquivo
    fclose(file);

    printf("Financial record created successfully!\n");
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
    getchar(); // Consumir o caractere de nova linha deixado pelo scanf

    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    char line[255];
    int found = 0; // Variável para indicar se o registro foi encontrado

    // Algoritmo de pesquisa sequencial
    while (fgets(line, sizeof(line), file)) {
        struct Finan finan;
        sscanf(line, "%d,%[^,],%[^,],%[^,],%d", &finan.id, finan.registration, finan.studentName, finan.dueDate, &finan.paid);
        if (finan.id == finanId) { // Comparar o ID atual com o ID desejado
            printf("ID: %d\n", finan.id);
            printf("Registration: %s\n", finan.registration);
            printf("Student Name: %s\n", finan.studentName);
            printf("Due Date: %s\n", finan.dueDate);
            printf("Paid: %d\n", finan.paid);
            found = 1; // Indicar que o registro foi encontrado
            break;
        }
    }

    if (!found) {
        printf("Finan not found.\n");
    }

    fclose(file);
}

void updateFinan() {
    int finanId;
    printf("Enter the ID of the finan you want to update: ");
    scanf("%d", &finanId);
    getchar(); // Consumir o caractere de nova linha deixado pelo scanf

    // Abrir o arquivo em modo de leitura
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Criar um arquivo temporário para armazenar os finans atualizados
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

            // Copiar os dados atuais para updatedFinan
            sscanf(line, "%d,%49[^,],%49[^,],%19[^,],%d", &updatedFinan.id, updatedFinan.registration, updatedFinan.studentName, updatedFinan.dueDate, &updatedFinan.paid);

            // Ler os novos dados do finan
            printf("What do you want to update?\n");
            printf("1. Student Name\n");
            printf("2. Due Date\n");
            printf("3. Paid\n");
            printf("Option: ");
            int option;
            scanf("%d", &option);
            getchar(); // Consumir o caractere de nova linha deixado pelo scanf

            switch (option) {
                case 1:
                    printf("Enter the new student name: ");
                    fgets(updatedFinan.studentName, sizeof(updatedFinan.studentName), stdin);
                    updatedFinan.studentName[strcspn(updatedFinan.studentName, "\n")] = 0;
                    break;
                case 2:
                    printf("Enter the new due date: ");
                    fgets(updatedFinan.dueDate, sizeof(updatedFinan.dueDate), stdin);
                    updatedFinan.dueDate[strcspn(updatedFinan.dueDate, "\n")] = 0;
                    break;
                case 3:
                    printf("Enter 1 if the finan is paid, or 0 if it's not paid: ");
                    scanf("%d", &updatedFinan.paid);
                    getchar(); // Consumir o caractere de nova linha deixado pelo scanf
                    break;
                default:
                    printf("Invalid option. No changes will be made.\n");
                    fclose(file);
                    fclose(tempFile);
                    remove("database/temp.csv"); // Remover o arquivo temporário
                    return;
            }

            // Escrever os dados atualizados no arquivo temporário
            fprintf(tempFile, "%d,%s,%s,%s,%d\n", updatedFinan.id, updatedFinan.registration, updatedFinan.studentName, updatedFinan.dueDate, updatedFinan.paid);
        } else {
            // Escrever os finans não atualizados no arquivo temporário
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remover o arquivo original
    if (remove("database/finan.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Renomear o arquivo temporário para o nome original
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
    getchar(); // Consumir o caractere de nova linha deixado pelo scanf

    // Abrir o arquivo em modo de leitura
    FILE *file = fopen("database/finan.csv", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    // Criar um arquivo temporário para armazenar os finans não deletados
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

    // Remover o arquivo original
    if (remove("database/finan.csv") != 0) {
        printf("Error deleting the file.\n");
        return;
    }

    // Renomear o arquivo temporário para o nome original
    if (rename("database/temp.csv", "database/finan.csv") != 0) {
        printf("Error renaming the file.\n");
        return;
    }

    printf("Finan deleted successfully!\n");
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
        getchar(); // Consumir o caractere de nova linha deixado pelo scanf

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

