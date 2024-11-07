//BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>

#define MAX 100


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
//https://github.com/malufreitas/a-estrela/blob/master/main.py
//https://wagnergaspar.com/como-implementar-a-estrutura-de-dados-fila-de-prioridade-em-c/
//https://www.youtube.com/watch?v=ECdLOLaIVx8

typedef struct no{
    int x, y;
    int heuristica;
} No;

typedef struct info{
    int x,y;
} pos;

typedef struct list {
 No info;
 struct list *prox;
} Lista;

// FUNÇÕES DO MENU
void menu_universo(int *escolha);
void menu_controladora(int *escolha);

//  FUNÇÕES BÁSICAS

void tamanhoTabuleiro(int *linhas, int *colunas); //LER TAMANHO DA SALA
int qtdSujeira(int *linhas, int *colunas); //LER QUANTIDADE SUJEIRAS
int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1, pos *vPosicoes); //GERAR TABULEIRO
void printMatriz(int *linhas, int *colunas, int **matriz, int escolha, int **visitado); //IMPRIMIR MATRIZ
void gerarPosicaoAspirador(int *linhas, int *colunas, int **matriz); //GERAR ASPIRADOR
void localizarAspirador(int **matriz, int *linhas, int *colunas, int *i, int *j); //LOCALIZAR ASPIRADOR
int avalia(int **matriz, int *linhas, int *colunas); //OBJETIVO CONCLUIDO
void sucessora(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas, int mov); //MOVIMENTACAO
void limpar(int movimento, int **matriz, int*i, int *j, int ia); //LIMPAR SUJEIRA
void freeMatriz(int **matriz, int *linhas); //LIMPAR MEMORIA ALOCADA
void freeVisitado(int **visitados, int *linhas);//LIMPA MEMORIA ALOCADA
int **copiarMatriz(int **matriz, int *linhas, int *colunas); //FUNCAO COPIAR MATRIZ
void estado(int **matriz, int *linhas, int *colunas, int escolha, int **visitado);

//TESTE IA:
//void po_dfs(int **matriz, int *linhas, int *colunas,int *i, int *j, int ia, int mov, int **visitado);

// FUNÇÕES PARA IA DO UNIVERSO OBSERVÁVEL:
int distanciaManhattan(int x1, int x2, int y1, int y2);
int dentroLimites(int x, int y, int *linhas, int *colunas);
void buscaGulosa(int **matriz,int *linhas, int *colunas, int inicioX, int inicioY, int objetivoX, int objetivoY);
void inicializaNos(No **nos, int *linhas, int *colunas, int objetivoX, int objetivoY);

Lista *insere(Lista *recebida, No valor){
    Lista *novo;
    novo= (Lista*) malloc(sizeof(Lista));
    novo->info = valor;
    novo->prox = recebida;
    return novo;
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); //PARA LIBERAR ACENTUAÇÃO
    srand(time(NULL)); //PARA GERAR CASAS ALEATÓRIAS
    SetConsoleOutputCP(CP_UTF8); // PARA COR DO ASPIRADOR MUDAR NO CONSOLE
    SetConsoleCP(CP_UTF8);

    int linhas = 0, colunas = 0, qtd_1, escolha, escolha2, ia = 0, mov = 0;
    int pos1, pos2;
    int retorno = 0, tecla;
    int **visitado;


    //GERA OS MENUS
    menu_universo(&escolha);
    menu_controladora(&escolha2);
    system("cls");

    //GERA AS DIMENSÕES E SUJEIRAS DO TABULEIRO
    tamanhoTabuleiro(&linhas, &colunas);
    qtd_1 = qtdSujeira(&linhas, &colunas);
    pos *vPosicoes = (pos *)malloc(qtd_1 * sizeof(pos));

    system("cls");


    //GERA O TABULEIRO
    int **matriz = gerarTabuleiro(&linhas, &colunas, qtd_1, vPosicoes);

    for(int i = 0; i < qtd_1; i++) {
        printf("Sujeira %d: (%d, %d)\n", i, vPosicoes[i].x, vPosicoes[i].y);
    }


    //ALOCA MEMÓRIA PARA O VISITADO
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

    pos posicoes_aspirador;
    posicoes_aspirador.x = pos1;
    posicoes_aspirador.y = pos2;
    pos posicoes_objetivo;
    posicoes_objetivo.x = vPosicoes[0].x;
    posicoes_objetivo.y = vPosicoes[0].y;
    //CASO SEJA MANUAL
    if(escolha2 == 1){
        while (retorno != 1) {

            printf("\n\n\n\n\n");
            estado(matriz, &linhas, &colunas, escolha, visitado); //GERA O ESTADO
            printMatriz(&linhas, &colunas, matriz, escolha, visitado); //PRINTA O ESTADO
            buscaGulosa(matriz, &linhas, &colunas, posicoes_aspirador.x, posicoes_aspirador.y, posicoes_objetivo.x, posicoes_objetivo.y);
            printf("\n\n\n\n\n");
            tecla = getch(); //PEGA A TECLA
            sucessora(tecla, &pos1, &pos2, matriz, &linhas, &colunas, mov); //LE OS MOVIMENTOS
            limpar(tecla, matriz, &pos1, &pos2, ia); //FAZ A LIMPEZA MANUAL
            retorno = avalia(matriz, &linhas, &colunas); //FUNÇÃO OBJETIVO
            system("cls");
        }
    }

    //CASO SEJA CONTROLADO POR IA
    if(escolha2 == 2){
        if(escolha == 1){
            //OBSERVÁVEL
            // eu posso calcular qual o ponto mais perto pro mais longo e ordernar eles na lista de prioridade, caso list!=NULL continuo adicionando a movimentação necessaria no array de caminhos
            // funções: calcular_distancia, percurso




        }else if(escolha == 2){
            //PARCIAL A
        }else if(escolha == 3){
            //PARCIAL B
        }
    }


    //LIBERA A MEMÓRIA DAS CASAS VISITADAS
    freeVisitado(visitado, &linhas);

    //LIBERA A MEMORIA DA MATRIZ
    freeMatriz(matriz, &linhas);
    free(vPosicoes);
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

int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1, pos *vPosicoes) {
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
    for(int i=0; i<qtd_1; i++){
        int rand_linha = rand() % (*linhas);
        int rand_coluna = rand() % (*colunas);



        // Coloca o 1 na posição se ainda for 0
        if (matriz[rand_linha][rand_coluna] == 0) {
            matriz[rand_linha][rand_coluna] = 1;
            vPosicoes[i].x = rand_linha;
            vPosicoes[i].y = rand_coluna;
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

void sucessora(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas, int mov) {
    int aux_i, aux_j;
    aux_i = *i;
    aux_j = *j;

    if ((movimento == KEY_UP || mov == 1) && (*i) > 0) {
        (*i)--;
    } else if ((movimento == KEY_DOWN || mov == 2) && (*i) < *linhas - 1) {
        (*i)++;
    } else if ((movimento == KEY_RIGHT || mov == 4) && (*j) < *colunas - 1) {
        (*j)++;
    } else if ((movimento == KEY_LEFT || mov == 3) && (*j) > 0) {
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

void limpar(int movimento, int **matriz, int *i, int *j, int ia) {
    if (movimento == KEY_CLEAN || ia == 1) {
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
                    if (i + 1 < *linhas) visitado[i + 1][j] = 1;                                 // Célula abaixo
                    if (i - 1 >= 0) visitado[i - 1][j] = 1;                                      // Célula acima
                    if (j + 1 < *colunas) visitado[i][j + 1] = 1;                                // Célula à direita
                    if (j - 1 >= 0) visitado[i][j - 1] = 1;                                      // Célula à esquerda
                    if ((i + 1 < *linhas)&&(j + 1 < *colunas)) visitado[i + 1][j+1] = 1;         // Célula abaixo direita
                    if ((i + 1 < *linhas)&&(j - 1 >= 0)) visitado[i + 1][j - 1] = 1;             // Célula abaixo esquerda
                    if ((i - 1 >= 0)&&(j + 1 < *colunas)) visitado[i - 1][j + 1] = 1;            // Célula cima direita
                    if ((i - 1 >= 0)&&(j - 1 >= 0)) visitado[i - 1][j - 1] = 1;                  // Célula esquerda direita
                }
            }
        }
    }
}

int distanciaManhattan(int x1, int x2, int y1, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

int dentroLimites(int x, int y, int *linhas, int *colunas) {
    return (x >= 0 && x < *linhas && y >= 0 && y < *colunas);
}

// Função para inicializar a matriz de nós com a heurística (distância de Manhattan)
void inicializaNos(No **nos, int *linhas, int *colunas, int objetivoX, int objetivoY) {
    for (int i = 0; i < *linhas; i++) {
        for (int j = 0; j < *colunas; j++) {
            nos[i][j].x = i;
            nos[i][j].y = j;
            // Calculando a heurística como a distância de Manhattan até o objetivo
            nos[i][j].heuristica = distanciaManhattan(i, j, objetivoX, objetivoY);
        }
    }
}

// Função de busca gulosa para percorrer a matriz
void buscaGulosa(int **matriz,int *linhas, int *colunas, int inicioX, int inicioY, int objetivoX, int objetivoY) {
    printf("Antes da memoria");


    No **nos = (No **)malloc(*linhas * sizeof(No *));
    for (int i = 0; i < *linhas; i++) {
        nos[i] = (No *)malloc(*colunas * sizeof(No));
    }
    int **visitados = (int **)malloc(*linhas * sizeof(int *));
    for (int i = 0; i < *linhas; i++) {
        visitados[i] = (int *)malloc(*colunas * sizeof(int));
        for (int j = 0; j < *colunas; j++) {
            visitados[i][j] = 0;
        }
    }  // Matriz para marcar células visitadas
    printf("antes de caminho");
    Lista *caminho = NULL;  // Vetor para armazenar o caminho
    printf("depois de caminho");
    // Inicializa a matriz de nós com as heurísticas
    inicializaNos(nos, &linhas, &colunas, objetivoX, objetivoY);

    No atual = nos[inicioX][inicioY];
    visitados[atual.x][atual.y] = 1;
    printf("antes do while");
    while (atual.x != objetivoX || atual.y != objetivoY) {
        caminho = insere(caminho, atual);

        // Encontrar o próximo nó com a menor heurística não visitada
        No proximo = {-1, -1, INT_MAX};  // Inicializa um nó "nulo"

        // Vizinhos: cima, baixo, esquerda, direita
        int direcoes[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (int i = 0; i < 4; i++) {
            int novaX = atual.x + direcoes[i][0];
            int novaY = atual.y + direcoes[i][1];

            // Verifica se a nova célula está dentro dos limites e não foi visitada
            if (dentroLimites(novaX, novaY, &linhas, &colunas) && !visitados[novaX][novaY]) {
                // Calculando a heurística com base na distância de Manhattan
                int heuristicaVizinho = nos[novaX][novaY].heuristica;

                if (heuristicaVizinho < proximo.heuristica) {
                    proximo.x = novaX;
                    proximo.y = novaY;
                    proximo.heuristica = heuristicaVizinho;
                }
            }
        }

        if (proximo.x == -1) {
            printf("Caminho não encontrado.\n");
            return;
        }

        visitados[proximo.x][proximo.y] = 1;
        atual = proximo;
    }

    caminho = insere(caminho, nos[objetivoX][objetivoY]);

    // Exibir o caminho encontrado
    printf("Caminho encontrado: ");
    while(caminho != NULL){
        printf("(%d, %d) \n", caminho->info.x, caminho->info.y);
        caminho = caminho->prox;
    }
    printf("\n");
}

