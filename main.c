#include <stdio.h>
#include <math.h>
#include "cabecalho.h"
#include "dados.h"
#include "paginas.h"
#include "monitorfuncs.h"

/*-------------------------------------------   UTILIDADE  -----------------------------------------------------------*/
int verificaConsistencia(FILE* arquivo){
    long atual = ftell(arquivo);    //Guarda a posicao atual
    fseek(arquivo, 0, SEEK_SET);    //Vai pro inicio
    char stats;
    fread(&stats, sizeof(char), 1, arquivo);    //Faz leitura no arquivo
    fseek(arquivo, atual, SEEK_SET);    //Volta pra posicao atual
    if(stats == '0'){
        printf("Falha no processamento do arquivo.\n");
        return 0;
    }else{
        return 1;
    }
}

long PosFimArquivo(FILE* arquivo){
    long atual = ftell(arquivo);
    fseek(arquivo,0,SEEK_END);
    long fim = ftell(arquivo);
    fseek(arquivo,atual,SEEK_SET);
    return fim;
}

int campoSelector(char* campo){
    if(strcmp(campo,"idServidor") == 0) return 1;
    else if(strcmp(campo,"salarioServidor") == 0) return 2;
    else if(strcmp(campo,"telefoneServidor") == 0) return 3;
    else if(strcmp(campo,"nomeServidor") == 0) return 4;
    else if(strcmp(campo,"cargoServidor") == 0) return  5;
    else return -1;
}

void preencheBin(FILE* bin, int qtd){
    char trash = '@';
    fwrite(&trash, sizeof(char), qtd, bin);
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
                    printf("%.2lf\n", dadosReturnSalario(registro));
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

    int n;
    char nomeDoCampo[40];
    char valor[80];

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
                printf("Falha no processamento do arquivo.\n");
                return -1;
            }

            //Vê a consistencia do arquivo
            if(!verificaConsistencia(filebin2)){
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
            long fimDoBin = PosFimArquivo(filebin2);
            if(posAtual == fimDoBin){
                //Nao tem registros pra serem lidos
                printf("Registro inexistente.\n");
                return -2;
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

                } else if(removido == '*'){
                    dadosGetTamReg(registro,filebin2);
                    fseek(filebin2, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                }
                //Termina a leitura do registro

                if (ftell(filebin2) > maxRange){
                    pagAtual++;
                }

                dadosApaga(registro);

            }

            printf("Numero de paginas de disco acessadas: %d\n",(pagAtual));

            fclose(filebin2);

            break;

        case 3:
            /* busca registros por campo
             * mostra valores do registro linha por linha
             * inclui metadados
             * mostra o numero de paginas de disco acessadas*/

            scanf("%s",filename);

            scanf("%s ",nomeDoCampo);
            scanf("%[^\n]",valor);

            //printf("%s\n", valor);

            //Verifica de arquivo pode ser aberto
            FILE* filebin3 = fopen(filename,"rb");
            if(filebin3 == NULL){
                printf("Falha no processamento do arquivo.\n");
                return -1;
            }

            //Vê a consistencia do arquivo
            if(!verificaConsistencia(filebin3)){
                return -1;
            }

            //Vê se o campo digitado é valido e transforma em uma valor integer
            int nCampo = campoSelector(nomeDoCampo);

            //Lê o cabecalho
            cabeca = criaCabecalho();
            for(int i=1; i<=5; i++){
                cabGetCampo(cabeca,filebin3,i);
            }

            //Vai para primeira pagina de dados
            fseek(filebin3, TAMPAG, SEEK_SET);

            long final = PosFimArquivo(filebin3);

            //Gerente de paginas
            gerente = criaGerenciador();    //Lembrar de dar free no final
            addPagina(gerente); //Primeira pagina = cabecalho
            addPagina(gerente); //Pagina atual = 2
            int lastSeeninPage = 0;

            //Percorre arquivo ate achar o final
            while(ftell(filebin3) < final){
                long maxRange = getPagina(gerente) * TAMPAG;

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
                                lastSeeninPage = getPagina(gerente);
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
                                lastSeeninPage = getPagina(gerente);
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
                                lastSeeninPage = getPagina(gerente);
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
                                lastSeeninPage = getPagina(gerente);
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
                                lastSeeninPage = getPagina(gerente);
                            }else{
                                fseek(filebin3, iniTam, SEEK_SET);
                                dadosGetTamReg(registro,filebin3);
                                fseek(filebin3, dadosReturnTamReg(registro), SEEK_CUR);
                            }
                            break;

                        default:
                            printf("Campo Invalido! Existentem:\n");
                            for(int i=1; i<=5; i++){
                                printf("\t- %s\n", cabReturnCampo(cabeca,i));
                            }
                            return -4;
                    }

                }else if(dadosReturnRemovido(registro) == '*'){
                    dadosGetTamReg(registro,filebin3);
                    fseek(filebin3, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                }

                dadosApaga(registro);

                if(ftell(filebin3) >= maxRange){
                    addPagina(gerente);
                }

            }

            apagaCabecalho(cabeca);

            if(lastSeeninPage > 0){
                printf("\nNumero de paginas de disco acessadas: %d\n", getPagina(gerente));
            }else{
                printf("Registro Inexistente.\n");
            }

            break;

        case 4:

            scanf("%s %d", filename, &n);

            char** valores = (char**) malloc(sizeof(char*) * n);    //Aloca um vetor de n strings
            int* nCampos = (int*) malloc(sizeof(int) * n);          //Aloca um vetor de n integers

            for(int i = 0; i<n; i++){
                scanf("%s", nomeDoCampo);

                valores[i] = (char*)malloc(sizeof(char) * 80);      //Aloca a string, tamanho maximo: 80 caracteres
                scan_quote_string(valores[i]);

                nCampos[i] = campoSelector(nomeDoCampo);

            }

            FILE* filebin4 = fopen(filename,"rb+");
            if(filebin4 == NULL){
                printf("Falha no processamento do arquivo.\n");
                return -1;
            }

            if(!verificaConsistencia(filebin4)){
                return -1;
            }

            for (int i=0; i<n; i++) {

                //Pula pra segunda pagina
                fseek(filebin4, TAMPAG, SEEK_SET);

                final = PosFimArquivo(filebin4);

                while(ftell(filebin4) < final){
                    DADOS* registro = dadosCria();

                    long iniReg = ftell(filebin4);

                    dadosGetRemovido(registro,filebin4);
                    if(dadosReturnRemovido(registro) == '-'){
                        long iniTam = ftell(filebin4);
                        dadosGetTamReg(registro,filebin4);

                        dadosGetEncadeamentoLista(registro,filebin4);
                        long iniCampos = ftell(filebin4);

                        switch (nCampos[i]){
                            case 1:
                                int valorInt = atoi(valores[i]);
                                dadosGetId(registro,filebin4);
                                if(dadosReturnId(registro) == valorInt){
                                    //Achou id
                                    fseek(filebin4, iniReg, SEEK_SET);  //Vai pro inicio do registro

                                    char nullify = '*';
                                    fwrite(&nullify, sizeof(char), 1, filebin4);//Marca registro com indicador de remocao

                                    dadosGetTamReg(registro,filebin4);

                                    long posEncad = ftell(filebin4);
                                    insereLista(registro, posEncad, filebin4);

                                    dadosGetEncadeamentoLista(registro,filebin4);   //Pula o encadeamentoLista, lendo ele
                                    preencheBin(filebin4, dadosReturnTamReg(registro)-8);// Tamanho do registro - (tam)encadLista
                                    fseek(filebin4, posEncad, SEEK_SET);    //Volta pra posicao logo depois do campo tamanho

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }else{
                                    //Nao eh o id procurado
                                    fseek(filebin4, iniTam, SEEK_SET);  //Vai pro campo de tamanho do registro
                                    dadosGetTamReg(registro,filebin4);

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }
                                break;

                            case 2:
                                double valorDouble = atof(valores[i]);
                                dadosGetId(registro,filebin4);
                                dadosGetSalario(registro,filebin4);
                                if(dadosReturnSalario(registro) == valorDouble){
                                    //Achou Salario
                                    fseek(filebin4, iniReg, SEEK_SET);  //Vai pro inicio do registro

                                    char nullify = '*';
                                    fwrite(&nullify, sizeof(char), 1, filebin4);//Marca registro com indicador de remocao


                                    dadosGetTamReg(registro,filebin4);

                                    long posEncad = ftell(filebin4);
                                    insereLista(registro, posEncad, filebin4);

                                    dadosGetEncadeamentoLista(registro,filebin4);   //Pula o encadeamentoLista, lendo ele
                                    preencheBin(filebin4, dadosReturnTamReg(registro)-8);
                                    fseek(filebin4, posEncad, SEEK_SET);    //Volta pra posicao logo depois do campo tamanho

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }else{
                                    //Nao eh o salario procurado
                                    fseek(filebin4, iniTam, SEEK_SET);  //Vai pro campo de tamanho do registro
                                    dadosGetTamReg(registro,filebin4);

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }
                                break;

                            case 3:
                                dadosGetId(registro,filebin4);
                                dadosGetSalario(registro,filebin4);
                                dadosGetTelefone(registro,filebin4);
                                if(strcmp(dadosReturnTelefone(registro),valores[i]) == 0){
                                    //Achou telefone
                                    fseek(filebin4, iniReg, SEEK_SET); //Vai pro inicio do registro

                                    char nullify = '*';
                                    fwrite(&nullify, sizeof(char), 1, filebin4);//Marca registro com indicador de remocao

                                    dadosGetTamReg(registro,filebin4);

                                    long posEncad = ftell(filebin4);
                                    insereLista(registro, posEncad, filebin4);

                                    dadosGetEncadeamentoLista(registro,filebin4);   //Pula o encadeamentoLista, lendo ele
                                    preencheBin(filebin4, dadosReturnTamReg(registro)-8);
                                    fseek(filebin4, posEncad, SEEK_SET);    //Volta pra posicao logo depois do campo tamanho

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }else{
                                    //Nao eh o telefone procurado
                                    fseek(filebin4, iniTam, SEEK_SET);  //Vai pro campo de tamanho do registro
                                    dadosGetTamReg(registro,filebin4);

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }
                                break;

                            case 4:
                                dadosGetId(registro,filebin4);
                                dadosGetSalario(registro,filebin4);
                                dadosGetTelefone(registro,filebin4);
                                dadosGetNome(registro,filebin4);
                                if((dadosReturnNome(registro)!=NULL && strcmp(dadosReturnNome(registro),valores[i]) == 0)
                                    ||(strcmp(valores[i],"") == 0 && dadosReturnNome(registro) == NULL)){
                                    //Achou nome
                                    fseek(filebin4, iniReg, SEEK_SET); //Vai pro inicio do registro

                                    char nullify = '*';
                                    fwrite(&nullify, sizeof(char), 1, filebin4);//Marca registro com indicador de remocao

                                    dadosGetTamReg(registro,filebin4);

                                    long posEncad = ftell(filebin4);
                                    insereLista(registro, posEncad, filebin4);

                                    dadosGetEncadeamentoLista(registro,filebin4);   //Pula o encadeamentoLista, lendo ele
                                    preencheBin(filebin4, dadosReturnTamReg(registro)-8);
                                    fseek(filebin4, posEncad, SEEK_SET);    //Volta pra posicao logo depois do campo tamanho

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }else{
                                    //Nao eh o nome procurado
                                    fseek(filebin4, iniTam, SEEK_SET);  //Vai pro campo de tamanho do registro
                                    dadosGetTamReg(registro,filebin4);

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }
                                break;

                            case 5:
                                dadosGetId(registro,filebin4);
                                dadosGetSalario(registro,filebin4);
                                dadosGetTelefone(registro,filebin4);
                                dadosGetNome(registro,filebin4);
                                dadosGetCargo(registro,filebin4);
                                if((dadosReturnCargo(registro)!=NULL && strcmp(dadosReturnCargo(registro),valores[i]) == 0)
                                    || (strcmp(valores[i],"") == 0 && dadosReturnCargo(registro))){
                                    //Achou cargo
                                    fseek(filebin4, iniReg, SEEK_SET); //Vai pro inicio do registro

                                    char nullify = '*';
                                    fwrite(&nullify, sizeof(char), 1, filebin4);//Marca registro com indicador de remocao

                                    dadosGetTamReg(registro,filebin4);

                                    long posEncad = ftell(filebin4);
                                    insereLista(registro, posEncad, filebin4);

                                    dadosGetEncadeamentoLista(registro,filebin4);   //Pula o encadeamentoLista, lendo ele
                                    preencheBin(filebin4, dadosReturnTamReg(registro)-8);
                                    fseek(filebin4, posEncad, SEEK_SET);    //Volta pra posicao logo depois do campo tamanho

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }else{
                                    //Nao eh o cargo procurado
                                    fseek(filebin4, iniTam, SEEK_SET);  //Vai pro campo de tamanho do registro
                                    dadosGetTamReg(registro,filebin4);

                                    fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                                }
                                break;

                            default: printf("Campo digitado eh invalido!\n");
                        }
                    }else if(dadosReturnRemovido(registro) == '*'){
                        dadosGetTamReg(registro,filebin4);
                        fseek(filebin4, dadosReturnTamReg(registro), SEEK_CUR); //Pula pro fim do registro
                    }else{
                        printf("SOMETHING WRONG RIGHT HERE!\nRemovido=\"%c\"\n",dadosReturnRemovido(registro));
                    }

                    dadosApaga(registro);
                }
            }

            binarioNaTela1(filebin4);

            //Be free
            for(int i=0; i<n; i++){
                free(valores[i]);
            }
            free(valores);
            free(nCampos);

            fclose(filebin4);


            break;

        case 5:

            scanf("%s", filename);

            scanf(" %d", &n);

            // Alocar vetores de tamanho n para os campos
            int* ids = (int*)malloc(n * sizeof(int));
            double* salarios = (double*)malloc(n * sizeof(double));
            char** telefones = (char**)malloc(n * sizeof(char*));
            char** nomes = (char**)malloc(n * sizeof(char*));
            char** cargos = (char**)malloc(n * sizeof(char*));

            for (int i = 0; i < n; ++i) {
                scanf("%s", valor);
                ids[i] = atoi(valor);

                scanf("%s", valor);
                if(strcmp(valor,"NULO") == 0){
                    salarios[i] = -1;
                }else{
                    salarios[i] = atof(valor);
                }

                telefones[i] = (char*)malloc(15 * sizeof(char));
                scanf("%s", valor);
                if(strcmp(valor,"NULO") == 0){
                    strcpy(telefones[i], "\0@@@@@@@@@@@@@");
                }else{
                    strcpy(telefones[i], valor);
                }

                nomes[i] = (char*)malloc(80 * sizeof(char));
                scan_quote_string(nomes[i]);

                cargos[i] = (char*)malloc(80 * sizeof(char));
                scan_quote_string(cargos[i]);

            }

            FILE* filebin5 = fopen(filename,"wb+");
            if(filebin5 == NULL){
                printf("Falha no processamento do arquivo.\n");
                return -1;
            }

            if(!verificaConsistencia(filebin5)){
                return -1;
            }

            for(int i = 0; i<n; i++){
               DADOS* newRegistro = dadosCria();

               dadosSetRemovido(newRegistro, '-');
               dadosSetEncadeamentoLista(newRegistro, -1);
               dadosSetId(newRegistro, ids[i]);
               dadosSetSalario(newRegistro, salarios[i]);
               dadosSetTelefone(newRegistro, telefones[i]);
               dadosSetNome(newRegistro, nomes[i]);
               dadosSetCargo(newRegistro, cargos[i]);
               dadosUpdateTamReg(newRegistro);

               removeLista(dadosReturnTamReg(newRegistro), filebin5); //Ja viu onde eh melhor inserir

               //Agora pode escrever o registro.
               dadosWriteRemovido(newRegistro, filebin5);
               dadosWriteTamReg(newRegistro, filebin5);
               dadosWriteEncadeamentoLista(newRegistro, filebin5);
               dadosWriteId(newRegistro, filebin5);
               dadosWriteSalario(newRegistro, filebin5);
               dadosWriteTelefone(newRegistro, filebin5);
               dadosWriteNome(newRegistro, filebin5);
               dadosWriteCargo(newRegistro, filebin5);

               dadosApaga(newRegistro);
            }

            //Free fest
            free(ids);
            free(salarios);
            for(int i=0; i<n; i++){
                free(telefones[i]);
                free(nomes[i]);
                free(cargos[i]);
            }
            free(telefones);
            free(nomes);
            free(cargos);
            break;

        default:
            opcao = 0;
    }

    return 0;
}