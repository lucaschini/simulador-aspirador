#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_CLEAN 99

// DEFINIÇÕES:
// 0 = PISO LIMPO
// 1 = PISO SUJO
// A = ASPIRADOR EM CIMA DE PISO LIMPO (2) 20
// & = ASPIRADOR EM CIMA DE PISO SUJO (3) 21
// C = TECLA PARA LIMPAR

int tamanhoTabuleiro(); //LER TAMANHO DA SALA
int qtdSujeira(int tamanho); //LER QUANTIDADE SUJEIRAS
int **gerarTabuleiro(int tamanho, int qtd_1); //GERAR TABULEIRO
void printMatriz(int tamanho, int **matriz); //IMPRIMIR MATRIZ
void gerarPosicaoAspirador(int tamanho, int **matriz); //GERAR ASPIRADOR
void localizarAspirador(int **matriz, int tamanho, int *i, int *j); //LOCALIZAR ASPIRADOR
int avalia(int **matriz, int tamanho); //OBJETIVO CONCLUIDO
void sucessora(int movimento, int *i, int *j, int **matriz, int tamanho); //MOVIMENTACAO
void limpar(int movimento, int **matriz, int*i, int *j); //LIMPAR SUJEIRA
void freeMatriz(int **matriz, int tamanho); //LIMPAR MEMORIA ALOCADA
int **copiarMatriz(int **matriz, int tamanho); //FUNCAO COPIAR MATRIZ



int main() {
    setlocale(LC_ALL, "portuguese");
    srand(time(NULL));


    int tamanho, qtd_1;
    int pos1, pos2;
    int retorno = 0, tecla;
    int copia;

    tamanho = tamanhoTabuleiro();
    qtd_1 = qtdSujeira(tamanho);

    system("cls");

    int **matriz = gerarTabuleiro(tamanho, qtd_1);

    gerarPosicaoAspirador(tamanho, matriz);
    localizarAspirador(matriz, tamanho, &pos1, &pos2);

    while (retorno != 1) {
        copia = copiarMatriz(matriz, tamanho);
        printf("\n\n\n\n\n");
        printMatriz(tamanho, matriz);
        printf("\n\n\n\n\n");
        tecla = getch();
        sucessora(tecla, &pos1, &pos2, matriz, tamanho);
        limpar(tecla, matriz, &pos1, &pos2);
        retorno = avalia(matriz, tamanho);
        system("cls");
    }

    freeMatrix(matriz, tamanho);
    return 0;
}


int tamanhoTabuleiro() {
    int linhas;
    printf(" Informe a quantidade de linhas matriz: ");
    scanf("%d", &linhas);

    if(linhas > 10){
        printf("\n A quantidade de Linhas inseridas foi maior que 10, portando o valor será 10.\n");
        linhas = 10;
    }
    return linhas;
}

int qtdSujeira(int tamanho) {
    int error = 0, qtd_sujeira;
    do {
        int total_elementos = (tamanho * tamanho);
        printf("\n Informe a quantidade de sujeiras (max: %d): ", total_elementos);
        scanf("%d", &qtd_sujeira);
        sleep(1);
        system("cls");

        if (qtd_sujeira > total_elementos) {
            printf("\n Erro: A quantidade de sujeiras não pode ser maior que o total de elementos da matriz.\n");
            sleep(1);
            system("cls");
        } else {
            error = 1;
        }
    } while (error != 1);

    return qtd_sujeira;
}

int **gerarTabuleiro(int tamanho, int qtd_1) {
    int **matriz = (int **)malloc(tamanho * sizeof(int *));
    for (int i = 0; i < tamanho; i++) {
        matriz[i] = (int *)malloc(tamanho * sizeof(int));
    }

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

void gerarPosicaoAspirador(int tamanho, int **matriz) {
    int x, y;

    x = rand() % tamanho;
    y = rand() % tamanho;
    if (matriz[x][y] == 1) {
        matriz[x][y] = 21;
    } else {
        matriz[x][y] = 20;
    }
}

void printMatriz(int tamanho, int **matriz) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {

            if (matriz[i][j] == 20) {

                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("\t A ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else if (matriz[i][j] == 21) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| FOREGROUND_INTENSITY);
                printf(" \t & ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            } else {

                printf("\t %d", matriz[i][j]);
            }
        }
            printf("\n\n");
    }

}

void localizarAspirador(int **matriz, int tamanho, int *i, int *j) {
    for (int x = 0; x < tamanho; x++) {
        for (int y = 0; y < tamanho; y++) {
            if (matriz[x][y] == 20 || matriz[x][y] == 21) {
                *i = x;
                *j = y;
                return;
            }
        }
    }
}

void sucessora(int movimento, int *i, int *j, int **matriz, int tamanho) {
    int aux_i, aux_j;
    aux_i = *i;
    aux_j = *j;

    if (movimento == KEY_UP && (*i) > 0) {
        (*i)--;
    } else if (movimento == KEY_DOWN && (*i) < tamanho - 1) {
        (*i)++;
    } else if (movimento == KEY_RIGHT && (*j) < tamanho - 1) {
        (*j)++;
    } else if (movimento == KEY_LEFT && (*j) > 0) {
        (*j)--;
    } else {
        return;
    }

    if (matriz[aux_i][aux_j] == 20) {
        matriz[aux_i][aux_j] = 0;
    } else if (matriz[aux_i][aux_j] == 21) {
        matriz[aux_i][aux_j] = 1;
    }

    if (matriz[*i][*j] == 0) {
        matriz[*i][*j] = 20;
    } else if (matriz[*i][*j] == 1) {
        matriz[*i][*j] = 21;
    }
}

void limpar(int movimento, int **matriz, int *i, int *j) {
    if (movimento == KEY_CLEAN) {
        if (matriz[*i][*j] == 21) {
            matriz[*i][*j] = 20;
        }
    }
}

int avalia(int **matriz, int tamanho) {
    for (int x = 0; x < tamanho; x++) {
        for (int y = 0; y < tamanho; y++) {
            if (matriz[x][y] == 1 || matriz[x][y] == 21) {
                return 0;
            }
        }
    }
    return 1;
}

void freeMatrix(int **matriz, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int **copiarMatriz(int **matriz, int tamanho) {
    int **novaMatriz = (int **)malloc(tamanho * sizeof(int *));
    for (int i = 0; i < tamanho; i++) {
        novaMatriz[i] = (int *)malloc(tamanho * sizeof(int));
        for (int j = 0; j < tamanho; j++) {
            novaMatriz[i][j] = matriz[i][j];
        }
    }
    return novaMatriz;
}
