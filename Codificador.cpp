#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio2.h>
#include <ctype.h>

#include "Arquivo.h"
#include "Arvore.h"
#include "Pilha.h"
#include "PilhaHuffman.h"
//#include "listaCelula.h"
//#include "listaArvore.h"

#define TF 2000

struct bits
{
	unsigned char b7:1;
	unsigned char b6:1;
	unsigned char b5:1;
	unsigned char b4:1;
	unsigned char b3:1;
	unsigned char b2:1;
	unsigned char b1:1;
	unsigned char b0:1;
};

union byte
{
	struct bits bi;
	unsigned char num;
};


void exibeHuffman(tree *raiz)
{
    static int n = -1;
    if(raiz != NULL)
    {
        n++;
        exibeHuffman(raiz->dir); 

        for (int i = 0; i < 5 * n; i++)
            printf(" ");

        printf("(%d, %d)\n", raiz->simbolo, raiz->freq);

        exibeHuffman(raiz->esq);

        n--;
    }
}
