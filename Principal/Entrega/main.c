//BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#include "pilha.h"

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

// DEFINIÇÕES:
// 0 = PISO LIMPO
// 1 = PISO SUJO
// A = ASPIRADOR EM CIMA DE PISO LIMPO (2) 20
// & = ASPIRADOR EM CIMA DE PISO SUJO (3) 21
// C = TECLA PARA LIMPAR

// TO SEE: https://www.hashtagtreinamentos.com/como-waze-funciona-python
// Insertion Sort
// https://joaoarthurbm.github.io/eda/posts/insertion-sort/

// FUNÇÕES DO MENU
void menu_universo(int *escolha);
void menu_controladora(int *escolha);

//  FUNÇÕES BÁSICAS

void tamanhoTabuleiro(int *linhas, int *colunas); //LER TAMANHO DA SALA
int qtdSujeira(int *linhas, int *colunas); //LER QUANTIDADE SUJEIRAS
int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1); //GERAR TABULEIRO
void printMatriz(int *linhas, int *colunas, int **matriz, int escolha, int **visitado, int **visualizado); //IMPRIMIR MATRIZ
void gerarPosicaoAspirador(int *linhas, int *colunas, int **matriz); //GERAR ASPIRADOR
void localizarAspirador(int **matriz, int *linhas, int *colunas, int *i, int *j); //LOCALIZAR ASPIRADOR
int avalia(int **matriz, int *linhas, int *colunas); //OBJETIVO CONCLUIDO
void sucessora(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas); //MOVIMENTACAO
void limpar(int movimento, int **matriz, int*i, int *j); //LIMPAR SUJEIRA
void freeMatriz(int **matriz, int *linhas); //LIMPAR MEMORIA ALOCADA
void freeVisitado(int **visitados, int *linhas);//LIMPA MEMORIA ALOCADA
void freeVisualizado(int **visualizado, int *linhas);
int **copiarMatriz(int **matriz, int *linhas, int *colunas); //FUNCAO COPIAR MATRIZ
void estado(int **matriz, int *linhas, int *colunas, int escolha, int **visitado, int **visualizado);

//TESTE IA A:
void po_dfs(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int *suj, Pilha *p); //PARCIALMENTE OBSERVAVEL DEPTH-FIRST SEARCH
int visitados(int *i, int *j, int **visitado, int *linhas, int *colunas); //VERIFICA SE AS CASAS ADJACENTES FORAM VISITADAS (VERIFICA SE ESTA PRESO)
void backtracking_A(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int *suj, Pilha *p); //FAZ O BACKTRACKING DANDO POP NA PILHA

//TESTE IA B:
void dfs(int **matriz, int **visitado, int *i, int *j, int *linhas, int *colunas, int escolha, int **visualizado, Pilha *p);
int objetivo(int **matriz, int *linhas, int *colunas, int **visualizado);
void backtracking_B(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int *suj, Pilha *p, int **visualizado);

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); //PARA LIBERAR ACENTUAÇÃO
    srand(time(NULL)); //PARA GERAR CASAS ALEATÓRIAS
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int linhas = 0, colunas = 0, qtd_1, escolha, escolha2;
    int pos1, pos2;
    int retorno = 0, tecla;
    int **visitado, **visualizado;
    Pilha *IA_A = CriaPilha(); //CRIO A PILHA PARA O CENARIO PARCIALMENTE A
    Pilha *backtracking = CriaPilha();

    //GERA OS MENUS
    menu_universo(&escolha);
    menu_controladora(&escolha2);
    system("cls");

    //GERA AS DIMENSÕES E SUJEIRAS DO TABULEIRO
    tamanhoTabuleiro(&linhas, &colunas);
    qtd_1 = qtdSujeira(&linhas, &colunas);
    int sujeira = qtd_1;
    system("cls");

    //GERA O TABULEIRO
    int **matriz = gerarTabuleiro(&linhas, &colunas, qtd_1);

    //ALOCA MEMÓRIA PARA O VISITADO
    visitado = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        visitado[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            visitado[i][j] = 0;
        }
    }

    visualizado = (int **)malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        visualizado[i] = (int *)malloc(colunas * sizeof(int));
        for (int j = 0; j < colunas; j++) {
            visualizado[i][j] = 0;
        }
    }

    //GERA ASPIRADOR
    gerarPosicaoAspirador(&linhas, &colunas, matriz);
    localizarAspirador(matriz, &linhas, &colunas, &pos1, &pos2);

    //CASO SEJA MANUAL
    if(escolha2 == 1){
        while (retorno != 1) {
            printf("\n\n\n\n\n");
            estado(matriz, &linhas, &colunas, escolha, visitado, visualizado); //GERA O ESTADO
            printMatriz(&linhas, &colunas, matriz, escolha, visitado, visualizado); //PRINTA O ESTADO
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
            //OBSERVÁVEL

        }else if (escolha == 2) {
            //PARCIALMENTE OBSERVÁVEL A
            while(sujeira != 0){ //CASO AINDA TENHAM SUJEIRAS
                po_dfs(matriz, &linhas, &colunas, &pos1, &pos2, visitado, &sujeira, IA_A); //REALIZ O DFS RECURSIVAMENTE ATÉ TRAVAR EM UMA CASA OU LIMPAR TUDO
                if(visitados(&pos1, &pos2, visitado, &linhas, &colunas) == 1){ //SE ESTIVER TRAVADO
                    backtracking_A(matriz, &linhas, &colunas, &pos1, &pos2, visitado, &sujeira, IA_A); //FAZ O BACKTRACKING ATÉ ONDE NECESSARIO (ATÉ NAO ESTAR MAIS PRESO)
                }
            }
                system("cls");
                estado(matriz, &linhas, &colunas, escolha, visitado, visualizado); //GERA O ESTADO
                printMatriz(&linhas, &colunas, matriz, escolha, visitado, visualizado); //PRINTA O ESTADO
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                printf("\n\n\tAinda tem Sujeira no Ambiente? Nao (Sala Limpa!) \n"); //SALA LIMPA!
                imprimePilha(IA_A); //IMPRIME A PILHA DE MOVIMENTOS
                libera(IA_A); //LIBERA A PILHA DA MEMÓRIA

        }else if(escolha == 3){
            //PARCIAL B
            estado(matriz, &linhas, &colunas, escolha, visitado, visualizado);
            printMatriz(&linhas, &colunas, matriz, escolha, visitado, visualizado);
            sleep(1);
            system("cls");
//            diagonal(matriz, &pos1, &pos2, escolha, visitado, &linhas, &colunas, visualizado);
            while(objetivo(matriz, &linhas, &colunas, visualizado) == 0){
                dfs(matriz, visitado, &pos1, &pos2, &linhas, &colunas, escolha, visualizado, backtracking);
                printf("preso");
                backtracking_B(matriz, &linhas, &colunas, &pos1, &pos2, visitado, NULL, backtracking, visualizado);
                system("cls");
                estado(matriz, &linhas, &colunas, escolha, visitado, visualizado);
                printMatriz(&linhas, &colunas, matriz, escolha, visitado, visualizado);
            }
            libera(backtracking_B);
        }
    }

    //LIBERA A MEMÓRIA DAS CASAS VISITADAS
    freeVisitado(visitado, &linhas);
    freeVisualizado(visualizado, &linhas);

    //LIBERA A MEMORIA DA MATRIZ
    freeMatriz(matriz, &linhas);
    return 0;
}


void menu_universo(int *escolha) {
    int tecla = 0;
    *escolha = 1; //ATRIBUIMOS VALOR A VARIAVEL PARA O MENU COMEÇAR NA PRIMEIRA OPÇÃO

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

        tecla = getch(); //LÊ A TECLA PRESSIONADA

        //NAVEGAÇÃO
        if (tecla == 72) {
            if (*escolha > 1) (*escolha)--; //SOBE PARA A POSIÇÃO ANTERIOR
        } else if (tecla == 80) {
            if (*escolha < 3) (*escolha)++; //DESCE PARA A PRÓXIMA SEÇÃO
        } else if (tecla == 27) {
            return *escolha; //RETORNA O NOVO VALOR DA ESCOLHA
        } else if (tecla == 13) {
            return -1; //RETORNA -1 CASO O USUÁRIO DE ESC
        }
    }
}

void menu_controladora(int *escolha) {
    int tecla = 0;
    *escolha = 1; //ATRIBUIMOS VALOR A VARIAVEL PARA O MENU COMEÇAR NA PRIMEIRA OPÇÃO

    while (1) {
        system("cls");

        printf("\n\n\n\t\t\t   ******************* Simulador aspirador ********************\n");
        printf("\t\t\t   *               | Selecione a controladora |               *\n");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   * %s Controladora Manual                                   *\n", (*escolha == 1) ? "->" : "  ");
        printf("\t\t\t   * %s Controladora por IA                                   *\n", (*escolha == 2) ? "->" : "  ");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   ************************************************************\n");

        tecla = getch(); //LÊ A TECLA PRESSIONADA

        //NAVEGAÇÃO
        if (tecla == 72) {
            if (*escolha > 1) (*escolha)--; //SOBE PARA A POSIÇÃO ANTERIOR
        } else if (tecla == 80) {
            if (*escolha < 2) (*escolha)++; //DESCE PARA A PRÓXIMA SEÇÃO
        } else if (tecla == 27) {
            return *escolha; //RETORNA O NOVO VALOR DA ESCOLHA
        } else if (tecla == 13) {
            return -1; //RETORNA -1 CASO O USUÁRIO DE ESC
        }
    }
}

void tamanhoTabuleiro(int *linhas, int *colunas) {
    printf(" Informe a quantidade de linhas matriz: ");
    scanf("%d",linhas);
    if(*linhas > 10){
        printf("\n A quantidade de Linhas inseridas foi maior que 10, portando o valor será 10.\n");
        *linhas = 10;
    }else if(*linhas < 1){
        printf("\n A quantidade de Linhas inseridas foi menor que 1, portando o valor será 1.\n");
        *linhas = 1;
    }

    printf("\n Informe a quantidade de colunas matriz: ");
    scanf("%d",colunas);
    if(*colunas > 10){
        printf("\n A quantidade de Colunas inseridas foi maior que 10, portando o valor será 10.\n");
        *colunas = 10;
    }else if(*colunas < 1){
        printf("\n A quantidade de Colunas inseridas foi menor que 1, portando o valor será 1.\n");
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

void printMatriz(int *linhas, int *colunas, int **matriz, int escolha, int **visitado, int **visualizado) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {

            if (matriz[i][j] == 20) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("\t A ");;
            } else if (matriz[i][j] == 21) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE| FOREGROUND_INTENSITY);
                printf("\t & ");;
            }else if((escolha == 2 || escolha == 3) && visitado[i][j] == 0 && visualizado[i][j] == 0){
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("\t ? ");
            }else if(visitado[i][j] == 1) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED);
                printf("\t %d", matriz[i][j]);
            }else{
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

void freeVisualizado(int **visualizado, int *linhas){
    for (int i = 0; i < linhas; i++) {
        free(visualizado[i]);
    }
    free(visualizado);
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

void estado(int **matriz, int *linhas, int *colunas, int escolha, int **visitado, int **visualizado) {
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
                    if (i + 1 < *linhas) visualizado[i + 1][j] = 1;                                 // C�lula abaixo
                    if (i - 1 >= 0) visualizado[i - 1][j] = 1;                                      // C�lula acima
                    if (j + 1 < *colunas) visualizado[i][j + 1] = 1;                                // C�lula � direita
                    if (j - 1 >= 0) visualizado[i][j - 1] = 1;                                      // C�lula � esquerda
                    if ((i + 1 < *linhas)&&(j + 1 < *colunas)) visualizado[i + 1][j+1] = 1;         // C�lula abaixo direita
                    if ((i + 1 < *linhas)&&(j - 1 >= 0)) visualizado[i + 1][j - 1] = 1;             // C�lula abaixo esquerda
                    if ((i - 1 >= 0)&&(j + 1 < *colunas)) visualizado[i - 1][j + 1] = 1;            // C�lula cima direita
                    if ((i - 1 >= 0)&&(j - 1 >= 0)) visualizado[i - 1][j - 1] = 1;                  // C�lula esquerda direita
                }
            }
        }
    }
}

//PARCIALMENTE OBSERVÁVEL A

void po_dfs(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int *suj, Pilha *p) {

    if(*suj == 0){
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //IMPORTA PARA MUDAR A COR

    system("cls");
    estado(matriz, linhas, colunas, 2, visitado, visitado); //USA O ESTADO PARA SABER ONDE É VISITADO E ONDE NÃO É
    printMatriz(linhas, colunas, matriz, 2, visitado, visitado); //PRINTA A MATRIZ
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); //MUDA A COR PARA BRANCO
    printf("\n\n\tAinda tem sujeira no Ambiente? SIM"); //PRINTA A QUANTIDADE DE SUJEIRAS RESTANTES

    //MARCA COMO VISITADO
    visitado[*i][*j] = 1;

    //USA A FUNÇÃO LIMPAR PARA LIMPAR
    sleep(1);
    if(matriz[*i][*j] == 21 || matriz[*i][*j] == 1){
        limpar(KEY_CLEAN, matriz, i, j);
        (*suj) --;
    }
    matriz[*i][*j] = 0;

    //SALVA A POSIÇÃO ATUAL NA PILHA
    push(p, *j);
    push(p, *i);

    //QND AS POS ADJ FOREM VIS == 1, ELE FAZ BACKTRACKING ATÉ ALGUMA NAO SER

    //CRIA UMA LISTA DE MOVIMENTOS PRIORITÁRIOS
    int mov_x[] = {-1, 0, 1, 0};
    int mov_y[] = {0, -1, 0, 1};

    //ATUALIZA NOVO_I E NOVO_J PARA AS POSIÇÕES SOLICITADAS
    for (int d = 0; d < 4; d++) {
        int novo_i = *i + mov_x[d];
        int novo_j = *j + mov_y[d];

        //VERIFICA SE A POSIÇÃO JA FOI VISITADA OU SE JA ESTA NO TABULEIRO
        if (novo_i >= 0 && novo_i < *linhas && novo_j >= 0 && novo_j < *colunas && visitado[novo_i][novo_j] == 0) {

            //ATUALIZA A NOVA POSIÇÃO
            *i = novo_i;
            *j = novo_j;

            //ATUALIZA A POSIÇÃO DO ASPIRADOR
            if(matriz[*i][*j] == 0){matriz[*i][*j] = 20;}
            if(matriz[*i][*j] == 1){matriz[*i][*j] = 21;}

            //CHAMA RECURSÃO
            po_dfs(matriz, linhas, colunas, i, j, visitado, suj, p);

        }
    }
}

int visitados(int *i, int *j, int **visitado, int *linhas, int *colunas){

    int verificador = 0; //CRIA VERIFICADOR
    //SALVA OS MOVIMENTOS POSSIVEIS EM DOIS VETORES
    int mov_x[] = {-1, 0, 1, 0};
    int mov_y[] = {0, -1, 0, 1};

    for (int d = 0; d < 4; d++) { //DEFINE TODAS AS MOVIMENTAÇÕES PARA NOVO I E NOVO J
        int novo_i = *i + mov_x[d];
        int novo_j = *j + mov_y[d];

        //VERIFICA SE A POSIÇÃO JA FOI VISITADA OU SE JA ESTA NO TABULEIRO
        if (novo_i >= 0 && novo_i < *linhas && novo_j >= 0 && novo_j < *colunas){
            if(visitado[novo_i][novo_j] == 0){
                return 0; //CASO TENHA UMA POSIÇÃO QUE AINDA NAO FOI VISITADA PROXIMA(NÃO É PARA TER), RETORNA 0
            }
        }
    }
    return 1; //CASO ESTEJA PRESO, RETORNA 1
}

void backtracking_A(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int *suj, Pilha *p){

    if (p->Topo == NULL) { //VERIFICA SE É VAZIO
        return;
    }
    pop(p); //RETIRA A POSIÇÃO DA CASA I ATUAL DA PILHA
    pop(p); //RETIRA A POSIÇÃO DA CASA J ATUAL DA PILHA

    int novo_i = pop(p); //SALVA O I DO MOVIMENTO ANTERIOR COMO NOVO I E JA O APAGA DA PILHA
    int novo_j = pop(p); //SALVA O J DO MOVIMENTO ANTERIOR COMO NOVO J E JA O APAGA DA PILHA

    //ATUALIZA A NOVA POSIÇÃO
    matriz[*i][*j] = 0;
    *i = novo_i;
    *j = novo_j;

    //ATUALIZA A POSIÇÃO DO ASPIRADOR
    matriz[*i][*j] = 20;
    estado(matriz, linhas, colunas, 2, visitado, visitado);
    printMatriz(linhas, colunas, matriz, 2, visitado, visitado);
}

//PARCIALMENTE OBSERVÁVEL B
void dfs(int **matriz, int **visitado, int *i, int *j, int *linhas, int *colunas, int escolha, int **visualizado, Pilha *p) {

    if(objetivo(matriz, linhas, colunas, visualizado) == 1){
        return;
    }

    int novo_i;
    int novo_j;

    //Verifica se alguma posição adjacente tem sujeira
    while(1){
        if ((*i + 1 < *linhas) && (matriz[*i + 1][*j] == 1)){ //abaixo
            novo_i = *i + 1;
            novo_j = *j;
         }else if ((*i - 1 >= 0) && (matriz[*i - 1][*j] == 1)){ //acima
            novo_i = *i - 1;
            novo_j = *j;
         } else if ((*j + 1 < *colunas) && (matriz[*i][*j + 1] == 1)){ //direita
            novo_j = *j + 1;
            novo_i = *i;
         }else if ((*j - 1 >= 0) && (matriz[*i][*j - 1] == 1)){ //esquerda
            novo_j = *j - 1;
            novo_i = *i;
         }else{
            break;
         }

        if (novo_i >= 0 && novo_i < *linhas && novo_j >= 0 && novo_j < *colunas){
            // Define o aspirar na próxima casa
            if(matriz[novo_i][novo_j]==0){
                    matriz[novo_i][novo_j] = 20;
            }else if(matriz[novo_i][novo_j]==1){
                    matriz[novo_i][novo_j] = 21;
            }


            // Define como limpa a casa atual
            matriz[*i][*j] = 0;

            // Move o aspirador para a nova posição
            *i = novo_i;
            *j = novo_j;

            //Guarda a posição atual na pilha
            printf("%d%d\n\n",*i,*j);
            push(p,*j);
            push(p,*i);

            //Printa a matriz após o movimento
            printf("movimento: sujeiras\n");
            estado(matriz, linhas, colunas, escolha, visitado, visualizado);
            printMatriz(linhas, colunas, matriz, escolha, visitado, visualizado);
            sleep(1);
            system("cls");
        }
    }

    //Possiveis movimentos por ordem de prioridade
    int mov_x[] = {-1, 0, +1, 0};
    int mov_y[] = {0, -1, 0, +1};

    // Percorre todas as direções possíveis
    for (int d = 0; d < 4; d++) {
        novo_i = *i + mov_x[d];
        novo_j = *j + mov_y[d];

        // Verifica se a nova posição está dentro dos limites e ainda não foi visitada
        if (novo_i >= 0 && novo_i < *linhas && novo_j >= 0 && novo_j < *colunas && visitado[novo_i][novo_j] == 0) {
            // Define como limpa a casa atual
            matriz[*i][*j] = 0;

            // Define o aspirar na proxima casa
            if(matriz[novo_i][novo_j]==0){
                matriz[novo_i][novo_j] = 20;
            }else if(matriz[novo_i][novo_j]==1){
                matriz[novo_i][novo_j] = 21;
            }

            // Move o aspirador para a nova posição
            printf("%d%d\n\n",*i,*j);
            *i = novo_i;
            *j = novo_j;

            //Guarda a posição atual na pilha
            push(p,*j);
            push(p,*i);

            //Printa a matriz após o movimento
            printf("movimento: prioridade\n");
            estado(matriz, linhas, colunas, escolha, visitado, visualizado);
            printMatriz(linhas, colunas, matriz, escolha, visitado, visualizado);
            sleep(1);
            system("cls");

            // Chama a função recursivamente para a nova posição
            dfs(matriz, visitado, i, j, linhas, colunas, escolha, visualizado, p);

        }
    }
}

void backtracking_B(int **matriz, int *linhas, int *colunas, int *i, int *j, int **visitado, int *suj, Pilha *p, int **visualizado){

    imprimePilha(p);

    if (p->Topo == NULL) { //VERIFICA SE É VAZIO
        return;
    }
    pop(p); //RETIRA A POSIÇÃO DA CASA I ATUAL DA PILHA
    pop(p); //RETIRA A POSIÇÃO DA CASA J ATUAL DA PILHA

    int novo_i = pop(p); //SALVA O I DO MOVIMENTO ANTERIOR COMO NOVO I E JA O APAGA DA PILHA
    int novo_j = pop(p); //SALVA O J DO MOVIMENTO ANTERIOR COMO NOVO J E JA O APAGA DA PILHA

    //ATUALIZA A NOVA POSIÇÃO
    matriz[*i][*j] = 0;
    *i = novo_i;
    *j = novo_j;
    //ATUALIZA A POSIÇÃO DO ASPIRADOR
    matriz[*i][*j] = 20;
    estado(matriz, linhas, colunas, 3, visitado, visualizado);
    printMatriz(linhas, colunas, matriz, 3, visitado, visualizado);
    sleep(1);
//    system("cls");
}

int objetivo(int **matriz, int *linhas, int *colunas, int **visualizado){
     for (int x = 0; x < *linhas; x++) {
        for (int y = 0; y < *colunas; y++) {
            if (matriz[x][y] == 1 || matriz[x][y] == 21 || visualizado[x][y] == 0){
                system("cls");
                return 0;
            }
        }
    }
    return 1;
}

