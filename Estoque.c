#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
//#include <locale.h>

#define ARQUIVO_BANCO "estoque.db" // arquivo de banco de dados

typedef struct {
    int codigo;
    char nome[50];
    int quantidade;
    float preco;
} Produto;

//DECLARAÇÃO DE VARIÁVEL GLOBAL PARA CONEXÃO COM O DB
sqlite3 *db;
char #err_msg = 0;

// Protótipo das funções do sistema
void carregarEstoque();
void salvarEstoque();
void cadastrarProduto();
void listarProdutos();
void buscarProduto();
void atualizarEstoque();
void excluirProduto();
void menu();

//Callback para listar produtos  (usado na função listarProdutos)s
//Esta função é chamada pelo SQLite para cada linha resultante de um SELECT
int callback_listar(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;
    // argv[0] é o código, [1] é o nome, [2] é a quantidade, [3] é o preço
    printf("Código %s | Nome: %s | Quantidade %s | Preço: R$%s \n",
    argv[0], argv[1], argv[2], argv[3] );
 }
int main() {
    int rc;

    //inicializa o banco de dados antes de carregar o arquivo texto
    inicializarBanco();

    menu();

    //FECHA CONEXÃO COM O BANCO DE DADOS AO SAIR DO PROGRAMA
    sqlite3_close(db);
    return 0;
}

// Carrega os produtos do arquivo para a memória
void carregarEstoque() {
    FILE *file = fopen(ARQUIVO, "r");
    if (file == NULL) return;

    while (fscanf(file, "%d %s %d %f", 
           &estoque[totalProdutos].codigo,
           estoque[totalProdutos].nome,
           &estoque[totalProdutos].quantidade,
           &estoque[totalProdutos].preco) != EOF) {
        totalProdutos++;
    }
    fclose(file);
}

// Salva os produtos da memória no arquivo
void salvarEstoque() {
    FILE *file = fopen(ARQUIVO, "w");
    if (file == NULL) {
        printf("Erro ao salvar arquivo!\n");
        return;
    }

    for (int i = 0; i < totalProdutos; i++) {
        fprintf(file, "%d %s %d %.2f\n", 
                estoque[i].codigo,
                estoque[i].nome,
                estoque[i].quantidade,
                estoque[i].preco);
    }
    fclose(file);
}

// Cadastra um novo produto
void cadastrarProduto() {
    if (totalProdutos >= MAX_PRODUTOS) {
        printf("Estoque cheio!\n");
        return;
    }

    Produto novo;
    printf("Código: ");
    scanf("%d", &novo.codigo);
    printf("Nome: ");
    scanf("%s", novo.nome);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Preço: ");
    scanf("%f", &novo.preco);

    estoque[totalProdutos] = novo;
    totalProdutos++;
    salvarEstoque();
    printf("Produto cadastrado!\n");
}

// Lista todos os produtos
void listarProdutos() {
    if (totalProdutos == 0) {
        printf("Nenhum produto cadastrado!\n");
        return;
    }

    printf("\n=== ESTOQUE ===\n");
    for (int i = 0; i < totalProdutos; i++) {
        printf("Código: %d | Nome: %s | Quantidade: %d | Preço: R$%.2f\n",
               estoque[i].codigo, estoque[i].nome, 
               estoque[i].quantidade, estoque[i].preco);
    }
}

// Busca um produto pelo código
void buscarProduto() {
    int codigo;
    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    for (int i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == codigo) {
            printf("Produto encontrado:\n");
            printf("Nome: %s | Quantidade: %d | Preço: R$%.2f\n",
                   estoque[i].nome, estoque[i].quantidade, estoque[i].preco);
            return;
        }
    }
    printf("Produto não encontrado!\n");
}

// Atualiza a quantidade em estoque
void atualizarEstoque() {
    int codigo, quantidade;
    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    for (int i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == codigo) {
            printf("Quantidade atual: %d\n", estoque[i].quantidade);
            printf("Nova quantidade: ");
            scanf("%d", &quantidade);
            estoque[i].quantidade = quantidade;
            salvarEstoque();
            printf("Estoque atualizado!\n");
            return;
        }
    }
    printf("Produto não encontrado!\n");
}

// Exclui um produto do estoque
void excluirProduto() {
    int codigo;
    printf("Digite o código do produto: ");
    scanf("%d", &codigo);

    for (int i = 0; i < totalProdutos; i++) {
        if (estoque[i].codigo == codigo) {
            for (int j = i; j < totalProdutos - 1; j++) {
                estoque[j] = estoque[j + 1];
            }
            totalProdutos--;
            salvarEstoque();
            printf("Produto excluído!\n");
            return;
        }
    }
    printf("Produto não encontrado!\n");
}

// Menu principal
void menu() {
    int opcao;
    do {
        printf("\n=== MENU ===\n");
        printf("1. Cadastrar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar produto\n");
        printf("4. Atualizar estoque\n");
        printf("5. Excluir produto\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: listarProdutos(); break;
            case 3: buscarProduto(); break;
            case 4: atualizarEstoque(); break;
            case 5: excluirProduto(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}