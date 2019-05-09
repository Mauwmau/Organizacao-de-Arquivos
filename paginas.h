//
// Created by Maurilio on 01/05/2019.
//

#ifndef TRAB1_PAGINAS_H
#define TRAB1_PAGINAS_H

#define TAMPAG 32000

typedef struct _paginas GERENCIADOR;

GERENCIADOR* criaGerenciador();
void apagaGerenciador(GERENCIADOR* g);

int getPagina(GERENCIADOR* g);
void addPagina(GERENCIADOR* g);

#endif //TRAB1_PAGINAS_H
