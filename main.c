#include <stdio.h>
#include <math.h>
#include "cabecalho.h"
#include "dados.h"
#include "paginas.h"

int main() {

    int opcao = 0;
    char filename[30];

    scanf("%d", &opcao);

    switch(opcao){
        case 1:
            /* lê um csv
             * grava infos em arquivo binario
             * mostra "arquivo.bin" na tela*/

            scanf("%s",filename);

            FILE* filecsv = fopen(filename,"r+");
            if(filecsv == NULL){
                printf("Falha no carregamento do arquivo.\n");
                return -1;
            }

            FILE* filebin = fopen("arquivoTrab1.bin","wb+");
            if(filebin == NULL)return -1;


            GERENCIADOR* gerente = criaGerenciador();

            CAB* cabeca;
            cabeca = criaCabecalho();

            cabSetStatus(cabeca,'0',filebin);
            cabReadAndWrite(cabeca,filecsv,filebin);
            addPagina(gerente);

            int actual = ftell(filecsv);
            fseek(filecsv,0,SEEK_END);
            long tamArq = ftell(filecsv);
            fseek(filecsv,actual,SEEK_SET);

            while (ftell(filecsv) < tamArq) {
                long maxRange = TAMPAG * (getPagina(gerente)+1);
                long registroIniBin = ftell(filebin);
                long registroIniCsv = ftell(filecsv);
                long registroFim = ftell(filebin);


                registroIniBin = ftell(filebin);
                registroIniCsv = ftell(filecsv);

                DADOS* regDados = dadosCria();
                dadosReadAndWrite(regDados,filecsv,filebin);
                dadosApaga(regDados);

                registroFim = ftell(filebin);

                if(registroFim > maxRange) {

                    fseek(filebin, registroIniBin, SEEK_SET);
                    while (ftell(filebin) < maxRange) {
                        char trash = '@';
                        fwrite(&trash, sizeof(char), 1, filebin);
                    }

                    fseek(filecsv, registroIniCsv, SEEK_SET);

                    addPagina(gerente);
                }

            }

            cabSetStatus(cabeca,'1',filebin);

            apagaCabecalho(cabeca);

            apagaGerenciador(gerente);

            fclose(filebin);
            fclose(filecsv);

            printf("arquivoTrab1.bin\n");

            break;

        case 2:
            /* mostra registros na tela
             * tem tratamento de lixo e valores nulos
             * mostra o numero de paginas de disco acessadas*/

            scanf("%s",filename);
            FILE* fileBin = fopen(filename,"rb");
            if(fileBin == NULL){
                printf("Falha no carregamento do arquivo.\n");
                return -1;
            }

            //Vê a consistencia do arquivo
            char stats;
            fread(&stats, sizeof(char), 1, fileBin);
            if(stats == '0'){
                printf("Falha no carregamento do arquivo.\n");
                return -1;
            }
            // Relativo a consistencia



            //Pula para a primeira pagina de dados
            int pagAtual = 1;
            long posicaoPaginaUm = pagAtual * TAMPAG;

            fseek(fileBin,posicaoPaginaUm,SEEK_SET);
            // Relativo a ir para a primeira pagina de dados;

            // Encontra a ultima posição valida do arquivo binario e volta pra onde estava
            long posAtual = ftell(fileBin);
            fseek(fileBin,0,SEEK_END);
            long fimDoBin = ftell(fileBin);
            fseek(fileBin,posAtual,SEEK_SET);
            if(posAtual == fimDoBin){
                //Nao tem registros pra serem lidos
                printf("Registro inexistente.\n");
            }
            // Realativo a achar a ultima posicao do binario

            pagAtual++;
            while(ftell(fileBin) < fimDoBin) {

                int maxRange = (pagAtual) * TAMPAG;

                DADOS* registro = dadosCria();

                //Lê registro
                dadosGetRemovido(registro,fileBin);
                char removido = dadosReturnRemovido(registro);

                if (removido == '-') {
                    fseek(fileBin, sizeof(int), SEEK_CUR);
                    fseek(fileBin, sizeof(long), SEEK_CUR);

                    dadosGetId(registro,fileBin);
                    int id = dadosReturnId(registro);
                    printf("%d ", id);

                    dadosGetSalario(registro,fileBin);
                    double salario = dadosReturnSalario(registro);
                    if (salario < 0) {
                       printf("         ");
                    } else {
                        printf("%.2lf ", salario);
                    }

                    dadosGetTelefone(registro,fileBin);
                    char* telefone = dadosReturnTelefone(registro);
                    if (telefone[0] != '\0') {
                        fwrite(telefone, sizeof(char), 14, stdout);
                        printf(" ");
                    } else {
                        printf("               ");
                    }

                    //Vê se tem nome
                    fseek(fileBin, sizeof(int), SEEK_CUR);
                    char tag;
                    fread(&tag, sizeof(char), 1, fileBin);
                    if (tag == 'n') {
                        // Normal
                        fseek(fileBin, -sizeof(char), SEEK_CUR);
                        fseek(fileBin, -sizeof(int), SEEK_CUR);

                        dadosGetNome(registro,fileBin);
                        printf("%d ",dadosReturnSizeNome(registro));
                        fwrite(dadosReturnNome(registro), sizeof(char), dadosReturnSizeNome(registro), stdout);
                        printf(" ");

                        fseek(fileBin, sizeof(int), SEEK_CUR);
                        char tagC;
                        fread(&tagC, sizeof(char), 1, fileBin);

                        if (tagC == 'c') {
                            //Tem nome e tem cargo
                            fseek(fileBin, -sizeof(char), SEEK_CUR);
                            fseek(fileBin, -sizeof(int), SEEK_CUR);

                            dadosGetCargo(registro,fileBin);
                            printf("%d ",dadosReturnSizeCargo(registro));
                            fwrite(dadosReturnCargo(registro), sizeof(char), dadosReturnSizeCargo(registro), stdout);
                            printf(" ");

                        } else {
                            //Tem nome mas nao tem cargo
                            fseek(fileBin, -sizeof(char), SEEK_CUR);
                            fseek(fileBin, -sizeof(int), SEEK_CUR);
                        }

                    } else if (tag == 'c') {
                        // Não tem nome mas tem cargo
                        fseek(fileBin, -sizeof(char), SEEK_CUR);
                        fseek(fileBin, -sizeof(int), SEEK_CUR);

                        dadosGetCargo(registro,fileBin);
                        printf("%d ",dadosReturnSizeCargo(registro));
                        fwrite(dadosReturnCargo(registro), sizeof(char), dadosReturnSizeCargo(registro), stdout);
                        printf(" ");
                    } else {
                        // Não tem nome nem cargo
                        fseek(fileBin, -sizeof(char), SEEK_CUR);
                        fseek(fileBin, -sizeof(int), SEEK_CUR);
                    }

                    printf("\n");

                }
                //Termina a leitura do registro

                if (ftell(fileBin) > maxRange){
                    pagAtual++;
                }

                dadosApaga(registro);

            }

            printf("Paginas Acessada: %d\n",(pagAtual));

            fclose(fileBin);

            break;

        case 3:
            /* busca registros por campo
             * mostra valores do registro linha por linha
             * inclui metadados
             * mostra o numero de paginas de disco acessadas*/
            break;

        default:
            opcao = 0;
    }

    return 0;
}