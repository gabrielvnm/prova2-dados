# Prova 2 Estrutura de dados

## Programa 1

Programa em C que organiza um arquivo de sensores no formato nome_sensor;medição_sensor;timestamp_sensor;

Os sensores são agrupados por nome, organizados por timestamp crescente e gravados em um arquivo txt com o nome do sensor para cada sensor diferente no arquivo

Não há limites para o número de sensores do arquivo, mas há um limite de linhas totais de 2000

### Argumentos para inicialização: 

./programa1 "nome do arquivo de sensores"

O tamanho máximo do nome do arquivo é 30 caracteres.


## Programa 2

Programa que realiza uma busca por data e hora em um arquivo de sensores informado pelo usuário e exibe na tela o nome do sensor, a medição do sensor e o timestamp do horário da medição

A busca retorna o timestamp mais próximo da data informada pelo usuário

### Argumentos para inicialização: 

./programa2 "nome do arquivo.extensao" dia mes ano hora minuto segundo

O formato da data deve ser dd mm aaaa hh mm ss.

O tamanho máximo do nome do arquivo é 20 caracteres.

## Programa 3

Programa que cria uma lista de 2000 linhas com sensores, medições e timestamps aleatórios separados por ;

O nome do sensor e o tipo de dado de cada sensor é informado pelo usuário na linha de comando

Os timestamps aleatórios são gerados dentro de um range de data inicial e final também informada pelo usuário

### Argumentos para inicialização: 

./programa3 dia mes ano hora minuto segundo dia mes ano hora minuto segundo "nome do sensor1" "tipo de dado sensor 1" ...

Não há limites para o número de sensores diferentes criados, mas cada sensor deve ter um tipo de dado associado

O limite para o tamanho do nome do sensor é 16 caracteres

Os códigos para cada tipo de dado são: 
 - BINARIO = boolean; imprime no arquivo um 0 ou 1
 - TEXTO = string; imprime uma de 3 strings possíveis: Alto, Médio, Baixo
 - CONJ_Q = float; imprime um número decimal com duas casas entre 0 e 5
 - CONJ_Z = int; imprime um número inteiro entre 0 e 100

O nome do arquivo gerado é sempre sensores_random.txt


