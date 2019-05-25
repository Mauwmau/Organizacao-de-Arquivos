//
// Created by Maurilio on 26/04/2019.
//

#include "dados.h"
#include "cabecalho.h"

/*-------------------------------------------   UTILIDADE  -----------------------------------------------------------*/
/*Completa uma string(char* string) de tamanho fixo(int size) com determinado caracter(char what)*/
void dadosCompleteWith(char* string, char what, int size){
    int i = 0;
    string[i] = '\0';
    for(i+=1;i<size;i++){
        string[i] = what;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*##########################################    DEFINICAO     ########################################################*/
struct _dados{
    char removido;
    int tamanhoRegistro;
    long encadeamentoLista;

    int idServidor;

    double salarioServidor;

    char telefoneServidor[15];

    int tamanhoNome;
    char tagNome;
    char* nomeServidor;

    int tamanhoCargo;
    char tagCargo;
    char* cargoServidor;
};
/*####################################################################################################################*/

/*CRIAR E APAGAR UM REGISTRO DE DADOS |===============================================================================*/
DADOS* dadosCria(){
    DADOS* d;
    d = (DADOS*)malloc(sizeof(DADOS));
    if(d == NULL) return NULL;

    d->removido = '-';
    d->tamanhoRegistro = 34;
    d->encadeamentoLista = -1;

    d->tamanhoNome = 0;
    d->tagNome = 'n';
    d->tagCargo = 'c';

    d->tamanhoCargo = 0;
    d->nomeServidor = NULL;
    d->cargoServidor = NULL;

    return d;

}

void dadosApaga(DADOS* dados){
    if(dados == NULL){
        printf("Erro ao apagar o registro de Dados.");
        return;
    }

    if(dados->nomeServidor != NULL){
        free(dados->nomeServidor);
        dados->nomeServidor = NULL;
    }
    if(dados->cargoServidor != NULL){
        free(dados->cargoServidor);
        dados->cargoServidor = NULL;
    }

    free(dados);
}
/*====================================================================================================================*/



/* REMOVIDO |=========================================================================================================*/
void dadosWriteRemovido(DADOS *dados, FILE *bin) {
    fwrite(&dados->removido, sizeof(char), 1, bin);
}

void dadosGetRemovido(DADOS* dados, FILE* bin){
    fread(&dados->removido, sizeof(char), 1, bin);
}

void dadosSetRemovido(DADOS* dados, char rem){
    dados->removido = rem;
}

char dadosReturnRemovido(DADOS* dados){
    return dados->removido;
}
/*====================================================================================================================*/



/* TAMANHO DO REGISTRO |==============================================================================================*/
void dadosWriteTamReg(DADOS *dados, FILE *bin) {
    fwrite(&dados->tamanhoRegistro, sizeof(int), 1, bin);
}

void dadosGetTamReg(DADOS* dados, FILE* bin){
    fread(&dados->tamanhoRegistro, sizeof(int), 1, bin);
}

void dadosSetTamReg(DADOS* dados, int tam){
    dados->tamanhoRegistro = tam;
}

void dadosUpdateTamReg(DADOS *dados) {
    if(dados->nomeServidor != NULL){
        dados->tamanhoRegistro += 4/*Tamanho do campo tamanhoNome=4bytes*/ +  dados->tamanhoNome;
    }
    if(dados->cargoServidor != NULL){
        dados->tamanhoRegistro+= 4/*Tamanho do campo tamanhoCargo=4bytes*/ + dados->tamanhoCargo;
    }
}

int dadosReturnTamReg(DADOS* dados){
    return dados->tamanhoRegistro;
}
/*====================================================================================================================*/



/* ENCADEAMENTO LISTA |===============================================================================================*/
void dadosWriteEncadeamentoLista(DADOS *dados, FILE *bin) {
    fwrite(&dados->encadeamentoLista, sizeof(long), 1, bin);
}

void dadosGetEncadeamentoLista(DADOS* dados, FILE *bin){
    fread(&dados->encadeamentoLista, sizeof(long), 1, bin);
}

void dadosSetEncadeamentoLista(DADOS* dados, long encad){
    dados->encadeamentoLista = encad;
}

long dadosReturnEncadeamentoLista(DADOS* dados){
    return dados->encadeamentoLista;
}

void insereLista(DADOS* dados, long posDados, FILE* bin){
    long atual;
    long proximo;
    long anterior = -1;
    int tamanhodoreg = -1;

    fseek(bin, sizeof(char), SEEK_SET);

    atual = ftell(bin);
    fread(&proximo, sizeof(long), 1, bin);

    while(proximo != -1 && tamanhodoreg < dadosReturnTamReg(dados)){
        fseek(bin , proximo, SEEK_SET);
        fseek(bin, -sizeof(int), SEEK_CUR);
        fread(&tamanhodoreg, sizeof(int), 1, bin);
        anterior = atual;
        atual = ftell(bin);
        fread(&proximo, sizeof(long), 1, bin);
    }

    if(tamanhodoreg != -1 && tamanhodoreg >= dadosReturnTamReg(dados)){
        fseek(bin , anterior, SEEK_SET);
        fwrite(&posDados, sizeof(long), 1, bin);

        dados->encadeamentoLista = atual;
    }else{
        fseek(bin, atual, SEEK_SET);
        fwrite(&posDados, sizeof(long), 1, bin);

        dados->encadeamentoLista = proximo;
    }

    fseek(bin ,posDados, SEEK_SET);
    dadosWriteEncadeamentoLista(dados, bin);

    fseek(bin, posDados, SEEK_SET);
}

long removeLista(int tamDados, FILE *bin){
    long anterior = -1;
    long atual;
    long proximo;
    int tamanhodoreg = -1;

    fseek(bin, sizeof(char), SEEK_SET); //Vai pro topoLista

    atual = ftell(bin);
    fread(&proximo, sizeof(long), 1, bin);

    while(proximo != -1 && tamanhodoreg < tamDados){
        fseek(bin , proximo, SEEK_SET);
        fseek(bin, -sizeof(int), SEEK_CUR);
        fread(&tamanhodoreg, sizeof(int), 1, bin);
        anterior = atual;
        atual = ftell(bin);
        fread(&proximo, sizeof(long), 1, bin);
    }

    if(tamanhodoreg != -1 && tamanhodoreg >= tamDados){
        fseek(bin, anterior, SEEK_SET);
        fwrite(&proximo, sizeof(long), 1, bin);
        fseek(bin, atual, SEEK_SET);

        //Vai pro comecinho do registro
        fseek(bin, -sizeof(int), SEEK_CUR);
        fseek(bin, -sizeof(char), SEEK_CUR);
        return atual;
    }else{
        fseek(bin, 0, SEEK_END);
        return ftell(bin);
    }

}
/*====================================================================================================================*/



/* ID SERVIDOR |======================================================================================================*/
void dadosReadId(DADOS *dados, FILE *csv) {
    fscanf(csv,"%d",&dados->idServidor);
}

void dadosGetId(DADOS* dados, FILE* bin){
    fread(&dados->idServidor, sizeof(int), 1, bin);
}

void dadosSetId(DADOS* dados, int id){
    dados->idServidor = id;
}

void dadosWriteId(DADOS *dados, FILE *bin) {
    fwrite(&dados->idServidor, sizeof(int), 1, bin);
}

int dadosReturnId(DADOS* dados){
    return dados->idServidor;
}
/*====================================================================================================================*/



/* SALARIO SERVIDOR |=================================================================================================*/
void dadosReadSalario(DADOS *dados, FILE *csv) {
    fscanf(csv,"%lf",&dados->salarioServidor);
}

void dadosGetSalario(DADOS* dados, FILE* bin){
    fread(&dados->salarioServidor, sizeof(double), 1, bin);
}

void dadosSetSalario(DADOS* dados, double sal){
    dados->salarioServidor = sal;
}

void dadosWriteSalario(DADOS *dados, FILE *bin) {
    fwrite(&dados->salarioServidor, sizeof(double), 1, bin);
}

double dadosReturnSalario(DADOS* dados){
    return dados->salarioServidor;
}
/*====================================================================================================================*/



/* TELEFONE SERVIDOR |================================================================================================*/
void dadosReadTelefone(DADOS *dados, FILE *csv) {
    char buffer[15];
    if (fscanf(csv,"%[^,\n\r]",buffer)) {
        strcpy(dados->telefoneServidor,buffer);
    }else {
        dadosCompleteWith(dados->telefoneServidor, '@', 14);
    }
}

void dadosGetTelefone(DADOS* dados, FILE* bin){
    fread(dados->telefoneServidor, sizeof(char), 14, bin);
    dados->telefoneServidor[14] = '\0'; //Para fins de compracao de string, necessario adicionar \0
}

void dadosSetTelefone(DADOS* dados, char* tel){
    strcpy(dados->telefoneServidor, tel);
}

void dadosWriteTelefone(DADOS *dados, FILE *bin) {
    fwrite(&dados->telefoneServidor, sizeof(char), 14, bin);
}

char* dadosReturnTelefone(DADOS* dados){
    return dados->telefoneServidor;
}
/*====================================================================================================================*/



/* NOME SERVIDOR |====================================================================================================*/
void dadosReadNome(DADOS *dados, FILE *csv) {
    char buffer[80];
    if (fscanf(csv,"%[^,\n\r]",buffer)) {
        int tamNome = strlen(buffer);
        if (tamNome > 0) {
            dados->nomeServidor = (char *) malloc((tamNome+1) * sizeof(char));
            strcpy(dados->nomeServidor, buffer);

            int i = strlen(dados->nomeServidor);
            dados->nomeServidor[i] = '\0';
            dados->tamanhoNome = tamNome+ 1/*\0*/ +1/*tagNome*/ ;
        }
    }
}

void dadosGetNome(DADOS* dados, FILE* bin){

    fread(&dados->tamanhoNome, sizeof(int), 1, bin);

    char tag;
    fread(&tag, sizeof(char),1,bin);

    if(tag == dados->tagNome) {
        if(dados->nomeServidor == NULL){
        dados->nomeServidor = (char *) malloc((dados->tamanhoNome-1) * sizeof(char));
        }
        fread(dados->nomeServidor, sizeof(char), dados->tamanhoNome-1, bin);
    }else{
        dados->tamanhoNome = 0;
        fseek(bin, -sizeof(char), SEEK_CUR);
        fseek(bin, -sizeof(int), SEEK_CUR);
    }
}

void dadosSetNome(DADOS* dados, char* nome){
    if(strcmp(nome, "") == 0){
        return;
    }

    int nomesize = strlen(nome);

    if(dados->nomeServidor == NULL){
        dados->nomeServidor = (char*)malloc((nomesize + 1) * sizeof(char));
    }

    strcpy(dados->nomeServidor, nome);

    dados->tamanhoNome = nomesize + 1 + 1;   //Quantidade de caracteres, inclui \0
                                                        //Exemplo: {o,i,\0} -> tamanho:2, quantidade de 'elementos':3
                                                        // + tagNome
}

void dadosWriteNome(DADOS *dados, FILE *bin) {
    if (dados->nomeServidor != NULL) {
        fwrite(&dados->tamanhoNome, sizeof(int), 1, bin);
        fwrite(&dados->tagNome, sizeof(char), 1,bin);
        fwrite(dados->nomeServidor, sizeof(char), dados->tamanhoNome-1, bin);
    }
}

char* dadosReturnNome(DADOS* dados){
    return dados->nomeServidor;
}

int dadosReturnSizeNome(DADOS* dados){
    return dados->tamanhoNome-1;
}
/*====================================================================================================================*/



/* CARGO SERVIDOR |===================================================================================================*/
void dadosReadCargo(DADOS *dados, FILE *csv) {
    char buffer[70];
    if (fscanf(csv,"%[^,\n\r]",buffer)) {
        int tam = strlen(buffer);
        if (tam > 0) {
            dados->cargoServidor = (char *) malloc((tam+1) * sizeof(char));
            strcpy(dados->cargoServidor, buffer);
            int i = strlen(dados->cargoServidor);
            dados->cargoServidor[i] = '\0';
            dados->tamanhoCargo = tam+1+1;
        }
    }
}

void dadosGetCargo(DADOS* dados, FILE* bin){

    fread(&dados->tamanhoCargo, sizeof(int), 1, bin);

    char tag;
    fread(&tag, sizeof(char), 1, bin);

    if (tag == dados->tagCargo) {
        if(dados->cargoServidor == NULL){
        dados->cargoServidor = (char*) malloc( dados->tamanhoCargo-1 * sizeof(char));
        }
        fread(dados->cargoServidor, sizeof(char), dados->tamanhoCargo-1, bin);
    }else{
        dados->tamanhoCargo = 0;
        fseek(bin, -sizeof(char), SEEK_CUR);
        fseek(bin, -sizeof(int), SEEK_CUR);
    }
}

void dadosSetCargo(DADOS* dados, char* cargo){
    if(strcmp(cargo, "") == 0){
        return;
    }

    int sizecargo = strlen(cargo);

    if(dados->cargoServidor == NULL){
        dados->cargoServidor = (char*) malloc((sizecargo+1) * sizeof(char));
    }

    strcpy(dados->cargoServidor, cargo);

    dados->tamanhoCargo = sizecargo + 1 + 1;    //Quantidade de caracteres, inclui \0
                                                //Exemplo: {o,i,\0} -> tamanho:2, quantidade de 'elementos':3
                                                // + tagCampo
}

void dadosWriteCargo(DADOS *dados, FILE *bin) {
    if (dados->cargoServidor != NULL) {
        fwrite(&dados->tamanhoCargo, sizeof(int), 1, bin);
        fwrite(&dados->tagCargo, sizeof(char), 1,bin);
        fwrite(dados->cargoServidor, sizeof(char), dados->tamanhoCargo-1, bin);
    }
}

char* dadosReturnCargo(DADOS* dados){
    return dados->cargoServidor;
}

int dadosReturnSizeCargo(DADOS* dados){
    return dados->tamanhoCargo-1;
}
/*====================================================================================================================*/

void dadosReadAndWrite(DADOS *dados, FILE *csv, FILE *bin) {

    dadosWriteRemovido(dados,bin);
    //printf("Removido: %c \n",dados->removido);

    int aux = ftell(bin);
    fseek(bin, sizeof(int),SEEK_CUR);

    dadosWriteEncadeamentoLista(dados,bin);
    //printf("Encadeamento Lista: %ld\n",dados->encadeamentoLista);

    dadosReadId(dados,csv);
    fseek(csv,1,SEEK_CUR);
    dadosWriteId(dados,bin);
    //printf("ID: %d\n",dados->idServidor);

    dadosReadSalario(dados,csv);
    fseek(csv,1,SEEK_CUR);
    dadosWriteSalario(dados,bin);
    //printf("Salario: %lf\n",dados->salarioServidor);

    dadosReadTelefone(dados,csv);
    fseek(csv,1,SEEK_CUR);
    dadosWriteTelefone(dados,bin);
    //printf("Telefone: %s\n",dados->telefoneServidor);

    dadosReadNome(dados,csv);
    fseek(csv,1,SEEK_CUR);
    dadosWriteNome(dados,bin);
    //printf("Nome: [%d](%c)%s \n",dados->tamanhoNome,dados->tagNome,dados->nomeServidor);

    dadosReadCargo(dados,csv);
    fseek(csv,1,SEEK_CUR);
    dadosWriteCargo(dados,bin);
    //printf("Cargo: [%d](%c)%s \n",dados->tamanhoCargo,dados->tagCargo,dados->cargoServidor);

    dadosUpdateTamReg(dados);

    long posFimRegistro = ftell(bin);
    fseek(bin,aux,SEEK_SET);
    dadosWriteTamReg(dados,bin);
    fseek(bin,posFimRegistro,SEEK_SET);

    //printf("Tamanho do Registro: %d \n",dados->tamanhoRegistro);

}