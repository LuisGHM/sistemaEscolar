#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Course {
    int id;
    char name[50];
    char description[100];
    char students[100];
};

void createCourse() {
    struct Course newCourse;
    int lastId = 0;

    // Abrir arquivo no modo de leitura para encontrar o último ID
    FILE *file = fopen("database/cursos.csv", "r");
    if (file == NULL) {
        // Se o arquivo não existir, crie-o e adicione o cabeçalho
        file = fopen("database/cursos.csv", "w");
        if (file == NULL) {
            printf("Erro ao criar o arquivo.\n");
            return;
        }
        fprintf(file, "ID,Nome,Descrição,Alunos\n");
        newCourse.id = 1;
        fclose(file);
    } else {
        // Ler o último ID existente
        char line[255];
        while (fgets(line, sizeof(line), file)) {
            sscanf(line, "%d", &lastId);
        }
        newCourse.id = lastId + 1;  // Incrementa o ID
        fclose(file);  // Fecha o arquivo após a leitura
    }

    // Reabre o arquivo no modo append para adicionar o novo curso
    file = fopen("database/cursos.csv", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    // Leitura de dados do curso
    printf("Digite o nome do curso: ");
    fgets(newCourse.name, sizeof(newCourse.name), stdin);
    printf("Digite a descrição do curso: ");
    fgets(newCourse.description, sizeof(newCourse.description), stdin);
    printf("Digite os alunos do curso: ");
    fgets(newCourse.students, sizeof(newCourse.students), stdin);

    // Removendo possíveis quebras de linha lidas por fgets
    newCourse.name[strcspn(newCourse.name, "\n")] = 0;
    newCourse.description[strcspn(newCourse.description, "\n")] = 0;
    newCourse.students[strcspn(newCourse.students, "\n")] = 0;

    // Escrevendo no arquivo
    fprintf(file, "%d,%s,%s,%s\n", newCourse.id, newCourse.name, newCourse.description, newCourse.students);

    // Fechar o arquivo
    fclose(file);

    printf("Curso cadastrado com sucesso!\n");
}

void readAllCourses() {
    FILE *file = fopen("database/cursos.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
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
    printf("Digite o ID do curso: ");
    scanf("%d", &courseId);
    getchar(); // Consume the newline character left by scanf

    FILE *file = fopen("database/cursos.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == courseId) {
            printf("%s", line);
            fclose(file);
            return;
        }
    }

    printf("Curso não encontrado.\n");
    fclose(file);
}

void updateCourse() {
    int courseId;
    printf("Digite o ID do curso que deseja atualizar: ");
    scanf("%d", &courseId);
    getchar(); // Consume the newline character left by scanf

    // Abrir arquivo no modo de leitura
    FILE *file = fopen("database/cursos.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Criar um arquivo temporário para armazenar os cursos atualizados
    FILE *tempFile = fopen("database/temp.csv", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(file);
        return;
    }

    char line[255];
    while (fgets(line, sizeof(line), file)) {
        int id;
        sscanf(line, "%d", &id);
        if (id == courseId) {
            struct Course updatedCourse;
            updatedCourse.id = id;

            // Leitura dos novos dados do curso
            printf("Digite o novo nome do curso: ");
            fgets(updatedCourse.name, sizeof(updatedCourse.name), stdin);
            printf("Digite a nova descrição do curso: ");
            fgets(updatedCourse.description, sizeof(updatedCourse.description), stdin);
            printf("Digite os novos alunos do curso: ");
            fgets(updatedCourse.students, sizeof(updatedCourse.students), stdin);

            // Removendo possíveis quebras de linha lidas por fgets
            updatedCourse.name[strcspn(updatedCourse.name, "\n")] = 0;
            updatedCourse.description[strcspn(updatedCourse.description, "\n")] = 0;
            updatedCourse.students[strcspn(updatedCourse.students, "\n")] = 0;

            // Escrevendo os dados atualizados no arquivo temporário
            fprintf(tempFile, "%d,%s,%s,%s\n", updatedCourse.id, updatedCourse.name, updatedCourse.description, updatedCourse.students);
        } else {
            // Escrevendo os cursos não atualizados no arquivo temporário
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(file);
    fclose(tempFile);

    // Remover o arquivo original
    if (remove("database/cursos.csv") != 0) {
        printf("Erro ao deletar o arquivo.\n");
        return;
    }

    // Renomear o arquivo temporário para o nome original
    if (rename("database/temp.csv", "database/cursos.csv") != 0) {
        printf("Erro ao renomear o arquivo.\n");
        return;
    }

    printf("Curso atualizado com sucesso!\n");
}

void deleteCourse() {
    int courseId;
    printf("Digite o ID do curso que deseja deletar: ");
    scanf("%d", &courseId);
    getchar(); // Consume the newline character left by scanf

    // Abrir arquivo no modo de leitura
    FILE *file = fopen("database/cursos.csv", "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Criar um arquivo temporário para armazenar os cursos não deletados
    FILE *tempFile = fopen("database/temp.csv", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
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

    // Remover o arquivo original
    if (remove("database/cursos.csv") != 0) {
        printf("Erro ao deletar o arquivo.\n");
        return;
    }

    // Renomear o arquivo temporário para o nome original
    if (rename("database/temp.csv", "database/cursos.csv") != 0) {
        printf("Erro ao renomear o arquivo.\n");
        return;
    }

    printf("Curso deletado com sucesso!\n");
}

void crudCourse() {
    int choice;

    do {
        printf("Escolha uma opção:\n");
        printf("1. Criar curso\n");
        printf("2. Ler todos os curso\n");
        printf("3. Achar um curso com base no ID\n");
        printf("4. Atualizar um curso\n");
        printf("5. Deletar curso\n");
        printf("0. Sair\n");
        printf("Opção: ");
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
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (choice != 0);
    return;
}