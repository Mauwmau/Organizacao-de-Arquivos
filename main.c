#include <stdio.h>
#include <math.h>
#include "cabecalho.h"
#include "dados.h"
#include "paginas.h"

/*-------------------------------------------   UTILIDADE  -----------------------------------------------------------*/
long PosFimArquivo(FILE* arquivo){
    long atual = ftell(arquivo);
    fseek(arquivo,0,SEEK_END);
    long fim = ftell(arquivo);
    fseek(arquivo,atual,SEEK_SET);
    return fim;
}

void func3printCampos(CAB* cabeca, DADOS* registro, FILE* filebin3){
    for(int i=1; i<=5; i++){
        printf("%s : ", cabReturnCampo(cabeca,i));
        switch(i){
            case 1:
                dadosGetId(registro,filebin3);
                printf("%d\n", dadosReturnId(registro));
                break;

            case 2:
                dadosGetSalario(registro,filebin3);
                if(dadosReturnSalario(registro) >= 0) {
                    printf("%lf\n", dadosReturnSalario(registro));
                }else{
                    printf("valor nao declarado\n");
                }
                break;

            case 3:
                dadosGetTelefone(registro,filebin3);
                if(dadosReturnTelefone(registro)[0] != '\0') {
                    printf("%s\n", dadosReturnTelefone(registro));
                }else{
                    printf("valor nao declarado\n");
                }
                break;

            case 4:
                dadosGetNome(registro,filebin3);
                if(dadosReturnNome(registro) != NULL) {
                    printf("%s\n", dadosReturnNome(registro));
                }else{
                    printf("valor nao declarado\n");
                }
                break;

            case 5:
                dadosGetCargo(registro,filebin3);
                if(dadosReturnCargo(registro) != NULL) {
                    printf("%s\n", dadosReturnCargo(registro));
                }else{
                    printf("valor nao declarado\n");
                }
                break;

            default:
                printf("Deu erado ne meu");
                return;
        }
    }
    printf("\n");
}
/*--------------------------------------------------------------------------------------------------------------------*/

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

            long tamArq = PosFimArquivo(filecsv);

            long registroIniBinAnterior;

            while (ftell(filecsv) < tamArq) {
                long maxRange = TAMPAG * (getPagina(gerente)+1);
                long registroIniBin = ftell(filebin);
                long registroIniCsv = ftell(filecsv);
                long registroFim;

                DADOS* regDados = dadosCria();
                dadosReadAndWrite(regDados,filecsv,filebin);
                dadosApaga(regDados);

                registroFim = ftell(filebin);

                if(registroFim > maxRange) {

                    fseek(filebin, registroIniBin, SEEK_SET);
                    int qtdDeLixo = 0;
                    while (ftell(filebin) < maxRange) {
                        qtdDeLixo++;
                        char trash = '@';
                        fwrite(&trash, sizeof(char), 1, filebin);
                    }

                    registroFim = ftell(filebin);

                    fseek(filebin, registroIniBinAnterior, SEEK_SET);
                    dadosGetRemovido(regDados, filebin);
                    dadosGetTamReg(regDados,filebin);
                    int newTam = dadosReturnTamReg(regDados) + qtdDeLixo;
                    dadosSetTamReg(regDados, newTam);
                    fseek(filebin, -sizeof(int), SEEK_CUR);
                    dadosWriteTamReg(regDados, filebin);
                    fseek(filebin, registroFim, SEEK_SET);

                    fseek(filecsv, registroIniCsv, SEEK_SET);

                    addPagina(gerente);
                }

                registroIniBinAnterior = registroIniBin;

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
            FILE* filebin2 = fopen(filename,"rb");
            if(filebin2 == NULL){
                printf("Falha no carregamento do arquivo.\n");
                return -1;
            }

            //Vê a consistencia do arquivo
            char stats;
            fread(&stats, sizeof(char), 1, filebin2);
            if(stats == '0'){
                printf("Falha no carregamento do arquivo.\n");
                return -1;
            }
            // Relativo a consistencia



            //Pula para a primeira pagina de dados
            int pagAtual = 1;
            long posicaoPaginaUm = pagAtual * TAMPAG;

            fseek(filebin2,posicaoPaginaUm,SEEK_SET);
            // Relativo a ir para a primeira pagina de dados;

            // Encontra a ultima posição valida do arquivo binario e volta pra onde estava
            long posAtual = ftell(filebin2);
            fseek(filebin2,0,SEEK_END);
            long fimDoBin = ftell(filebin2);
            fseek(filebin2,posAtual,SEEK_SET);
            if(posAtual == fimDoBin){
                //Nao tem registros pra serem lidos
                printf("Registro inexistente.\n");
            }
            // Realativo a achar a ultima posicao do binario

            pagAtual++;
            while(ftell(filebin2) < fimDoBin) {

                int maxRange = (pagAtual) * TAMPAG;

                DADOS* registro = dadosCria();

                //Lê registro
                dadosGetRemovido(registro,filebin2);
                char removido = dadosReturnRemovido(registro);

                if (removido == '-') {
                    fseek(filebin2, sizeof(int), SEEK_CUR);
                    fseek(filebin2, sizeof(long), SEEK_CUR);

                    dadosGetId(registro,filebin2);
                    int id = dadosReturnId(registro);
                    printf("%d ", id);

                    dadosGetSalario(registro,filebin2);
                    double salario = dadosReturnSalario(registro);
                    if (salario < 0) {
                       printf("         ");
                    } else {
                        printf("%.2lf ", salario);
                    }

                    dadosGetTelefone(registro,filebin2);
                    char* telefone = dadosReturnTelefone(registro);
                    if (telefone[0] != '\0') {
                        fwrite(telefone, sizeof(char), 14, stdout);
                        printf(" ");
                    } else {
                        printf("               ");
                    }

                    dadosGetNome(registro,filebin2);
                    if(dadosReturnNome(registro) != NULL) {
                        printf("%d ", dadosReturnSizeNome(registro)-1);
                        fwrite(dadosReturnNome(registro), sizeof(char), dadosReturnSizeNome(registro), stdout);
                        printf(" ");
                    }

                    dadosGetCargo(registro,filebin2);
                    if(dadosReturnCargo(registro) != NULL){
                        printf("%d ", dadosReturnSizeCargo(registro)-1);
                        fwrite(dadosReturnCargo(registro), sizeof(char), dadosReturnSizeCargo(registro), stdout);
                        printf(" ");
                    }

                    printf("\n");

                }
                //Termina a leitura do registro

                if (ftell(filebin2) > maxRange){
                    pagAtual++;
                }

                dadosApaga(registro);

            }

            printf("Paginas Acessada: %d\n",(pagAtual));

            fclose(filebin2);

            break;

        case 3:
            /* busca registros por campo
             * mostra valores do registro linha por linha
             * inclui metadados
             * mostra o numero de paginas de disco acessadas*/

            scanf("%s",filename);

            char nomeDoCampo[40];
            char valor[80];

            scanf("%s ",nomeDoCampo);
            scanf("%[^\n]",valor);

            printf("%s\n", valor);

            //Verifica de arquivo pode ser aberto
            FILE* filebin3 = fopen(filename,"rb");
            if(filebin3 == NULL){
                printf("Falha no processamento do arquivo.\n");
                return -1;
            }

            //Vê a consistencia do arquivo
            fread(&stats, sizeof(char), 1, filebin3);
            if(stats == '0'){
                printf("Falha no carregamento do arquivo.\n");
                return -1;
            }

            //Vê se o campo digitado é valido
            int nCampo = 0;
            if(strcmp(nomeDoCampo,"idServidor") == 0) nCampo = 1;
            else if(strcmp(nomeDoCampo,"salarioServidor") == 0) nCampo = 2;
            else if(strcmp(nomeDoCampo,"telefoneServidor") == 0) nCampo = 3;
            else if(strcmp(nomeDoCampo,"nomeServidor") == 0) nCampo = 4;
            else if(strcmp(nomeDoCampo,"cargoServidor") == 0) nCampo = 5;
            else nCampo = -1;

            //Lê o cabecalho
            cabeca = criaCabecalho();
            for(int i=1; i<=5; i++){
                cabGetCampo(cabeca,filebin3,i);
            }

            //Vai para primeira pagina de dados
            fseek(filebin3, TAMPAG, SEEK_SET);

            long final = PosFimArquivo(filebin3);

            //Variavel para verificar se algo foi encontrado
            int hasFind = 0;

            //Percorre arquivo ate achar o final
            while(ftell(filebin3) < final){
                DADOS* registro = dadosCria();

                dadosGetRemovido(registro,filebin3);
                if('-' == dadosReturnRemovido(registro)){

                    long iniTam = ftell(filebin3);
                    dadosGetTamReg(registro,filebin3);

                    dadosGetEncadeamentoLista(registro,filebin3);

                    long iniCampos = ftell(filebin3);

                    switch(nCampo){
                        case 1:
                            dadosGetId(registro,filebin3);
                            int valorInt = atoi(valor);
                            if(valorInt == dadosReturnId(registro)){
                                fseek(filebin3, iniCampos, SEEK_SET);
                                func3printCampos(cabeca,registro,filebin3);
                                hasFind = 1;
                            }else{
                                /*
                                dadosGetSalario(registro,filebin3);
                                dadosGetTelefone(registro,filebin3);
                                dadosGetNome(registro,filebin3);
                                dadosGetCargo(registro,filebin3);
                                */
                               fseek(filebin3, iniTam, SEEK_SET);
                               dadosGetTamReg(registro,filebin3);
                               fseek(filebin3, dadosReturnTamReg(registro), SEEK_CUR);
                            }
                            break;

                        case 2:
                            dadosGetId(registro,filebin3);
                            dadosGetSalario(registro,filebin3);
                            double valorDouble = atof(valor);
                            if(valorDouble == dadosReturnSalario(registro)){
                                fseek(filebin3, iniCampos, SEEK_SET);
                                func3printCampos(cabeca,registro,filebin3);
                                hasFind = 1;
                            }else{
                                /*
                                dadosGetTelefone(registro,filebin3);
                                dadosGetNome(registro,filebin3);
                                dadosGetCargo(registro,filebin3);
                                */
                                fseek(filebin3, iniTam, SEEK_SET);
                                dadosGetTamReg(registro,filebin3);
                                fseek(filebin3, dadosReturnTamReg(registro), SEEK_CUR);
                            }
                            break;

                        case 3:
                            dadosGetId(registro,filebin3);
                            dadosGetSalario(registro,filebin3);
                            dadosGetTelefone(registro,filebin3);
                            if(strcmp(valor, dadosReturnTelefone(registro)) == 0){
                                fseek(filebin3, iniCampos, SEEK_SET);
                                func3printCampos(cabeca,registro,filebin3);
                                hasFind = 1;
                            }else{
                                /*
                                dadosGetNome(registro,filebin3);
                                dadosGetCargo(registro,filebin3);
                                */
                                fseek(filebin3, iniTam, SEEK_SET);
                                dadosGetTamReg(registro,filebin3);
                                fseek(filebin3, dadosReturnTamReg(registro), SEEK_CUR);
                            }
                            break;

                        case 4:
                            dadosGetId(registro,filebin3);
                            dadosGetSalario(registro,filebin3);
                            dadosGetTelefone(registro,filebin3);
                            dadosGetNome(registro,filebin3);

                            if(dadosReturnNome(registro) != NULL && strcmp(valor, dadosReturnNome(registro)) == 0){
                                fseek(filebin3, iniCampos, SEEK_SET);
                                func3printCampos(cabeca,registro,filebin3);
                                hasFind = 1;
                            }else{
                                /*dadosGetCargo(registro,filebin3);*/
                                fseek(filebin3, iniTam, SEEK_SET);
                                dadosGetTamReg(registro,filebin3);
                                fseek(filebin3, dadosReturnTamReg(registro), SEEK_CUR);
                            }
                            break;

                        case 5:
                            dadosGetId(registro,filebin3);
                            dadosGetSalario(registro,filebin3);
                            dadosGetTelefone(registro,filebin3);
                            dadosGetNome(registro,filebin3);
                            dadosGetCargo(registro,filebin3);

                            if(dadosReturnCargo(registro) != NULL && strcmp(valor, dadosReturnCargo(registro)) == 0){
                                fseek(filebin3, iniCampos, SEEK_SET);
                                func3printCampos(cabeca,registro,filebin3);
                                hasFind = 1;
                            }
                            break;

                        default:
                            printf("Campo Invalido! Existentem:\n");
                            for(int i=1; i<=5; i++){
                                printf("\t- %s\n", cabReturnCampo(cabeca,i));
                            }
                            return -4;
                    }

                }

                dadosApaga(registro);
            }

            apagaCabecalho(cabeca);

            if(hasFind == 0){
                printf("Registro Inexistente.\n");
            }

            break;

        default:
            opcao = 0;
    }

    return 0;
}