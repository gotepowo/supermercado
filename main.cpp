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
	int qtdProdutos;
	char nomeArquivo[100];

	fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
	nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;

    FILE *arquivo;

	arquivo = fopen(nomeArquivo, "r");

	fscanf(arquivo, "%d", &qtdProdutos);

	produto produtos[qtdProdutos];

}	