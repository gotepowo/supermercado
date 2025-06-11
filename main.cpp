#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif
// configura ambiente para windows
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

struct venda{
	char data[11];
	char hora[9];
	char cpfCliente[14];
	itemVenda *itens;
};

struct noVenda{
	venda dadosVenda;
	noVenda *proximo;
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

void dividirListaProduto(struct produto *fonte, struct produto **frente, struct produto **tras) {
    struct produto *rapido;
    struct produto *devagar;
    devagar = fonte;
    rapido = fonte->proximo;

    while (rapido != NULL) {
        rapido = rapido->proximo;
        if (rapido != NULL) {
            devagar = devagar->proximo;
            rapido = rapido->proximo;
        }
    }

    *frente = fonte;
    *tras = devagar->proximo;
    devagar->proximo = NULL;
}

// Mescla duas listas ordenadas por nome
struct produto* mesclarOrdenadoPorNome(struct produto *a, struct produto *b) {
    struct produto *resultado = NULL;

    if (a == NULL) return b;
    else if (b == NULL) return a;

    if (strcmp(a->nome, b->nome) <= 0) {
        resultado = a;
        resultado->proximo = mesclarOrdenadoPorNome(a->proximo, b);
    } else {
        resultado = b;
        resultado->proximo = mesclarOrdenadoPorNome(a, b->proximo);
    }

    return resultado;
}

// Função principal para aplicar mergesort na lista de produtos
void mergeSortProdutos(struct produto **cabecaRef) {
    struct produto *cabeca = *cabecaRef;
    struct produto *a;
    struct produto *b;

    if ((cabeca == NULL) || (cabeca->proximo == NULL)) {
        return; // Lista vazia ou com um elemento está ordenada
    }

    dividirListaProduto(cabeca, &a, &b);

    mergeSortProdutos(&a);
    mergeSortProdutos(&b);

    *cabecaRef = mesclarOrdenadoPorNome(a, b);
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

void cadastrarVenda(produto *listaDeProdutos, noVenda **listaDeVendas){
	itemVenda *carrinho = NULL;
	itemVenda *novoItem, *itemAtual;
	produto *produtoAtual;
	char continuar;
	float totalVenda = 0.0;
	bool primeiraCompra = true;
    char cpfCliente[13];


	do{
		while(primeiraCompra){
			printf("Informe o seu CPF antes de começar a comprar: ");
			limparBufferTeclado();
			fgets(cpfCliente, sizeof(cpfCliente), stdin);

			printf("CPF informado: %s\n", cpfCliente);
			printf("Esse é o CPF correto? (S/N)\n");
			limparBufferTeclado();
			char resposta;
			scanf(" %c", &resposta);
			if(resposta != 's' || resposta != 'S'){
				primeiraCompra = false;
			} else {
				continue;
			}
		}

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
		if(continuar == 'S' || continuar == 's'){
			primeiraCompra = false;
		}
	} while(continuar == 'S' || continuar == 's');
	
	printf("---------------Venda Finalizada---------------\n");

	if(carrinho == NULL){
		printf("Carrinho vazio, nenhuma venda contabilizada\n");
	} else {
		noVenda *novaVendaNode = (noVenda*) malloc(sizeof(noVenda));
		time_t t = time(NULL);
		struct tm *tm = localtime(&t);

		strftime(novaVendaNode->dadosVenda.data, sizeof(novaVendaNode->dadosVenda.data), "%Y/%m/%d", tm);
        strftime(novaVendaNode->dadosVenda.hora, sizeof(novaVendaNode->dadosVenda.hora), "%H:%M:%S", tm);
        strcpy(novaVendaNode->dadosVenda.cpfCliente, cpfCliente);
        novaVendaNode->dadosVenda.itens = carrinho;
        novaVendaNode->proximo = NULL;

		if (*listaDeVendas == NULL) {
            *listaDeVendas = novaVendaNode;
        } else {
            noVenda *vendaAtual = *listaDeVendas;
            while (vendaAtual->proximo != NULL) {
                vendaAtual = vendaAtual->proximo;
            }
            vendaAtual->proximo = novaVendaNode;
        }

		itemAtual = carrinho;
		printf("Venda registrada com sucesso. Detalhes: \n");
        printf("CPF do cliente: %s\n", cpfCliente);
		printf("Data: %s | Hora: %s\n", novaVendaNode -> dadosVenda.data, novaVendaNode -> dadosVenda.hora);
		printf("Detalhes da venda:\n");
		while(itemAtual != NULL){
			float subtotal = itemAtual -> quantidade * itemAtual -> precoUnitario;
			printf("->%d %s a %.2f reais cada, total %.2f\n", itemAtual ->quantidade, itemAtual -> nomeProduto, itemAtual -> precoUnitario, subtotal);
			totalVenda += subtotal;
			
			produtoAtual = buscaPorCodigo(listaDeProdutos, itemAtual -> codigoProduto);
			if(produtoAtual != NULL){
				produtoAtual -> estoque -= itemAtual -> quantidade;
			}
			itemVenda *proximoItem = itemAtual -> proximo;
			free(itemAtual);
			itemAtual = proximoItem;
		}
		printf("\nVALOR TOTAL DA VENDA: R$%.2f\n\n", totalVenda);
	}
}

void salvarVendasDoDia(noVenda *listaDeVendas) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    char dataDeHoje[11];
    strftime(dataDeHoje, sizeof(dataDeHoje), "%Y/%m/%d", tm_info);

    char nomeArquivo[50];
    strftime(nomeArquivo, sizeof(nomeArquivo), "vendas%d%m%Y.txt", tm_info);

    FILE *arquivo = fopen(nomeArquivo, "w");

    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo de relatorio");
        return;
    }

    printf("Salvando vendas do dia %s no arquivo %s...\n", dataDeHoje, nomeArquivo);

    noVenda *vendaAtual = listaDeVendas;
    bool algumaVendaSalva = false;
    float totalVendasDoDia = 0.0;

    while (vendaAtual != NULL) {
        if (strcmp(vendaAtual->dadosVenda.data, dataDeHoje) == 0) {
            algumaVendaSalva = true;
            float totalVendaIndividual = 0.0;

            fprintf(arquivo, "%s\n", vendaAtual->dadosVenda.data);
            fprintf(arquivo, "%s\n", vendaAtual->dadosVenda.hora);
            fprintf(arquivo, "%s\n", vendaAtual->dadosVenda.cpfCliente);

            itemVenda *item = vendaAtual->dadosVenda.itens;
            while(item != NULL) {
                fprintf(arquivo, "%d %s %d %.2f\n",
                    item->codigoProduto,
                    item->nomeProduto,
                    item->quantidade,
                    item->precoUnitario
                );
                totalVendaIndividual += item->quantidade * item->precoUnitario;
                item = item->proximo;
            }

            fprintf(arquivo, "%.2f\n\n", totalVendaIndividual);
            totalVendasDoDia += totalVendaIndividual;
        }
        vendaAtual = vendaAtual->proximo;
    }

    if (algumaVendaSalva) {
        printf("Relatorio de vendas de hoje salvo com sucesso!\n");
    } else {
        printf("Nenhuma venda encontrada para o dia de hoje.\n");
        fclose(arquivo);
        remove(nomeArquivo);
        return;
    }

    fclose(arquivo);
}

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

    fclose(arquivo);
       
    mergeSortProdutos(&produtos);

	do{
		printLista();
		int escolha = escolhaUsuario();
		switch(escolha){
			case 1:
			cadastrarVenda(produtos, &listaDeVendas);
			break;
			
			case 2:
			break;
			
			case 3:
			break;
			
			case 4:
			break;

			case 5:
			salvarVendasDoDia(listaDeVendas);
			printf("Programa encerrado. Finalizando CMD.\n");
			continuar = false;
			break;
		}
	} while(continuar == true);
	return 0;
}	
