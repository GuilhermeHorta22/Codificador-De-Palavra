#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio2.h>
#include<time.h>

#include "tadHuffman.h"

#define TF 1000

void insereLista(char palavra[30], AuxStr **list) 
{
	AuxStr *novo = (AuxStr*)malloc(sizeof(AuxStr));	
	strcpy(novo->palavra, palavra);
	novo->prox = *list;
	*list = novo;
}

char buscaPalavra(char palavra[30], AuxStr *list) 
{
    while(list != NULL && strcmp(list->palavra, palavra) != 0)
        list = list->prox;
    
    if(list == NULL)
    	return 1;
    return 0;
}

void separaFrase(char frase[], AuxStr **list) 
{
	int i=0, j;
	char palavra[30];
	
	while(frase[i] != '\0') 
	{
		if(frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-') 
		{
			for(j=0 ; frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-' ; j++) 
			{
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
	
	if(frase[i] != '\0') 
	{
		palavra[0] = ' ';
		palavra[1] = '\0';
		insereLista(palavra, *&list);
	}
}

void criarCopia(AuxStr *list, AuxStr **alterado) 
{
	AuxStr *aux = list;
	while(aux != NULL) 
	{
		insereLista(aux->palavra, &*alterado);
		aux = aux->prox;
	}
}

void separarTudo(char frase[], AuxStr **list) 
{
	AuxStr *aux = NULL;
	int i=0, j;
	char palavra[30];
	
	while(frase[i] != '\0') 
	{
		if(frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-') 
		{
			for(j=0 ; frase[i] > 64 && frase[i] < 91 || frase[i] > 96 && frase[i] < 123 || frase[i] == '-' ; j++) 
			{
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
		else 
		{
			if(frase[i] == ' ') 
			{
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

void contaPalavras(AuxStr **aux, char frase[]) 
{
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

void qtdePalavra(char frase[], AuxStr **palavras) 
{
	AuxStr *auxS = *palavras;
	
	while(auxS != NULL) 
	{
		contaPalavras(&auxS, frase);
		auxS = auxS->prox;
	}
}

void exibeTabelaHuffman(struct auxString *list) 
{
    printf("\n             --- TABELA DE HUFFMAN ---\n");
    printf("%-10s %-20s %-12s %-20s\n", "Simbolo", "Palavra", "Frequencia", "Codigo");

    while(list != NULL) 
    {
        printf("%-10c %-20s %-12d %-20s\n", list->simbolo, list->palavra, list->qtde, list->codHuff);
        list = list->prox;
    }
    printf("\n\n");
}

int main(void) 
{
	AuxStr *list = NULL, *auxL, *alterado = NULL;
	ListTree *lTree = NULL, *auxTree;
	int TL = 0;
	char frase[TF], auxC[TF];
	strcpy(frase, "A tecnologia transforma o mundo ao conectar pessoas, ideias e oportunidades. Por meio de solucoes criativas, inteligentes e acessiveis, e possivel aproximar comunidades, otimizar processos e impulsionar o desenvolvimento.");
	printf("Texto usado para codificacao: %s\n\n", frase);

	//separando a frase em uma lista que vai ter cada palavra apenas 1 vez
	separaFrase(frase, &list);
	
	//contando quantas vezes a palavra se repete na frase
	qtdePalavra(frase, &list);
	
	auxL = list;
	while(auxL != NULL) 
	{
		criaLista(&lTree, &auxL);
		auxL = auxL->prox;
	}
	
	//cria a arvore huffman
	criaHuffman(&lTree);
	
	//cria a o codigo huffman para cada palavra
	adicionaCodHuff(lTree->arv, auxC, &TL, &list);
	
	//exibe a tabela com simbolo, palavra, frenquencia e codigo huffman
	exibeTabelaHuffman(list);
	
	//salvando as informações no arquivo binario - item B
	salvarArqBin(list);
	
	//exibe a arvore montada graficamente (da para melhorar)
	exibeh(lTree->arv,0);
	
	auxL = NULL;
	
	//separa a frase de maneira que repete as palavras e espaços
	separarTudo("A tecnologia transforma o mundo ao conectar pessoas, ideias e oportunidades. ", &auxL);
	
	//cria uma copia da list da frase original
	criarCopia(auxL, &alterado);
	
	//cria um arquivo.txt que guarda dentro dele a combinação de bits da frase acima
	comprimeFrase(alterado, list);
}
