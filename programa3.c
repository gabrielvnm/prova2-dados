#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <string.h>
#include <conio.h>
#include <stdbool.h>


void sairComQualquerTecla();
time_t gerar_timestamp_aleatorio(int dia, int mes, int ano);
void limparTela();

void limparTela(){
    #ifdef _WIN32 
        system("cls");
    #else
        system("clear");
    #endif
}
//funçao que adiciona uma hora min seg aleatorio na data informada pelo usuario
time_t gerar_timestamp_aleatorio(int dia, int mes, int ano) {
    struct tm t;
    time_t timestamp_inicial, timestamp_final;
    
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    
    timestamp_inicial = mktime(&t);
    if (timestamp_inicial == -1) {
        printf("Data invalida.\n");
        return -1;
    }

    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;
    
    timestamp_final = mktime(&t);
    if (timestamp_final == -1) {
        printf("Data inválida.\n");
        return -1;
    }

    time_t timestamp_aleatorio = timestamp_inicial + rand() % (timestamp_final - timestamp_inicial + 1);
    
    return timestamp_aleatorio;
}


int main(int argc, char * argv[]){ 
    int dia;
    int mes;
    int ano;
    char *nome_sensores[] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"};
    int linhas_arquivo = 1000;
    float a = 5.0; // range do valor do float
    float valor;
    int nome_aleatorio;

    limparTela();
    dia = atoi(argv[1]);
    mes = atoi(argv[2]);
    ano = atoi(argv[3]);

    srand((unsigned int)time(NULL));

    FILE *arquivo = fopen("sensores_random.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return 0;
    }
    
    limparTela();
    
    // cada for escreve uma linha no arquivo
    for (int j =0; j < linhas_arquivo-1; j++){    
        
        time_t timestamp_usuario = gerar_timestamp_aleatorio(dia, mes, ano);
 
        nome_aleatorio = rand()%5;
        valor = ((float)rand()/(float)(RAND_MAX)) * a;
        fprintf(arquivo,"%s;",nome_sensores[nome_aleatorio]);
        fprintf(arquivo,"%d;",timestamp_usuario);
        fprintf(arquivo,"%.2f\n",valor);

    }
    // a ultima linha tem que estar fora do for pra não adicionar um \n na ultima linha
    
    time_t timestamp_usuario = gerar_timestamp_aleatorio(dia, mes, ano); 
    nome_aleatorio = rand()%5;
    valor = ((float)rand()/(float)(RAND_MAX)) * a;
    fprintf(arquivo,"%s;",nome_sensores[nome_aleatorio]);
    fprintf(arquivo,"%d;",timestamp_usuario);
    fprintf(arquivo,"%.2f",valor);
    printf("Arquivo criado: sensores_random.txt\n");
    fclose(arquivo);

    return 0;  

}
