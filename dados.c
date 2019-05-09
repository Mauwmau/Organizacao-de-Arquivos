//
// Created by Maurilio on 26/04/2019.
//

#include "dados.h"

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

    char telefoneServidor[14];

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
    d->tamanhoRegistro = 44;
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
/*====================================================================================================================*/



/* TAMANHO DO REGISTRO |==============================================================================================*/
void dadosWriteTamReg(DADOS *dados, FILE *bin) {
    fwrite(&dados->tamanhoRegistro, sizeof(int), 1, bin);
}

void dadosSetTamReg(DADOS *dados) {
    dados->tamanhoRegistro += (dados->tamanhoNome) + (dados->tamanhoCargo);
}
/*====================================================================================================================*/



/* ENCADEAMENTO LISTA |===============================================================================================*/
void dadosWriteEncadeamentoLista(DADOS *dados, FILE *bin) {
    fwrite(&dados->encadeamentoLista, sizeof(long), 1, bin);
}
/*====================================================================================================================*/



/* ID SERVIDOR |======================================================================================================*/
void dadosReadId(DADOS *dados, FILE *csv) {
    fscanf(csv,"%d",&dados->idServidor);
    printf("ID: %d\n",dados->idServidor);
}

void dadosWriteId(DADOS *dados, FILE *bin) {
    fwrite(&dados->idServidor, sizeof(int), 1, bin);
}
/*====================================================================================================================*/



/* SALARIO SERVIDOR |=================================================================================================*/
void dadosReadSalario(DADOS *dados, FILE *csv) {
    fscanf(csv,"%lf",&dados->salarioServidor);
    printf("Salario: %lf\n",dados->salarioServidor);
}

void dadosWriteSalario(DADOS *dados, FILE *bin) {
    fwrite(&dados->salarioServidor, sizeof(double), 1 , bin);
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
    printf("Telefone: %s \n",dados->telefoneServidor);
}

void dadosWriteTelefone(DADOS *dados, FILE *bin) {
    fwrite(&dados->telefoneServidor, sizeof(char), 14, bin);
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

            dados->tamanhoNome = tamNome+1;
        }
    }
    if(dados->nomeServidor != NULL)printf("Nome: %s \n",dados->nomeServidor);
    else printf("Nome eh null\n");
}

void dadosWriteNome(DADOS *dados, FILE *bin) {
    if (dados->nomeServidor != NULL) {
        fwrite(&dados->tamanhoNome, sizeof(int), 1, bin);
        fwrite(&dados->tagNome, sizeof(char), 1,bin);
        fwrite(dados->nomeServidor, sizeof(char), dados->tamanhoNome, bin);
    }
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
            dados->tamanhoCargo = tam+1;
        }
    }
    if(dados->cargoServidor != NULL)printf("Cargo: %s \n",dados->cargoServidor);
    else printf("Cargo eh null\n");
}

void dadosWriteCargo(DADOS *dados, FILE *bin) {
    if (dados->cargoServidor != NULL) {
        fwrite(&dados->tamanhoCargo, sizeof(int), 1, bin);
        fwrite(&dados->tagCargo, sizeof(char), 1,bin);
        fwrite(dados->cargoServidor, sizeof(char), dados->tamanhoCargo, bin);
    }
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

    dadosSetTamReg(dados);

    long posFimRegistro = ftell(bin);
    fseek(bin,aux,SEEK_SET);
    dadosWriteTamReg(dados,bin);
    fseek(bin,posFimRegistro,SEEK_SET);

    //printf("Tamanho do Registro: %d \n",dados->tamanhoRegistro);

}