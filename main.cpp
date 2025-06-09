#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

void configurar_ambiente() {
    #if defined(_WIN32) || defined(_WIN64)
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
    #else
        setlocale(LC_ALL, "");
    #endif
}

struct produto{
	int codigo;
	char nome[100];
	float preco;
	int estoque;
	struct produto *proximo;
};

struct itemVenda{
	int codigoProduto;
	char nomeProduto[100];
	int quantidade;
	float precoUnitario;
	struct itemVenda *proximo;
};

void printLista(){
	printf("[1] Cadastrar Venda\n");
	printf("[2] Listar vendas por data\n");
	printf("[3] Alterar estoque e preco de produto\n");
	printf("[4] Remover produto do estoque\n");
	printf("[5] Sair\n");
}

int escolhaUsuario(){
	int escolha;
	scanf("%d", &escolha);
	while(escolha <= 0 || escolha > 5){
		printf("Escolha invalida, tente novamente.\n");
		scanf("%d", &escolha);
	}
	return escolha;
}

void limparBufferTeclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

produto* buscaPorCodigo(produto *cabeca, int codigoBusca){
	produto *atual = cabeca;
	while(atual != NULL){
		if(atual -> codigo == codigoBusca){
			return atual;
		}
		atual = atual -> proximo;
	}
	return NULL;
}

void exibirProdutos(produto *cabeca){
	produto *atual = cabeca;
    int contador = 0;
    const int itensPorPagina = 10;

	printf("\n -- Lista de produtos disponiveis -- \n");
	while(atual != NULL){
		if(atual -> estoque > 0){
			printf("Codigo: %-5d | Nome: %-30s | Preço: R$ %-7.2f\n", atual->codigo, atual->nome, atual->preco);
		}
		contador++;
		atual = atual -> proximo;

		if(contador > 0 && contador % itensPorPagina == 0){
			printf("\n -- Pressione 1 para exibir mais ou digite 0 para finalizar --\n");
			limparBufferTeclado();
			char ch = getchar();
			if(ch == '0'){
				limparBufferTeclado();
				break;
			} else if(ch == '1'){
				contador = 0;
			}
		}
	}
}

void exibirLista(produto *cabeca){
    produto *atual = cabeca;

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

void cadastrarVenda(produto *listaDeProdutos){
	itemVenda *carrinho = NULL;
	itemVenda *novoItem, *itemAtual;
	produto *produtoAtual;
	char continuar;
	float totalVenda = 0.0;

	do{
		exibirProdutos(listaDeProdutos);

		int codigoBusca, quantidadeDesejada;

		printf("Digite o código do produto desejado: ");
		scanf("%d", &codigoBusca);

		produtoAtual = buscaPorCodigo(listaDeProdutos, codigoBusca);

		if(produtoAtual == NULL){
			printf("Produto de código %d não encontrado.\n", codigoBusca);
		} else {
			printf("Produto selecionado: %s\n", produtoAtual -> nome);
			
			printf("Digite a quantidade desejada: ");
			scanf("%d", &quantidadeDesejada);
			if(quantidadeDesejada < 0){
				printf("Quantidade inválida.\n");
			} else if (quantidadeDesejada > produtoAtual -> estoque){
				printf("Quantidade desejada maior do que a quantia de estoque atual.\n");
				printf("Estoque atual: %d\n", produtoAtual->estoque);
				printf("Deseja comprar a quantidade em estoque? (S/N)\n");
				char escolha = getchar();
				if(escolha == 'S' || escolha == 's'){
					quantidadeDesejada = produtoAtual -> estoque;
					printf("Quantia de produto ajustada para %d\n", quantidadeDesejada);
				} else {
					printf("Item não adicionado ao carrinho.");
					quantidadeDesejada = 0;
				}
			}
			if(quantidadeDesejada > 0){
				novoItem = (itemVenda*) malloc(sizeof(itemVenda));
				novoItem -> codigoProduto = produtoAtual -> codigo;
				strcpy(novoItem->nomeProduto, produtoAtual->nome);
				novoItem -> precoUnitario = produtoAtual -> preco;
				novoItem -> quantidade = quantidadeDesejada;
				novoItem -> proximo = NULL;

				if(carrinho == NULL){
					carrinho = novoItem;
				} else {
					itemAtual = carrinho;
					while(itemAtual -> proximo != NULL){
						itemAtual = itemAtual -> proximo;
					}
					itemAtual -> proximo = novoItem;
				}
				printf("%d do produto %s adicionado(s) ao carrinho.\n", quantidadeDesejada, novoItem -> nomeProduto);
			}
		}
		printf("\nDeseja adicionar outro produto ao carrinho? (S/N)\n");
		limparBufferTeclado();
		continuar = getchar();
	} while(continuar == 'S' || continuar == 's');

	printf("\n-----Finalizando Venda-----\n");
	if(carrinho == NULL){
		printf("Carrinho vazio, nenhum item contabilizado na venda.\n");
	} else {
		itemAtual = carrinho;
		printf("Detalhes da venda:\n");
		while(itemAtual != NULL){
			float subtotal = itemAtual -> quantidade * itemAtual -> precoUnitario;
			printf("->%d %s a %.2f reais cada, total %.2f", itemAtual ->quantidade, itemAtual -> nomeProduto, itemAtual -> precoUnitario, subtotal);
			totalVenda += subtotal;
			
			produtoAtual = buscaPorCodigo(listaDeProdutos, itemAtual -> codigoProduto);
			if(produtoAtual != NULL){
				produtoAtual -> estoque -= itemAtual -> quantidade;
			}
			itemVenda *proximoItem = itemAtual -> proximo;
			free(itemAtual);
			itemAtual = proximoItem;
		}
		printf("\nVALOR TOTAL DA VENDA: R$%.2f\n", totalVenda);
	}
	printf("------------------------------------\n");
}

int main(){
	configurar_ambiente();
	bool continuar = true;
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
    if (fscanf(arquivo, "%d", &codigoProduto) != 1) {
        fprintf(stderr, "ERRO: Falha ao ler o codigo do produto #%d. Arquivo mal formatado ou incompleto.\n", i + 1);
        break;
    }

    if (fscanf(arquivo, " %[^\n]", nomeProduto) != 1) {
        fprintf(stderr, "ERRO: Falha ao ler o nome do produto #%d.\n", i + 1);
        break;
    }

    if (fscanf(arquivo, "%f", &precoProduto) != 1) {
        fprintf(stderr, "ERRO: Falha ao ler o preco do produto #%d.\n", i + 1);
        break;
    }
    
    if (fscanf(arquivo, "%d", &estoqueProduto) != 1) {
        fprintf(stderr, "ERRO: Falha ao ler o estoque do produto #%d.\n", i + 1);
        break;
    }
	
		produto *novoProduto = criarProduto(codigoProduto, nomeProduto, precoProduto, estoqueProduto);
		produtos = inserirNoFinal(produtos, novoProduto);
	}

	do{
		printLista();
		int escolha = escolhaUsuario();
		switch(escolha){
			case 1:
			cadastrarVenda(produtos);
			break;
			
			case 2:
			break;
			
			case 3:
			break;
			
			case 4:
			break;

			case 5:
			printf("Programa encerrado. Finalizando CMD.\n");
			continuar = false;
			break;
		}
	} while(continuar == true);
	return 0;
}	