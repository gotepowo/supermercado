#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <stdbool.h>

struct produto{
	int codigo;
	char nome[100];
	float preco;
	int estoque;
	bool foiUsado;
	bool deletado = false;
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

void configurar_ambiente();
void printLista();
void limparBufferTeclado();
void removerQuebraDeLinha(char *str);
produto* criarProduto(int codigo, const char* nome, float preco, int estoque);
produto* inserirNoFinal(produto *cabeca, produto *novoProduto);
produto* buscaPorCodigo(produto *cabeca, int codigoBusca);
void dividirListaProduto(struct produto *fonte, struct produto **frente, struct produto **tras);
struct produto* mesclarOrdenadoPorNome(struct produto *a, struct produto *b);
void mergeSortProdutos(struct produto **cabecaRef);
void exibirProdutos(produto *cabeca);
void liberarLista(produto *cabeca);
void cadastrarVenda(produto *listaDeProdutos, noVenda **listaDeVendas);
void salvarVendasDoDia(noVenda *listaDeVendas);
void escreverArquivo(produto *listaProdutos, int n, char nomeArquivo[100]);
void removerProduto(produto *produtos, int *n);
void alterarEstoque(produto *produtos);
void buscaPorData();
void escolhaUsuario(produto *produtos, noVenda **listaDeVendas, int *qtdProdutos, char nomeArquivo[100], bool *continuar);
void exibirLista(produto *cabeca);