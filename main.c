#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Produto {
    int id;
    char nome[50];
    int quantidade;
    float preco;
};

int idExiste(int id);
void cadastrar();
void listar();
void buscar();
void remover();
void menu();

int idExiste(int id) {
    struct Produto p;
    FILE *file = fopen("estoque.dat", "rb");

    if (file == NULL) {
        return 0;
    }

    while (fread(&p, sizeof(struct Produto), 1, file)) {
        if (p.id == id) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void cadastrar() {
    struct Produto p;

    printf("\nID: ");
    if (scanf("%d", &p.id) != 1) {
        printf("Entrada invalida!\n");
        while (getchar() != '\n');
        return;
    }

    if (idExiste(p.id)) {
        printf("Erro: ID ja cadastrado!\n");
        return;
    }

    printf("Nome: ");
    scanf("%s", p.nome);

    printf("Quantidade: ");
    if (scanf("%d", &p.quantidade) != 1) {
        printf("Entrada invalida!\n");
        while (getchar() != '\n');
        return;
    }

    printf("Preco: ");
    if (scanf("%f", &p.preco) != 1) {
        printf("Entrada invalida!\n");
        while (getchar() != '\n');
        return;
    }

    FILE *file = fopen("estoque.dat", "ab");

    if (file == NULL) {
        printf("Erro ao abrir arquivo!\n");
        return;
    }

    fwrite(&p, sizeof(struct Produto), 1, file);
    fclose(file);

    printf("\nProduto cadastrado com sucesso!\n");
}

void listar() {
    struct Produto p;

    FILE *file = fopen("estoque.dat", "rb");

    if (file == NULL) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    printf("\n===== LISTA DE PRODUTOS =====\n");

    while (fread(&p, sizeof(struct Produto), 1, file)) {
        printf("\nID: %d\n", p.id);
        printf("Nome: %s\n", p.nome);
        printf("Quantidade: %d\n", p.quantidade);
        printf("Preco: %.2f\n", p.preco);
        printf("---------------------------\n");
    }

    fclose(file);
}

void buscar() {
    int id, encontrado = 0;
    struct Produto p;

    FILE *file = fopen("estoque.dat", "rb");

    if (file == NULL) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    printf("\nDigite o ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada invalida!\n");
        while (getchar() != '\n');
        return;
    }

    while (fread(&p, sizeof(struct Produto), 1, file)) {
        if (p.id == id) {
            printf("\n=== PRODUTO ENCONTRADO ===\n");
            printf("ID: %d\n", p.id);
            printf("Nome: %s\n", p.nome);
            printf("Quantidade: %d\n", p.quantidade);
            printf("Preco: %.2f\n", p.preco);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nProduto nao encontrado.\n");
    }

    fclose(file);
}

void remover() {
    int id, encontrado = 0;
    struct Produto p;

    FILE *file = fopen("estoque.dat", "rb");

    if (file == NULL) {
        printf("\nNenhum produto cadastrado.\n");
        return;
    }

    printf("\nDigite o ID para remover: ");
    if (scanf("%d", &id) != 1) {
        printf("Entrada invalida!\n");
        while (getchar() != '\n');
        fclose(file);
        return;
    }

    FILE *temp = fopen("temp.dat", "wb");

    if (temp == NULL) {
        printf("Erro ao criar arquivo temporario!\n");
        fclose(file);
        return;
    }

    while (fread(&p, sizeof(struct Produto), 1, file)) {
        if (p.id == id) {
            encontrado = 1;
        } else {
            fwrite(&p, sizeof(struct Produto), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);
    
    if (remove("estoque.dat") != 0) {
        printf("Erro ao deletar arquivo original!\n");
        return;
    }

    if (rename("temp.dat", "estoque.dat") != 0) {
        printf("Erro ao renomear arquivo!\n");
        return;
    }

    if (encontrado) {
        printf("\nProduto removido com sucesso!\n");
    } else {
        printf("\nProduto nao encontrado.\n");
    }
}


void menu() {
    printf("\n===== CONTROLE DE ESTOQUE =====\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Buscar produto\n");
    printf("4 - Remover produto\n");
    printf("5 - Sair\n");
    printf("Escolha: ");
}


int main() {
    int opcao;

    do {
        menu();

        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida!\n");
            while (getchar() != '\n');
            opcao = 0;
        }

        switch(opcao) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: buscar(); break;
            case 4: remover(); break;
            case 5: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }

    } while(opcao != 5);

    return 0;
}
