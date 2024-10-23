#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    printf("Hello world!");

}

int dimensoes(int t) {
    // Se o valor fornecido for válido para as dimensões da matriz
    if (t >= 0 && t < 10) {
        int sala[t][t] = {0};
        return sala[t][t];  // Retorna o valor na posição [valor][valor]
    }

    return -1; // Retorna -1 se o valor estiver fora das dimensões da matriz
}


// Função para definir as salas sujas aleatoriamente
void definirSalasSujas(int sala[t][t], int quantidade) {
    int x, y;
    srand(time(0));  // Inicializa o gerador de números aleatórios com base no tempo atual

    int count = 0;
    while (count < quantidade) {
        // Gera coordenadas aleatórias para linha e coluna
        x = rand() % t;
        y = rand() % t;

        // Verifica se a sala já está suja, para evitar duplicações
        if (sala[x][y] == 0) {
            sala[x][y] = 1;  // Define a sala como suja
            count++;  // Incrementa o contador de salas sujas definidas
        }
    }
}


// Função para gerar uma posição aleatória para o aspirador
void gerarPosicaoAspirador(int sala[t][t], int &xAspirador, int &yAspirador) {
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