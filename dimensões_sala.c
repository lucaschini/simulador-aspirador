#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    printf("Hello world!");

}

// Função para gerar uma posição aleatória para o aspirador
void gerarPosicaoAspirador(int t, int &xAspirador, int &yAspirador) {
    int x, y;
    srand(time(0));  // Inicializa o gerador de números aleatórios com base no tempo atual

    while (true) {
        // Gera coordenadas aleatórias para linha e coluna
        xAspirador = rand() % t;
        yAspirador = rand() % t;

        break;  // Encontrou uma posição válida, então sai do loop
    }
}


void imprimirMatriz(int sala[t][t], int xAspirador, int yAspirador) {
    printf("\nEstado atual das salas (A = Aspirador):\n");
    for (int i = 0; i < t; i++) {
        for (int j = 0; j < t; j++) {
            if (i == xAspirador && j == yAspirador) {
                printf("A ");  // Marca a posição do aspirador
            } else {
                printf("%d ", sala[i][j]);  // 0 para limpo, 1 para sujo
            }
        }
        printf("\n");
    }
}