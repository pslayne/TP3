#include "vetor.h"
#include <fstream>
#include <iostream>
using namespace std;

//recebe um ponteiro pra estoque, e dois ponteiros pra int
estoque* ler_bin(estoque* vetor, int* n, int* tam) {
	ifstream fin;

	//abre o arquivo bin�rio
	fin.open("estoque.bin", ios_base::in | ios_base::binary);
	*n = 0;

	if (!fin.is_open()) {
		//se n�o abrir manda tam pra 0 e manda o vetor pra expans�o
		*tam = 0;
		vetor = expansao(vetor, n, tam);
	} else {
		//se abrir pega o cabe�alho, que � o n�mero de produtos 
		fin.read((char*) tam, sizeof(int));
		//cria um vetor do tamanho de tam
		vetor = new estoque[*tam];

		//l� os produtos coloca no vetor e manda pra expans�o 
		for (int i = 0; i < *tam; i++) {
			fin.read((char*)& (vetor + i)->nome, 30 * sizeof(char));
			fin.read((char*)& (vetor + i)->preco, sizeof(float));
			fin.read((char*)& (vetor + i)->quantidade, sizeof(int));
			fin.read((char*)& (vetor + i)->vazio, sizeof(bool));
		}
		vetor = expansao(vetor, n, tam);
	}
	fin.close();

	return vetor;
}

estoque* expansao(estoque* vetor, int* n, int* tam) {
	//2 elevado ao n�mero de expans�es
	int b = (int)pow(2, *n);
	
	//cria um vetor auxiliar
	estoque* vetor_aux = new estoque[*tam];

	//transfere os valores do vetor original para o auxiliar
	for (int i = 0; i < *tam; i++) 
		vetor_aux[i] = vetor[i];

	//deleta o vetor
	delete[] vetor;

	//cria um novo sob o mesmo ponteiro do novo tamanho
	vetor = new estoque[*tam + b];

	//transfere os valores do vetor auxiliar para o original
	for (int i = 0; i < *tam; i++) {
		vetor[i] = vetor_aux[i];
		vetor[i].vazio = false;
	}

	//deleta o auxiliar
	delete[] vetor_aux;

	//manda o booleano dos espa�os novos (e consequentemente vazios) para true
	for (int i = *tam; i < *tam + b; i++)
		vetor[i].vazio = true;

	//atualiza tam e n
	*tam += b;
	++*n;

	return vetor;
}

void escrever_bin(estoque* vetor, int* tam) {
	int cont = 0;
	ofstream fout;

	//conta os falsos (n�o vazios)
	for (int i = 0; i < *tam; i++) {
		if (!vetor[i].vazio)
			cont++;
	}
	//abre o arquivo bin�rio para sobrescrever
	fout.open("estoque.bin", ios_base::out | ios_base::binary | ios_base::trunc);

	//escrita do cabe�alho
	fout.write((char*)& cont, sizeof(int));

	//escrita dos produtos
	for (int i = 0; i < cont; i++) {
		fout.write((char*) & (vetor + i)->nome, 30 * sizeof(char));
		fout.write((char*) & (vetor + i)->preco, sizeof(float));
		fout.write((char*) & (vetor + i)->quantidade, sizeof(int));
		fout.write((char*) & (vetor + i)->vazio, sizeof(bool));
	}

	//fecha o arquivo
	fout.close();
}
