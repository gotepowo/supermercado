#include "funcoes.h"

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

void printLista(){
	printf("[1] Cadastrar Venda\n");
	printf("[2] Listar vendas por data\n");
	printf("[3] Alterar estoque e preco de produto\n");
	printf("[4] Remover produto do estoque\n");
	printf("[5] Sair\n");
}

void limparBufferTeclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void removerQuebraDeLinha(char *str) {
    str[strcspn(str, "\n")] = 0;
}

produto* criarProduto(int codigo, const char* nome, float preco, int estoque){
	produto *novoProduto = (produto*) malloc(sizeof(produto));

	novoProduto -> codigo = codigo;
	strcpy(novoProduto -> nome, nome);
	novoProduto -> preco = preco;
    novoProduto -> estoque = estoque;
	novoProduto -> proximo = NULL;
	novoProduto -> foiUsado = false;
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

void dividirListaProduto(struct produto *fonte, struct produto **frente, struct produto **tras){
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

struct produto* mesclarOrdenadoPorNome(struct produto *a, struct produto *b){
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

void mergeSortProdutos(struct produto **cabecaRef) {
    struct produto *cabeca = *cabecaRef;
    struct produto *a;
    struct produto *b;

    if ((cabeca == NULL) || (cabeca->proximo == NULL)) {
        return;
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
        if(atual -> estoque > 0 && !(atual -> deletado)){
            printf("Codigo: %-5d | Nome: %-30s | Preço: R$ %-7.2f\n", atual->codigo, atual->nome, atual->preco);
            contador++;

            if(contador > 0 && contador % itensPorPagina == 0){
                printf("\n -- Pressione 1 para exibir mais ou digite 0 para finalizar --\n");
                char buffer[100];
                char caractere;
				limparBufferTeclado();
                while(true){
                    fgets(buffer, sizeof(buffer), stdin);
                    if (strlen(buffer) == 2 && buffer[1] == '\n') {
                        caractere = buffer[0];
                        if(caractere == '1' || caractere == '0'){
                            break;
                        }
                    }
                    printf("Por favor insira uma opção válida (0 ou 1): ");
                }
                
                if(caractere == '0'){
                    return;
                }
            }
        }
        atual = atual -> proximo;
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

			char resposta;
			scanf(" %c", &resposta);
			if(resposta == 's' || resposta == 'S'){
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
				limparBufferTeclado();
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

				produtoAtual -> foiUsado = true;

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
		printf("\nCarrinho vazio, nenhuma venda contabilizada\n\n");
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

    FILE *arquivo = fopen(nomeArquivo, "a");

    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo de relatorio");
        return;
    } else {
		printf("Salvando vendas do dia %s no arquivo %s...\n", dataDeHoje, nomeArquivo);
	}


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

void escreverArquivo(produto *listaProdutos, int n, char nomeArquivo[100]){
	nomeArquivo[strcspn(nomeArquivo, "\n")] = 0;
	FILE *arquivo = fopen(nomeArquivo, "w");
	fprintf(arquivo, "%d\n", n);
	while(listaProdutos != NULL){
		if(!(listaProdutos -> deletado)){
			fprintf(arquivo, "%d\n", listaProdutos -> codigo);
			fprintf(arquivo, "%s\n", listaProdutos -> nome);
			fprintf(arquivo, "%.2f\n", listaProdutos -> preco);
			fprintf(arquivo, "%d\n", listaProdutos -> estoque);
			fprintf(arquivo, "\n");
		}
		listaProdutos = listaProdutos -> proximo;
	}

	fclose(arquivo);
}

void removerProduto(produto *produtos, int *n){
    exibirProdutos(produtos);
	printf("Informe o codigo do produto a ser removido: ");
	int codigo;
	scanf("%d", &codigo);
	produto *produtoAtual = buscaPorCodigo(produtos, codigo);
	if(produtoAtual -> foiUsado){
		printf("\nProduto já foi utilizado em uma compra hoje.\n");
		printf("Produto não foi removido do estoque \n\n");
	} else {
		produtoAtual -> deletado = true;
		printf("\nProduto removido com sucesso.\n");
		*n -= 1;
	}
}

void alterarEstoque(produto *produtos){
	int codigo, estoque;
	float preco;
	exibirLista(produtos);
	printf("Informe o código do produto a ser alterado: ");
	scanf("%d", &codigo);

	produto *produtoAtual = buscaPorCodigo(produtos, codigo);
    printf("Produto selecionado: %s\n", produtoAtual -> nome);

	printf("Informe o novo preço do produto: ");
	scanf("%f", &preco);
	produtoAtual -> preco = preco;
	printf("\nNovo preço do produto: R$%.2f\n\n", produtoAtual->preco);

	printf("Informe o estoque a adicionar ao produto: ");
	scanf("%d", &estoque);
	produtoAtual -> estoque += estoque;
	printf("\nNovo estoque do produto: %d\n\n", produtoAtual ->estoque);
}

void buscaPorData(){
	int dia, mes, ano;
	char dataArquivo[100], nomeArquivo[100];
	char cpf[15], hora[20], data[20], linhaLida[256];
	float valorTotal;

	printf("Informe a data que deseja que deseja buscar vendas: ");
	limparBufferTeclado();

	fgets(dataArquivo, sizeof(dataArquivo), stdin);

	removerQuebraDeLinha(dataArquivo);

	sscanf(dataArquivo, "%d/%d/%d", &ano, &mes, &dia);

	sprintf(nomeArquivo, "vendas%02d%02d%d.txt", dia, mes, ano);

	FILE *arquivo = fopen(nomeArquivo, "r");

	if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo de vendas para esta data");
        return;
    }


	printf("\n---------Relatório de vendas---------\n\n");

	while (fgets(data, sizeof(data), arquivo) != NULL) {
        fgets(hora, sizeof(hora), arquivo);
        fgets(cpf, sizeof(cpf), arquivo);

        removerQuebraDeLinha(data);
        removerQuebraDeLinha(hora);
        removerQuebraDeLinha(cpf);

        while (fgets(linhaLida, sizeof(linhaLida), arquivo) != NULL) {
            if (strchr(linhaLida, ' ') != NULL) {
                continue;
            } else {
                valorTotal = atof(linhaLida);
                break;
            }
        }
        
        printf("Data: %s\n", data);
        printf("Hora: %s\n", hora);
        printf("CPF: %s\n", cpf);
        printf("Valor Total da Venda: %.2f\n\n", valorTotal);

        fgets(linhaLida, sizeof(linhaLida), arquivo);
    }
	fclose(arquivo);
}

void escolhaUsuario(produto *produtos, noVenda **listaDeVendas, int *qtdProdutos, char nomeArquivo[100], bool *continuar){
	int escolha;
	scanf("%d", &escolha);
	while(escolha <= 0 || escolha > 5){
		printf("Escolha invalida, tente novamente.\n");
        limparBufferTeclado();
		scanf("%d", &escolha);
	}
	switch(escolha){
			case 1:
			cadastrarVenda(produtos, listaDeVendas);
			break;
			
			case 2:
			buscaPorData();
			break;
			
			case 3:
			alterarEstoque(produtos);
			break;
			
			case 4:
			removerProduto(produtos, qtdProdutos);
			break;

			case 5:
			if(*listaDeVendas != NULL){
                salvarVendasDoDia(*listaDeVendas);
            } else {
				printf("Nenhuma venda realizada no dia de hoje. Arquivo de vendas não foi alterado.\n");
			}
            escreverArquivo(produtos, *qtdProdutos, nomeArquivo);
			printf("Programa encerrado. Finalizando CMD.\n");
			*continuar = false;
			break;
		}
}

void exibirLista(produto *cabeca){
    produto *atual = cabeca;
    int contador = 0;
    const int itensPorPagina = 10;

    printf("\n -- Lista de produtos disponiveis -- \n");
    while(atual != NULL){
        if(!(atual -> deletado)){
            printf("Codigo: %-5d | Nome: %-30s | Preço: R$ %-7.2f | Estoque: %d\n", atual->codigo, atual->nome, atual->preco, atual->estoque);
            contador++;

            if(contador > 0 && contador % itensPorPagina == 0){
                printf("\n -- Pressione 1 para exibir mais ou digite 0 para finalizar --\n");
                char buffer[100];
                char caractere;
				limparBufferTeclado();
                while(true){
                    fgets(buffer, sizeof(buffer), stdin);
                    if (strlen(buffer) == 2 && buffer[1] == '\n') {
                        caractere = buffer[0];
                        if(caractere == '1' || caractere == '0'){
                            break;
                        }
                    }
                    printf("Por favor insira uma opção válida (0 ou 1): ");
                }
                
                if(caractere == '0'){
                    return;
                }
            }
        }
        atual = atual -> proximo;
    }
}