#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void menu(int *escolha) {
    int tecla = 0;
    *escolha = 1; //ATRIBUIMOS VALOR A VARIAVEL PARA O MENU COMEÇAR NA PRIMEIRA OPÇÃO

    while (1) {
        system("cls");

        printf("\n\n\n\t\t\t   ******************* Simulador aspirador ********************\n");
        printf("\t\t\t   *                  | Selecione a busca |                   *\n");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   * %s IA Observável                                          *\n", (*escolha == 1) ? "->" : " ");
        printf("\t\t\t   * %s IA Parcialmente observável A                           *\n", (*escolha == 2) ? "->" : " ");
        printf("\t\t\t   * %s IA Parcialmente observável B                           *\n", (*escolha == 3) ? "->" : " ");
        printf("\t\t\t   * %s Navegação manual                                       *\n", (*escolha == 4) ? "->" : " ");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   ************************************************************\n");

        tecla = getch(); //LÊ A TECLA PRESSIONADA

        //NAVEGAÇÃO
        if (tecla == 72) {
            if (*escolha > 1) (*escolha)--; //SOBE PARA A POSIÇÃO ANTERIOR
        } else if (tecla == 80) {
            if (*escolha < 4) (*escolha)++; //DESCE PARA A PRÓXIMA SEÇÃO
        } else if (tecla == 27) {
            return *escolha; //RETORNA O NOVO VALOR DA ESCOLHA
        } else if (tecla == 13) {
            return -1; //RETORNA -1 CASO O USUÁRIO DE ESC
        }
    }
}

int main()
{
    setlocale(LC_ALL,"portuguese");
    int *escolha;
    menu(&escolha);
    printf("%d",escolha);

    return 0;
}
