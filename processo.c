#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"


Processo* ler_processos_csv(const char* nome_arquivo, int* quantidade) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        *quantidade = 0;
        return NULL;
    }

    Processo* processos = malloc(MAX_PROCESSOS * sizeof(Processo));
    if (!processos) {
        fclose(arquivo);
        perror("Erro ao alocar memória");
        *quantidade = 0;
        return NULL;
    }

    char linha[1024];
    int i = 0;

    fgets(linha, sizeof(linha), arquivo); // Ignora cabeçalho

    while (fgets(linha, sizeof(linha), arquivo) && i < MAX_PROCESSOS) {
        Processo p = {0};
        char numero[64], data[64], classe_str[128], assunto_str[128];
        int id;

        int lidos = sscanf(linha, "%d,\"%[^\"]\",%[^,],\"%[^\"]\",%[^,],%d",
                           &p.id, numero, data, classe_str, assunto_str, &p.ano_eleicao);

        if (lidos < 6) {
            // tenta versão sem aspas (linhas com apenas um valor em classe/assunto)
            lidos = sscanf(linha, "%d,\"%[^\"]\",%[^,],%[^,],%[^,],%d",
                           &p.id, numero, data, classe_str, assunto_str, &p.ano_eleicao);
        }

        if (lidos == 6) {
            strcpy(p.numero, numero);
            strncpy(p.data_ajuizamento, data, 19);
            p.data_ajuizamento[19] = '\0';

            // classes
            p.qtd_classes = 0;
            char* token = strtok(classe_str, "{},");
            while (token && p.qtd_classes < 2) {
                p.id_classe[p.qtd_classes++] = atoi(token);
                token = strtok(NULL, "{},");
            }

            // assuntos
            p.qtd_assuntos = 0;
            token = strtok(assunto_str, "{},");
            while (token && p.qtd_assuntos < MAX_ASSUNTOS) {
                p.id_assunto[p.qtd_assuntos++] = atoi(token);
                token = strtok(NULL, "{},");
            }

            processos[i++] = p;
        }
    }

    fclose(arquivo);
    *quantidade = i;
    return processos;
}



void ordenar_por_id(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processos[j].id > processos[j + 1].id) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(processos[j].data_ajuizamento, processos[j + 1].data_ajuizamento) < 0) {
                Processo temp = processos[j];
                processos[j] = processos[j + 1];
                processos[j + 1] = temp;
            }
        }
    }
}

void salvar_em_csv(const char *nome_arquivo, Processo processos[], int n) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (fp == NULL) {
        printf("Erro ao criar o arquivo %s\n", nome_arquivo);
        return;
    }

    fprintf(fp, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d,\"%s\",\"%s\",{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            fprintf(fp, "%d", processos[i].id_classe[j]);
            if (j < processos[i].qtd_classes - 1) fprintf(fp, ",");
        }

        fprintf(fp, "},{");
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            fprintf(fp, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].qtd_assuntos - 1) fprintf(fp, ",");
        }
        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }

    fclose(fp);
    printf("Arquivo '%s' salvo com sucesso.\n", nome_arquivo);
}

int contar_por_classe(Processo processos[], int total_processos, int classe_busca) {
    int contador = 0;
    for (int i = 0; i < total_processos; i++) {
        for (int j = 0; j < processos[i].qtd_classes; j++) {
            if (processos[i].id_classe[j] == classe_busca) {
                contador++;
                break;
            }
        }
    }
    return contador;
}
int contar_assuntos_distintos(Processo processos[], int n) {
    int assuntos[1000];
    int total_distintos = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < processos[i].qtd_assuntos; j++) {
            int atual = processos[i].id_assunto[j];
            int encontrado = 0;

            for (int k = 0; k < total_distintos; k++) {
                if (assuntos[k] == atual) {
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                assuntos[total_distintos++] = atual;
            }
        }
    }

    return total_distintos;
}

void listar_processos_varios_assuntos(Processo processos[], int n) {
    printf("\nProcessos com mais de um id_assunto:\n");
    printf("\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < n; i++) {
        if (processos[i].qtd_assuntos > 1) {
            printf("%d,\"%s\",\"%s\",{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);
            
            for (int j = 0; j < processos[i].qtd_classes; j++) {
                printf("%d", processos[i].id_classe[j]);
                if (j < processos[i].qtd_classes - 1) printf(",");
            }

            printf("},{");
            for (int j = 0; j < processos[i].qtd_assuntos; j++) {
                printf("%d", processos[i].id_assunto[j]);
                if (j < processos[i].qtd_assuntos - 1) printf(",");
            }
            printf("},%d\n", processos[i].ano_eleicao);
        }
    }
}
int dias_em_tramitacao(Processo processos[], int total_processos, int id_busca) {
    for (int i = 0; i < total_processos; i++) {
        if (processos[i].id == id_busca) {
            struct tm data_ajuizamento = {0};

            // Formato esperado: "2016-04-20 15:03:40.000"
            int ano, mes, dia, hora, min, seg;
            if (sscanf(processos[i].data_ajuizamento, "%d-%d-%d %d:%d:%d",
                       &ano, &mes, &dia, &hora, &min, &seg) == 6) {
                data_ajuizamento.tm_year = ano - 1900;
                data_ajuizamento.tm_mon = mes - 1;
                data_ajuizamento.tm_mday = dia;
                data_ajuizamento.tm_hour = hora;
                data_ajuizamento.tm_min = min;
                data_ajuizamento.tm_sec = seg;

                time_t t_ajuizamento = mktime(&data_ajuizamento);
                time_t t_atual = time(NULL);

                double segundos = difftime(t_atual, t_ajuizamento);
                int dias = (int)(segundos / (60 * 60 * 24));

                printf("Processo ID: %d\n", processos[i].id);
                printf("Data de ajuizamento: %s\n", processos[i].data_ajuizamento);
                printf("Dias em tramitacao: %d dias\n", dias);
                return dias;
            } else {
                printf("Erro ao interpretar a data do processo.\n");
                return -1;
            }
        }
    }
    printf("Processo com ID %d não encontrado.\n", id_busca);
    return -1;
}

