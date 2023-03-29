/**
 * Código de emultiemplo para paralelização de dados com pthreads.
 * Problema: Contabilizar o número de consoantes e vogais em um arquivo temultito.
 * Solução paralela: Atribuir a contabilização para threads. Cada thread processa
 * uma parte do temultito. Ao final, junta-se as respostas.
 */

#include <pthread.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "matriz.h"

float vetorGlobal[100];
int pos = 0;

/* estrutura para passar os parâmetros para as threads */
typedef struct matrizDate
{
    int r;
    int c;
    int **matriz;
} matrizDate;

/* estrutura que guarda os dados para utilizar no calculo da media aritimetica */
typedef struct aritmethicDates
{
    int line;
    int inicio;
    int mediasAmout;
    int *array;
    int *arrayMedias;
    matrizDate *dates;
} aritmethicDates;

float vetor_respostas[99];

/* escreve as respotas no arquivo de saida */
void writeMediasLineares();

/* cria uma matriz com valores aleatorios */
int **newMatriz(int, int);

/* cria uma estrutura de matrizDate */
matrizDate *newMatrizDate(int, int);

/* escreve os dados da matriz no arquivo de saida */
void writeMatrizInFile(int **, int, int);

/* copia os dados de uma entrada para um determinada matriz */
void copyDatesForMatriz(matrizDate *);

/* realiza o calculo da media aritimetica da linhas */
void *aritmethicCalculate(void *);

/* calcula a media geometrica da colunas */
void *geometricCalculate(void *);

/* valida a quantidade de entradas */
bool validateInput(int);

/* define as posiçoes que cada thread vai calcular */
void definePositionCalulate(aritmethicDates *, int, int);

/* chamada na função threads_create(), realiza as duas operações pedidas */
void *execThread(void *dates);

int main(int argc, char **argv)
{
    if (validateInput(argc))
        return 0;

    int rows = atoi(argv[1]);         // numero de linhas da matriz
    int columns = atoi(argv[2]);      // numero de colunas da matriz
    int threadsAmout = atoi(argv[3]); // quantidade de threads que serão criadas
    int status = 0;                   // variavel que recebe o retorno da função thread_create()
    int *mediasLineares = malloc(threadsAmout * sizeof(int));
    int **matriz = newMatriz(rows, columns);  // iniciando uma matriz vazia
    writeMatrizInFile(matriz, rows, columns); // preenchendo a matriz criada com os valores do arquivo de entrada

    pthread_t threadsRows[threadsAmout];    // threads para calcular média aritimetica
    pthread_t threadsColumns[threadsAmout]; // threads para calcular média geometrica

    matrizDate *dateMatriz = newMatrizDate(rows, columns); // iniciando uma estrutura para informações da matriz
    copyDatesForMatriz(dateMatriz);                        // copiando os dados para a nova estrutura

    printf("\n Matriz %ix%i\n", rows, columns);
    print_matrix(dateMatriz->matriz, dateMatriz->r, dateMatriz->c); // printando a matriz para conferência

    /* criando as threads para calcular media aritimetica */
    for (int i = 0; i < threadsAmout; i++)
    {
        /* criand e preenche a estrutura que sera enviada como parametro */
        aritmethicDates *datesRecent = malloc(sizeof(aritmethicDates));
        datesRecent->array = malloc(threadsAmout * sizeof(int));
        definePositionCalulate(datesRecent, rows, threadsAmout);
        datesRecent->inicio = datesRecent->array[i - 1];
        datesRecent->dates = dateMatriz;
        datesRecent->line = datesRecent->array[i];
        datesRecent->arrayMedias = mediasLineares;
        datesRecent->mediasAmout = 0;

        /* criação das threads */
        status = pthread_create(&threadsRows[i], NULL, aritmethicCalculate, datesRecent);
    }

    /* aguarda a finalização das threads */
    for (int i = 0; i < threadsAmout; i++)
        pthread_join(threadsRows[i], NULL);

    writeMediasLineares();

    printf("\n Os resultados estão no arquivo ./respostas.txt\n");
    
    return 0;
}

void writeMediasLineares(){
    FILE* saida = fopen("respostas.txt", "w");

    fprintf(saida, "%s", "medias: ");
    for (int i = 0; i < pos; i++)
    {
        fprintf(saida, "%f  ", vetorGlobal[i]); 
    }

    fclose(saida);
}

int **newMatriz(int rows, int columns)
{
    int **matriz = create_matrix(rows, columns);
    generate_elements(matriz, rows, columns, 99);

    return matriz;
}

matrizDate *newMatrizDate(int rows, int columns)
{
    matrizDate *newMatriz = malloc(sizeof(matrizDate)); // aloca a matriz dinamicamente
    newMatriz->r = rows;
    newMatriz->c = columns;
    newMatriz->matriz = malloc(rows * sizeof(int *)); // aloca as linhas da matriz da estrutura

    /* alocando as colunas da matriz da estrutura */
    for (int i = 0; i < rows; i++)
    {
        newMatriz->matriz[i] = malloc(columns * sizeof(int));
    }

    return newMatriz;
}

void copyDatesForMatriz(matrizDate *dates)
{
    FILE *arqOrigem = fopen("matrizTexto.in", "r"); // abrindo arquivo para leitura

    int i = 0;
    int j = 0;
    int number = 0; // variavel qu guarda o valor lido no arquivo

    while (i < dates->r)
    {
        /* lendo o arquivo */
        fscanf(arqOrigem, "%i", &number);

        /* escreve o valor lido, na linha indexada pelo indice i */
        if (j < dates->c)
        {
            dates->matriz[i][j++] = number;
        }

        /* troca o indexador i, quando o numero maximo de colunas é atingido */
        if (j == dates->c)
        {
            j = 0;
            i++;
        }
    }

    fclose(arqOrigem); // fecha o arquivo leitura
}

void writeMatrizInFile(int **matriz, int rows, int columns)
{
    /* abrindo arquivo para leitura */
    FILE *arqMatriz = fopen("matrizTexto.in", "w");

    /* copiando os dados da matriz para o arquivo */
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            fprintf(arqMatriz, "%i ", matriz[i][j]);

        fprintf(arqMatriz, "\n");
    }

    /* fechando arquivo */
    fclose(arqMatriz);
}

void definePositionCalulate(aritmethicDates *dates, int rows, int size)
{

    int sizePartition = rows / size;

    for (int i = 0; i < size; i++)
    {
        dates->array[i] = sizePartition * (i + 1);
    }

    dates->array[size - 1] += rows % size;
}

void *execThread(void *dates)
{
    aritmethicCalculate(dates);
}

void *aritmethicCalculate(void *dates)
{

    aritmethicDates *localdates = dates;
    matrizDate *matriz = localdates->dates;

    float soma = 0;
    float somaRow = 0;
    float medeiLinear = 0;

    for (int i = localdates->inicio; i < localdates->line; i++)
    {
        for (int j = 0; j < matriz->c; j++)
        {
            soma += matriz->matriz[i][j];
        }

        medeiLinear = soma / matriz->c;
        vetorGlobal[pos++] = medeiLinear;
        soma = 0;
    }

    return NULL;
}

void *geometricCalculate(void *param)
{
    // data_chunk *dados = param;

    // double media_geometrica, multi;
    // media_geometrica = 0;
    // multi = 1;

    // // printf("Thread id: %ld\n", pthread_self());

    // int posicao = dados->pos_inicio;
    // for (int j = 0; j < M; j++)
    // {
    //     multi *= dados->vetor[posicao];
    //     posicao += (N);
    // }

    // double aux = 1.0 / M;
    // media_geometrica = pow(multi, aux);

    // printf("* Media geométrica da coluna %i: %f\n", dados->pos_inicio, media_geometrica);

    // vetor_respostas[dados->num_seq] = media_geometrica;
    return NULL;
}

bool validateInput(int number)
{
    if (number < 4)
    {
        printf("É necessário informar o numero de linhas e colunas da matriz, e o numero de threads!\n");
        return true;
    }
    return false;
}