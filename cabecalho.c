//
// Created by Maurilio on 24/04/2019.
//

#include "cabecalho.h"

/*-------------------------------------------   UTILIDADE  -----------------------------------------------------------*/
/*Completa uma string(char* string) de tamanho fixo(int size) com determinado caracter(char what)*/
void completeWith(char* string, char what, int size){
    int i = strlen(string);
    string[i] = '\0';
    for(i+=1;i<size;i++){
        string[i] = what;
    }
}
/*--------------------------------------------------------------------------------------------------------------------*/

/*##########################################    DEFINICAO     ########################################################*/
struct _cabecalho{
    char status;
    long topoLista;

    char tagCampo1;
    char desCampo1[40];

    char tagCampo2;
    char desCampo2[40];

    char tagCampo3;
    char desCampo3[40];

    char tagCampo4;
    char desCampo4[40];

    char tagCampo5;
    char desCampo5[40];

};
/*####################################################################################################################*/

/* CRIAR E APAGAR UM CABECALHO |======================================================================================*/
CAB* criaCabecalho(){
    CAB* cabecalho;
    cabecalho = (CAB*)malloc(sizeof(CAB));

    if(cabecalho == NULL)return NULL;

    cabecalho->status = '0';
    cabecalho->topoLista = -1;

    cabecalho->tagCampo1 = 'i';
    strcpy(cabecalho->desCampo1,"\0");

    cabecalho->tagCampo2 = 's';
    strcpy(cabecalho->desCampo2,"\0");

    cabecalho->tagCampo3 = 't';
    strcpy(cabecalho->desCampo3,"\0");

    cabecalho->tagCampo4 = 'n';
    strcpy(cabecalho->desCampo4,"\0");

    cabecalho->tagCampo5 = 'c';
    strcpy(cabecalho->desCampo5,"\0");

    return cabecalho;
}

void apagaCabecalho(CAB* cabecalho){
    if(cabecalho != NULL){
        free(cabecalho);
    }
}
/*====================================================================================================================*/


/*LÊ E ESCREVE UM CABECALHO (não usar, pois deve ser feito campo a campo) |===========================================*/
void getCabecalho(CAB* cabecalho,FILE* csv){
    if(csv == NULL){
        printf("Problema ao abrir arquivo.\n");
        return;
    }
    if(cabecalho == NULL){
        printf("Problema com cabecalho.\n");
        return;
    }

    char buffer[40];

    fscanf(csv,"%[^,\n\r]",buffer);
    strcpy(cabecalho->desCampo1,buffer);
    fseek(csv,1,SEEK_CUR);

    fscanf(csv,"%[^,\n\r]",buffer);
    strcpy(cabecalho->desCampo2,buffer);
    fseek(csv,1,SEEK_CUR);

    fscanf(csv,"%[^,\n\r]",buffer);
    strcpy(cabecalho->desCampo3,buffer);
    fseek(csv,1,SEEK_CUR);

    fscanf(csv,"%[^,\n\r]",buffer);
    strcpy(cabecalho->desCampo4,buffer);
    fseek(csv,1,SEEK_CUR);

    fscanf(csv,"%[^,\n\r]",buffer);
    strcpy(cabecalho->desCampo5,buffer);
    fseek(csv,1,SEEK_CUR);

    printf("%s\n",cabecalho->desCampo1);
    printf("%s\n",cabecalho->desCampo2);
    printf("%s\n",cabecalho->desCampo3);
    printf("%s\n",cabecalho->desCampo4);
    printf("%s\n",cabecalho->desCampo5);
}

void writeCabecalho(CAB* cabecalho,FILE* bin){

    fwrite(&cabecalho->status,sizeof(char),1,bin);
    fwrite(&cabecalho->topoLista,sizeof(long),1,bin);

    fwrite(&cabecalho->tagCampo1, sizeof(char),1,bin);
    fwrite(cabecalho->desCampo1, sizeof(char),40,bin);

    fwrite(&cabecalho->tagCampo2, sizeof(char),1,bin);
    fwrite(cabecalho->desCampo2, sizeof(char),40,bin);

    fwrite(&cabecalho->tagCampo3, sizeof(char),1,bin);
    fwrite(cabecalho->desCampo3, sizeof(char),40,bin);

    fwrite(&cabecalho->tagCampo4, sizeof(char),1,bin);
    fwrite(cabecalho->desCampo4, sizeof(char),40,bin);

    fwrite(&cabecalho->tagCampo5, sizeof(char),1,bin);
    fwrite(cabecalho->desCampo5, sizeof(char),40,bin);

    char trash = '@';
    while(ftell(bin) < 32000){
        fwrite(&trash, sizeof(char),1,bin);
    }
}
/*====================================================================================================================*/



/* REFERENTE AO STATUS |==============================================================================================*/
void cabSetStatus(CAB *cabecalho, char s, FILE *bin) {
    cabecalho->status = s;
    int temp = ftell(bin);

    fseek(bin,0,SEEK_SET);
    cabWriteStatus(cabecalho,bin);
    fseek(bin,temp,SEEK_SET);
}

void cabGetStatus(CAB* cabecalho, FILE* bin){
    fseek(bin,0,SEEK_SET);
    fread(&cabecalho->status, sizeof(char), 1, bin);
}

void cabWriteStatus(CAB *cabecalho, FILE *bin) {
    fwrite(&cabecalho->status, sizeof(char), 1, bin);
}
/*====================================================================================================================*/



/* REFENTE AO TOPO LISTA |============================================================================================*/
void cabGetTopoLista(CAB* cabecalho, FILE* bin){
    fread(&cabecalho->topoLista, sizeof(long), 1, bin);
}

void cabWriteTopoLista(CAB *cabecalho, FILE *bin) {
    fwrite(&cabecalho->topoLista,sizeof(long),1,bin);
}
/*====================================================================================================================*/



/* REFERENTE AOS CAMPOS 1-5 |=========================================================================================*/
void cabReadCampo(CAB *cabecalho, FILE *csv, int qual) {
    if (qual >= 1 && qual <= 5) {
        char buffer[40];
        fscanf(csv,"%[^,\n\r]",buffer);

        switch(qual){
            case 1:
                strcpy(cabecalho->desCampo1,buffer);
                completeWith(cabecalho->desCampo1,'@',sizeof(cabecalho->desCampo1));
                break;

            case 2:
                strcpy(cabecalho->desCampo2,buffer);
                completeWith(cabecalho->desCampo2,'@',sizeof(cabecalho->desCampo2));
                break;

            case 3:
                strcpy(cabecalho->desCampo3,buffer);
                completeWith(cabecalho->desCampo3,'@',sizeof(cabecalho->desCampo3));
                break;

            case 4:
                strcpy(cabecalho->desCampo4,buffer);
                completeWith(cabecalho->desCampo4,'@',sizeof(cabecalho->desCampo4));
                break;

            case 5:
                strcpy(cabecalho->desCampo5,buffer);
                completeWith(cabecalho->desCampo5,'@',sizeof(cabecalho->desCampo5));
                break;

            default:
                printf("Erro ao ler campo %d\n", qual);
        }
    }else{
        printf("Existem apenas os campos 1-5 \n");
    }
}

void cabGetCampo(CAB* cabecalho, FILE* bin, int qual){
    if(qual >= 1 && qual <= 5){

        char campo[40];

        fseek(bin, sizeof(char) + sizeof(long),SEEK_SET);   // Vai pro comeco do arquivo e pula o status e topoLista
        long campoOffset = (qual-1) * (41 * sizeof(char));  // Define o Offset
        fseek(bin, campoOffset, SEEK_CUR);                  // Vai pro campo desejado
        fseek(bin, sizeof(char), SEEK_CUR);                 // Pula a tag

        switch (qual){
            case 1:
                fread(cabecalho->desCampo1, sizeof(char), 40, bin);
                break;

            case 2:
                fread(cabecalho->desCampo2, sizeof(char), 40, bin);
                break;

            case 3:
                fread(cabecalho->desCampo3, sizeof(char), 40, bin);
                break;

            case 4:
                fread(cabecalho->desCampo4, sizeof(char), 40, bin);
                break;

            case 5:
                fread(cabecalho->desCampo5, sizeof(char), 40, bin);
                break;

            default:
                printf("Erro ao ler campo %d\n", qual);
        }

    }
}

void cabWriteCampo(CAB *cabecalho, FILE *bin, int qual) {
    if(qual >= 1 && qual <=5) {

        switch (qual) {
            case 1:
                fwrite(&cabecalho->tagCampo1, sizeof(char), 1, bin);
                fwrite(cabecalho->desCampo1, sizeof(char), 40, bin);
                break;

            case 2:
                fwrite(&cabecalho->tagCampo2, sizeof(char), 1, bin);
                fwrite(cabecalho->desCampo2, sizeof(char), 40, bin);
                break;

            case 3:
                fwrite(&cabecalho->tagCampo3, sizeof(char), 1, bin);
                fwrite(cabecalho->desCampo3, sizeof(char), 40, bin);
                break;

            case 4:
                fwrite(&cabecalho->tagCampo4, sizeof(char), 1, bin);
                fwrite(cabecalho->desCampo4, sizeof(char), 40, bin);
                break;

            case 5:
                fwrite(&cabecalho->tagCampo5, sizeof(char), 1, bin);
                fwrite(cabecalho->desCampo5, sizeof(char), 40, bin);
                break;

            default:
                printf("Erro ao escrever campo %d\n", qual);
        }
    }else{
        printf("Existem apenas os campos 1-5 \n");
    }
}

char* cabReturnCampo(CAB* cabecalho, int qual){
    switch(qual){
        case 1:
            return cabecalho->desCampo1;

        case 2:
            return cabecalho->desCampo2;

        case 3:
            return cabecalho->desCampo3;

        case 4:
            return cabecalho->desCampo4;

        case 5:
            return cabecalho->desCampo5;

        default:
            printf("Erro ao ler campo %d\n", qual);
    }
    return NULL;
}
/*====================================================================================================================*/


void cabReadAndWrite(CAB *cabecalho, FILE *csv, FILE *bin) {

    cabWriteStatus(cabecalho,bin);
    cabWriteTopoLista(cabecalho,bin);

    int i;
    for(i = 1; i<=5; i++){
        cabReadCampo(cabecalho,csv,i);
        fseek(csv,1,SEEK_CUR);
        cabWriteCampo(cabecalho,bin,i);
    }
    while(ftell(bin) < 32000){
        char trash = '@';
        fwrite(&trash, sizeof(char), 1, bin);
    }
}