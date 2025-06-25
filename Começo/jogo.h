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

// Variáveis globais
extern Pergunta* perguntas;
extern int totalPerguntas;

// Funções
void carregarPerguntasCSV(const char* nomeArquivo);
void salvarBinario(const char* nomeArquivo);
void carregarBinario(const char* nomeArquivo);
void exportarCSV(const char* nomeArquivo);
void listarPerguntas(void);
void adicionarPergunta(void);
void pesquisarPergunta(void);
void excluirPergunta(int indice);
void removerPergunta(int indice);
void perguntaAleatoriaPorNivel(int nivel, Pergunta* p);

// Funções específicas com interface gráfica (caso estejam em jogo_raylib.c)
int menuRaylib(void);
void jogarComRaylib(void);

#endif