#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct produto{
	int codigo;
	char nome[100];
	float preco;
	int estoque;
};


int main(){
	int qtdProdutos;
	char nomeArquivo[100];

	fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
	nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;

    FILE *arquivo;

	arquivo = fopen(nomeArquivo, "r");

	fscanf(arquivo, "%d", &qtdProdutos);

	produto produtos[qtdProdutos];

	for(int i = 0; i < qtdProdutos; i++){
		fscanf(arquivo, "%d", &produtos[i].codigo);
		fscanf(arquivo, " %[^\n]", produtos[i].nome);
		fscanf(arquivo, "%f", &produtos[i].preco);
		fscanf(arquivo, "%d", &produtos[i].estoque);
	}
	for(int i = 0; i < qtdProdutos; i++){
		printf("Codigo: %d\n", produtos[i].codigo);
		printf("Nome: %s\n", produtos[i].nome);
		printf("Preco: %.2f\n", produtos[i].preco);
		printf("Estoque: %d\n", produtos[i].estoque);
		if(i != qtdProdutos - 1){
			printf("\n");
		}
	}
}	