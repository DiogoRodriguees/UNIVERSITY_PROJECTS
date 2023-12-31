#include <signal.h> // signal()
#include <stdio.h>  // fopen()

bool programe_executing = 1;

void finalizar_sem_residuo(int signal) {
    puts("O programa foi interrompido");
}

void escrever_no_arquivo(char *texto, char *arquivo) {}

int main(int argc, char **argv)
{
    /* Caminho do arquivo recebido por parametro */
    char *caminho_do_arquivo = argv[1];

    /* Tratamento para os sinais de SIGTERM e SIGINT */
    signal(SIGTERM, finalizar_sem_residuo);
    signal(SIGINT, finalizar_sem_residuo);

    /* Loop inifito até o programa receber o sinal de interrupção */
    while (programe_executing)
    {
        /* Função para escrever o conteudo no arquivo */
        escrever_no_arquivo("texto de teste", caminho_do_arquivo);
    }

    return 0;
}