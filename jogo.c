#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "interface_raylib.h"

extern Pergunta* perguntas;
extern int totalPerguntas;

void removerPergunta(int indice); // Declarar se estiver em outro arquivo
void perguntaAleatoriaPorNivel(int nivel, Pergunta* p);

int menuRaylib() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Jogo do Milhão - One Piece");
    SetTargetFPS(60);

    const char* opcoes[] = {
        "Jogar",
        "Listar Perguntas",
        "Adicionar Pergunta",
        "Exportar CSV",
        "Pesquisar Pergunta",
        "Excluir Pergunta",
        "Sair"
    };

    int totalOpcoes = sizeof(opcoes) / sizeof(opcoes[0]);
    int selecionado = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) selecionado = (selecionado + 1) % totalOpcoes;
        if (IsKeyPressed(KEY_UP)) selecionado = (selecionado - 1 + totalOpcoes) % totalOpcoes;
        if (IsKeyPressed(KEY_ENTER)) {
            CloseWindow();
            return selecionado + 1;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("JOGO DO MILHÃO - ONE PIECE", 180, 40, 30, DARKBLUE);
        for (int i = 0; i < totalOpcoes; i++) {
            Color cor = (i == selecionado) ? RED : BLACK;
            DrawText(opcoes[i], 350, 150 + i * 40, 20, cor);
        }
        EndDrawing();
    }

    CloseWindow();
    return 7;
}

void esperar(float segundos) {
    double inicio = GetTime();
    while (GetTime() - inicio < segundos && !WindowShouldClose()) {
        BeginDrawing();
        EndDrawing(); // evita travar janela
    }
}

void jogarComRaylib() {
    srand(time(NULL));
    int totalAcertos = 0;
    int perguntasPorNivel[] = {2, 2, 4, 4, 3};

    InitWindow(1000, 600, "Jogo do Milhão - One Piece");
    SetTargetFPS(60);

    for (int nivel = 1; nivel <= 5; nivel++) {
        for (int i = 0; i < perguntasPorNivel[nivel - 1]; i++) {
            Pergunta p;
            perguntaAleatoriaPorNivel(nivel, &p);
            if (!p.ativo) continue;

            char resposta = 0;
            int respondido = 0;

            while (!respondido && !WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);

                DrawText(TextFormat("Nível %d", nivel), 30, 20, 30, DARKBLUE);
                DrawText(p.texto, 30, 80, 24, BLACK);
                for (int j = 0; j < 4; j++) {
                    char letra = 'A' + j;
                    DrawText(TextFormat("%c) %s", letra, p.alternativa[j]), 50, 150 + j * 60, 22, BLACK);
                }

                DrawText("Pressione A, B, C ou D para responder", 30, 450, 20, DARKGRAY);

                if (IsKeyPressed(KEY_A)) { resposta = 'A'; respondido = 1; }
                if (IsKeyPressed(KEY_B)) { resposta = 'B'; respondido = 1; }
                if (IsKeyPressed(KEY_C)) { resposta = 'C'; respondido = 1; }
                if (IsKeyPressed(KEY_D)) { resposta = 'D'; respondido = 1; }

                EndDrawing();
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);
            if (toupper(resposta) == toupper(p.letra)) {
                DrawText("✅ Resposta Correta!", 350, 250, 30, DARKGREEN);
                totalAcertos++;
            } else {
                DrawText(TextFormat("❌ Errado! Correta: %c", p.letra), 300, 250, 30, RED);
                DrawText(TextFormat("Fim de jogo. Acertos: %d", totalAcertos), 250, 320, 24, BLACK);
                EndDrawing();
                esperar(3.0f);
                CloseWindow();
                return;
            }
            EndDrawing();
            esperar(2.0f);
        }

        if (nivel != 5) {
            int continuar = 0;
            while (!continuar && !WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(TextFormat("Nível %d completo! Deseja continuar? (S/N)", nivel), 150, 250, 26, DARKBLUE);
                EndDrawing();

                if (IsKeyPressed(KEY_S)) continuar = 1;
                if (IsKeyPressed(KEY_N)) {
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawText(TextFormat("Você desistiu com %d acertos.", totalAcertos), 250, 300, 26, BLACK);
                    EndDrawing();
                    esperar(2.5f);
                    CloseWindow();
                    return;
                }
            }
        }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("🎉 Parabéns! Você acertou todas (%d)", totalAcertos), 150, 280, 26, DARKGREEN);
    EndDrawing();
    esperar(3.0f);
    CloseWindow();
}
