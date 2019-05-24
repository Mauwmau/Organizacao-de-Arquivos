//
// Created by Maurilio on 24/04/2019.
//

#ifndef TRAB1_CABECALHO_H
#define TRAB1_CABECALHO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _cabecalho CAB;

CAB* criaCabecalho();
void apagaCabecalho(CAB*);

void getCabecalho(CAB*,FILE* csv);

void writeCabecalho(CAB*,FILE* bin);


void cabSetStatus(CAB*, char s, FILE* bin);
void cabGetStatus(CAB*, FILE* bin);
void cabWriteStatus(CAB*, FILE* bin);


void cabGetTopoLista(CAB*, FILE* bin);
void cabSetTopoLista(CAB*, long top);
void cabWriteTopoLista(CAB*, FILE* bin);
long cabReturnTopoLista(CAB*);


void cabReadCampo(CAB*,FILE* csv, int qual);
void cabGetCampo(CAB*, FILE* bin, int qual);
void cabWriteCampo(CAB*, FILE* bin, int qual);
char* cabReturnCampo(CAB*, int qual);

void cabReadAndWrite(CAB*, FILE* csv, FILE* bin);

#endif //TRAB1_CABECALHO_H
