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

// void cadastrarVenda(produto *cabeca){
// } //WIP

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
	SetConsoleOutputCP(65001);
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
			printf("\n -- Pressione ENTER para exibir mais ou digite 0 para finalizar --\n");
			char ch = getchar();
			if(ch == '0'){
				limparBufferTeclado();
				break;
			}	
		}
	}
	printf("----------------------------------------\n");
}

void exibirLista(produto *cabeca){
    produto *atual = cabeca;

    while (atual != NULL) {
        printf("Codigo: %d\n", atual->codigo);
        printf("Nome: %s\n", atual->nome);
        printf("Preco: %.2f\n", atual->preco);
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
	configurar_ambiente();
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

	printLista();
	int escolha = escolhaUsuario();
	switch(escolha){
		case 1:
		exibirProdutos(produtos);
		break;
		
		case 2:
		break;
		
		case 3:
		break;
		
		case 4:
		break;

		case 5:
		break;
	}
	return 0;
}	