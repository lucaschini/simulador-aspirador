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

// DEFINIÇÕES:
// 0 = PISO LIMPO
// 1 = PISO SUJO
// A = ASPIRADOR EM CIMA DE PISO LIMPO (2) 20
// & = ASPIRADOR EM CIMA DE PISO SUJO (3) 21
// C = TECLA PARA LIMPAR

// TO SEE: https://www.hashtagtreinamentos.com/como-waze-funciona-python
// Insertion Sort
// https://joaoarthurbm.github.io/eda/posts/insertion-sort/
// https://github.com/malufreitas/a-estrela/blob/master/main.py
// https://wagnergaspar.com/como-implementar-a-estrutura-de-dados-fila-de-prioridade-em-c/
// https://www.youtube.com/watch?v=ECdLOLaIVx8

// ESTRUTURAS PARA A*
// Estrutura para representar uma célula (ou nó)
typedef struct no {
    int x, y;              // Coordenadas da célula
    int custoG;            // Custo g(x) para o A*
    int custoH;            // Heurística h(x) para o A*
    struct no* pai;   // Ponteiro para o nó pai
    struct no* prox;     // Ponteiro para o próximo nó na lista
} Celula;

typedef struct cel {
    int x,y;
} Coordenada;

typedef struct list{
    Coordenada info;
    struct list *prox;
} Lista;

// FUNÇÕES DO MENU
void menu_universo(int *escolha);
void menu_controladora(int *escolha);

//  FUNÇÕES BÁSICAS

void tamanhoTabuleiro(int *linhas, int *colunas); //LER TAMANHO DA SALA

int qtdSujeira(int *linhas, int *colunas); //LER QUANTIDADE SUJEIRAS

int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1, Coordenada *objetivo); //GERAR TABULEIRO

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
Celula *criarCelula(int x, int y, int custoG, int custoH, Celula *pai);
void addCelula(Celula** head, Celula* novaCelula);
void removeCelula(Celula** head, Celula* celulaToRemove);

Celula* celulaComMenorCusto(Celula* head);
int distanciaManhattan(int x1, int x2, int y1, int y2);
int estaNaLista(Celula *head, int x, int y);

void Path(Celula* no, Lista **movs);

void busca(int *startX, int *startY, int endX, int endY, int** matriz, int *linhas, int *colunas, Lista **movs);

void freeLista(Celula* head);

void sucessoraA(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas);
int movimento(int *i, int *j, Lista *movs);

Lista *Insere (Lista *recebida, int x, int y);

Lista *apagaum(Lista *aux);




int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8"); //PARA LIBERAR ACENTUAÇÃO
    srand(time(NULL)); //PARA GERAR CASAS ALEATÓRIAS
    SetConsoleOutputCP(CP_UTF8); // PARA COR DO ASPIRADOR MUDAR NO CONSOLE
    SetConsoleCP(CP_UTF8);

    int linhas = 0, colunas = 0, qtd_1, escolha, escolha2, ia = 0, mov = 0;
    int pos1, pos2;
    int retorno = 0, tecla;
    int **visitado;
    Coordenada *objetivo;


    //GERA OS MENUS
    menu_universo(&escolha);
    menu_controladora(&escolha2);
    system("cls");

    //GERA AS DIMENSÕES E SUJEIRAS DO TABULEIRO
    tamanhoTabuleiro(&linhas, &colunas);
    qtd_1 = qtdSujeira(&linhas, &colunas);

    system("cls");


    //GERA O TABULEIRO
    int **matriz = gerarTabuleiro(&linhas, &colunas, qtd_1, &objetivo);

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
    //CASO SEJA MANUAL
    if(escolha2 == 1){
        while (retorno != 1) {

            printf("\n\n\n\n\n");
            estado(matriz, &linhas, &colunas, escolha, visitado); //GERA O ESTADO
            printMatriz(&linhas, &colunas, matriz, escolha, visitado); //PRINTA O ESTADO
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
            Lista *movs = NULL;
            int retorno = 0;
            printf("\n\n\n\n\n");
            printf("%d", objetivo->x);
            printf("%d", objetivo->y);
            estado(matriz, &linhas, &colunas, escolha, visitado); //GERA O ESTADO
            printMatriz(&linhas, &colunas, matriz, escolha, visitado); //PRINTA O ESTADO
            printf("\n\n\n\n\n");
            printf("%d", objetivo->x);
            printf("%d", objetivo->y);
            busca(&pos1, &pos2, objetivo->x, objetivo->y, matriz, &linhas, &colunas, &movs);
                Lista *temp = movs;
                printf("Caminho encontrado:\n");
                while (temp != NULL) {
                printf("Posição: (%d, %d)\n", temp->info.x, temp->info.y);
                temp = temp->prox;
                }

            while (retorno != 1) {
            sucessoraA(movimento(&pos1, &pos2, &movs), &pos1, &pos2, matriz, &linhas, &colunas); //LE OS MOVIMENTOS
            printf("\n\n\n\n\n");
            estado(matriz, &linhas, &colunas, escolha, visitado); //GERA O ESTADO
            printMatriz(&linhas, &colunas, matriz, escolha, visitado); //PRINTA O ESTADO
            printf("\n\n\n\n\n");
            retorno = avalia(matriz, &linhas, &colunas); //FUNÇÃO OBJETIVO
            system("cls");
            }



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

int **gerarTabuleiro(int *linhas, int *colunas, int qtd_1, Coordenada *objetivo) {
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
            objetivo->x = rand_linha;
            objetivo->y = rand_coluna;
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
// todo criar funcao para descrever o movimento

int movimento(int *i, int *j, Lista *movs){

    if(*i -1 == movs->info.x){
        apagaum(movs);
        return 1;
    } else if(*i + 1 == movs->info.x){
        apagaum(movs);
        return 2;
    } else if(*j + 1 == movs->info.y){
        apagaum(movs);
        return 4;
    } else if(*j - 1 == movs->info.y){
        apagaum(movs);
        return 3;
    }

}

void sucessoraA(int movimento, int *i, int *j, int **matriz, int *linhas, int *colunas) {
    int aux_i, aux_j;
    aux_i = *i;
    aux_j = *j;

    if ((movimento == 1) && (*i) > 0) {
        (*i)--;
    } else if ((movimento == 2) && (*i) < *linhas - 1) {
        (*i)++;
    } else if ((movimento == 4) && (*j) < *colunas - 1) {
        (*j)++;
    } else if ((movimento == 3) && (*j) > 0) {
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


// A*

Celula *criarCelula(int x, int y, int custoG, int custoH, Celula *pai){
    Celula *novoNo = (Celula*)malloc(sizeof(Celula));
    novoNo->x = x;
    novoNo->y = y;
    novoNo->custoG = custoG;
    novoNo->custoH = custoH;
    novoNo->pai = pai;
    novoNo->prox = NULL;
    return novoNo;
}

int distanciaManhattan(int x1, int x2, int y1, int y2){
    return abs(x1 - x2) + abs(y1 - y2);
}

void addCelula(Celula** head, Celula* novaCelula) {
    novaCelula->prox = *head;
    *head = novaCelula;
}

void removeCelula(Celula** head, Celula* celulaToRemove) {
    if (*head == NULL) return;

    if (*head == celulaToRemove) { // caso a primeira ja seja a ser removida
        *head = (*head)->prox;
        free(celulaToRemove);
        return;
    }

    Celula *temp = *head;
    while (temp->prox != celulaToRemove) {
        temp = temp->prox;
    }

    if (temp->prox == celulaToRemove) {
        temp->prox = celulaToRemove->prox;
        free(celulaToRemove);
    }
}

Celula *celulaComMenorCusto(Celula* head) {
    Celula* lowest = head;
    Celula* temp = head;
    while (temp) {
        if ((temp->custoG + temp->custoH) < (lowest->custoG + lowest->custoH)) {
            lowest = temp;
        }
        temp = temp->prox;
    }
    return lowest;
}

int estaNaLista(Celula *head, int x, int y) {
    Celula* temp = head;
    while (temp) {
        if (temp->x == x && temp->y == y) {
            return 1;
        }
        temp = temp->prox;
    }
    return 0;
}

void freeLista(Celula* head) {
    while (head) {
        Celula *temp = head;
        head = head->prox;
        free(temp);
    }
}

Lista *insere(Lista *recebida, int x, int y){
    Lista *novo;
    novo= (Lista*) malloc(sizeof(Lista));
    novo->info.x = x;
    novo->info.y = y;
    novo->prox = recebida;
    return novo;
}

void Path(Celula* no, Lista **movs) {
    if (no->pai != NULL) {
        Path(no->pai, movs);
    }
    *movs = insere(movs, no->x, no->y);
}

void busca(int *startX, int *startY, int endX, int endY, int** matriz, int *linhas, int *colunas, Lista **movs) { // mudar o end pra lista de objetivos, fazer busca retornar os movimentos e alterar sucessora
    Celula* listaAberta = NULL;
    Celula* listaFechada = NULL;

    // Adiciona o nó inicial (posição inicial do aspirador) *talvez se eu deixar isso num while e a cada objetivo calcular o proximo caminho funcione à lista aberta
    Celula* celulaInicial = criarCelula(*startX, *startY, 0, distanciaManhattan(*startX, *startY, endX, endY), NULL);
    addCelula(&listaAberta, celulaInicial);

    int dx[] = {-1, 1, 0, 0}; // Movimentos: cima, baixo, esquerda, direita ( talvez usar sucessora
    int dy[] = {0, 0, -1, 1};
    printf("\n\nAntes while");
    while (listaAberta != NULL) {
        // Encontra o nó com o menor custo na lista aberta
        Celula *celulaAtual = celulaComMenorCusto(listaAberta);
        printf("\n\nAntes if");
        // Se o nó atual é o destino, imprime o caminho e finaliza
        if (celulaAtual->x == endX && celulaAtual->y == endY) {
            printf("Caminho encontrado");
            Path(celulaAtual, movs); // inves de printar, posso mandar a sucessora funcionar com o no->pai
            break;
        }

        // Move o nó atual da lista aberta para a lista fechada
        removeCelula(&listaAberta, celulaAtual);
        addCelula(&listaFechada, celulaAtual);

        // Explora cada vizinho do nó atual
        for (int i = 0; i < 4; i++) {
            int nx = celulaAtual->x + dx[i];
            int ny = celulaAtual->y + dy[i];
            printf("\n\nDentro for");
            // Verifica se o vizinho está dentro dos limites
            if (nx >= 0 && nx < *linhas && ny >= 0 && ny < *colunas) {
                // Se o vizinho já está na lista fechada, ignore
                if (estaNaLista(listaFechada, nx, ny)) continue;

                int custoG = celulaAtual->custoG + 1;
                int custoH = distanciaManhattan(nx, ny, endX, endY);

                // Se o vizinho já está na lista aberta, atualize o custo se necessário
                // todo: transformar a lista aberta em uma lista de prioridades
                if (estaNaLista(listaAberta, nx, ny)) {
                    Celula* noJaExiste = listaAberta;
                    while (noJaExiste) {
                        if (noJaExiste->x == nx && noJaExiste->y == ny) {
                            if (custoG < noJaExiste->custoG) {
                                noJaExiste->custoG = custoG;
                                noJaExiste->pai = celulaAtual;
                            }
                            break;
                        }
                        noJaExiste = noJaExiste->prox;
                    }
                } else {
                    // Adiciona o vizinho à lista aberta
                    Celula* noVizinho = criarCelula(nx, ny, custoG, custoH, celulaAtual);
                    addCelula(&listaAberta, noVizinho);
                }
            }
        }
    }

    printf("Caminho não encontrado.\n");
    freeLista(listaAberta);
    freeLista(listaFechada);
}

Lista *apagaum(Lista *aux){
	Lista *apagar;
	apagar = aux;
	aux = aux->prox;
	free(apagar);
    return aux;
}
