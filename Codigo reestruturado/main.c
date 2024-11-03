//BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>


//MOVIMENTOS MANUAIS
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_CLEAN 99

//MOVIMENTOS PARA IA
#define CIMA 1
#define BAIXO 2
#define ESQUERDA 3
#define DIREITA 4

// DEFINI��ES:
// 0 = PISO LIMPO
// 1 = PISO SUJO
// A = ASPIRADOR EM CIMA DE PISO LIMPO (2) 20
// & = ASPIRADOR EM CIMA DE PISO SUJO (3) 21
// C = TECLA PARA LIMPAR

// TO SEE: https://www.hashtagtreinamentos.com/como-waze-funciona-python
// Insertion Sort
// https://joaoarthurbm.github.io/eda/posts/insertion-sort/

// FUN��ES DO MENU
void menu_universo(int *escolha);
void menu_controladora(int *escolha);

//  FUN��ES B�SICAS

void tamanhoTabuleiro(int *linhas, int *colunas); //LER TAMANHO DA SALA
int qtdSujeira(int *linhas, int *colunas); //LER QUANTIDADE SUJEIRAS
int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1); //GERAR TABULEIRO
void printMatriz(int *linhas, int *colunas, int **matriz, int escolha, int **visitado); //IMPRIMIR MATRIZ
void gerarPosicaoAspirador(int *linhas, int *colunas, int **matriz); //GERAR ASPIRADOR
void localizarAspirador(int **matriz, int *linhas, int *colunas, int *i, int *j); //LOCALIZAR ASPIRADOR
int avalia(int **matriz, int *linhas, int *colunas); //OBJETIVO CONCLUIDO
void sucessora(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas); //MOVIMENTACAO
void limpar(int movimento, int **matriz, int*i, int *j); //LIMPAR SUJEIRA
void freeMatriz(int **matriz, int *linhas); //LIMPAR MEMORIA ALOCADA
void freeVisitado(int **visitados, int *linhas);//LIMPA MEMORIA ALOCADA
int **copiarMatriz(int **matriz, int *linhas, int *colunas); //FUNCAO COPIAR MATRIZ
void estado(int **matriz, int *linhas, int *colunas, int escolha, int **visitado);

//TESTE IA:
void po_dfs(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int suj);



int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); //PARA LIBERAR ACENTUA��O
    srand(time(NULL)); //PARA GERAR CASAS ALEAT�RIAS
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    int linhas = 0, colunas = 0, qtd_1, escolha, escolha2;
    int pos1, pos2;
    int retorno = 0, tecla;
    int **visitado;

    //GERA OS MENUS
    menu_universo(&escolha);
    menu_controladora(&escolha2);
    system("cls");

    //GERA AS DIMENS�ES E SUJEIRAS DO TABULEIRO
    tamanhoTabuleiro(&linhas, &colunas);
    qtd_1 = qtdSujeira(&linhas, &colunas);
    system("cls");

    //GERA O TABULEIRO
    int **matriz = gerarTabuleiro(&linhas, &colunas, qtd_1);

    //ALOCA MEM�RIA PARA O VISITADO
    visitado = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        visitado[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            visitado[i][j] = 0;
        }
    }

    //GERA ASPIRADOR
    gerarPosicaoAspirador(&linhas, &colunas, matriz);
    localizarAspirador(matriz, &linhas, &colunas, &pos1, &pos2);

    //CASO SEJA MANUAL
    if(escolha2 == 1){
        while(retorno!= 1){
            printf("\n\n\n\n\n");
            estado(matriz, &linhas, &colunas, escolha, visitado); //GERA O ESTADO
            printMatriz(&linhas, &colunas, matriz, escolha, visitado); //PRINTA O ESTADO
            printf("\n\n\n\n\n");
            tecla = getch(); //PEGA A TECLA
            sucessora(tecla, &pos1, &pos2, matriz, &linhas, &colunas); //LE OS MOVIMENTOS
            limpar(tecla, matriz, &pos1, &pos2); //FAZ A LIMPEZA MANUAL
            retorno = avalia(matriz, &linhas, &colunas); //FUN��O OBJETIVO
            system("cls");
        }
    }

    //CASO SEJA CONTROLADO POR IA
    if(escolha2 == 2){
        if(escolha == 1){
            //OBSERV�VEL

        }else if (escolha == 2) {
            po_dfs(matriz, &linhas, &colunas, &pos1, &pos2, visitado, qtd_1);
            system("cls");
            printf("\n Sala Limpa! \n");
        }else if(escolha == 3){
            //PARCIAL B
        }
    }


    //LIBERA A MEM�RIA DAS CASAS VISITADAS
    freeVisitado(visitado, &linhas);

    //LIBERA A MEMORIA DA MATRIZ
    freeMatriz(matriz, &linhas);
    return 0;
}


void menu_universo(int *escolha) {
    int tecla = 0;
    *escolha = 1; //ATRIBUIMOS VALOR A VARIAVEL PARA O MENU COME�AR NA PRIMEIRA OP��O

    while (1) {
        system("cls");

        printf("\n\n\n\t\t\t   ******************* Simulador aspirador ********************\n");
        printf("\t\t\t   *                  | Selecione a busca |                   *\n");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   * %s Universo Observavel                                   *\n", (*escolha == 1) ? "->" : "  ");
        printf("\t\t\t   * %s Universo Parcialmente observavel A                    *\n", (*escolha == 2) ? "->" : "  ");
        printf("\t\t\t   * %s Universo Parcialmente observavel B                    *\n", (*escolha == 3) ? "->" : "  ");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   ************************************************************\n");

        tecla = getch(); //L� A TECLA PRESSIONADA

        //NAVEGA��O
        if (tecla == 72) {
            if (*escolha > 1) (*escolha)--; //SOBE PARA A POSI��O ANTERIOR
        } else if (tecla == 80) {
            if (*escolha < 3) (*escolha)++; //DESCE PARA A PR�XIMA SE��O
        } else if (tecla == 27) {
            return *escolha; //RETORNA O NOVO VALOR DA ESCOLHA
        } else if (tecla == 13) {
            return -1; //RETORNA -1 CASO O USU�RIO DE ESC
        }
    }
}


void menu_controladora(int *escolha) {
    int tecla = 0;
    *escolha = 1; //ATRIBUIMOS VALOR A VARIAVEL PARA O MENU COME�AR NA PRIMEIRA OP��O

    while (1) {
        system("cls");

        printf("\n\n\n\t\t\t   ******************* Simulador aspirador ********************\n");
        printf("\t\t\t   *               | Selecione a controladora |               *\n");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   * %s Controladora Manual                                   *\n", (*escolha == 1) ? "->" : "  ");
        printf("\t\t\t   * %s Controladora por IA                                   *\n", (*escolha == 2) ? "->" : "  ");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   ************************************************************\n");

        tecla = getch(); //L� A TECLA PRESSIONADA

        //NAVEGA��O
        if (tecla == 72) {
            if (*escolha > 1) (*escolha)--; //SOBE PARA A POSI��O ANTERIOR
        } else if (tecla == 80) {
            if (*escolha < 2) (*escolha)++; //DESCE PARA A PR�XIMA SE��O
        } else if (tecla == 27) {
            return *escolha; //RETORNA O NOVO VALOR DA ESCOLHA
        } else if (tecla == 13) {
            return -1; //RETORNA -1 CASO O USU�RIO DE ESC
        }
    }
}


void tamanhoTabuleiro(int *linhas, int *colunas) {
    printf(" Informe a quantidade de linhas matriz: ");
    scanf("%d",linhas);
    if(*linhas > 10){
        printf("\n A quantidade de Linhas inseridas foi maior que 10, portando o valor ser� 10.\n");
        *linhas = 10;
    }else if(*linhas < 1){
        printf("\n A quantidade de Linhas inseridas foi menor que 1, portando o valor ser� 1.\n");
        *linhas = 1;
    }

    printf("\n Informe a quantidade de colunas matriz: ");
    scanf("%d",colunas);
    if(*colunas > 10){
        printf("\n A quantidade de Colunas inseridas foi maior que 10, portando o valor ser� 10.\n");
        *colunas = 10;
    }else if(*colunas < 1){
        printf("\n A quantidade de Colunas inseridas foi menor que 1, portando o valor ser� 1.\n");
        *colunas = 1;
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
            printf("\n Erro: A quantidade de sujeiras n�o pode ser maior que o total de elementos da matriz.\n");
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

    // Adiciona os lixos em posi��es aleat�rias
    int colocados = 0;
    while (colocados < qtd_1) {
        int rand_linha = rand() % (*linhas);
        int rand_coluna = rand() % (*colunas);


        // Coloca o 1 na posi��o se ainda for 0
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

void printMatriz(int *linhas, int *colunas, int **matriz, int escolha, int **visitado) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {

            if (matriz[i][j] == 20) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("\t A ");;
            } else if (matriz[i][j] == 21) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| FOREGROUND_INTENSITY);
                printf("\t & ");;
            }else if((escolha == 2 || escolha == 3) && visitado[i][j] == 0){
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("\t ? ");
            }else {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
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

    if ((movimento == KEY_UP) && (*i) > 0) {
        (*i)--;
    } else if ((movimento == KEY_DOWN) && (*i) < *linhas - 1) {
        (*i)++;
    } else if ((movimento == KEY_RIGHT) && (*j) < *colunas - 1) {
        (*j)++;
    } else if ((movimento == KEY_LEFT) && (*j) > 0) {
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

void freeVisitado(int **visitados, int *linhas){
    for (int i = 0; i < linhas; i++) {
        free(visitados[i]);
    }
    free(visitados);
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

void estado(int **matriz, int *linhas, int *colunas, int escolha, int **visitado) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (escolha == 1) {

    }
    else if (escolha == 2) {
        for (int i = 0; i < *linhas; i++) {
            for (int j = 0; j < *colunas; j++) {
                if (matriz[i][j] == 20 || matriz[i][j] == 21) {
                    visitado[i][j] = 1;
                }
            }
        }
    }
    else if (escolha == 3) {
        for (int i = 0; i < *linhas; i++) {
            for (int j = 0; j < *colunas; j++) {
                if (matriz[i][j] == 20 || matriz[i][j] == 21) {
                    visitado[i][j] = 1;
                    if (i + 1 < *linhas) visitado[i + 1][j] = 1;                                 // C�lula abaixo
                    if (i - 1 >= 0) visitado[i - 1][j] = 1;                                      // C�lula acima
                    if (j + 1 < *colunas) visitado[i][j + 1] = 1;                                // C�lula � direita
                    if (j - 1 >= 0) visitado[i][j - 1] = 1;                                      // C�lula � esquerda
                    if ((i + 1 < *linhas)&&(j + 1 < *colunas)) visitado[i + 1][j+1] = 1;         // C�lula abaixo direita
                    if ((i + 1 < *linhas)&&(j - 1 >= 0)) visitado[i + 1][j - 1] = 1;             // C�lula abaixo esquerda
                    if ((i - 1 >= 0)&&(j + 1 < *colunas)) visitado[i - 1][j + 1] = 1;            // C�lula cima direita
                    if ((i - 1 >= 0)&&(j - 1 >= 0)) visitado[i - 1][j - 1] = 1;                  // C�lula esquerda direita
                }
            }
        }
    }
}

void po_dfs(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int suj) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    system("cls");
    estado(matriz, linhas, colunas, 2, visitado);
    printMatriz(linhas, colunas, matriz, 2, visitado);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("\n\n\t Quantidade de sujeiras: %d", suj);
    sleep(1);

    // Marca a c�lula atual como visitada
    visitado[*i][*j] = 1;

    // Limpa a sujeira na posi��o atual, se houver
    sleep(1);
    if(matriz[*i][*j] == 21){
        limpar(KEY_CLEAN, matriz, i, j);
        suj --;
    }

    // Vetores de dire��o para cima, baixo, esquerda e direita
    int mov_x[] = {-1, 1, 0, 0};
    int mov_y[] = {0, 0, -1, 1};

    // Percorre todas as dire��es poss�veis
    for (int d = 0; d < 4; d++) {
        int novo_i = *i + mov_x[d];
        int novo_j = *j + mov_y[d];

        // Verifica se a nova posi��o est� dentro dos limites e ainda n�o foi visitada
        if (novo_i >= 0 && novo_i < *linhas && novo_j >= 0 && novo_j < *colunas && visitado[novo_i][novo_j] == 0) {
            // Redefine a posi��o atual como limpa antes de mover para a pr�xima c�lula
            matriz[*i][*j] = 0;

            // Move o aspirador para a nova posi��o
            *i = novo_i;
            *j = novo_j;

            // Atualiza a posi��o do aspirador na nova c�lula
            if(matriz[*i][*j] == 0){matriz[*i][*j] = 20;}
            if(matriz[*i][*j] == 1){matriz[*i][*j] = 21;}

            // Chama a fun��o recursivamente para a nova posi��o
            po_dfs(matriz, linhas, colunas, i, j, visitado, suj);

            // Retorna � posi��o anterior ap�s explorar o caminho, redefinindo a posi��o atual
            matriz[*i][*j] = 0;
            *i -= mov_x[d];
            *j -= mov_y[d];

            // Recoloca o aspirador na c�lula anterior ap�s o retorno
            matriz[*i][*j] = 20;
        }
    }
}

