#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <locale.h>

int main() {

    setlocale(LC_ALL, "portuguese");
    srand(time(NULL));

    int linhas, qtd_1;

    printf("Informe a quantidade de linhas matriz: ");
    scanf("%d", &linhas);

    if(linhas > 10){
        printf("\n A quantidade de Linhas inseridas foi maior que 10, portando o valor será 10.");
        linhas = 10;
    }

    int total_elementos = linhas * linhas;
    printf("\n Informe a quantidade de sujeiras (max: %d): ", total_elementos);
    scanf("%d", &qtd_1);

    if (qtd_1 > total_elementos) {
        printf("Erro: A quantidade de sujeiras não pode ser maior que o total de elementos da matriz.\n");
        return 1;
    }

    int **matriz = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int *)malloc(linhas * sizeof(int));
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < linhas; j++) {
            matriz[i][j] = 0;
        }
    }

    // Adiciona os lixos em posições aleatórias
    int colocados = 0;
    while (colocados < qtd_1) {
        int rand_linha = rand() % linhas;
        int rand_coluna = rand() % linhas;

        // Coloca o 1 na posição se ainda for 0
        if (matriz[rand_linha][rand_coluna] == 0) {
            matriz[rand_linha][rand_coluna] = 1;
            colocados++;
        }
    }

    printf("\nMatriz gerada:\n");
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < linhas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}
