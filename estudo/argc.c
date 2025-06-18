//programa pra cadastrar usuario atraves da linha de comando
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>


typedef struct{
    char nome[128];
    int mat;
}cadastro_t;

int interpreta(int argc, char * argv[]);
int registra(cadastro_t * cad);
void limparTela();
void sairComQualquerTecla();

void sairComQualquerTecla(){
    printf("Aperte qualquer tecla para continuar: \n");
    while (1){
        if (_kbhit()){
            char tecla = getch();
            break;
        }
    }
}

void limparTela(){
    #ifdef _WIN32 
        system("cls");
    #else
        system("clear");
    #endif
}

int registra(cadastro_t * cad){

    FILE *arquivo = fopen("cadastro.txt", "a+");

    if (arquivo == NULL){
        return -1;
    }

    fprintf(arquivo, "\"%s\" %d\n",cad->nome,cad->mat);

    fclose(arquivo);

    return 0;
}
int interpreta(int argc, char * argv[]){
    
    //for (int i = 2; i < argc; i++) {
        
        printf("Argumento : %s\n", argv[2]);

        // Convert the argument to an integer and print it (if it is a valid number)
        int num = atoi(argv[2]);
        if (num != 0 || argv[2][0] == '0') {  // Checking if argument can be converted to an integer
            printf("Matricula convertida para integer: %d\n", num);
        } else {
            printf("'%s' nao e um numero de matricula valido!.\n", argv[2]);
            return -1;
        }

    //}
    return 0;
}

int main(int argc, char * argv[]){
    cadastro_t cadastro;

    if (argc != 3){
        printf("Erro! Digite o nome do aluno entre aspas duplas \" seguido da matricula!");
        return 0;
    }
    
    if (interpreta(argc, argv) != 0){
        printf("Erro na linha de comando \r\n");
        return 0;
    }


    
    //printf("voce digitou: %s %s \t\n",argv[1], argv[2]);

    strcpy(cadastro.nome, argv[1]);
    cadastro.mat = atoi(argv[2]);
    //printf("voce digitou: %s %d \n",cadastro.nome, cadastro.mat);
    
    if (registra(&cadastro) != 0){
        printf("Erro ao registrar no arquivo \r\n");
        return 0;
    }

    return 0;
}



