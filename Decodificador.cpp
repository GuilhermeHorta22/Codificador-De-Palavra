
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio2.h>
#include<time.h>

#include "tadHuff.h"

void insere(AuxStr **a, AuxStr b) {
	AuxStr *novo = (AuxStr*) malloc(sizeof(AuxStr));
	
	strcpy(novo->codHuff, b.codHuff);
	strcpy(novo->palavra, b.palavra);
	novo->qtde = b.qtde;
	novo->simbolo = b.simbolo;
	
	if(*a == NULL) {
		novo->prox = NULL;
		*a = novo;
	}
	else {
		novo->prox = *a;
		*a = novo;
	}
}

void transforma(AuxStr **a, FILE *ptrArq) {
	AuxStr aux;
	
	fread(&aux, sizeof(AuxStr), 1, ptrArq);
	while(!feof(ptrArq)) {
		insere(*&a, aux);
		fread(&aux, sizeof(AuxStr), 1, ptrArq);
	}
}

int main(void) {
	Tree *arv;
	FILE *ptrTabela = fopen("tabela.dat", "rb"), *ptrFrase = fopen("frase-comprimida.txt", "r");
	AuxStr *tab = NULL, *exibe;
	
	transforma(&tab, ptrTabela);
	recuperaArv(&arv, tab);
	exibeFrase(ptrFrase, arv, tab);
}
