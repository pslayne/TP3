#include "menu.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

estoque* pedir(estoque* estoq, int tam) {
	cout << endl;
	cout << "Pedir" << endl;
	cout << "-----" << endl;
	
	char arq[30], nome_pizzaria[30], lp[50], produtos[80];
	int cont = 0;

	ifstream fin;
	ofstream fout;

	///mostra uma vez e repete até que seja inserido um arquivo válido
	
		//recebe o nome do arquivo
		cout << "Arquivo: "; cin >> arq;
		fin.open(arq);
		while (!fin.is_open()) {
			cout << "\nArquivo inválido! Por favor, digite novamente: ";
			cin >> arq;
			fin.open(arq);
		}

	//lê o nome da pizzaria e a "linha perdida"
	fin.getline(nome_pizzaria, 30);
	fin.getline(lp, 50);

	//conta os produtos até chegar ao fim do arquivo
	while (!fin.eof()) {
		fin.getline(produtos, 80);
		++cont;
	}
	//fecha
	fin.close();
	
	produto* prod = new produto[cont];
	produto p;
	//abre
	fin.open(arq);

	//lê o nome da pizzaria e a "linha perdida"
	fin.getline(nome_pizzaria, 30);
	fin.getline(lp, 80);

	//lê os produtos pra o vetor
	for (int i = 0; i < cont; ) {
		fin >> p.nome;
		//coloca todos os caracteres em minúculo pra facilitar a comparação
		for (int j = 0; p.nome[j]; j++) {
			if (isupper(p.nome[j]))
				p.nome[j] = tolower(p.nome[j]);
		}
		fin >> p.quant;
		
		bool ex = false;
		for (int j = 0; j < cont; j++) {
			//cout << prod[j].nome << " " << p.nome << endl;
			if (!strcmp(prod[j].nome, p.nome)) {
				prod[j].quant += p.quant;
				ex = true;
			}
		}
		if (!ex) {
			prod[i++] = p;
		}
	}
	fin.close();
	
	cout << "prod\n" << endl;
	for (int i = 0; i < cont; i++) {
		cout << prod[i].nome << " " << prod[i].quant << endl;
	}

	estoque* recibo = new estoque[cont];
	bool falha = false;
	int a = 0;

	//checa o estoque
	for (int i = 0; i < tam; i++) {
		if (estoq[i].vazio) {
			cout << "Pedido falhou! No momento não dispomos de todos os itens solicitados." << endl;
		} else {
			for (int j = 0; j < cont; j++) {
				if (estoq[i].nome == prod[j].nome && estoq[i].quantidade >= prod[j].quant) {
					strcpy_s(recibo[a].nome, prod[j].nome);
					recibo[a].preco = estoq[i].preco;
					recibo[a].quantidade = prod[j].quant;
					a++;
				}
				else if (estoq[i].nome == prod[j].nome && estoq[i].quantidade < prod[j].quant) {
					if (!falha) {
						cout << "Pedido falhou! " << endl;
						falha = true;
					}
					cout << prod[j].nome << ": solicitado = " << prod[j].quant << " / em estoque = " << estoq[i].quantidade << endl;
				}
			}
		}
	}

	//se não falhar atualiza o estoque e gera o recibo
	if (!falha) {
		for (int i = 0; i < tam; i++) {
			if (!estoq[i].vazio) {
				for (int j = 0; j < a + 1; j++) {
					if (estoq[i].nome == recibo[j].nome)
						estoq[i].quantidade -= recibo[j].quantidade;
					else
						continue;
				}
			}
		}
		//ajusta a extensão 
		int c = 0;
		for (int i = 0; arq[i]; i++)
			c++;
		arq[c - 1] = 'c'; arq[c - 2] = 'f'; arq[c - 3] = 'n'; arq[c - 4] = '.';

		//gera o recibo
		fout.open(arq);

		fout << nome_pizzaria << endl;
		fout << lp << endl;

		//calcula o valor da compra, se há desconte e o valor final
		float compra = 0, desconto = 0, total = 0;
		for (int i = 0; i < cont; i++) {
			fout << recibo[i].nome << ": " << recibo[i].quantidade << "Kg a R$" << recibo[i].preco << "/Kg = R$" << recibo[i].quantidade * recibo[i].preco << endl;
			compra += recibo[i].quantidade * recibo[i].preco;
		}

		if (compra >= 1000) {
			desconto = (float)0.1 * compra;
			total = compra - desconto;
		}

		fout << lp << endl;
		fout << "compra = R$" << compra << endl;
		fout << "desconto = R$" << desconto << endl;
		fout << "total = R$" << total << endl;
	}

	/*for (int i = 0; i < cont; i++)
		cout << prod[i].nome << " " << prod[i].quant << endl;*/

	/*for (int i = 0; i < a + 1; i++)
		cout << recibo[i].nome << " " << recibo[i].quantidade << " " << recibo[i].preco << endl;*/

	cout << endl;
	//libera a memória alocada dinamicamente
	delete[] prod;
	delete[] recibo;

	return estoq;
	}

estoque* excluir(estoque* estoq, int tam) {
	cout << endl;
	cout << "Excluir" << endl;
	cout << "-------" << endl;

	int cont = 0, escolha;
	char conf;
	
	//conta as posições não vazias
	for (int i = 0; i < tam; i++) {
		if (!estoq[i].vazio)
			cont++;
	}

	//se cont for 0
	if (!cont) {
		cout << "\nnão há produtos em estoque." << endl;
	} else {
		//se n, lista os produtos começando de 1
		for (int i = 0; i < cont; i++) {
			cout << i + 1 << ") " << estoq[i].nome << endl;
		}

		cout << "\nnúmero do produto: "; cin >> escolha;
		//decrementa escolha para poder usar no índice do vetor
		--escolha;
		for (int i = 0; i <= cont; i++) {
			if (escolha == i) {
				//confere
				cout << "deseja excluir \"" << estoq[i].nome << "\"? (s/n) "; cin >> conf;

				if (conf == 's' || conf == 'S') {
					//decrementa cont para poder usar no índice do vetor, na última posição não vazia
					--cont;
					//substitui o valor de onde se quer apagar pelo último valor do vetor
					strcpy_s(estoq[i].nome, estoq[cont].nome);
					estoq[i].preco = estoq[cont].preco;
					estoq[i].quantidade = estoq[cont].quantidade;

					//manda vazio da última posição para verdadeiro
					estoq[cont].vazio = true;

					cout << "\"" << estoq[i].nome << "\" excluído com sucesso.\n" << endl;
				} else {
					cout << "\"" << estoq[i].nome << "\" não foi excluído.\n" << endl;
				}
			}
		}
	}
	return estoq;
}

estoque* adicionar(estoque* estoq, int tam) {
	cout << endl;
	cout << "Adicionar" << endl;
	cout << "---------" << endl;

	estoque produto;
	cout << "produto: "; cin >> produto.nome;

	//manda todas as letras para minúsculo
	for (int i = 0; produto.nome[i]; i++) {
		if (isupper(produto.nome[i]))
			produto.nome[i] = tolower(produto.nome[i]);
		}

	cout << "preço: "; cin >> produto.preco;
	cout << "quantidade: "; cin >> produto.quantidade;

	int falsos = 0;

	//conta os não vazios
	for (int i = 0; i < tam; i++) {
		if (!estoq[i].vazio) {
			falsos++;
		}
	}

	//cout << tam << " " << falsos << "\n" << endl;

	//compara o nome
	for (int i = 0; i < tam; i++) {
		//atualiza
		if (!strcmp(estoq[i].nome, produto.nome)) {
			estoq[i].preco = produto.preco;
			estoq[i].quantidade = estoq[i].quantidade + produto.quantidade;
			cout << "\nproduto atualizado! \n" << endl;
			break;
			} else {
			//insere
				strcpy_s(estoq[falsos].nome, produto.nome);
				estoq[falsos].preco = produto.preco;
				estoq[falsos].quantidade = produto.quantidade;
				cout << "\nproduto adicionado! \n" << endl;
				//manda vazio para falso
				estoq[falsos].vazio = false;
				break;
			}
		}
	return estoq;
}

void listar(estoque* estoq, int t) {
	cout << endl;
	cout << "Listar" << endl;
	cout << "------" << endl;

	//conta os não vazios
	int cont = 0;
	for (int i = 0; i < t; i++) {
		if (!estoq[i].vazio)
			cont++;
	}
	cout << "\n" << cont << endl;
	//mensagem se não houver nenhum
	if (!cont) {
		cout << "\nnão há produtos em estoque." << endl;
	}
	else {
		//lista se houver
		for (int i = 0; i < cont; i++) {
			cout << i+1 << " - " << estoq[i].nome << " - R$" << estoq[i].preco << " - " << estoq[i].quantidade << "Kg" << "\n" << endl;
		}
	}
}