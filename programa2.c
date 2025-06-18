#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <ctype.h>

#define MAX_NOME_ARQUIVO 30
#define MAX_LINHAS 1001
#define NOME_SENSOR 16

//tem que fazer tudo aqui ainda!!!

typedef struct{
    char nome_sensor[NOME_SENSOR];
    long long int timestamp_sensor;
    char medicao[10];
}Sensores;

void abrirArquivo();
int obterNumeroLinhas();
time_t capturar_timestamp_valido();
void limparTela();
void sairComQualquerTecla();
int binSearch();
int interpretarComando();
int validarNomeSensor();

void sairComQualquerTecla(){
    printf("Aperte qualquer tecla para continuar: \n");
    while (1){
        if (_kbhit()){
            char tecla = getch();
            break;
        }
    }
}

int validarNomeSensor(char * argv[]){
    
    if (strlen(argv[1])> NOME_SENSOR){
        printf("Argumento %s excede o tamanho maximo de caracteres!\n",argv[1]);
        return -1;
    }
    return 0;
}
//funçao pra verificar se os argumentos da linha de comando são integer
int interpretarComando(char * argv[]){
    
    for (int i = 2; i< 8; i++){
        int num = isdigit(argv[i][0]);
        if (num == 0){
            printf("argumento %s nao e um numero!\n", argv[i]);
            //sairComQualquerTecla();
            return -1;
        }        
    }
    return 0;
}
// função pra receber o valor do usuario e transformar em timestamp
time_t capturar_timestamp_valido(int dia, int mes, int ano, int hora, int min, int seg) {
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

void limparTela(){
#ifdef _WIN32 // verifica se o sistema é Windows ou não
    system("cls");
#else
    system("clear");
#endif
}

//função pra abrir o arquivo e guardar como integer
//tem que trocar a ordem e o tipo de dado do fscanf
void abrirArquivo(char nome_arquivo[MAX_NOME_ARQUIVO], int numero_linhas, Sensores *sensor){
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    if (!arquivo) {
        perror("Erro ao abrir arquivo.");
        return;
    }
    Sensores sensortemp;
    int i = 0;

    while (fscanf(arquivo, "%16[^;];%10[^;];%lld\n", sensortemp.nome_sensor, sensortemp.medicao, &sensortemp.timestamp_sensor)==3){
        
        sensor[i] = sensortemp;
        i++;
    }
    
    fclose(arquivo);
    return;
}

int obterNumeroLinhas(char nome_arquivo[MAX_NOME_ARQUIVO]){

    FILE *arquivo;
    int contador_linhas = 0;
    char buffer;

    arquivo = fopen(nome_arquivo, "r");

    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo\n");
        return -1;
    }

    while ((buffer = fgetc(arquivo)) != EOF) {
        if (buffer == '\n') {
            contador_linhas++;
        }
    }
    fseek(arquivo, -1, SEEK_END);
    buffer = fgetc(arquivo);
    if (buffer != '\n') {
   
        fseek(arquivo, 0, SEEK_END);
        long tamanho = ftell(arquivo);
        if (tamanho > 0) {
            contador_linhas++;
        }
    }

    if (contador_linhas> MAX_LINHAS){
        printf("Arquivo excede o tamanho máximo!");
        return -1;
    }
    return contador_linhas;
}

// função de pesquisa binaria
int binSearch(Sensores *sensor, int left, int right, int key) { 
      
    int mid;
  
    while (left <= right) {

        mid = left + (right - left) / 2;
        if (sensor[mid].timestamp_sensor == key) {
            return mid;
        }
        if (sensor[mid].timestamp_sensor < key) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    
    return mid;
}

int main (int argc, char * argv[]){
    
    if (argc != 8 ){
        printf("Erro na linha de comando! Digite um nome valido de arquivo e a data e hora no formato dd mm aaaa hh mm ss!\n");
        return -1;
    }
    if (interpretarComando(argv) !=0){
        printf("Erro na linha de comando! Data deve estar no formato dd mm aaaa hh mm ss!\n");
        return -1;
    }
    if(validarNomeSensor(argv)!=0){
        printf("Erro na linha de comando! Nome do sensor Exece o limite de caracteres!\n");
        return -1;
    }
    limparTela();
    long long int key;
    int numero_linhas;
    int dia, mes, ano, hora, min, seg;

    dia = atoi(argv[2]);
    mes = atoi(argv[3]);
    ano = atoi(argv[4]);
    hora = atoi(argv[5]);
    min = atoi(argv[6]);
    seg = atoi(argv[7]);

    if (capturar_timestamp_valido(dia, mes, ano, hora, min, seg) == -1){
        printf("Data inválida. Tente novamente.\n");
        return -1;
    }

    printf("abrindo o arquivo: %s\n",argv[1]);
    key = capturar_timestamp_valido(dia, mes, ano, hora, min, seg);
    printf("timestamp informado: %d\n", key);
    numero_linhas = obterNumeroLinhas(argv[1]);
    Sensores sensor[numero_linhas]; 
    abrirArquivo(argv[1], numero_linhas, sensor);
    
    sairComQualquerTecla();

    int result = binSearch(sensor, 0, numero_linhas-1, key); 
    if (result == 0) {
        printf("Atencao: So existem leituras posteriores a data informada!\n");
        printf("Elemento mais proximo esta na posicao %d\n", result+1); // resultado da pesquisa, lembrando que a lista começa na posição 0
        printf("Resultado da busca: %s %lld %s\n", sensor[result].nome_sensor, sensor[result].timestamp_sensor, sensor[result].medicao);
    }
    else if (result == numero_linhas-1){
        printf("Atencao: So existem leituras anteriores a data informada!\n");
        printf("Elemento mais proximo esta na posicao %d\n", result+1); // resultado da pesquisa, lembrando que a lista começa na posição 0
        printf("Resultado da busca: %s %lld %s\n", sensor[result].nome_sensor, sensor[result].timestamp_sensor, sensor[result].medicao);
    }
    else {
        printf("Elemento mais proximo esta na posicao %d\n", result+1); // resultado da pesquisa, lembrando que a lista começa na posição 0
        printf("Resultado da busca: %s %lld %s\n", sensor[result].nome_sensor, sensor[result].timestamp_sensor, sensor[result].medicao);
    } 
    return 0;
}