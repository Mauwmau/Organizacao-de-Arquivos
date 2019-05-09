//
// Created by Maurilio on 01/05/2019.
//

#include "paginas.h"
#include <stdio.h>
#include <stdlib.h>

struct _paginas{
    int qtdPaginas;
    int tamPagina;
};

GERENCIADOR* criaGerenciador(){
    GERENCIADOR* g;
    g = (GERENCIADOR*)malloc(sizeof(GERENCIADOR));
    if(g == NULL)return NULL;
    g-> qtdPaginas = 0;
    g-> tamPagina = TAMPAG;
    return g;
}

void apagaGerenciador(GERENCIADOR* g){
    if(g != NULL){
        free(g);
    }
}

int getPagina(GERENCIADOR* g){
    return g->qtdPaginas;
}

void addPagina(GERENCIADOR* g){
    g->qtdPaginas++;
}