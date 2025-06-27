#ifndef JOGO_H
#define JOGO_H

// Definição da struct
typedef struct {
    char texto[200];
    char alternativa[4][100];
    char letra;
    int dificuldade;
    int ativo;
} Pergunta;

// Funções de arquivo
void carregarPerguntasCSV(const char* nomeArquivo, Pergunta** perguntas, int* totalPerguntas);
void salvarBinario(const char* nomeArquivo, Pergunta* perguntas, int totalPerguntas);
void carregarBinario(const char* nomeArquivo, Pergunta** perguntas, int* totalPerguntas);
void exportarCSV(const char* nomeArquivo, Pergunta* perguntas, int totalPerguntas);

// Funções principais
void listarPerguntas(Pergunta* perguntas, int totalPerguntas);
void adicionarPergunta(Pergunta** perguntas, int* totalPerguntas);
void pesquisarPergunta(Pergunta* perguntas, int totalPerguntas);
void excluirPergunta(Pergunta* perguntas, int* totalPerguntas, int indice);
void removerPergunta(Pergunta* perguntas, int* totalPerguntas, int indice);
void perguntaAleatoriaPorNivel(int nivel, Pergunta* destino, Pergunta* perguntas, int totalPerguntas);

// Funções com interface Raylib
int menuRaylib(void);
void jogarComRaylib(Pergunta* perguntas, int totalPerguntas);

#endif