#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio2.h>
#include<time.h>

#include "tadHuff.h"

void insereLista(char palavra[30], AuxStr **list) {
	AuxStr *novo = (AuxStr*)malloc(sizeof(AuxStr));
	
	strcpy(novo->palavra, palavra);
	novo->prox = *list;
	*list = novo;
}

char buscaPalavra(char palavra[30], AuxStr *list) {
    while (list != NULL && strcmp(list->palavra, palavra) != 0) {
        list = list->prox;
    }
    if(list == NULL)
    	return 1;
    return 0;
}

void separaFrase(char frase[], AuxStr **list) {
	int i=0, j;
	char palavra[30];
	
	while(frase[i] != '\0') {
		if(frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-') {
			for(j=0 ; frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-' ; j++) {
				if(frase[i] > 64 && frase[i] < 91)
					palavra[j] = frase[i]+32;
				else
					palavra[j] = frase[i];
				i++; 
			}
			palavra[j] = '\0';
			i--;
			if(buscaPalavra(palavra, *list))
				insereLista(palavra, &*list);
		}
		i++;
	}
	for(i=0 ; frase[i] != ' ' ; i++)
		i++;
	
	if(frase[i] != '\0') {
		palavra[0] = ' ';
		palavra[1] = '\0';
		insereLista(palavra, *&list);
	}
}

void ajustar(AuxStr *list, AuxStr **alterado) {
	AuxStr *aux = list;
	
	while(aux != NULL) {
		insereLista(aux->palavra, &*alterado);
		aux = aux->prox;
	}
}

void clivar(char frase[], AuxStr **list) {
	AuxStr *aux = NULL;
	int i=0, j;
	char palavra[30];
	
	while(frase[i] != '\0') {
		if(frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-') {
			for(j=0 ; frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-' ; j++) {
				if(frase[i] > 64 && frase[i] < 91)
					palavra[j] = frase[i]+32;
				else
					palavra[j] = frase[i];
				i++; 
			}
			palavra[j] = '\0';
			i--;
			insereLista(palavra, *&list);
		}
		else {
			if(frase[i] == ' ') {
				for(j=0 ; frase[i] == ' ' ; i++, j++)
					palavra[j] = frase[i];
				palavra[j] = '\0';
				i--;
				insereLista(palavra, *&list);
			}
		}
		i++;
	}
}

void contaPalavras(AuxStr **aux, char frase[]) {
	int i=0,j;
	(*aux)->qtde = 0;
	char palavra[30];
	if(strcmp((*aux)->palavra," ")!=0)
		while(frase[i]!='\0')
		{
			if(frase[i]>64&&frase[i]<91||frase[i]>96&&frase[i]<123||frase[i]=='-')
			{
				
				for(j=0;frase[i]>64&&frase[i]<91||frase[i]>96&&frase[i]<123||frase[i]=='-';j++)
				{
					if(frase[i]>64&&frase[i]<91)
						palavra[j] = frase[i]+32;
					else
						palavra[j] = frase[i];
					i++;
				}
				palavra[j] = '\0';
				i--;
				if(strcmp((*aux)->palavra,palavra)==0)
					(*aux)->qtde++;	
			}
				i++;		
		}
	else
		for(i=0;frase[i]!='\0';i++)
			if(frase[i]==' ')
				(*aux)->qtde++;
}

void setQtdeAll(char frase[], AuxStr **palavras) {
	AuxStr *auxS = *palavras;
	
	while(auxS != NULL) {
		contaPalavras(&auxS, frase);
		auxS = auxS->prox;
	}
}

int main(void) {
	AuxStr *list = NULL, *auxL, *alterado = NULL;
	ListTree *lTree = NULL, *auxTree;
	int TL = 0;
	char frase[1000], auxC[30];
	strcpy(frase, "Amo como ama o amor. Nao conheco nenhuma outra razao para amar senao amar.Que queres que te diga, alem de que te amo, se o que quero dizer-te e que te amo?");
	printf("Texto usado para codificacao: %s\n\n", frase);

	separaFrase(frase, &list);
	
	setQtdeAll(frase, &list);
	
	auxL = list;
	while(auxL != NULL) {
		criaLista(&lTree, &auxL);
		auxL = auxL->prox;
	}
	
	criaHuffman(&lTree);
	
	adicionaCodHuff(lTree->arv, auxC, &TL, &list);
	
	salvaTabelaArq(list);
	
	auxL = NULL;
	clivar("amo, nao conheco outra razao de e para amar, amo, alem de amar", &auxL);
	ajustar(auxL, &alterado);
	comprimeFrase(alterado, list);
}
