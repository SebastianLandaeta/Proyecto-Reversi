/*	Proyecto 1: Reversi
    Ingeniería Informática
    Estructuras de Datos
    Sección 1
    Alumnos:
        Sebastián Landaeta (CI: 28.240.979)
        Antonietta Palazzo (CI: 28.619.939)
        Valery Medina (CI: 29.643.312)
*/

// == LIBRERIAS ==
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include <cstdlib>
#include <clocale>
#include <windows.h>

// == MACROS ==
#define VACIO 0
#define NEGRA 1
#define BLANCA 2
#define POSIBLE 3

#define FILAS 8
#define COLUMNAS 8
#define FICHAS FILAS*COLUMNAS

using namespace std;

// == CLASES ==

// Contiene cosas necesarias para hacer ciertas cosas
class Utilidad
{
    // == MÉTODOS ==
    public:
        // Función color
        void color(int n)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
        }

        // Función gotoxy
        void gotoxy(int X, int Y)
        {
            HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD dwPos;
            dwPos.X = X;
            dwPos.Y = Y;
            SetConsoleCursorPosition(hcon, dwPos);
        }
};

class Jugador
{
    // == ATRIBUTOS ==
    public:
        string nombre;
        int n_fichas;
};

class Partida
{
    // == ATRIBUTOS ==
    protected:
        int tabla[FILAS][COLUMNAS]; // Tablero de juego
        Jugador j1, j2;             // Jugadores de la partida
        bool turno;                 // Identifica a qué usuario le toca jugar
        int **posibles;             // Todas las jugadas posibles en un turno

    // == MÉTODOS ==
    public:
        // Prepara la partida. El cómo lo hará dependerá del tipo de partida a jugar
        virtual void configuracion(){};

        // Muestra quien será el primer jugador (fichas negras)
        void primer_jugador(Jugador jugador)
        {
            cout << jugador.nombre;
            cout << " será el primer jugador.\n";
            cout << "\n       ___________________________________________________________________________________________\n\n\t";
            system("pause");
        }
};

class Jugador_vs_Jugador : public Partida
{
    public:
        // Prepara la partida para dos jugadores humanos
        void configuracion()
        {
            // Semilla para el randomizer
            srand(time(0));
            
            // Inicializar contadores de fichas
            j1.n_fichas = 2, j2.n_fichas = 2;
            
            // Inicializar tabla
            for (int i = 0; i < FILAS; i++)
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    tabla[i][j] = VACIO;
                }
            }

            // Colocar las fichas negras
            tabla[3][4] = NEGRA;
            tabla[4][3] = NEGRA;

            // Colocar las fichas blancas
            tabla[3][3] = BLANCA;
            tabla[4][4] = BLANCA;

            // Reservar espacio para las filas
            posibles = new int*[4];

            // Reservar espacio para las columnas
            for (int i = 0; i < 4; i++) 
            {
                posibles[i] = new int[2];
            }

            // Meter los posibles movimientos iniciales en la matriz
            
            posibles[0][0] = 3;
            posibles[0][1] = 2;

            posibles[1][0] = 2;
            posibles[1][1] = 3;

            posibles[2][0] = 5;
            posibles[2][1] = 4;

            posibles[3][0] = 4;
            posibles[3][1] = 5;

            // Pedir los nombres de los jugadores
            cout << "       ___________________________________________________________________________________________\n\n\n";
	        cout << "\t\t\t         = INGRESE LOS NOMBRES DE LOS JUGADORES =\n\n\n";
             
            cout << "\t\t\t---> Primer jugador: ";
            getline(cin, j1.nombre);

            cout << "\n\t\t\t---> Segundo jugador: ";
            getline(cin, j2.nombre);

            // Elegir aleatoriamente quien será el primer jugador (fichas negras)
            int aleatorio = rand()%2;

            // Mostrar quien será el primer jugador (fichas negras)
            if (aleatorio == 0)
            {
                 
                primer_jugador(j1);
            }
            else
            {
                primer_jugador(j2);
            }
        }
};

// Permite interactuar y visualizar el menú del juego
class Menu
{
    // == ATRIBUTOS ==
    private:
        bool salir = FALSE;
        char opcion;
        char modo;

    // == MÉTODOS ==
    public:
        // Muestra el menú del juego
        void iniciar()
        {
            do
            {
                // Mostrar la interfaz del menú
                titulo();

                // Almacenar la opción escogida por el usuario
                cin >> opcion;
                fflush(stdin);

                switch (opcion)
                {
                    case '1': // Comenzar partida
                        system("cls");

                        mostrar_modos();
                        cin >> modo;
                        fflush(stdin);

                        system("cls");
                        crear_partida();
                        break;

                    case '0': // Salir del juego
                        system("cls");
                        cout << "       ___________________________________________________________________________________________\n\n\n";
                        cout << "\n\t\t\t\t\t  = Gracias por Jugar <(* U *)/ =\n\n\n";
                        cout << "       ___________________________________________________________________________________________\n\n\t";
                        system("pause");
                        salir = TRUE;
                        break;

                    default: // Mostrar mensaje en caso de equivocación
                        system("cls");
                        cout << "\n\t\t\t\t     -Opción Incorrecta - Intente Nuevamente-\n\n";
                        system("pause");
                }

                system("cls");
            } while (salir == FALSE);
        }

        // Muestra la interfaz del menú
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

            cout << "       ___________________________________________________________________________________________\n\n";
            cout << "       Opción (número) --> ";
        }

        // Muestra los modos de juego disponibles
        void mostrar_modos()
        {
            cout << "       ___________________________________________________________________________________________\n\n\n";

            cout << "\t\t\t\t\t1.\t1. Jugador vs Jugador\n\n";
            cout << "\t\t\t\t\t2.\t2. Jugador vs CPU\n\n";
            cout << "\t\t\t\t\t3.\t3. CPU vs CPU\n\n";
            cout << "       ___________________________________________________________________________________________\n\n";
            cout << "      Opción (número) --> ";
        }


        // Crea la partida en función del modo de juego elegido
        void crear_partida()
        {
            switch (modo)
            {
                // Se crea una partida de jugador contra jugador
                case '1':
                    Jugador_vs_Jugador partida;
                    partida.configuracion();
                    break;
            }
        }
};

// Función principal
int main()
{
    // Posibilitar el uso de caracteres especiales del idioma español
    setlocale(LC_CTYPE, "Spanish");
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    // Cambiar el tí­tulo de la ventana por el nombre del juego
    SetConsoleTitleA("Reversi");

    // Inicio formal del programa
    Menu menu;
    menu.iniciar();

    return 0;
}