#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define PAGE_SIZE 20
#define RAM_SIZE 40
#define PROCESS_SIZE 130
#define ALGORITMO 3

int global_time = 0;

/*****************************************************************************************
 *                                 Estrutura das Páginas                                 *
 *****************************************************************************************/
typedef struct
{
    // int page_number;
    int frame_number;
    int r_bit;
    int m_bit;
    int v_bit;
    int time_loaded;
} Page;
typedef struct no
{
    int dado;
    struct no *prox;
} No;

/*****************************************************************************************
 *                                 Implementaçã da Lista                                 *
 *****************************************************************************************/
typedef struct noh
{
    int dado;
    struct noh *prox;
    struct noh *ant;
} NO;

typedef struct
{
    NO *sentinela;
    int elements_amout;
} List;

List *create_list(int size)
{
    List *list = malloc(sizeof(List));
    list->elements_amout = 0;
    list->sentinela = malloc(sizeof(NO));
    list->sentinela->prox = list->sentinela;
    list->sentinela->ant = list->sentinela;

    return list;
}

int search_page_list(List *l, int pos)
{

    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (inicio != l->sentinela)
    {
        if (i == pos)
        {
            return inicio->dado;
        }

        inicio = inicio->prox;
        i++;
    }

    return i;
}

int page_in_lst(List *l, int page_number)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (inicio != l->sentinela)
    {
        if (inicio->dado == page_number)
        {
            return i;
        }
        inicio = inicio->prox;
        i++;
    }

    return -1;
}

void insert_list(List *l, int page_number, int pos)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    NO *new_no = malloc(sizeof(NO));
    new_no->dado = page_number;

    if (inicio == l->sentinela)
    {
        new_no->prox = l->sentinela;
        new_no->ant = l->sentinela;

        l->sentinela->prox = new_no;
        l->sentinela->ant = new_no;

        l->elements_amout++;

        return;
    }

    if (pos == l->elements_amout)
    {
        l->sentinela->ant->prox = new_no;
        new_no->ant = l->sentinela->ant;
        l->sentinela->ant = new_no;
        new_no->prox = l->sentinela;

        l->elements_amout++;

        return;
    }

    while (inicio != l->sentinela)
    {
        if (i == pos)
        {
            inicio->ant->prox = new_no;
            new_no->ant = inicio->ant;

            inicio->ant = new_no;
            new_no->prox = inicio;
            break;
        }

        inicio = inicio->prox;
        i++;
    }

    l->elements_amout++;
}

void remove_list(List *l, int pos)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (l->sentinela != inicio)
    {
        if (i == pos)
        {
            inicio->ant->prox = inicio->prox;
            inicio->prox->ant = inicio->ant;
            break;
        }
        i++;
        inicio = inicio->prox;
    }

    l->elements_amout--;
}

/*****************************************************************************************
 *                                 Implementaçã da Fila                                  *
 *****************************************************************************************/

typedef struct fila
{
    No *inicio;
    No *fim;
    int qtde;
} Fila;

Fila *fila_criar()
{
    Fila *f = (Fila *)malloc(sizeof(Fila));
    f->qtde = 0;
    f->inicio = NULL;
    f->fim = NULL;
    return f;
}

bool fila_inserir(Fila *f, int page_number)
{
    if (f == NULL)
        return false;

    No *novo = (No *)malloc(sizeof(No));
    if (f->qtde == 0)
    {
        f->inicio = novo;
        f->inicio->dado = page_number;
        f->inicio->prox = NULL;
    }
    else
    {
        f->fim->prox = novo;
    }
    f->fim = novo;
    f->fim->dado = page_number;
    f->fim->prox = NULL;
    f->qtde++;
}

bool fila_remover(Fila *f)
{
    if (f == NULL)
        return false;
    if (f->qtde == 0)
        return false;

    No *aux = f->inicio;
    f->inicio = f->inicio->prox;
    f->qtde--;
    free(aux);
    aux = NULL;

    return true;
}

void fila_destruir(Fila **enderecoFila)
{
    Fila *f = *enderecoFila;
    No *aux;

    while (f->inicio != NULL)
    {
        aux = f->inicio;
        f->inicio = f->inicio->prox;
        free(aux);
    }

    free(f);
    *enderecoFila = NULL;
}

// limpa os bits de referencia(r_bit) para 0
void update_bits(Page **page_table, int quantity_pages)
{
    for (int i = 0; i < quantity_pages; i++)
    {
        if (page_table[i])
        {
            page_table[i]->r_bit = 0;
        }
    }
}

/*****************************************************************************************
 *                           Algoritmo NRU - Not Recently Used                           *
 *****************************************************************************************/
void nru(Page **page_table, int **ram, int quantity_pages, int ram_size, int id)
{
    int position_sub_memory = -1; // posição que aponta para a memoria (-1 significa que a memoria possui espaço livre)
    int page_target = -1;         // posição da pagina vitima
    int class = -1;               // menor classe encontrada (-1 valor de inicialização)

    // verifica se tem memoria livre
    for (int i = 0; i < ram_size; i++)
    {
        if ((*ram)[i] == 0)
        {
            position_sub_memory = i;
            (*ram)[position_sub_memory] = 1; // Modifica o valor da RAM
            break;
        }
    }

    // não tem espaço na meoria
    if (position_sub_memory == -1)
    {
        // passa pela paginas tabela
        for (int i = 0; i < quantity_pages; i++)
        {
            printf("page: %d, bit R: %d, bit M: %d \n", i, page_table[i]->r_bit, page_table[i]->m_bit);
            if (page_table[i]->v_bit == 1)
            {
                // verifica se classe 0
                if (page_table[i]->r_bit == 0 && page_table[i]->m_bit == 0)
                {
                    page_target = i;
                    class = 0;
                    break;
                }

                // verifica se classe 1
                if (page_table[i]->r_bit == 0 && page_table[i]->m_bit == 1)
                {
                    // verifica se nenhuma class 1 foi encontrada até o memento
                    if (class != 1)
                    {
                        page_target = i;
                        class = 1;
                    }
                }

                // verifica se classe 2
                if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 0)
                {
                    // verifica se essa é a menor classe até o memento
                    if (class > 2)
                    {
                        page_target = i;
                        class = 2;
                    }
                }

                // verifica se classe 3
                if (page_table[i]->r_bit == 1 && page_table[i]->m_bit == 1)
                {
                    // verifica se nenhuma classe menor foi encontrada
                    if (class == -1 && class != 3)
                    {
                        page_target = i;
                        class = 3;
                    }
                }
            }
        }

        // busca frame number da pagina vitima
        position_sub_memory = page_table[page_target]->frame_number;

        printf("Pagina %d substituida.\n", page_target);

        // tira a referenciade memoria da pagina vitima
        page_table[page_target]->v_bit = 0;
        // page_table[page_target]->r_bit = 0;

        // referencia na memoria a pagina desejada
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
    // tem espaço na memoria
    else
    {
        // referencia a pagina na memoria
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
}

/*****************************************************************************************
 *                                    Algoritmo FIFO                                     *
 *****************************************************************************************/
void fifo(Page **page_table, int **ram, int quantity_pages, int ram_size, Fila **fila, int id)
{
    int position_sub_memory = -1; // posição que aponta para a memoria (-1 significa que a memoria possui espaço livre)
    int page_target = -1;         // numero da pagina vitima

    // inseri na memoria ram enquanto tiver posição livre
    for (int i = 0; i < ram_size; i++)
    {
        if ((*ram)[i] == 0)
        {
            position_sub_memory = i;
            (*ram)[position_sub_memory] = 1; // Modifica o valor da RAM
            fila_inserir(*fila, id);
            break;
        }
    }

    // se não existe posição livre, substitui
    if (position_sub_memory == -1)
    {

        page_target = (*fila)->inicio->dado;
        position_sub_memory = page_table[page_target]->frame_number;

        // remove a página mais antiga
        fila_remover(*fila);
        fila_inserir(*fila, id);

        printf("Pagina %d substituida.\n", page_target);

        // pagina vitima deixa de apontar para memoria
        page_table[page_target]->v_bit = 0;
        page_table[page_target]->frame_number = -1;

        // acessando pagina que deseja acessar memoria
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
    // existe posição livre
    else
    {
        // referencia a pagina desejada na memoria
        page_table[id]->frame_number = position_sub_memory;
        page_table[id]->v_bit = 1;
        page_table[id]->r_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_sub_memory);
    }
}

/*****************************************************************************************
 *                                Algoritmo Segunda Chance                               *
 *****************************************************************************************/
int find_old_page(List *l, Page **table)
{
    int i = 0;
    NO *inicio = l->sentinela->prox;

    while (table[inicio->dado]->r_bit != 0)
    {
        table[inicio->dado]->r_bit = 0;
        inicio = inicio->prox;
        if (inicio == l->sentinela)
        {
            i = 0;
            inicio = inicio->prox;
            continue;
        }
        i++;
    }

    return i;
}

void second_chance(Page **page_table, int *ram, int quantity_pages, int ram_size, List *list, int id)
{
    int position_free_memory = -1; // -1 indica que não existe quadros livres na memoria

    // inseri a pagina se existe quadro livre
    for (int i = 0; i < ram_size; i++)
    {
        if (ram[i] == 0)
        {
            position_free_memory = i;
            insert_list(list, id, list->elements_amout);
            ram[position_free_memory] = 1;
            break;
        }
    }

    // caso memória cheia
    if (position_free_memory == -1)
    {
        // busca a pagina alvoj
        int page_old = find_old_page(list, page_table);
        position_free_memory = page_table[page_old]->frame_number;
        printf("PAGE OLD: %i\n", page_old);
        printf("POS FREE MEMORY: %i\n", page_table[page_old]->frame_number);

        // verifica se a pagina ja esta na lista
        int pos_in_list = page_in_lst(list, id);
        if (pos_in_list == -1)
        {
            insert_list(list, id, list->elements_amout);
        }
        else
        {
            remove_list(list, pos_in_list);
            insert_list(list, id, list->elements_amout);
        }

        page_table[page_old]->frame_number = -1;
        page_table[page_old]->r_bit = 0;
        page_table[page_old]->v_bit = 0;

        page_table[id]->frame_number = position_free_memory;
        page_table[id]->r_bit = 1;
        page_table[id]->v_bit = 1;
        page_table[id]->time_loaded = global_time;

        remove_list(list, page_old);
    }
    else
    {
        page_table[id]->frame_number = position_free_memory;
        page_table[id]->r_bit = 1;
        page_table[id]->v_bit = 1;
        page_table[id]->time_loaded = global_time;

        printf("Pagina %d carregada no quadro %d.\n", id, position_free_memory);
    }
}

char *algoritmoEscohido(int num)
{
    switch (num)
    {
    case 1:
        return "NRU";
        break;
    case 2:
        return "FIFO";
        break;
    case 3:
        return "SEGUNDA CHANCE";
        break;

    default:
        break;
    }

    return "NENHUM";
}

void print_list(List *l, Page **table)
{
    NO *inicio = l->sentinela->prox;

    while (inicio != l->sentinela)
    {
        printf("PG NUM:%i,  R:%i\n", inicio->dado, table[inicio->dado]->r_bit);
        inicio = inicio->prox;
    }
}

void print_table_page(Page **table, int size)
{
    printf("___________________________________________________\n");
    printf("  PG NUM     PG FRAME      R      M      V   \n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < size; i++)
    {
        printf("    %i           %0*d         %i      %i      %i   \n", i, 2, table[i]->frame_number, table[i]->r_bit, table[i]->m_bit, table[i]->v_bit);
    }
    printf("---------------------------------------------------\n");
}

int main()
{
    system("clear");
    int physical_address;
    int page_fault = 0;
    int refresh_interval = 4;

    int quantity_frames = RAM_SIZE / PAGE_SIZE;
    int *RAM = (int *)calloc(quantity_frames, sizeof(int));

    int quantity_pages = PROCESS_SIZE / PAGE_SIZE;

    // se a quantidade de paginas for um numero quebrado, incrementa
    if (PROCESS_SIZE % PAGE_SIZE != 0)
    {
        quantity_pages++;
    }

    printf("Tamanho do processo: %d\n", PROCESS_SIZE);
    printf("Tamanho da pagina: %d\n", PAGE_SIZE);
    printf("Tamanho da ram: %d\n", RAM_SIZE);
    printf("Algoritmo: %s\n", algoritmoEscohido(ALGORITMO));
    printf("Quantidade de paginas: %d\n", quantity_pages);
    printf("Quantidade de quadros: %d\n", quantity_frames);

    printf("\n\nInsira a OP e o ADDRESS\n");
    Page **page_table = (Page **)malloc(quantity_pages * sizeof(Page *));

    for (int i = 0; i < quantity_pages; i++)
    {
        Page *new_page = (Page *)malloc(sizeof(Page));
        // new_page->page_number = i;
        new_page->frame_number = -1;
        new_page->r_bit = 0;
        new_page->m_bit = 0;
        new_page->v_bit = 0;
        new_page->time_loaded = global_time;
        page_table[i] = new_page;
    }

    int op;
    int address;

    Fila *fila = NULL;

    if (ALGORITMO == 2)
    {
        fila = fila_criar();
    }

    List *list;
    if (ALGORITMO == 3)
    {
        list = create_list(quantity_frames);
    }

    while (scanf("%d %x", &op, &address) == 2)
    {
        printf("Operacao: %d, Endereco Logico: 0x%x\n", op, address);

        int page_number = address / PAGE_SIZE;

        // calcula o deslocamento
        int offset = address % PAGE_SIZE;

        if (op == 1)
        {
            page_table[page_number]->m_bit = 1;
        }

        if (!page_table[page_number]->v_bit)
        {
            page_fault++;
            printf("Ocorreu page fault\n");
            // Lógica de substituição de página
            switch (ALGORITMO)
            {
            case 1:
                nru(page_table, &RAM, quantity_pages, quantity_frames, page_number);
                break;
            case 2:
                fifo(page_table, &RAM, quantity_pages, quantity_frames, &fila, page_number);
                break;
            case 3:
                second_chance(page_table, RAM, quantity_pages, quantity_frames, list, page_number);
                break;
            default:
                printf("Erro: Algoritmo de substituicao de pagina invalido.\n");
            }

            // Cálcula o endereço físico
            physical_address = page_table[page_number]->frame_number * PAGE_SIZE + (offset);
            printf("Endereco Fisico: %d * %d + %x = 0x%x\n", page_table[page_number]->frame_number, PAGE_SIZE, offset, physical_address);
        }
        else
        {
            printf("Pagina %i ja esta no quadro %i.\n", page_number, page_table[page_number]->frame_number);
        }

        print_list(list, page_table);
        print_table_page(page_table, quantity_pages);
        // intervalo de tempo que atribui 0 aos bits de referencia
        // funciona como o clock do sistema
        global_time++;
        if (global_time % refresh_interval == 0)
        {
            update_bits(page_table, quantity_pages);
        }

        printf("\n");
    }

    printf("Total de page fault: %d\n", page_fault);
    free(fila);
    free(page_table);
    return 0;
}