#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <string.h>
#include <conio.h>
#include <stdbool.h>

// arquivo pra colocar o corpo das funções

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
        printf("Data inválida.\n");
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