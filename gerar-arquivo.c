#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include "gerar-arquivo.h"

int main(){ 
    int dia;
    int mes;
    int ano;
    char *nome_sensores[] = {"TEMP", "PRES", "VIBR", "UMID", "FLUX"};
    int linhas_arquivo = 2000;
    float a = 5.0; // range do valor do float
    float valor;
    int nome_aleatorio;

    limparTela();

    srand((unsigned int)time(NULL));

    FILE *arquivo = fopen("sensores_random.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }
    
    
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
