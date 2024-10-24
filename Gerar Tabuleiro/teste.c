#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <locale.h>

// Por definição
// 0 = Piso limpo
// 1 = Piso sujo
// A = Aspirador
// A* = Aspirador encima de uma sujeira

int tamanhoTabuleiro();
int qtdSujeira(int tamanho);
int gerarTabuleiro(int tamanho, int qtd_1);
void printMatriz(int tamanho, int **matriz);
void gerarPosicaoAspirador(int tamanho, int **matriz);

int main() {

    setlocale(LC_ALL, "portuguese");
    srand(time(NULL));

    int tamanho, qtd_1;

    tamanho = tamanhoTabuleiro();
    qtd_1 = qtdSujeira(tamanho);

    int matriz = (int *)malloc(tamanho * sizeof(int));

    matriz = gerarTabuleiro(tamanho, qtd_1);

    gerarPosicaoAspirador(tamanho, matriz);

    printMatriz(tamanho, matriz);



    free(matriz);

    return 0;
}

int tamanhoTabuleiro(){
    int linhas;
    printf("Informe a quantidade de linhas matriz: ");
    scanf("%d", &linhas);

    if(linhas > 10){
        printf("\n A quantidade de Linhas inseridas foi maior que 10, portando o valor será 10.");
        linhas = 10;
    }
    return linhas;
}

int qtdSujeira(int tamanho){
    int error=0, qtd_sujeira;
    do{

        int total_elementos = (tamanho * tamanho);
        printf("\n Informe a quantidade de sujeiras (max: %d): ", total_elementos);
        scanf("%d", &qtd_sujeira);

        if (qtd_sujeira > total_elementos) {
            printf("Erro: A quantidade de sujeiras não pode ser maior que o total de elementos da matriz.\n");
            sleep(2);
            system("cls");
        } else {
            error = 1;
        }
    }while(error != 1);


    return qtd_sujeira;

}

int gerarTabuleiro(int tamanho, int qtd_1){

    int **matriz = (int **)malloc(tamanho * sizeof(int *));
    for (int i = 0; i < tamanho; i++) {
        matriz[i] = (int *)malloc(tamanho * sizeof(int));
    }
    // perguntar pro vinnie

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            matriz[i][j] = 0;
        }
    }


    // Adiciona os lixos em posições aleatórias
    int colocados = 0;
    while (colocados < qtd_1) {
        int rand_linha = rand() % tamanho;
        int rand_coluna = rand() % tamanho;

        // Coloca o 1 na posição se ainda for 0
        if (matriz[rand_linha][rand_coluna] == 0) {
            matriz[rand_linha][rand_coluna] = 1;
            colocados++;
        }
    }

    return matriz;


}

void printMatriz(int tamanho, int **matriz){
    printf("\nMatriz gerada:\n\n");
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {

            if(matriz[i][j] == 2){
                printf(" A ");
            }
            else if (matriz[i][j] == 3){
                printf(" & ");
            }
            else {
                printf(" %d ", matriz[i][j]);
            }
        }
            printf("\n");
    }

}

void gerarPosicaoAspirador(int tamanho, int **matriz) {
    int x, y;

    x = rand() % tamanho;
    y = rand() % tamanho;
    if(matriz[x][y] == 1){
        matriz[x][y] = 3;
    }
    else{
      matriz[x][y] = 2;
    }
}
