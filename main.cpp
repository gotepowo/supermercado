#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct produto{
	int codigo;
	char nome[100];
	float preco;
	int estoque;
	struct produto *proximo;
};

void printLista(){
	printf("[1] Cadastrar Venda\n");
	printf("[2] Listar vendas por data\n");
	printf("[3] Alterar estoque e preco de produto\n");
	printf("[4] Remover produto do estoque\n");
	printf("[5] Sair\n");
}

void escolhaUsuario(int *escolha){
	scanf("%d", escolha);
	if(*escolha <= 0 || *escolha > 5){
		printf("Escolha inválida, tente novamente.\n");
	}
}
produto* criarProduto(int codigo, const char* nome, float preco, int estoque){
	produto *novoProduto = (produto*) malloc(sizeof(produto));

	novoProduto -> codigo = codigo;
	strcpy(novoProduto -> nome, nome);
	novoProduto -> preco = preco;
    novoProduto -> estoque = estoque;
	novoProduto -> proximo = NULL;
	return novoProduto;
}

produto* inserirNoFinal(produto *cabeca, produto *novoProduto){
	if(cabeca == NULL){
		return novoProduto;
	}
	produto *atual = cabeca;
	while(atual -> proximo != NULL){
		atual = atual -> proximo;
	}
	atual -> proximo = novoProduto;
	return cabeca;
}

void exibirLista(produto *cabeca){
    produto *atual = cabeca;
    if (atual == NULL) {
        printf("Lista de produtos vazia.\n");
        return;
    }
    while (atual != NULL) {
        printf("Codigo: %d\n", atual->codigo);
        printf("Nome: %s\n", atual->nome);
        printf("Preco: %.2f\n", atual->preco);
        printf("Estoque: %d\n", atual->estoque);
        if (atual->proximo != NULL) {
            printf("\n");
        }
        atual = atual->proximo;
    }
}

void liberarLista(produto *cabeca) {
    produto *atual = cabeca;
    produto *proximoNo;
    while (atual != NULL) {
        proximoNo = atual->proximo; 
        free(atual);                
        atual = proximoNo;          
    }
}

int main(){
	int escolha;
	int qtdProdutos;
	char nomeArquivo[100];
	produto *produtos = NULL;

	printf("Informe o nome do arquivo: ");
	fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
	nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;

    FILE *arquivo;
	arquivo = fopen(nomeArquivo, "r");

	if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return EXIT_FAILURE;
    } else {
		printf("Arquivo lido com sucesso. Iniciando programa\n");
	}

	if (fscanf(arquivo, "%d", &qtdProdutos) != 1) {
        fprintf(stderr, "Erro ao ler a quantidade de produtos do arquivo.\n");
        fclose(arquivo);
        return EXIT_FAILURE;
    }

	int codigoProduto;
	char nomeProduto[100];
	float precoProduto;
	int estoqueProduto;
	for(int i = 0; i < qtdProdutos; i++){
		fscanf(arquivo, "%d", &codigoProduto);
		fscanf(arquivo, " %[^\n]", nomeProduto);
		fscanf(arquivo, "%f", &precoProduto);
		fscanf(arquivo, "%d", &estoqueProduto);

		produto *novoProduto = criarProduto(codigoProduto, nomeProduto, precoProduto, estoqueProduto);
		produtos = inserirNoFinal(produtos, novoProduto);
	}

	liberarLista(produtos);
	produtos = NULL;
	printLista();
	escolhaUsuario(&escolha);
	return 0;
}	