#include <iostream>
#include <fstream>
#include <cctype>
#include <string.h>
#include <math.h>
#include "menu.h"
#include "vetor.h"
using namespace std;

int main() {
	system("chcp 1252 > null");
	int n, tam;
	estoque* vetor;
	vetor = new estoque[0];
	vetor = ler_bin(vetor, &n, &tam);
	
	//opção
	char op;
	//contador de espaços vazios do vetor
	int espaco;
	bool condition = true;

	//menu
	do{
		cout << endl;
		cout << "SISTEMA DE CONTROLE" << endl;
		cout << "-------------------" << endl;
		cout << "(P)edir" << endl;
		cout << "(A)dicionar" << endl;
		cout << "(E)xcluir" << endl;
		cout << "(L)istar" << endl;
		cout << "(S)air" << endl;
		cout << "-------------------" << endl;
		cout << "opção: \b";
		cin >> op;

		switch (op) {
		//pedir
		case 'p':
		case 'P': vetor = pedir(vetor, tam);
			break;

		//adicionar
		case 'a':
		case 'A': espaco = 0;
			for (int i = 0; i < tam; i++) {
				if (vetor[i].vazio)
					espaco++;
			}
			if (!espaco)
				vetor = expansao(vetor, &n, &tam);
			vetor = adicionar(vetor, tam);
			break;

		//excluir
		case 'e':
		case 'E': vetor = excluir(vetor, tam);
			break;

		//listar
		case 'l':
		case 'L': listar(vetor, tam);
			break;

		//sair
		case 's':
		case 'S': condition = false;
			break;

		default: cout << "opção inválida, tente novamente "; cin >> op;
	}
	} while (condition); //continua até que condition seja falso

	escrever_bin(vetor, &tam);

	delete[] vetor;
}