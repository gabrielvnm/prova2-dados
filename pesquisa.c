#include <stdio.h>

struct cadastro_t 
{
    char nome[128];
    int mat;
};

int pesquisa(int mat, struct cadastro_t * cad);

int main(int argc, char * argv[])
{
    struct cadastro_t cadastro;

    if (argc < 2)
    {
        printf("Numero de matricula nao fornecido na linha de comando \r\n");
        return -1;
    }

    int mat = atoi(argv[1]);

    if (pesquisa(mat, &cadastro) != 0)
    {
        printf("\r\n cadastro nao encontrado");
    }
    else
    {
        puts(cadastro.nome);
    }

    return 0;
}

int pesquisa(int mat, struct cadastro_t * cad)
{
    return 0;
}