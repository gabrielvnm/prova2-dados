#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>

#define MAX_LINHAS 2001
#define NOME_SENSOR 16
#define MAX_LEITURA 10


typedef struct{
    char nome_sensor[NOME_SENSOR];
    char medicao[MAX_LEITURA];
    long int timestamp_sensor;
}Sensores ;

void abrirArquivo();
int obterNumeroLinhas();
void limparTela();
void selection_();
void selection();
void selection_nome();
void selectionNome();
int posicaoNome();
void sairComQualquerTecla();
void criarArquivo();
void gravarArquivo();
int obterNumeroSensores();



void limparTela(){
#ifdef _WIN32 // verifica se o sistema é Windows ou não
    system("cls");
#else
    system("clear");
#endif
}

// função pra abrir o arquivo e guardar numa struct da função main
void abrirArquivo(char nome_arquivo[], int numero_linhas, Sensores *sensor){
    FILE* arquivo = fopen(nome_arquivo, "r");
    
    if (!arquivo) {
        perror("Erro ao abrir arquivo.");
        return;
    }
    Sensores sensortemp; // struct buffer 
    int i = 0;

    // o programa ta lendo a medição do sensor como string pq não vai fazer nenhuma operação com o valor, entao nao faz tanta diferença o tipo de dado
    while (fscanf(arquivo, "%16[^;];%10[^;];%ld;\n", sensortemp.nome_sensor, sensortemp.medicao, &sensortemp.timestamp_sensor)==3){
        sensor[i] = sensortemp;
        i++;
    }
    fclose(arquivo);
    return;
}
// função pra obter um integer com o numero de linhas do arquivo
int obterNumeroLinhas(char nome_arquivo[]){

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

//função pra verificar quantos sensores diferentes tem no arquivo
int obterNumeroSensores(Sensores *sensor, int size){
    int j = 0;
    for (int i = 0; i < size ; i++){
        if (strcmp(sensor[i].nome_sensor, sensor[i+1].nome_sensor)!=0) { 
            j++;
        }   
    }
    return j;
}

// função selection pra colocar as structs em ordem alfabetica pelo nome do sensor
void selection_nome(Sensores *sensor, int size){
    int i;  
    int j;  
    int min_idx; 

    Sensores temp;
    
    for (i = 0; i < size - 1; i++) { 
        min_idx = i;
        for (j = i+1; j < size; j++) {
            if (strcmp(sensor[j].nome_sensor, sensor[min_idx].nome_sensor) < 0 ) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            temp = sensor[i];
            sensor[i] = sensor[min_idx];
            sensor[min_idx] = temp;
        }
    }
}

void selectionNome(Sensores *sensor, int size){
    for (int i = 0; i < size + 1; i++) 
    {
        selection_nome(sensor, size -i);
    }
}

// função selection pra ordenar a lista
void selection_(Sensores *sensor, int size){
    int i; 
    int j; 
    int max_idx;

    Sensores temp;

    for (i = 0; i < size - 1; i++) { 
        max_idx = i;
        for (j = i+1; j < size; j++) {
            if (sensor[j].timestamp_sensor > sensor[max_idx].timestamp_sensor) { //pra ordenar ascendente ou descendente é só trocar esse >
                max_idx = j;
            }
        }
        if (max_idx != i) {
            temp = sensor[i];
            sensor[i] = sensor[max_idx];
            sensor[max_idx] = temp;
        }
    }
}

void selection(Sensores *sensor, int size){
    for (int i = 0; i < size + 1; i++) {
        selection_(sensor, size -i);
    }
}

// função pra gravar as linhas da struct em um arquivo
void gravarArquivo(FILE *arquivo, Sensores *sensor, int size){

    for (int i = 0; i < size-1; i++){
        fprintf(arquivo, "%s;",sensor[i].nome_sensor); 
        fprintf(arquivo, "%s;",sensor[i].medicao);
        fprintf(arquivo, "%ld\n",sensor[i].timestamp_sensor);
    }
    fprintf(arquivo, "%s;", sensor[size-1].nome_sensor);
    fprintf(arquivo, "%s;",sensor[size-1].medicao);
    fprintf(arquivo, "%ld",sensor[size-1].timestamp_sensor);
    fclose(arquivo);

}
// função pra criar um arquivo diferente com cada nome de sensor e abrir no modo w
void criarArquivo(Sensores *sensor, int size){
    char nome_arquivo[20] = "\0"; // inicializando a string do nome de arquivo com NULL terminator
    strcat(nome_arquivo, sensor[0].nome_sensor);// colocando o nome do sensor na string
    strcat(nome_arquivo, ".txt");// adicionando .txt no fim do nome do arquivo
    
    FILE *arquivo = fopen(nome_arquivo, "w");
            if (arquivo == NULL) {
                printf("Erro ao criar o arquivo!\n");
                return;
            }
            gravarArquivo(arquivo, sensor, size);
            printf("arquivo %s criado!\n",nome_arquivo);
            sairComQualquerTecla();
    
    return;
}

// funcao pra retornar a posicao na lista quando o nome troca
int posicaoNome(Sensores *sensor, int size, int start){
    int i = start; 

    for (i; i < size ; i++){
        if (strcmp(sensor[i].nome_sensor, sensor[start].nome_sensor)!=0) { 
            return i;
        }   
    }
    return size;
}

// funçao pra pausar a execução do programa até apertar alguma tecla
// ajuda muito na hora de debugar
void sairComQualquerTecla(){
    printf("Aperte qualquer tecla para continuar: \n");
    while (1){
        if (_kbhit()){
            char tecla = getch();
            break;
        }
    }
}

int main (int argc, char * argv[]){
    // validação dos argumentos, só precisa do nome do arquivo então é mais simples
    if (argc != 2 ){
        printf("Erro na linha de comando! Digite o nome do arquivo sem espaços ou entre aspas duplas!");
        return -1;
    }

    int key;
    int numero_linhas;
    int posicao;
    int start = 0;
    int size;

    limparTela();
    printf("abrindo o arquivo: %s\n",argv[1]);


    numero_linhas = obterNumeroLinhas(argv[1]);
    Sensores sensor[numero_linhas]; 
    Sensores sensor_alfabetico[numero_linhas];

    // nessa função vai abrir o arquivo e copiar o conteudo pro array de struct da função main
    abrirArquivo(argv[1], numero_linhas, sensor);
    //colocando a struct em ordem alfabetica
    selection_nome(sensor, numero_linhas);
    int numero_sensores = obterNumeroSensores(sensor,numero_linhas);

    // repetição pra copiar as coisas pra struct buffer e gravar num arquivo jasmyn dias
    for (int k = 0; k < numero_sensores; k++){ 
        // separando a struct por nome do sensor
        posicao = posicaoNome(sensor, numero_linhas, start);
        int j = 0;

        for (int i = start; i < posicao; i++) { 
            sensor_alfabetico[j] = sensor[i];            
            j++;
        }  
        selection(sensor_alfabetico, j);
        // criando um arquivo com a struct buffer
        criarArquivo(sensor_alfabetico, posicao-start);
        // atualizando a posição na struct inicial pra obter o proximo nome de sensor
        start = posicao;
    }
    return 0;
}