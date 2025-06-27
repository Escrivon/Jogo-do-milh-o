#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "jogo.h"

// ---------------- FUNÇÕES PRINCIPAIS ----------------

void carregarPerguntasCSV(const char* nomeArquivo, Pergunta** perguntas, int* totalPerguntas) {
    FILE* arq = fopen(nomeArquivo, "r");
    if (!arq) {
        printf("Arquivo CSV não encontrado.\n");
        return;
    }

    while (!feof(arq)) {
        Pergunta p;
        if (fscanf(arq, " %90[^,],%30[^,],%30[^,],%30[^,],%30[^,], %c,%d\n",
                   p.texto,
                   p.alternativa[0],
                   p.alternativa[1],
                   p.alternativa[2],
                   p.alternativa[3],
                   &p.letra,
                   &p.dificuldade) == 7) {
            p.ativo = 1;
            *perguntas = realloc(*perguntas, (*totalPerguntas + 1) * sizeof(Pergunta));
            (*perguntas)[(*totalPerguntas)++] = p;
        }
    }

    fclose(arq);
}

void salvarBinario(const char* nomeArquivo, Pergunta* perguntas, int totalPerguntas) {
    FILE* arq = fopen(nomeArquivo, "wb");
    if (!arq) return;
    fwrite(&totalPerguntas, sizeof(int), 1, arq);
    fwrite(perguntas, sizeof(Pergunta), totalPerguntas, arq);
    fclose(arq);
}

void carregarBinario(const char* nomeArquivo, Pergunta** perguntas, int* totalPerguntas) {
    FILE* arq = fopen(nomeArquivo, "rb");
    if (!arq) return;
    fread(totalPerguntas, sizeof(int), 1, arq);
    *perguntas = malloc(*totalPerguntas * sizeof(Pergunta));
    fread(*perguntas, sizeof(Pergunta), *totalPerguntas, arq);
    fclose(arq);
}

void exportarCSV(const char* nomeArquivo, Pergunta* perguntas, int totalPerguntas) {
    FILE* arq = fopen(nomeArquivo, "w");
    if (!arq) return;
    for (int i = 0; i < totalPerguntas; i++) {
        if (perguntas[i].ativo) {
            fprintf(arq, "%s,%s,%s,%s,%s,%c,%d\n",
                    perguntas[i].texto,
                    perguntas[i].alternativa[0],
                    perguntas[i].alternativa[1],
                    perguntas[i].alternativa[2],
                    perguntas[i].alternativa[3],
                    perguntas[i].letra,
                    perguntas[i].dificuldade);
        }
    }
    fclose(arq);
}

void listarPerguntas(Pergunta* perguntas, int totalPerguntas) {
    for (int i = 0; i < totalPerguntas; i++) {
        if (perguntas[i].ativo) {
            printf("[%d] %s (Dificuldade: %d)\nA)%s B)%s C)%s D)%s\nCorreta: %c\n\n",
                   i + 1,
                   perguntas[i].texto,
                   perguntas[i].dificuldade,
                   perguntas[i].alternativa[0],
                   perguntas[i].alternativa[1],
                   perguntas[i].alternativa[2],
                   perguntas[i].alternativa[3],
                   perguntas[i].letra);
        }
    }
}

void adicionarPergunta(Pergunta** perguntas, int* totalPerguntas) {
    Pergunta p;
    printf("Digite o texto da pergunta: ");
    getchar();  // limpa buffer
    fgets(p.texto, sizeof(p.texto), stdin);
    p.texto[strcspn(p.texto, "\n")] = '\0';

    for (int i = 0; i < 4; i++) {
        printf("Digite a alternativa %c: ", 'A' + i);
        fgets(p.alternativa[i], sizeof(p.alternativa[i]), stdin);
        p.alternativa[i][strcspn(p.alternativa[i], "\n")] = '\0';
    }

    printf("Digite a letra correta (A-D): ");
    scanf(" %c", &p.letra);

    printf("Digite a dificuldade (1 a 5): ");
    scanf("%d", &p.dificuldade);

    p.ativo = 1;

    *perguntas = realloc(*perguntas, (*totalPerguntas + 1) * sizeof(Pergunta));
    (*perguntas)[(*totalPerguntas)++] = p;

    printf("Pergunta adicionada com sucesso!\n");
}

void pesquisarPergunta(Pergunta* perguntas, int totalPerguntas) {
    char termo[200];
    printf("Digite o termo de busca: ");
    getchar();  // limpa buffer
    fgets(termo, sizeof(termo), stdin);
    termo[strcspn(termo, "\n")] = '\0';

    int achou = 0;
    for (int i = 0; i < totalPerguntas; i++) {
        if (perguntas[i].ativo && strstr(perguntas[i].texto, termo)) {
            printf("[%d] %s (Dificuldade: %d)\nA)%s B)%s C)%s D)%s\nCorreta: %c\n\n",
                   i + 1,
                   perguntas[i].texto,
                   perguntas[i].dificuldade,
                   perguntas[i].alternativa[0],
                   perguntas[i].alternativa[1],
                   perguntas[i].alternativa[2],
                   perguntas[i].alternativa[3],
                   perguntas[i].letra);
            achou = 1;
        }
    }

    if (!achou) {
        printf("Nenhuma pergunta encontrada com o termo: %s\n", termo);
    }
}

void excluirPergunta(Pergunta* perguntas, int* totalPerguntas, int indice) {
    if (indice < 0 || indice >= *totalPerguntas) {
        printf("Índice inválido!\n");
        return;
    }

    for (int i = indice; i < *totalPerguntas - 1; i++) {
        perguntas[i] = perguntas[i + 1];
    }

    (*totalPerguntas)--;
    perguntas = realloc(perguntas, *totalPerguntas * sizeof(Pergunta));
    printf("Pergunta excluída com sucesso!\n");
}

void removerPergunta(Pergunta* perguntas, int* totalPerguntas, int indice) {
    if (indice >= 0 && indice < *totalPerguntas) {
        perguntas[indice].ativo = 0;
    }
}

void perguntaAleatoriaPorNivel(int nivel, Pergunta* p, Pergunta* perguntas, int totalPerguntas) {
    int indices[100];
    int cont = 0;

    for (int i = 0; i < totalPerguntas; i++) {
        if (perguntas[i].ativo && perguntas[i].dificuldade == nivel) {
            indices[cont++] = i;
        }
    }

    if (cont == 0) {
        p->ativo = 0;
        return;
    }

    int sorteado = indices[rand() % cont];
    *p = perguntas[sorteado];
    perguntas[sorteado].ativo = 0;
}

// ---------------- MAIN MODULARIZADO ----------------

int main() {
    Pergunta* perguntas = NULL;
    int totalPerguntas = 0;

    carregarBinario("perguntas.bin", &perguntas, &totalPerguntas);
    if (totalPerguntas == 0) {
        carregarPerguntasCSV("onepiece.csv", &perguntas, &totalPerguntas);
    }

    int opcao;
    do {
        opcao = menuRaylib();  // menu gráfico

        int indice;
        switch (opcao) {
            case 1:
                jogarComRaylib(perguntas, totalPerguntas);
                for (int i = 0; i < totalPerguntas; i++) {
                    perguntas[i].ativo = 1;
                }//for
                break;
            case 2:
                listarPerguntas(perguntas, totalPerguntas);
                break;
            case 3:
                adicionarPergunta(&perguntas, &totalPerguntas);
                break;
            case 4:
                exportarCSV("exportado.csv", perguntas, totalPerguntas);
                printf("Exportado com sucesso!\n");
                break;
            case 5:
                pesquisarPergunta(perguntas, totalPerguntas);
                break;
            case 6:
                listarPerguntas(perguntas, totalPerguntas);
                printf("Digite o índice da pergunta a excluir: ");
                scanf("%d", &indice);
                excluirPergunta(perguntas, &totalPerguntas, indice - 1);
                break;
            case 7:
                salvarBinario("perguntas.bin", perguntas, totalPerguntas);
                printf("Salvando e saindo...\n");
                break;
        }//do
    } while (opcao != 7);

    free(perguntas);
    return 0;
}//main