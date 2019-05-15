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
char dadosReturnRemovido(DADOS* dados);

void dadosWriteTamReg(DADOS* dados, FILE* bin);
void dadosGetTamReg(DADOS* dados, FILE* bin);
void dadosSetTamReg(DADOS* dados);
int dadosReturnTamReg(DADOS* dados);

void dadosWriteEncadeamentoLista(DADOS* dados, FILE *bin);
void dadosGetEncadeamentoLista(DADOS* dados, FILE *bin);
long dadosReturnEncadeamentoLista(DADOS* dados);

void dadosReadId(DADOS* dados, FILE* csv);
void dadosGetId(DADOS* dados, FILE* bin);
void dadosWriteId(DADOS *dados, FILE* bin);
int dadosReturnId(DADOS* dados);

void dadosReadSalario(DADOS* dados, FILE* csv);
void dadosGetSalario(DADOS* dados, FILE* bin);
void dadosWriteSalario(DADOS *dados, FILE* bin);
double dadosReturnSalario(DADOS* dados);

void dadosReadTelefone(DADOS* dados, FILE* csv);
void dadosGetTelefone(DADOS* dados, FILE* bin);
void dadosWriteTelefone(DADOS *dados, FILE* bin);
char* dadosReturnTelefone(DADOS* dados);

void dadosReadNome(DADOS* dados, FILE* csv);
void dadosGetNome(DADOS* dados, FILE* bin);
void dadosWriteNome(DADOS *dados, FILE* bin);
char* dadosReturnNome(DADOS* dados);
int dadosReturnSizeNome(DADOS* dados);

void dadosReadCargo(DADOS* dados, FILE* csv);
void dadosGetCargo(DADOS* dados, FILE* bin);
void dadosWriteCargo(DADOS *dados, FILE* bin);
char* dadosReturnCargo(DADOS* dados);
int dadosReturnSizeCargo(DADOS* dados);

void dadosReadAndWrite(DADOS* dados, FILE* csv, FILE* bin);

#endif //TRAB1_DADOS_H
