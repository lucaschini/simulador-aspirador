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

// TO SEE: https://www.hashtagtreinamentos.com/como-waze-funciona-python
// Insertion Sort
// https://joaoarthurbm.github.io/eda/posts/insertion-sort/

void tamanhoTabuleiro(int *linhas, int *colunas); //LER TAMANHO DA SALA

int qtdSujeira(int *linhas, int *colunas); //LER QUANTIDADE SUJEIRAS

int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1); //GERAR TABULEIRO

void printMatriz(int *linhas, int *colunas, int **matriz); //IMPRIMIR MATRIZ

void gerarPosicaoAspirador(int *linhas, int *colunas, int **matriz); //GERAR ASPIRADOR

void localizarAspirador(int **matriz, int *linhas, int *colunas, int *i, int *j); //LOCALIZAR ASPIRADOR

int avalia(int **matriz, int *linhas, int *colunas); //OBJETIVO CONCLUIDO

void sucessora(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas); //MOVIMENTACAO

void limpar(int movimento, int **matriz, int*i, int *j); //LIMPAR SUJEIRA

void freeMatriz(int **matriz, int *linhas); //LIMPAR MEMORIA ALOCADA

int **copiarMatriz(int **matriz, int *linhas, int *colunas); //FUNCAO COPIAR MATRIZ



int main() {
    setlocale(LC_ALL, "portuguese");
    srand(time(NULL));


    int linhas = 0, colunas = 0, qtd_1;
    int pos1, pos2;
    int retorno = 0, tecla;
    int copia;

    tamanhoTabuleiro(&linhas, &colunas);
    qtd_1 = qtdSujeira(&linhas, &colunas);

    system("cls");

    int **matriz = gerarTabuleiro(&linhas, &colunas, qtd_1);

    gerarPosicaoAspirador(&linhas, &colunas, matriz);
    localizarAspirador(matriz, &linhas, &colunas, &pos1, &pos2);

    while (retorno != 1) {
        copia = copiarMatriz(matriz, &linhas, &colunas);
        printf("\n\n\n\n\n");
        printMatriz(&linhas, &colunas, matriz);
        printf("\n\n\n\n\n");
        tecla = getch();
        sucessora(tecla, &pos1, &pos2, matriz, &linhas, &colunas);
        limpar(tecla, matriz, &pos1, &pos2);
        retorno = avalia(matriz, &linhas, &colunas);
        system("cls");
    }

    freeMatriz(matriz, &linhas);
    return 0;
}


void tamanhoTabuleiro(int *linhas, int *colunas) {
    printf(" Informe a quantidade de linhas matriz: ");
    scanf("%d",linhas);
    printf(" Informe a quantidade de colunas matriz: ");
    scanf("%d",colunas);

    if(*linhas > 10){
        printf("\n A quantidade de Linhas inseridas foi maior que 10, portando o valor será 10.\n");
        *linhas = 10;
    } else if(*colunas > 10){
        printf("\n A quantidade de Colunas inseridas foi maior que 10, portando o valor será 10.\n");
        *colunas = 10;
    }
}

int qtdSujeira(int *linhas, int *colunas) {
    int error = 0, qtd_sujeira, tamanho = 0;
    tamanho = (*linhas) * (*colunas);
    do {
        printf("\n Informe a quantidade de sujeiras (max: %d): ", tamanho);
        scanf("%d", &qtd_sujeira);
        sleep(1);
        system("cls");

        if (qtd_sujeira > tamanho) {
            printf("\n Erro: A quantidade de sujeiras não pode ser maior que o total de elementos da matriz.\n");
            sleep(1);
            system("cls");
        } else {
            error = 1;
        }
    } while (error != 1);

    return qtd_sujeira;
}

int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1) {
    int **matriz = (int **)malloc(*linhas * sizeof(int *));
    for (int i = 0; i < *linhas; i++) {
        matriz[i] = (int *)malloc(*colunas * sizeof(int));
    }

    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            matriz[i][j] = 0;
        }
    }



    // Adiciona os lixos em posições aleatórias
    int colocados = 0;
    while (colocados < qtd_1) {
        int rand_linha = rand() % (*linhas);
        int rand_coluna = rand() % (*colunas);


        // Coloca o 1 na posição se ainda for 0
        if (matriz[rand_linha][rand_coluna] == 0) {
            matriz[rand_linha][rand_coluna] = 1;
            colocados++;
        }
    }

    return matriz;
}

void gerarPosicaoAspirador(int *linhas, int *colunas, int **matriz) {
    int x, y;

    x = rand() % *linhas;
    y = rand() % *colunas;
    if (matriz[x][y] == 1) {
        matriz[x][y] = 21;
    } else {
        matriz[x][y] = 20;
    }
}

void printMatriz(int *linhas, int *colunas, int **matriz) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {

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
            printf("\n\n\n\n");
    }

}

void localizarAspirador(int **matriz, int *linhas, int *colunas, int *i, int *j) {
    for (int x = 0; x < *linhas; x++) {
        for (int y = 0; y < *colunas; y++) {
            if (matriz[x][y] == 20 || matriz[x][y] == 21) {
                *i = x;
                *j = y;
                return;
            }
        }
    }
}

void sucessora(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas) {
    int aux_i, aux_j;
    aux_i = *i;
    aux_j = *j;

    if (movimento == KEY_UP && (*i) > 0) {
        (*i)--;
    } else if (movimento == KEY_DOWN && (*i) < *linhas - 1) {
        (*i)++;
    } else if (movimento == KEY_RIGHT && (*j) < *colunas - 1) {
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

int avalia(int **matriz, int *linhas, int *colunas) {
    for (int x = 0; x < *linhas; x++) {
        for (int y = 0; y < *colunas; y++) {
            if (matriz[x][y] == 1 || matriz[x][y] == 21) {
                return 0;
            }
        }
    }
    return 1;
}

void freeMatriz(int **matriz, int *linhas) {
    for (int i = 0; i < *linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

int **copiarMatriz(int **matriz, int *linhas, int *colunas) {
    int **novaMatriz = (int **)malloc(*linhas * sizeof(int *));
    for (int i = 0; i < *linhas; i++) {
        novaMatriz[i] = (int *)malloc(*colunas * sizeof(int));
        for (int j = 0; j < *colunas; j++) {
            novaMatriz[i][j] = matriz[i][j];
        }
    }
    return novaMatriz;
}
