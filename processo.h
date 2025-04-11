#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_CLASSES 10
#define MAX_ASSUNTOS 10
#define MAX_PROCESSOS 20000

typedef struct {
    int id;
    char numero[30];
    char data_ajuizamento[25];
    int id_classe[MAX_CLASSES];
    int qtd_classes;
    int id_assunto[MAX_ASSUNTOS];
    int qtd_assuntos;
    int ano_eleicao;
} Processo;
Processo* ler_processos_csv(const char* nome_arquivo, int* quantidade);
// Funções de ordenação
void ordenar_por_id(Processo processos[], int n);
void ordenar_por_data(Processo processos[], int n);

// Funções de contagem
int contar_por_classe(Processo processos[], int n, int classe_busca);
int contar_assuntos_distintos(Processo processos[], int n);

// Funções de listagem e exportação
void listar_processos_varios_assuntos(Processo processos[], int n);
void salvar_em_csv(const char *nome_arquivo, Processo processos[], int n);

// Cálculo de dias em tramitação
int dias_em_tramitacao(Processo processos[], int total_processos, int id_busca);

#endif
