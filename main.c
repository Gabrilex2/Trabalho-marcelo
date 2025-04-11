#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    int total = 0;
    Processo* processos = ler_processos_csv("csv.csv", &total);

    if (!processos || total == 0) {
        printf("Nenhum processo lido.\n");
        return 1;
    }

    printf("Total de processos lidos: %d\n", total);

    int opcao;
    do {
        printf("\n==== MENU ====\n");
        printf("1. Ordenar por ID e salvar em CSV\n");
        printf("2. Ordenar por data de ajuizamento e salvar em CSV\n");
        printf("3. Contar processos por classe\n");
        printf("4. Contar assuntos distintos\n");
        printf("5. Listar processos com mais de um assunto\n");
        printf("6. Ver dias em tramitacao por ID do processo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                ordenar_por_id(processos, total);
                salvar_em_csv("ordenado_por_id.csv", processos, total);
                break;

            case 2:
                ordenar_por_data(processos, total);
                salvar_em_csv("ordenado_por_data.csv", processos, total);
                break;

            case 3: {
                int classe_desejada;
                printf("Digite o ID da classe que deseja buscar: ");
                scanf("%d", &classe_desejada);
                int total_encontrado = contar_por_classe(processos, total, classe_desejada);
                printf("Total de processos com a classe %d: %d\n", classe_desejada, total_encontrado);
                break;
            }

            case 4: {
                int distintos = contar_assuntos_distintos(processos, total);
                printf("Assuntos distintos: %d\n", distintos);
                break;
            }

            case 5:
                listar_processos_varios_assuntos(processos, total);
                break;

            case 6: {
                int id_procurado;
                printf("Digite o ID do processo para verificar dias de tramitação: ");
                scanf("%d", &id_procurado);
                dias_em_tramitacao(processos, total, id_procurado);
                break;
            }

            case 0:
                printf("Encerrando...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    free(processos);
    return 0;
}
