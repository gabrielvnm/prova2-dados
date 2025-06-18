#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <conio.h>

//#define MAX_NOME_ARQUIVO 30
#define MAX_LINHAS 2001
#define NOME_SENSOR 16
#define MAX_LEITURA 10


typedef struct{
    char nome_sensor[NOME_SENSOR];
    char medicao[MAX_LEITURA];
    long long int timestamp_sensor;
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


// função que limpa a tela do terminal, quality of life /heh
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
    // esse while aqui vai continuar rodando até que o numero de inputs recebidos do arquivo seja diferente de 3
    // cada vez que ele roda ele guarda as informações no buffer e depois passa o buffer pra proxima posição
    // da struct da função main
    while (fscanf(arquivo, "%16[^;];%10[^;];%lld;\n", sensortemp.nome_sensor, sensortemp.medicao, &sensortemp.timestamp_sensor)==3){
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

//argumentos: lista de sensores, tamanho da lista
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
            if (strcmp(sensor[j].nome_sensor, sensor[min_idx].nome_sensor) < 0 ) { // comparando o valor ascii das duas strings
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
// argumentos sao a lista de elementos q vai ser organizada e o tamanho da lista
// ela passa os valores por referencia entao o return é void
void selection_(Sensores *sensor, int size){
    int i; 
    int j; 
    int min_idx;

    Sensores temp;

    for (i = 0; i < size - 1; i++) { 
        min_idx = i;
        for (j = i+1; j < size; j++) {
            if (sensor[j].timestamp_sensor < sensor[min_idx].timestamp_sensor) { 
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

void selection(Sensores *sensor, int size){
    for (int i = 0; i < size + 1; i++) {
        selection_(sensor, size -i);
    }
}

// função pra gravar as linhas da struct organizada em um arquivo
// ela roda dentro da função de criar arquivo, entao o arquivo ja vai estar aberto com o nome certo
void gravarArquivo(FILE *arquivo, Sensores *sensor, int size){

    for (int i = 0; i < size-1; i++){
        fprintf(arquivo, "%s;",sensor[i].nome_sensor); 
        fprintf(arquivo, "%s;",sensor[i].medicao);
        fprintf(arquivo, "%lld\n",sensor[i].timestamp_sensor);
    }
    // esse ultimo tem que rodar fora do for pra não printar mais um \n no arquivo
    fprintf(arquivo, "%s;", sensor[size-1].nome_sensor);
    fprintf(arquivo, "%s;",sensor[size-1].medicao);
    fprintf(arquivo, "%lld",sensor[size-1].timestamp_sensor);
    fclose(arquivo);

}
// função pra criar um arquivo diferente com cada nome de sensor e abrir no modo w
void criarArquivo(Sensores *sensor, int size){
    char nome_arquivo[20] = "\0"; // inicializando a string do nome de arquivo com NULL terminator
    strcat(nome_arquivo, sensor[0].nome_sensor);// colocando o nome do sensor na string
    printf("nome do sensor: %s\n",nome_arquivo);
    strcat(nome_arquivo, ".txt");// adicionando .txt no fim do nome do arquivo
    printf("nome do arquivo: %s\n",nome_arquivo);
    
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
    int i = start; // inicializando o indice na posicao informada pra comparar depois

    for (i; i < size ; i++){
        if (strcmp(sensor[i].nome_sensor, sensor[start].nome_sensor)!=0) { // se a lista estiver em ordem alfabetica ele vai retornar a posição do elemento diferente
            return i;
        }   
    }
    return size; // se nao tiver nenhum elemento diferente vai retornar a posicao final do array de struct
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
    
    int key;
    int numero_linhas;
    //char nome_arquivo[MAX_NOME_ARQUIVO];
    int posicao;
    int start = 0;
    //int numero_sensores = 5;
    int size;
    //char extensao[20];

    limparTela();

    if (argc != 2 ){
        printf("Erro na linha de comando! Digite o nome do arquivo sem espaços ou entre aspas duplas!");
        return -1;
    }
    
    printf("abrindo o arquivo: %s\n",argv[1]);
    sairComQualquerTecla();


    numero_linhas = obterNumeroLinhas(argv[1]);
    Sensores sensor[numero_linhas]; 
    Sensores sensor_alfabetico[numero_linhas];

    // nessa função vai abrir o arquivo e copiar o conteudo pro array de struct da função main
    abrirArquivo(argv[1], numero_linhas, sensor);
    /*for (int i = 0; i < numero_linhas; i++){
        printf("sensor capturado linha %d: %s %s %d\n", i, sensor[i].nome_sensor, sensor[i].medicao, sensor[i].timestamp_sensor);
        
    }
    sairComQualquerTecla();
    limparTela();*/
    //colocando a struct em ordem alfabetica
    selection_nome(sensor, numero_linhas);
    /*for (int i = 0; i < numero_linhas; i++){
        printf("sensor principal em ordem alfabetica linha %d: %s %s %d\n", i, sensor[i].nome_sensor, sensor[i].medicao, sensor[i].timestamp_sensor);
        
    }*/

    //deu certo até aqui!!!!
    //sairComQualquerTecla();
    //limparTela();
    int numero_sensores = obterNumeroSensores(sensor,numero_linhas);
    printf("numero de sensores: %d\n",numero_sensores);

    // repetição pra copiar as coisas pra struct buffer e gravar num arquivo
    for (int k = 0; k < numero_sensores; k++){ // int k representa o nome do sensor atual que esta sendo ordenado e gravado
        // separando a struct por nome do sensor
        posicao = posicaoNome(sensor, numero_linhas, start);
        int j = 0; // essa variavel vai guardar o numero de elementos na struct buffer

        //colocando a struct em ordem na outra struct de buffer 
        for (int i = start; i < posicao; i++) { //
            sensor_alfabetico[j] = sensor[i];            
            //printf("sensor alfabetico %d: %s %s %d\n", j, sensor_alfabetico[j].nome_sensor, sensor_alfabetico[j].medicao, sensor_alfabetico[j].timestamp_sensor);
            j++;
        }  
        //sairComQualquerTecla();
        //limparTela();
        selection(sensor_alfabetico, j);
        /*for (int i = 0; i< posicao-start;i++){
            printf("sensor alfabetico ordenado %d: %s %s %d\n",i , sensor_alfabetico[i].nome_sensor, sensor_alfabetico[i].medicao, sensor_alfabetico[i].timestamp_sensor);
        }
        sairComQualquerTecla();*/
        
        
        

        // criando um arquivo com a struct buffer
        criarArquivo(sensor_alfabetico, posicao-start);

        // atualizando a posição na struct inicial pra obter o proximo nome de sensor
        start = posicao;
        sairComQualquerTecla();
        limparTela();
    }
    
    
    
    return 0;
}