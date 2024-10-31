#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void menu(int *escolha) {
    int tecla = 0;
    *escolha = 1; //ATRIBUIMOS VALOR A VARIAVEL PARA O MENU COME�AR NA PRIMEIRA OP��O

    while (1) {
        system("cls");

        printf("\n\n\n\t\t\t   ******************* Simulador aspirador ********************\n");
        printf("\t\t\t   *                  | Selecione a busca |                   *\n");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   * %s IA Observ�vel                                          *\n", (*escolha == 1) ? "->" : " ");
        printf("\t\t\t   * %s IA Parcialmente observ�vel A                           *\n", (*escolha == 2) ? "->" : " ");
        printf("\t\t\t   * %s IA Parcialmente observ�vel B                           *\n", (*escolha == 3) ? "->" : " ");
        printf("\t\t\t   * %s Navega��o manual                                       *\n", (*escolha == 4) ? "->" : " ");
        printf("\t\t\t   *                                                          *\n");
        printf("\t\t\t   ************************************************************\n");

        tecla = getch(); //L� A TECLA PRESSIONADA

        //NAVEGA��O
        if (tecla == 72) {
            if (*escolha > 1) (*escolha)--; //SOBE PARA A POSI��O ANTERIOR
        } else if (tecla == 80) {
            if (*escolha < 4) (*escolha)++; //DESCE PARA A PR�XIMA SE��O
        } else if (tecla == 27) {
            return *escolha; //RETORNA O NOVO VALOR DA ESCOLHA
        } else if (tecla == 13) {
            return -1; //RETORNA -1 CASO O USU�RIO DE ESC
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
