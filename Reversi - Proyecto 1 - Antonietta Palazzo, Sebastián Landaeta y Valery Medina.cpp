/*	Proyecto 1: Reversi
    Ingenier�a Inform�tica
	Estructuras de Datos
	Secci�n 1
	Alumnos: 
        Sebasti�n Landaeta (CI: 28.240.979)
        Antonietta Palazzo (CI: 28.619.939)
        Valery Medina (CI: 29.643.312)
*/

// == LIBRERIAS ==
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <clocale>
#include <windows.h>

using namespace std;

// == CLASES ==
class Menu
{
    // == M�TODOS ==
    public:
        // Muestra el men� del juego, para que los usuarios puedan interactuar con las opciones disponibles
        void menu()
        {
            int menu = 0;
            char opcion;

            do
            {
		        // Mostrar la interfaz del men�
                titulo();
        
		        // Almacenar la opci�n escogida por el usuario
                cin >> opcion;
                fflush(stdin);

                switch (opcion)
                {
                    case '1': // Comenzar partida
                        system("cls");
                        modos();
                        int opmodo;
                        cin >> opmodo;
                        fflush(stdin);
                        break;

			        case '0': // Salir del juego
				        system("cls");
				        cout << "       ___________________________________________________________________________________________\n\n\n";
				        cout << "\n\t\t\t\t\t  = Gracias por Jugar <(* U *)/ =\n\n\n";
				        cout << "       ___________________________________________________________________________________________\n\n\t";
                        system("pause");
                        menu = 1;
			            break;

                    default: // Mostrar mensaje en caso de equivocaci�n
                        system("cls");
                        cout << "\n\t\t\t\t     -Opci�n Incorrecta - Intente Nuevamente-\n\n";
                        system("pause");
                }

                system("cls");
            } while (menu == 0);
        }

        // Muestra la interfaz del men�
        void titulo()
        {
            cout << "       ___________________________________________________________________________________________\n\n\n";
    
            cout << "\t    ######     #########   ##        ##   #########   ######       ####     ########## \n";
            cout << "\t    #######    #########   ##        ##   #########   #######     ######    ########## \n";
            cout << "\t    ##    ##   ##          ##        ##   ##          ##    ##   ##    ##       ##     \n";
            cout << "\t    ##    ##   ##          ##        ##   ##          ##    ##    ###           ##     \n";
            cout << "\t    #######    ######      ##        ##   ######      #######      ###          ##     \n";
            cout << "\t    ######     ######      ###      ###   ######      ######        ###         ##     \n";
            cout << "\t    ##   ##    ##           ###    ###    ##          ##   ##        ###        ##     \n";
            cout << "\t    ##    ##   ##             ##  ##      ##          ##    ##   ##    ##       ##     \n";
            cout << "\t    ##    ##   #########       ####       #########   ##    ##    ######    ########## \n";
            cout << "\t    ##    ##   #########        ##        #########   ##    ##     ####     ########## \n\n\n"; 
	
	        cout << "\t\t\t\t\t     1. Comenzar Juego\n\n\n";
	        cout << "\t\t\t\t\t     0. Salir del juego\n\n";
	
	        cout << "       ___________________________________________________________________________________________\n";
            cout << "\n       Opci�n (n�mero) --> "; 
        }
        
        // Muestra los modos de juego disponibles
        void modos()
        {
            cout << "       ___________________________________________________________________________________________\n\n\n";

            cout << "\t\t\t\t\t1.\t1P vs 2P\n\n" ;
            cout << "\t\t\t\t\t2.\t1P vs CPU\n\n";
            cout << "\t\t\t\t\t3.\tCPU vs CPU\n\n";
            cout << "       ___________________________________________________________________________________________\n";
            cout << "\n       Opci�n (n�mero) --> ";
        }
};

// Funci�n principal
int main()
{
    // Posibilitar el uso de caracteres especiales del idioma espa�ol
	setlocale(LC_CTYPE, "Spanish");
    SetConsoleCP(1252);
	SetConsoleOutputCP(1252);

	// Cambiar el t�tulo de la ventana por el nombre del juego
	SetConsoleTitleA("Reversi");
     
    // Inicio formal del programa
    Menu menu;
    menu.menu();

    return 0;
}