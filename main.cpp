#include "funcoes.h"

int main(){
	configurar_ambiente();
	bool continuar = true;
	int qtdProdutos;
	char nomeArquivo[100];
	produto *produtos = NULL;
	noVenda *listaDeVendas = NULL;


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

    fclose(arquivo);
       
    mergeSortProdutos(&produtos);

	do{
		printLista();
		escolhaUsuario(produtos, &listaDeVendas, &qtdProdutos, nomeArquivo, &continuar);
	} while(continuar);
	return 0;
}