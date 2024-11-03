#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>

typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

typedef struct {
    Produto produto;
    int quantidade;
} Carrinho;

void limpaBuffer(void);
void esvaziarCarrinho(Carrinho *carrinho);
void menu(Produto *produtos, Carrinho *carrinho);
void cadastrarProduto(Produto *produtos, int *quanti_cadastrados);
void listarProdutos(Produto *produtos, int quanti_cadastrados);
void comprarProduto(Produto *produtos, Carrinho *carrinho, int quanti_cadastrados, int *itens_carrinho);
void visualizarCarrinho(Carrinho *carrinho, int itens_carrinho);
int pegarProdutoPorCodigo(Produto *produtos, int quanti_cadastrados, int codigo);
int temNoCarrinho(Carrinho *carrinho, int codigo);
void notaFiscal(Carrinho *carrinho, int itens_carrinho, float preco_total);
void fecharPedido(Carrinho *carrinho, int *itens_carrinho);

int main() {
    setlocale(LC_ALL, "Portuguese");

    Produto produtos[50];
    Carrinho carrinho[50];
    esvaziarCarrinho(carrinho); // Inicializando para não haver lixo de memória.

    menu(produtos, carrinho);
    return 0;
}

void limpaBuffer(void) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu(Produto *produtos, Carrinho *carrinho) {
    int op = 0;
    int quanti_cadastrados = 0;
    int itens_carrinho = 0;
    do {
        puts("");
        puts("1 - Cadastrar produtos");
        puts("2 - Listar produtos");
        puts("3 - Comprar produto");
        puts("4 - Visualizar carrinho");
        puts("5 - Fechar pedido");
        puts("6 - Sair do sistema:");
        puts("Digite a operação que deseja: ");
        scanf("%d", &op);
        switch (op) {
            case 1:
                cadastrarProduto(produtos, &quanti_cadastrados);
                break;
            case 2:
                listarProdutos(produtos, quanti_cadastrados);
                break;
            case 3:
                comprarProduto(produtos, carrinho, quanti_cadastrados, &itens_carrinho);
                break;
            case 4:
                visualizarCarrinho(carrinho, itens_carrinho);
                break;
            case 5:
                fecharPedido(carrinho, &itens_carrinho);
                break;
            case 6:
                printf("Saindo do sistema!\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
                break;
        }
    } while (op != 6);
}

void cadastrarProduto(Produto *produtos, int *quanti_cadastrados) {
    int verificador = 0;
    int codigo_produtos;
    do {
        if(*quanti_cadastrados > 50){
            printf("Limite máximo de 50 produtos atingidos! \n");
            break;
        }
        printf("Digite o código do produto: ");
        scanf("%d", &produtos[*quanti_cadastrados].codigo);
        limpaBuffer();
        codigo_produtos = pegarProdutoPorCodigo(produtos, *quanti_cadastrados, produtos[*quanti_cadastrados].codigo);
        printf("Digite o nome do produto: ");
        fgets(produtos[*quanti_cadastrados].nome, 30, stdin);
        produtos[*quanti_cadastrados].nome[strcspn(produtos[*quanti_cadastrados].nome, "\n")] = '\0';
        printf("Digite o preço: ");
        scanf("%f", &produtos[*quanti_cadastrados].preco);
        limpaBuffer();
        if (produtos[*quanti_cadastrados].preco <= 0) {
            puts("Preço inválido!");
            puts("Tente novamente!");
        } else if (produtos[*quanti_cadastrados].codigo <= 0 || codigo_produtos != -1) {
            puts("Código inválido!");
            puts("Tente novamente!");
        } else if (strlen(produtos[*quanti_cadastrados].nome) <= 0) {
            puts("Nome inválido!");
            puts("Tente novamente!");
        } else {
            verificador++;
            (*quanti_cadastrados)++;
        }
    } while (verificador == 0);
}

void listarProdutos(Produto *produtos, int quanti_cadastrados) {
    int i;
    if (quanti_cadastrados > 0) {
        printf("Produtos: \n\n");
        for (i = 0; i < quanti_cadastrados; i++) {
            printf("%d - Código: %d\n", i + 1, produtos[i].codigo);
            printf("    Nome: %s\n", produtos[i].nome);
            printf("    Preço: %.2f\n\n", produtos[i].preco);
        }
    } else {
        printf("Nenhum produto cadastrado!\n");
    }
    sleep(3);
}

void comprarProduto(Produto *produtos, Carrinho *carrinho, int quanti_cadastrados, int *itens_carrinho) {
    int codigo;
    char op = 's';
    do {
        printf("Digite o código do produto que você deseja: ");
        scanf("%d", &codigo);
        limpaBuffer();
        int codigo_produtos = pegarProdutoPorCodigo(produtos, quanti_cadastrados, codigo);
        if (codigo_produtos != -1) { // Verifica se o produto foi encontrado
            int posicao_carrinho = temNoCarrinho(carrinho, produtos[codigo_produtos].codigo);
            if (posicao_carrinho != -1) { //verifica se já está no carrinho
                carrinho[posicao_carrinho].quantidade++;
            } else {
                carrinho[*itens_carrinho].produto = produtos[codigo_produtos];
                carrinho[*itens_carrinho].quantidade = 1;
                (*itens_carrinho)++;
            }
        } else {
            puts("Este código não existe. Tente novamente!");
        }
        printf("Deseja continuar a comprar? [S/N]\n");
        scanf(" %c", &op);
        limpaBuffer();
    } while (op != 'N' && op != 'n');
}

void visualizarCarrinho(Carrinho *carrinho, int itens_carrinho) {
    int i;
    printf("Carrinho: \n\n");
    if (itens_carrinho > 0) {
        for (i = 0; i < itens_carrinho; i++) {
            printf("%d - Código: %d\n", i + 1, carrinho[i].produto.codigo);
            printf("    Nome: %s\n", carrinho[i].produto.nome);
            printf("    Preço: %.2f\n", carrinho[i].produto.preco);
            printf("    Quantidade: %d\n", carrinho[i].quantidade);
        }
        sleep(3);
    } else {
        printf("Nenhum item no carrinho!\n");
    }
}

int pegarProdutoPorCodigo(Produto *produtos, int quanti_cadastrados, int codigo) {
    int i;
    for (i = 0; i < quanti_cadastrados; i++) {
        if (produtos[i].codigo == codigo) {
            return i; //retorna a posicao se o item estiver cadastrado
        }
    }
    return -1; //retorna -1 caso não encontre o produto
}

int temNoCarrinho(Carrinho *carrinho, int codigo) {
    int i;
    for (i = 0; i < 50; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            return i; //retorna a posicao se o item estiver no carrinho
        }
    }
    return -1; //retorna -1 caso não encontre no carrinho
}

void esvaziarCarrinho(Carrinho *carrinho) {
    int i;
    for (i = 0; i < 50; i++) {
        carrinho[i].produto.codigo = -1;
        carrinho[i].quantidade = 0;
    }
}

void notaFiscal(Carrinho *carrinho, int itens_carrinho, float preco_total){ // cria arquivo txt com os produtos comprados e o preço total da compra.
    int i; 
    FILE *p = fopen("Nota_Fiscal.txt", "w");
    if (p == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
    }
    for(i = 0; i < itens_carrinho; i++){
        fprintf(p, "Produto %d: \n\n", i+1);
        fprintf(p, "Código %d\n", carrinho[i].produto.codigo);
        fprintf(p, "Nome: %s\n", carrinho[i].produto.nome);
        fprintf(p, "Preço: %.2f\n", carrinho[i].produto.preco);
        fprintf(p, "Produto %d: \n", i+1);
        fprintf(p, "Quantidade: %d\n\n\n", carrinho[i].quantidade);
    }
    fprintf(p, "Preço total: %.2f", preco_total);
    fclose(p);
}

void fecharPedido(Carrinho *carrinho, int *itens_carrinho) { //encerra o pedido e esvazia o carrinho;
    float preco_total = 0;
    int i;
    for (i = 0; i < *itens_carrinho; i++) {
        preco_total += carrinho[i].produto.preco * carrinho[i].quantidade;
    }
    printf("O preço final será de: %.2f\n", preco_total);
    notaFiscal(carrinho, *itens_carrinho, preco_total);
    esvaziarCarrinho(carrinho);
}
