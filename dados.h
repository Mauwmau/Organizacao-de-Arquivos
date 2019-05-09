//
// Created by Maurilio on 26/04/2019.
//

#ifndef TRAB1_DADOS_H
#define TRAB1_DADOS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _dados DADOS;

DADOS* dadosCria();

void dadosApaga(DADOS* dados);


/*
 * READ     -    Lê do arquivo CSV
 * GET      -    Lê do arquivo binario
 * WRITE    -    Escreve no binario
 * SET      -    Permite Set-ups de valores no binario
 */


void dadosWriteRemovido(DADOS* dados, FILE* bin);
void dadosGetRemovido(DADOS* dados, FILE* bin);

void dadosWriteTamReg(DADOS* dados, FILE* bin);
void dadosGetTamReg(DADOS* dados, FILE* bin);
void dadosSetTamReg(DADOS* dados);

void dadosWriteEncadeamentoLista(DADOS* dados, FILE *bin);
void dadosGetEncadeamentoLista(DADOS* dados, FILE *bin);

void dadosReadId(DADOS* dados, FILE* csv);
void dadosWriteId(DADOS *dados, FILE* bin);

void dadosReadSalario(DADOS* dados, FILE* csv);
void dadosWriteSalario(DADOS *dados, FILE* bin);

void dadosReadTelefone(DADOS* dados, FILE* csv);
void dadosWriteTelefone(DADOS *dados, FILE* bin);

void dadosReadNome(DADOS* dados, FILE* csv);
void dadosWriteNome(DADOS *dados, FILE* bin);

void dadosReadCargo(DADOS* dados, FILE* csv);
void dadosWriteCargo(DADOS *dados, FILE* bin);

void dadosReadAndWrite(DADOS* dados, FILE* csv, FILE* bin);

#endif //TRAB1_DADOS_H
