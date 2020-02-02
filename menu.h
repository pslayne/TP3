#pragma once

struct produto {
	char nome[30];
	int quant;
};

struct estoque {
	char nome[30];
	float preco;
	int quantidade;
	bool vazio;
};

estoque* pedir(estoque*, int);
estoque* excluir(estoque*, int);
estoque* adicionar(estoque*, int);
void listar(estoque*, int);