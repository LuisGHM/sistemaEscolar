#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarações de funções
void deleteStudent();
void updateStudent();

struct StudentAdmin {
    int id;
    char nameComplete[100];
    char email[50];
    char birthday[11];
};

typedef struct StudentNode {
    int id;
    char nameComplete[100];
    char email[50];
    char birthday[11];
    struct StudentNode* next;
} StudentNode;

// Função para criar um novo nó
StudentNode* createNode(int id, const char* name, const char* email, const char* birthday) {
    StudentNode* newNode = (StudentNode*)malloc(sizeof(StudentNode));
    newNode->id = id;
    strcpy(newNode->nameComplete, name);
    strcpy(newNode->email, email);
    strcpy(newNode->birthday, birthday);
    newNode->next = NULL;
    return newNode;
}

// Função para adicionar um nó no final da lista
void appendNode(StudentNode** head, int id, const char* name, const char* email, const char* birthday) {
    StudentNode* newNode = createNode(id, name, email, birthday);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    StudentNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Função para liberar a memória da lista
void freeList(StudentNode* head) {
    StudentNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Função para remover um nó da lista
void deleteNode(StudentNode** head, int id) {
    StudentNode* temp = *head;
    StudentNode* prev = NULL;

    // Se o nó a ser removido é o primeiro nó
    if (temp != NULL && temp->id == id) {
        *head = temp->next;
        free(temp);
        return;
    }

    // Procura o nó a ser removido, mantendo o ponteiro para o nó anterior
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }

    // Se o nó não foi encontrado
    if (temp == NULL) return;

    // Desvincula o nó da lista e libera a memória
    prev->next = temp->next;
    free(temp);
}

// Função para ler estudantes de um arquivo CSV e criar uma lista encadeada
StudentNode* loadStudentsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    StudentNode* head = NULL;
    char line[255];
    // Pular o cabeçalho
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        int id;
        char name[100], email[50], birthday[11];
        sscanf(line, "%d,%99[^,],%49[^,],%10[^\n]", &id, name, email, birthday);
        appendNode(&head, id, name, email, birthday);
    }

    fclose(file);
    return head;
}

// Função para salvar a lista de estudantes em um arquivo CSV
void saveStudentsToFile(StudentNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Escrever o cabeçalho
    fprintf(file, "ID,NameComplete,Email,Birthday\n");

    StudentNode* temp = head;
    while (temp != NULL) {
        fprintf(file, "%d,%s,%s,%s\n", temp->id, temp->nameComplete, temp->email, temp->birthday);
        temp = temp->next;
    }

    fclose(file);
}

// Função para atualizar um nó na lista
void updateNode(StudentNode* head, int id) {
    StudentNode* temp = head;

    // Encontra o nó a ser atualizado
    while (temp != NULL && temp->id != id) {
        temp = temp->next;
    }

    // Se o nó não foi encontrado
    if (temp == NULL) {
        printf("Student not found.\n");
        return;
    }

    // Atualiza os dados do nó
    printf("What do you want to update?\n");
    printf("1. Full Name\n");
    printf("2. Email\n");
    printf("3. Birthday\n");
    printf("Option: ");
    int option;
    scanf("%d", &option);
    getchar(); // Consome o caractere de nova linha deixado pelo scanf

    switch (option) {
        case 1:
            printf("Enter the new Full Name: ");
            fgets(temp->nameComplete, sizeof(temp->nameComplete), stdin);
            temp->nameComplete[strcspn(temp->nameComplete, "\n")] = 0;
            break;
        case 2:
            printf("Enter the new Email: ");
            fgets(temp->email, sizeof(temp->email), stdin);
            temp->email[strcspn(temp->email, "\n")] = 0;
            break;
        case 3:
            printf("Enter the new Birthday (DD/MM/AAAA): ");
            fgets(temp->birthday, sizeof(temp->birthday), stdin);
            temp->birthday[strcspn(temp->birthday, "\n")] = 0;
            break;
        default:
            printf("Invalid option. No changes will be made.\n");
            return;
    }

    printf("Student updated successfully!\n");
}

// Função principal para atualizar um estudante
void updateStudent() {
    int studentId;
    printf("Enter the ID of the Student you want to update: ");
    scanf("%d", &studentId);
    getchar(); // Consome o caractere de nova linha deixado pelo scanf

    // Carrega a lista de estudantes do arquivo
    StudentNode* head = loadStudentsFromFile("database/student.csv");
    if (head == NULL) {
        return;
    }

    // Atualiza o estudante na lista
    updateNode(head, studentId);

    // Salva a lista atualizada no arquivo
    saveStudentsToFile(head, "database/student.csv");

    // Libera a memória da lista
    freeList(head);
}

// Função principal para deletar um estudante
void deleteStudent() {
    int studentId;
    printf("Enter the ID of the Student you want to delete: ");
    scanf("%d", &studentId);
    getchar(); // Consome o caractere de nova linha deixado pelo scanf

    // Carrega a lista de estudantes do arquivo
    StudentNode* head = loadStudentsFromFile("database/student.csv");
    if (head == NULL) {
        return;
    }

    // Remove o estudante da lista
    deleteNode(&head, studentId);

    // Salva a lista atualizada no arquivo
    saveStudentsToFile(head, "database/student.csv");

    // Libera a memória da lista
    freeList(head);

    printf("Student deleted successfully!\n");
}

// Outras funções permanecem as mesmas

void createStudent() {
    struct StudentAdmin newStudent;
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
        fprintf(file, "ID,NameComplete,Email,Birthday\n");
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

    // Reopen the file in append mode to add the new student
    file = fopen("database/student.csv", "a");
    if (file == NULL) {
        printf("Error opening the file for writing.\n");
        return;
    }

    // Read student data
    printf("Enter the Student Full Name: ");
    fgets(newStudent.nameComplete, sizeof(newStudent.nameComplete), stdin);
    printf("Enter the Student Email: ");
    fgets(newStudent.email, sizeof(newStudent.email), stdin);
    printf("Enter the Student Birthday (DDMMAAAA): ");
    fgets(newStudent.birthday, sizeof(newStudent.birthday), stdin);

    // Remove possible newline characters read by fgets
    newStudent.nameComplete[strcspn(newStudent.nameComplete, "\n")] = 0;
    newStudent.email[strcspn(newStudent.email, "\n")] = 0;
    newStudent.birthday[strcspn(newStudent.birthday, "\n")] = 0;

    // Write to the file
    fprintf(file, "%d,%s,%s,%s\n", newStudent.id, newStudent.nameComplete, newStudent.email, newStudent.birthday);

    // Close the file
    fclose(file);

    printf("Student created successfully!\n");
}

// Read all students
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
    int id;
    char *token;
    char nameComplete[100], email[50], birthday[11];

    // Ignora o cabeçalho do arquivo
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        // Extrai o ID da linha
        token = strtok(line, ",");
        id = atoi(token);

        // Separa os demais dados do estudante
        token = strtok(NULL, ",");
        strcpy(nameComplete, token);
        token = strtok(NULL, ",");
        strcpy(email, token);
        token = strtok(NULL, ",");
        strcpy(birthday, token);

        // Compara o ID extraído com o ID procurado
        if (id == studentId) {
            printf("ID: %d\n", id);
            printf("Name Complete: %s\n", nameComplete);
            printf("Email: %s\n", email);
            printf("Birthday: %s\n", birthday);
            fclose(file);
            return;
        }
    }

    printf("Student not found.\n");
    fclose(file);
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