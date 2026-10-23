#include <stdio.h>
#include <string.h>

#define MAX_PRODUTOS 100
#define ARQUIVO "estoque.txt"

// struct que representa um produto
struct Produto {
    int codigo;
    char nome[50];
    float preco;
    int quantidade;
};

// descarta o que sobrou digitado no teclado (evita o bug do 10.5 no codigo)
void limparBuffer() {
    while (getchar() != '\n');
}

// calcula o valor em estoque de UM produto (preco x quantidade)
float calcularValorProduto(struct Produto p) {
    return p.preco * p.quantidade;
}

// calcula o valor total de TODOS os produtos
float calcularValorTotal(struct Produto produtos[], int total) {
    float soma = 0;
    int i;
    for (i = 0; i < total; i++) {
        soma = soma + calcularValorProduto(produtos[i]);
    }
    return soma;
}

// cadastra um produto novo no array (o codigo e gerado automaticamente)
void cadastrarProduto(struct Produto produtos[], int *total) {
    struct Produto novo;
    int i;

    if (*total >= MAX_PRODUTOS) {
        printf("\nEstoque cheio! Nao e possivel cadastrar mais produtos.\n");
        return;
    }

    // codigo automatico: 101 para o primeiro, depois maior codigo + 1
    novo.codigo = 101;
    for (i = 0; i < *total; i++) {
        if (produtos[i].codigo >= novo.codigo) {
            novo.codigo = produtos[i].codigo + 1;
        }
    }

    printf("\n--- Cadastro de produto ---\n");
    printf("Codigo do produto: %d\n", novo.codigo);

    printf("Nome, sem espaco: ");
    scanf("%49s", novo.nome);
    limparBuffer();

    printf("Preco: ");
    while (scanf("%f", &novo.preco) != 1 || novo.preco < 0) {
        limparBuffer();
        printf("Valor invalido! Digite o preco novamente: ");
    }
    limparBuffer();

    printf("Quantidade: ");
    while (scanf("%d", &novo.quantidade) != 1 || novo.quantidade < 0) {
        limparBuffer();
        printf("Valor invalido! Digite a quantidade (numero inteiro): ");
    }
    limparBuffer();

    produtos[*total] = novo;
    *total = *total + 1;

    printf("\nProduto cadastrado com sucesso!\n");
}

// mostra todos os produtos com o valor em estoque de cada um
void listarProdutos(struct Produto produtos[], int total) {
    int i;

    printf("\n========================================\n");
    printf(" LISTA DE PRODUTOS\n");
    printf("========================================\n");

    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        printf("\nCodigo: %d\n", produtos[i].codigo);
        printf("Produto: %s\n", produtos[i].nome);
        printf("Preco: R$ %.2f\n", produtos[i].preco);
        printf("Quantidade em Estoque: %d\n", produtos[i].quantidade);
        printf("Valor em Estoque: R$ %.2f\n", calcularValorProduto(produtos[i]));
    }

    printf("\nValor Total em Estoque: R$ %.2f\n", calcularValorTotal(produtos, total));
}

// procura um produto pelo codigo e mostra seus dados
void consultarProduto(struct Produto produtos[], int total) {
    int codigo, i, achou = 0;

    printf("\nDigite o codigo do produto (numero inteiro): ");
    while (scanf("%d", &codigo) != 1) {
        limparBuffer();
        printf("Valor invalido! Digite o codigo novamente: ");
    }
    limparBuffer();

    for (i = 0; i < total; i++) {
        if (produtos[i].codigo == codigo) {
            printf("\nCodigo: %d\n", produtos[i].codigo);
            printf("Produto: %s\n", produtos[i].nome);
            printf("Preco: R$ %.2f\n", produtos[i].preco);
            printf("Quantidade em Estoque: %d\n", produtos[i].quantidade);
            printf("Valor em Estoque: R$ %.2f\n", calcularValorProduto(produtos[i]));
            achou = 1;
        }
    }

    if (achou == 0) {
        printf("\nProduto com codigo %d nao encontrado.\n", codigo);
    }
}

// mostra o valor em estoque de cada produto e o total geral
void mostrarValorEstoque(struct Produto produtos[], int total) {
    int i;

    printf("\n========================================\n");
    printf(" RESUMO DO ESTOQUE\n");
    printf("========================================\n");

    if (total == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        printf("\nCodigo: %d\n", produtos[i].codigo);
        printf("Produto: %s\n", produtos[i].nome);
        printf("Valor em Estoque: R$ %.2f\n", calcularValorProduto(produtos[i]));
    }

    printf("\nValor Total em Estoque: R$ %.2f\n", calcularValorTotal(produtos, total));
}

// grava todos os produtos no arquivo de texto (uma linha por produto)
void salvarProdutos(struct Produto produtos[], int total) {
    FILE *arquivo;
    int i;

    arquivo = fopen(ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("\nErro ao criar o arquivo %s!\n", ARQUIVO);
        return;
    }

    for (i = 0; i < total; i++) {
        fprintf(arquivo, "%d %s %.2f %d\n", produtos[i].codigo, produtos[i].nome,
                produtos[i].preco, produtos[i].quantidade);
    }

    fclose(arquivo);
    printf("\n%d produto(s) salvo(s) em %s.\n", total, ARQUIVO);
}

// le os produtos do arquivo e devolve quantos foram carregados
int carregarProdutos(struct Produto produtos[]) {
    FILE *arquivo;
    int total = 0;

    arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        return 0; // arquivo ainda nao existe
    }

    while (total < MAX_PRODUTOS &&
           fscanf(arquivo, "%d %49s %f %d", &produtos[total].codigo, produtos[total].nome,
                  &produtos[total].preco, &produtos[total].quantidade) == 4) {
        total++;
    }

    fclose(arquivo);
    return total;
}

void mostrarMenu() {
    printf("\n========================================\n");
    printf(" SISTEMA DE CONTROLE DE ESTOQUE\n");
    printf("========================================\n\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Consultar produto\n");
    printf("4 - Salvar dados\n");
    printf("5 - Carregar dados\n");
    printf("6 - Calcular valor do estoque\n");
    printf("7 - Sair\n\n");
    printf("Digite uma opcao: ");
}

int main() {
    struct Produto produtos[MAX_PRODUTOS];
    int total = 0;
    int opcao = 0;

    // carrega os dados gravados quando o programa inicia
    total = carregarProdutos(produtos);
    if (total > 0) {
        printf("%d produto(s) carregado(s) do arquivo %s.\n", total, ARQUIVO);
    }

    do {
        mostrarMenu();

        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = 0;
        } else {
            limparBuffer();
        }

        if (opcao == 1) {
            cadastrarProduto(produtos, &total);
        } else if (opcao == 2) {
            listarProdutos(produtos, total);
        } else if (opcao == 3) {
            consultarProduto(produtos, total);
        } else if (opcao == 4) {
            salvarProdutos(produtos, total);
        } else if (opcao == 5) {
            total = carregarProdutos(produtos);
            printf("\n%d produto(s) carregado(s) do arquivo %s.\n", total, ARQUIVO);
        } else if (opcao == 6) {
            mostrarValorEstoque(produtos, total);
        } else if (opcao == 7) {
            printf("\nEncerrando o programa...\n");
        } else {
            printf("\nOpcao invalida! Escolha um numero de 1 a 7.\n");
        }
    } while (opcao != 7);

    return 0;
}