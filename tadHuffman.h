struct tree 
{
	char simbolo;
	int qtde;
	struct tree *dir, *esq;
};
typedef struct tree Tree;

struct listTree 
{
	struct tree *arv;
	struct listTree *prox;
};
typedef struct listTree ListTree;

struct auxString 
{
	char palavra[30], codHuff[30], simbolo;
	int qtde;
	struct auxString *prox;
};
typedef struct auxString AuxStr;

char criaSimbolo() 
{
	return rand() % 26 + 65;
}

char existeSimbolo(ListTree *L, char simb) 
{
	while(L != NULL && L->arv->simbolo != simb)
		L = L->prox;
		
	if(L != NULL && L->arv->simbolo == simb)
		return 1;
	return 0;
}

void ordenaLista(ListTree **L, ListTree **novo) 
{
	ListTree *ant=NULL, *aux=*L;
	
	while(aux != NULL && (*novo)->arv->qtde > aux->arv->qtde) 
	{
		ant = aux;
		aux = aux->prox;
	}
	
	if(ant == NULL) 
	{
		(*novo)->prox = *L;
		*L = *novo;
	}
	else 
	{
		if(aux == NULL)
			ant->prox = *novo;
		else 
		{
			ant->prox = *novo;
			(*novo)->prox = aux;
		}
	}
}

void criaLista(ListTree **listI, AuxStr **auxS) 
{
	ListTree *novoList = (ListTree*)malloc(sizeof(ListTree));
	Tree *novoTree = (Tree*)malloc(sizeof(Tree));
	
	novoTree->qtde = (*auxS)->qtde;
	novoTree->esq = novoTree->dir = NULL;
	novoTree->simbolo = criaSimbolo();
	
	while(existeSimbolo(*listI, novoTree->simbolo))
		novoTree->simbolo = criaSimbolo();
	
	novoList->arv = novoTree;
	novoList->prox = NULL;
	(*auxS)->simbolo = novoTree->simbolo;

	if(*listI == NULL)
		*listI = novoList;
	else
		ordenaLista(&*listI, &novoList);
}

void criaNovaCaixaL(Tree **pri, Tree **seg, ListTree **novo) {
	Tree *noTree = (Tree*)malloc(sizeof(Tree));
	
	*novo = (ListTree*)malloc(sizeof(ListTree));
	
	noTree->qtde = (*pri)->qtde + (*seg)->qtde;
	noTree->simbolo = '#';
	
	if((*pri)->qtde > (*seg)->qtde) {
		noTree->esq = *seg;
		noTree->dir = *pri;
	}
	else {
		noTree->dir = *seg;
		noTree->esq = *pri;
	}
	
	(*novo)->arv = noTree;
	(*novo)->prox = NULL;
	
}

void criaHuffman(ListTree **listI) 
{
    ListTree *novo;
    
    while((*listI) && (*listI)->prox) 
	{
        ListTree *list1 = *listI;
        ListTree *list2 = list1->prox;

        criaNovaCaixaL(&list1->arv, &list2->arv, &novo);

        *listI = list2->prox;

        free(list1);
        free(list2);

        ordenaLista(listI, &novo);
    }
}

void exibeh(tree *raiz)
{
    static int n = -1;
    if (raiz != NULL)
    {
        n++;
        exibeh(raiz->dir); 

        for (int i = 0; i < 5 * n; i++)
            printf(" ");

        printf("(%d, %d)\n", raiz->simbolo, raiz->qtde);

        exibeh(raiz->esq);

        n--;
    }
}

void adicionaCodHuff(Tree *raiz, char cod[1000], int *TL, AuxStr **list) 
{
	AuxStr *aux;
	
	if(raiz != NULL) 
	{
		if(raiz->esq != NULL) 
		{
			cod[*TL] = '0';
			*TL = *TL + 1;
			cod[*TL] = '\0';
		}
		else
			*TL = *TL + 1;
			
		adicionaCodHuff(raiz->esq, cod, &*TL, &*list);
		*TL = *TL - 1;
		if(raiz->dir != NULL) 
		{
			cod[*TL] = '1';
			*TL = *TL + 1;
			cod[*TL] = '\0';
		}
		else
			*TL = *TL + 1;
		
		adicionaCodHuff(raiz->dir, cod, &*TL, &*list);
		*TL = *TL - 1;
		aux = *list;
		while(aux != NULL && aux->simbolo != raiz->simbolo)
			aux = aux->prox;
		
		if(aux != NULL)
			strcpy(aux->codHuff, cod);
	}
}

void salvarArqBin(AuxStr *list) 
{
	AuxStr Reg;
	FILE *ptrArq = fopen("tabela.dat", "wb");
	
	while(list!=NULL) 
	{
		strcpy(Reg.palavra, list->palavra);
		strcpy(Reg.codHuff, list->codHuff);
		Reg.qtde = list->qtde;
		Reg.simbolo = list->simbolo;
		Reg.prox = NULL;
		
		fwrite(&Reg,sizeof(AuxStr),1,ptrArq);
		
		list = list->prox;
	}
	fclose(ptrArq);
}

void comprimeFrase(AuxStr *aux, AuxStr *map) 
{
	FILE *ptrArq = fopen("frase-comprimida.txt", "w+");
	char cod[250];
	int TL=0, i;
	AuxStr *mapAux = map;
	
	while(aux != NULL) 
	{
		while(mapAux != NULL && strcmp(aux->palavra, mapAux->palavra) != 0)
			mapAux = mapAux->prox;
		
		if(mapAux != NULL) 
		{
			for(i=0 ; mapAux->codHuff[i] != '\0' ; i++, TL++)
				cod[TL] = mapAux->codHuff[i];
		}
		mapAux = map;
		aux = aux->prox;
	}
	printf("\n%s", cod);
	fprintf(ptrArq, "%s\n", cod);
}

Tree* criaNoTree(char simbolo) 
{
	Tree *novo = (Tree*)malloc(sizeof(Tree));
	novo->simbolo = simbolo;
	novo->esq = novo->dir = NULL;
	novo->qtde = 0;
	
	return novo;
}

void recuperaArv(Tree **raiz, AuxStr *list) {
	Tree *aux;
	int i;
	
	*raiz = criaNoTree('$');
	aux = *raiz;
	
	while(list != NULL) 
	{
		for(i=0 ; list->codHuff[i] != '\0' ; i++) 
		{
			if(list->codHuff[i] == '0') 
			{
				if((*raiz)->esq != NULL) 
				{
					if(list->codHuff[i+1] == '\0')
						(*raiz)->esq->simbolo = list->simbolo;
					else
						*raiz = (*raiz)->esq;
				}
				else 
				{
					if(list->codHuff[i+1] != '\0') 
					{
						(*raiz)->esq = criaNoTree('$');
						*raiz = aux;
						i = -1;
					}
					else
						(*raiz)->esq = criaNoTree(list->simbolo);
				}
			}
			else {
				if(list->codHuff[i] == '1') 
				{
					if((*raiz)->dir != NULL) 
					{
						if(list->codHuff[i+1] == '\0')
							(*raiz)->dir->simbolo = list->simbolo;
						else
							*raiz = (*raiz)->dir;
					}
					else 
					{
						if(list->codHuff[i+1] != '\0') 
						{
							(*raiz)->dir = criaNoTree('$');
							*raiz = aux;
							i = -1;
						}
						else
							(*raiz)->dir = criaNoTree(list->simbolo);
					}
				}
			}
		}
		*raiz = aux;
		list = list->prox;
	}
}

void exibeSimbolo(AuxStr *aux, char simb) 
{
	while(aux != NULL && aux->simbolo != simb)
		aux = aux->prox;
	
	if(aux != NULL)
		printf("%s",aux->palavra);
}

void exibeFrase(FILE *ptr, Tree *arv, AuxStr *tab) {
	Tree *auxTree = arv;
	AuxStr *a;
	char bit, frase[300];
	int i=0, TL=0, j=0;
	
	bit = getc(ptr);
	
	while(!feof(ptr) && bit != '\0') {
		while(!feof(ptr) && auxTree->dir != NULL && auxTree->esq != NULL) {
			if(bit == '0')
				auxTree = auxTree->esq;
			else {
				if(bit == '1')
					auxTree = auxTree->dir;
			}
			bit = getc(ptr);
		}
		a = tab;
		while(a != NULL && a->simbolo != auxTree->simbolo)
			a = a->prox;
		
		if(a != NULL) {
			for(j=0 ; a->palavra[j] != '\0' ; j++, TL++)
				frase[TL] = a->palavra[j];
			frase[TL] = 32;
			TL++;
		}
		auxTree = arv;
		if(!feof(ptr))
			bit = getc(ptr);
	}
	frase[TL] = '\0';
	printf("\n%s", frase);
	fclose(ptr);
}

void exibeArv(Tree *a) {
    int i;
    static int n = -1;

    if (a != NULL) {
        n++;
        exibeArv(a->dir);
        for (i = 0; i < 5 * n; i++) {
            printf(" ");
        }
        printf(" (%c)\n", a->simbolo);
        exibeArv(a->esq);
        n--;
    }
}
