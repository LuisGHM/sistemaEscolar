#include <stdio.h>

void createCourse() {
    printf("Função createCourse() chamada.\n");
}

void readCourse() {
    printf("Função readCourse() chamada.\n");
}

void updateCourse() {
    printf("Função updateCourse() chamada.\n");
}

void deleteCourse() {
    printf("Função deleteCourse() chamada.\n");
}

void crudCourse() {
    int choice;

    do {
        printf("Escolha uma opção:\n");
        printf("1. Criar curso\n");
        printf("2. Ler curso\n");
        printf("3. Atualizar curso\n");
        printf("4. Deletar curso\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createCourse();
                break;
            case 2:
                readCourse();
                break;
            case 3:
                updateCourse();
                break;
            case 4:
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