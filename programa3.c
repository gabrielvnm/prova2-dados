#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <ctype.h>

#define LINHAS_ARQUIVO 2000
#define NOME_SENSOR 16

// tem que olhar ainda:
// se tiver mto afim faz um arquivo ai pra colocar as funcoes

void sairComQualquerTecla();
time_t gerar_timestamp_aleatorio_();
void limparTela();
time_t capturar_timestamp_valido();
int validarComando();
int validarNomeSensor();
int validarInputData();

void sairComQualquerTecla(){
    printf("Aperte qualquer tecla para continuar: \n");
    while (1){
        if (_kbhit()){
            char tecla = getch();
            break;
        }
    }
}

//função de validar input de data do usuario
int interpretarComando(char * argv[]){
    
    for (int i = 1; i< 12; i++){
        int num = isdigit(argv[i][0]);
        if (num == 0){
            printf("argumento %s nao e um numero!", argv[i]);
            return -1;
        }        
    }
    return 0;
}
//funcao pra validar se as datas sao validas
int validarInputData(int dia, int mes, int ano, int hora, int min, int seg, int dia2, int mes2, int ano2, int hora2, int min2, int seg2){
    if (dia > 31 || dia <1 || dia2 > 31 || dia2 <1){
        printf("Data invalida! Tente novamente\n");
        return -1;
    }
    else if (mes > 12 || mes <1 || mes2 > 12 || mes2 <1){
        printf("Data invalida! Tente novamente\n");
        return -1;
    }
    else if (ano > 2025 || ano <1900 || ano2 > 2025 || ano2 <1900){
        printf("Data invalida! Tente novamente\n");
        return -1;
    }
    else if (hora > 23 || hora<0 || hora2> 23 || hora2<0){
        printf("Data invalida! Tente novamente\n");
        return -1;
    }
    else if (min > 59 || min <0 || min2 > 59 || min2 <0){
        printf("Data invalida! Tente novamente\n");
        return -1;
    }
    else if (seg > 59 || seg <0 || seg2 > 59 || seg2 <0){
        printf("Data invalida! Tente novamente\n");
        return -1;
    }
    else{
        return 0;
    }
}

//função pra verificar se algum nome de sensor excede o tamanho maximo
int validarNomeSensor(int argc, char * argv[]){

    for (int i = 13; i< argc; i+=2 ){
        if (strlen(argv[i])> NOME_SENSOR){
            printf("Argumento %d excede o tamanho maximo de caracteres!\n",i);
            return -1;
        }
    }

    return 0;
}

void limparTela(){
    #ifdef _WIN32 
        system("cls");
    #else
        system("clear");
    #endif
}

time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) 
{
    struct tm t;

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;

    time_t timestamp = mktime(&t);
    if (timestamp == -1) {
        printf("Data inválida. Tente novamente.\n");
        return -1;
    } else {
        return timestamp;
    }
}

//alterei a função pq nao tava dando certo do jeito la do AVA
time_t gerar_timestamp_aleatorio(int dia, int mes, int ano, int hora, int min, int seg, int dia2, int mes2, int ano2, int hora2, int min2, int seg2) {
    
    time_t timestamp_inicial, timestamp_final;
    
    struct tm t;
    struct tm t2;

    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;
    t2.tm_year = ano2 - 1900;
    t2.tm_mon = mes2 - 1;
    t2.tm_mday = dia2;
    t2.tm_hour = hora2;
    t2.tm_min = min2;
    t2.tm_sec = seg2;
    t2.tm_isdst = -1;
        
    timestamp_inicial = mktime(&t);
    if (timestamp_inicial == -1) {
        printf("Data inválida.\n");
        return -1;
    }

    timestamp_final = mktime(&t2);
    if (timestamp_final == -1) {
        printf("Data inválida.\n");
        return -1;
    }

    time_t timestamp_aleatorio = timestamp_inicial + (((double)rand() / RAND_MAX) * (timestamp_final - timestamp_inicial));//timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
    
    return timestamp_aleatorio;
} 

int main(int argc, char * argv[]){ 

    //argumentos: 
    //argv[1] ate argv [6] = (dia mes ano hora min seg) inicial
    //argv[7] ate argv [12] = (dia mes ano hora min seg) final
    //argv[13] em diante(numeros impares) nome do sensor pra gerar no arquivo txt
    //argv[14] em diante(numeros pares) = tipo de dado do sensor

    //validacao de input
    if (argc < 15 || (argc%2)!=1){
        printf("Erro na linha de comando 1! Digite uma data de inicio dd mm aaaa hh mm ss,\numa data final dd mm aaaa hh mm ss\ne o nome do sensores seguidos do tipo de dado!\n");
        return -1;
    }
    if (interpretarComando(argv)!=0){
        printf("Erro na linha de comando 2! Datas informadas nao sao numeros!\n");
        return -1;
    }
    if (validarNomeSensor(argc, argv)!=0){
        printf("Erro na linha de comando 3! Nome do sensor excede o limite de 16 caracteres!\n");
        return -1;
    }

    int dia, mes, ano, hora, min, seg, dia2, mes2, ano2, hora2, min2, seg2; // variaveis pra passar pras funções de timestamp
    char *tipo_dado[] = {"CONJ_Z", "CONJ_Q", "TEXTO", "BINARIO"}; // lista de inputs validos, INT FLOAT STRING BOOL
    int tipo_dado_[argc-12]; // array pra guardar qual dos tipos de dado vem do input
    char *random_string[] = {"Baixo", "Medio", "Alto"}; // lista de outputs para tipo de dado TEXTO
    float a = 5.0; // range do valor do float
    
    limparTela();
    dia = atoi(argv[1]);
    mes = atoi(argv[2]);
    ano = atoi(argv[3]);
    hora = atoi(argv[4]);
    min = atoi(argv[5]);
    seg = atoi(argv[6]);
    dia2 = atoi(argv[7]);
    mes2 = atoi(argv[8]);
    ano2 = atoi(argv[9]);
    hora2 = atoi(argv[10]);
    min2 = atoi(argv[11]);
    seg2 = atoi(argv[12]);

    //validaçoes de input de data
    if (validarInputData(dia, mes, ano, hora, min, seg, dia2, mes2, ano2, hora2, min2, seg2)!=0){
        printf("Erro na linha de comando 4! Datas invalidas informadas!\n");
        return -1;
    }
    time_t inicio = converter_para_timestamp(dia, mes, ano, hora, min, seg);
    time_t fim = converter_para_timestamp(dia2, mes2, ano2, hora2, min2, seg2);
    if (inicio > fim){
        printf("Range de datas invalido! Tente novamente\n");
        return -1;
    }
    printf("timestamp inicio %ld\n",inicio);
    printf("timestamp fim %ld\n",fim);
    
    //validacao do tipo de dado do sensor
    int k = 0;
    for (int i = 14 ; i < argc ; i+=2){
        tipo_dado_[k] = -1 ; // inicializa a variavel em -1 pra validar depois
        for (int j = 0 ; j<4; j++){    
            if (strcmp(argv[i], tipo_dado[j]) == 0){
                tipo_dado_[k] = j;
            }
        }
        if (tipo_dado_[k] == -1){
            printf("Erro na linha de comando 5! Informe um valor valido para o tipo de dado do sensor: CONJ_Z, CONJ_Q, TEXTO, BINARIO\n");
            return -1;
        }
        k++;
    }

    srand((unsigned int)time(NULL));

    FILE *arquivo = fopen("sensores_random.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return -1;
    }

    int numero_sensores = (argc-13)/2;
    int valorint;
    float valorfloat;

    for (int j =0; j < LINHAS_ARQUIVO-1; j++){    
        
        time_t timestamp_usuario = gerar_timestamp_aleatorio(dia, mes, ano, hora, min, seg, dia2, mes2, ano2, hora2, min2, seg2);
        int nome_aleatorio = rand()%numero_sensores;
        int posicao_argv = nome_aleatorio*2 + 13; // kkkk eu fiz uma equação pra chegar nessa conta aqui fala serio
        int posicao_tipo_dado = (posicao_argv-13)/2;
        fprintf(arquivo,"%s;",argv[posicao_argv]);
        switch(tipo_dado_[posicao_tipo_dado]){
            default:
                printf("Erro gravando arquivo, linha %d!\n",j);
                fclose(arquivo);
                return -1;
            case 0:
                valorint= rand()%100;
                fprintf(arquivo, "%d;",valorint);
                break;
            case 1:
                valorfloat= ((float)rand()/(float)(RAND_MAX)) * a;
                fprintf(arquivo,"%.2f;",valorfloat);
                break;
            case 2:
                valorint= rand()%3;
                fprintf(arquivo,"%s;",random_string[valorint]);
                break;
            case 3:                
                valorint = rand()%2;
                fprintf(arquivo,"%d;",valorint);
                break;
        }
        fprintf(arquivo,"%ld;\n",timestamp_usuario);
    }
    
    time_t timestamp_usuario = gerar_timestamp_aleatorio(dia, mes, ano, hora, min, seg, dia2, mes2, ano2, hora2, min2, seg2);
    int nome_aleatorio = rand()%numero_sensores;
    int posicao_argv = nome_aleatorio*2 + 13; 
    int posicao_tipo_dado = (posicao_argv-13)/2;
    fprintf(arquivo,"%s;",argv[posicao_argv]);
    switch(tipo_dado_[posicao_tipo_dado]){
        default:
            printf("Erro gravando arquivo, linha final!\n");
            fclose(arquivo);
            return -1;
        case 0:
            valorint = rand()%100;
            fprintf(arquivo, "%d;",valorint);
            break;
        case 1:
            valorfloat = ((float)rand()/(float)(RAND_MAX)) * a;
            fprintf(arquivo,"%.2f;",valorfloat);
            break;
        case 2:
            valorint = rand()%3;
            fprintf(arquivo,"%s;",random_string[valorint]);
            break;
        case 3:
            valorint = rand()%2;
            fprintf(arquivo,"%d;",valorint);
    }
    fprintf(arquivo,"%ld;",timestamp_usuario);
    printf("Sucesso! Arquivo criado: sensores_random.txt\n");
    fclose(arquivo);

    return 0;  

}