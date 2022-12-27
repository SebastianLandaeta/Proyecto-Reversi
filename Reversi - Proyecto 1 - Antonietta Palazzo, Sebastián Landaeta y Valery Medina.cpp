/*	Proyecto 1: Reversi
    Ingenierùa Informùtica
    Estructuras de Datos
    Secciùn 1
    Alumnos:
        Sebastiùn Landaeta (CI: 28.240.979)
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
#include <conio.h>

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
    // == M…TODOS ==
    public:
        // Pinta elementos en pantalla
        void color(int n)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
        }

        // Mueve el cursor a una coordenada especÌfica
        void gotoxy(int X, int Y)
        {
            HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
            COORD dwPos;
            dwPos.X = X;
            dwPos.Y = Y;
            SetConsoleCursorPosition(hcon, dwPos);
        }
};

// Contiene todos los datos de los jugadores
class Jugador
{
    // == ATRIBUTOS ==
    public:
        string nombre;    // Nombre del jugador
        int color_fichas; // Fichas asignadas
        int n_fichas;     // N˙mero de fichas de su color en el tablero
};

// Contiene todo lo referente a las partidas y al funcionamiento del juego
class Partida
{
    // == ATRIBUTOS ==
    protected:
        int tabla[FILAS][COLUMNAS]; // Tablero de juego
        bool turno = TRUE;          // Identifica a quÈ usuario le toca jugar
        Jugador j1, j2;             // Jugadores de la partida
        int n_turnos = 0;           // Cantidad de turnos
        Utilidad utilidad;          // Objeto para dar estilo al juego
        int cont_f = 0, cont_c = 0; // Coordenada fila y columna (se usan en la colocaciÛn de fichas)
        int x = 14, y = 4;          // Coordenada fila y columna (se usan para el gotoxy)
        int aux_f, aux_c;

    // == M…TODOS ==
    public:
        // Prepara la partida. El cÛmo lo har· depender· del tipo de partida a jugar
        virtual void configuracion(){};

        // Muestra quien ser· el primer jugador (fichas negras)
        void primer_jugador(Jugador jugador)
        {
            utilidad.gotoxy(41,12);
            cout << jugador.nombre;
            
            utilidad.color(7);
            cout << " ser· el primer jugador.\n";
            
            utilidad.color(8);
            cout << "\n       ___________________________________________________________________________________________\n\n\t";
            utilidad.color(7);
            
            system("pause");
        }

        // Muestra el tablero de juego
        void mostrar_tablero()
        {
            system("cls");

            cout << "       ___________________________________________________________________________________________\n\n\n";

            cout << "\t    ";

            for (int i = 0; i < FILAS; i++) // Imprimir letras
            {
                char letra = 65 + i;
                cout << "  " << letra << " ";
            }

            cout << "\n";

            for (int i = 0; i < FILAS; i++)
            {
                utilidad.color(7);
                cout << "\t  ";
                cout << i + 1 << " ";

                for (int j = 0; j < COLUMNAS; j++)
                {
                    utilidad.color(32);
                    cout << "|";

                    if (tabla[i][j] == VACIO)
                    {
                        cout << "   ";  // CondiciÛn para mostrar los espacios vacÌos
                    }                
                    
                    if (tabla[i][j] == NEGRA) // CondiciÛn para mostrar las fichas negras
                    {
                        utilidad.color(32);
                        cout << " O "; 
                    }

                    if (tabla[i][j] == BLANCA) // CondiciÛn para mostrar las fichas blancas
                    {
                        utilidad.color(47);
                        cout << " O ";
                    }

                    if (tabla[i][j] == POSIBLE) // CondiciÛn para mostrar las posibles jugadas
                    {
                        utilidad.color(36);
                        cout << " ? ";
                    }

                    utilidad.color(32);
            
                    if (j == 7)
                    {
                        cout << "\n";
                    }
                }
            }

            utilidad.color(7);

            cout << "\n\n       ___________________________________________________________________________________________\n\n";
        }

        // Muestra la informaciÛn del jugador en pantalla
        void mostrar_jugador(Jugador jugador)
        {
            for (int i = 3; i < 13; i++)
            {
                utilidad.gotoxy(54, i);
                cout << "|";
            }

            utilidad.gotoxy(70, 3);
            jugador.color_fichas == NEGRA ? utilidad.color(12) : utilidad.color(11);
            cout << "Datos de Jugador";

            utilidad.gotoxy(57, 5);
            utilidad.color(7);
            cout << "--> Nombre: ";
            jugador.color_fichas == NEGRA ? utilidad.color(12) : utilidad.color(11);
            cout << jugador.nombre;
            
            utilidad.gotoxy(57, 7);
            utilidad.color(7);
            cout << "--> Color de Fichas: ";
            jugador.color_fichas == NEGRA ? utilidad.color(12) : utilidad.color(11);
            cout << (jugador.color_fichas == NEGRA ? "Negras" : "Blancas");

            utilidad.gotoxy(57, 9);
            utilidad.color(7);
            cout << "--> N˙mero de Fichas: ";
            jugador.color_fichas == NEGRA ? utilidad.color(12) : utilidad.color(11);
            cout << jugador.n_fichas;

            utilidad.gotoxy(57, 11);
            utilidad.color(7);
            cout << "--> Turno de Partida: ";
            jugador.color_fichas == NEGRA ? utilidad.color(12) : utilidad.color(11);
            cout << n_turnos;
            utilidad.color(7);

            utilidad.gotoxy(6, 16);
        }

        // Comienza la partida
        void juego()
        {
            char aux;

            mostrar_tablero();

            if (turno == TRUE) // j1 es el primer jugador
            {
                mostrar_jugador(j1);
            }
            else // j2 es el primer jugador
            {
                mostrar_jugador(j2);
            }

            utilidad.gotoxy(x, y);

            while (1) // Moverte por la tabla para realizar una jugada
            {
                if (kbhit())
                {
                    aux = getch();

                    if (((int)aux == 72) && (y >= 5)) // Flecha arriba
                    {
                        y--;
                        cont_f--;
                        utilidad.gotoxy(x, y);
                    }

                    if (((int)aux == 80) && (y <= 10)) // Flecha abajo
                    {
                        y++;
                        cont_f++;
                        utilidad.gotoxy(x, y);
                    }

                    if (((int)aux == 75) && (x >= 15)) // Flecha izquierda
                    {
                        x -= 4;
                        cont_c--;
                        utilidad.gotoxy(x, y);
                    }

                    if (((int)aux == 77) && (x <= 41)) // Flecha derecha
                    {
                        x += 4 ;
                        cont_c++;
                        utilidad.gotoxy(x, y);
                    }

                    if ((int)aux == 13) // Enter
                    {

                        aux_f = cont_f, aux_c = cont_c;

                        if (turno == TRUE)
                        {
                            if (jugada(j1) == TRUE)
                            {
                                j1.n_fichas++;
                            }
                        }
                        else
                        {
                            if (jugada(j2) == TRUE)
                            {
                                j2.n_fichas++;
                            }
                        }
                        
                        break;
                    }
                }
            }
            
            turno = !turno;

            borrar_posibles();

            mostrar_tablero();

            if (turno == TRUE)
            {
                mostrar_jugador(j1);
            }
            else
            {
                mostrar_jugador(j2);
            }

            utilidad.gotoxy(0, 16);

            for (int i = 0; i < FILAS; i++)
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    cout << "[" << i << "][" << j << "]: " << tabla[i][j] << " "; 
                }
                cout << endl;
            }
        }
        
        // Comprueba si puedes realizar la jugada deseada o no, y en caso de poder, la hace
        bool jugada(Jugador jugador)
        {
            if (tabla[cont_f][cont_c] == POSIBLE) // Si es posible realizar la jugada...
            {
                if (jugador.color_fichas == NEGRA) // Turno del negro
                {
                    tabla[cont_f][cont_c] = NEGRA;
                }
                else // Turno del blanco
                {
                    tabla[cont_f][cont_c] = BLANCA;
                }

                convertir_fichas(jugador);
                return TRUE;
            }
            else // Si no es posible realizar la jugada...
            {
                utilidad.gotoxy(0, 16);
                cout << "El movimiento que desea realizar no es v·lido. Por favor, intentelo de nuevo.\n";
                system("pause");
                return FALSE;
            }
        }
        
        // Una vez realizada la jugada, se convierten las fichas flanqueadas al color del contrario
        void convertir_fichas(Jugador jugador)
        {
            if (((cont_f > 1) && (cont_f < 6)) && ((cont_c > 1) && (cont_c < 6))) // Comprobar desde todas las direcciones
            {
                comprobar_arriba(jugador);
                comprobar_abajo(jugador);
                comprobar_izquierda(jugador);
                comprobar_derecha(jugador);
                comprobar_arriba_derecha(jugador);
                comprobar_arriba_izquierda(jugador);
                comprobar_abajo_derecha(jugador);
                comprobar_abajo_izquierda(jugador);
            } 
        }
        
        // Comprueba hacia arriba y cambia las fichas de color
        void comprobar_arriba(Jugador jugador)
        {
            aux_f--;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_f--;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_f--;
                }
            }

            aux_f = cont_f;
        }

        // Comprueba hacia abajo y cambia las fichas de color
        void comprobar_abajo(Jugador jugador)
        {
            aux_f++;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_f++;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_f++;
                }
            }

            aux_f = cont_f;
            jugador.n_fichas++;
        }

        // Comprueba hacia la izquierda y cambia las fichas de color
        void comprobar_izquierda(Jugador jugador)
        {
            aux_c--;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar hacia la izquierda
                {
                    tabla[aux_f][aux_c] = NEGRA;
                    aux_c--;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar hacia la izquierda
                {
                    tabla[aux_f][aux_c] = BLANCA;
                    aux_c--;
                }
            }

            aux_c = cont_c;
        }

        // Comprueba hacia la derecha y cambia las fichas de color
        void comprobar_derecha(Jugador jugador)
        {
            aux_c++;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_c++;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_c++;
                }
            }

            aux_c = cont_c;
        }
        
        // Comprueba diagonalmente hacia arriba a la derecha y cambia las fichas de color
        void comprobar_arriba_derecha(Jugador jugador)
        {
            aux_f--;
            aux_c++;

            if (jugador.color_fichas == NEGRA)
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_f--;
                    aux_c++;
                }
            }
            else
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_f--;
                    aux_c++;
                }
            }

            aux_f = cont_f;
            aux_c = cont_c;
        }

        // Comprueba diagonalmente hacia abajo a la derecha y cambia las fichas de color
        void comprobar_abajo_derecha(Jugador jugador)
        {
            aux_f++;
            aux_c++;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA)
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_f++;
                    aux_c++;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA)
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_f++;
                    aux_c++;
                }
            }

            aux_f = cont_f;
            aux_c = cont_c;
        }

        // Comprueba diagonalmente hacia arriba a la izquierda y cambia las fichas de color
        void comprobar_arriba_izquierda(Jugador jugador)
        {
            aux_f--;
            aux_c--;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_f--;
                    aux_c--;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_f--;
                    aux_c--;
                }
            }

            aux_f = cont_f;
            aux_c = cont_c;
        }

        // Comprueba diagonalmente hacia abajo a la izquierda y cambia las fichas de color
        void comprobar_abajo_izquierda(Jugador jugador)
        {
            aux_f++;
            aux_c--;

            if (jugador.color_fichas == NEGRA) // Negras
            {
                while (tabla[aux_f][aux_c] == BLANCA) // Comprobar
                {
                    tabla[aux_f][aux_c] = NEGRA; // Cambiar
                    aux_f++;
                    aux_c--;
                }
            }
            else // Blancas
            {
                while (tabla[aux_f][aux_c] == NEGRA) // Comprobar
                {
                    tabla[aux_f][aux_c] = BLANCA; // Cambiar
                    aux_f++;
                    aux_c--;
                }
            }

            aux_f = cont_f;
            aux_c = cont_c;
        }

        // Borra de la tabla las posibles jugadas (los "?")
        void borrar_posibles()
        {
            for (int i = 0; i < FILAS; i++)
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    if (tabla[i][j] == POSIBLE)
                    {
                        tabla[i][j] = VACIO;
                    }
                }
            }
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

            // Meter los posibles movimientos iniciales en la matriz
            tabla[2][3] = POSIBLE;
            tabla[3][2] = POSIBLE;

            tabla[4][5] = POSIBLE;
            tabla[5][4] = POSIBLE;
                
            // Pedir los nombres de los jugadores
            utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n\n";
            
            utilidad.color(7);
	        cout << "\t\t\t         = INGRESE LOS NOMBRES DE LOS JUGADORES =\n\n\n";
             
            cout << "\t\t\t---> ";
            utilidad.color(12);
			cout << "Primer jugador";
			utilidad.color(7);
			cout << ": ";
            getline(cin, j1.nombre);
			
            cout << "\n\t\t\t---> ";
            utilidad.color(11);
			cout << "Segundo jugador";
			utilidad.color(7);
			cout << ": ";
            getline(cin, j2.nombre);


            // Elegir aleatoriamente quien ser· el primer jugador (fichas negras)
            int aleatorio = rand()%2;

            // Mostrar quien ser· el primer jugador (fichas negras)
            if (aleatorio == 0)
            {
                utilidad.color(12);
                primer_jugador(j1);
                turno = TRUE;
                j1.color_fichas = NEGRA;
                j2.color_fichas = BLANCA;
            }
            else
            {
                utilidad.color(11);
				primer_jugador(j2);
                turno = FALSE;
                j2.color_fichas = NEGRA;
                j1.color_fichas = BLANCA;
            }
        }
};

// Permite interactuar y visualizar el men˙ del juego
class Menu
{
    // == ATRIBUTOS ==
    private:
        bool salir = FALSE;
        char opcion;
        char modo;
        Utilidad utilidad;

    // == M…TODOS ==
    public:
        // Muestra el men˙ del juego
        void iniciar()
        {
            do
            {
                // Mostrar la interfaz del men˙
                titulo();

                // Almacenar la opciÛn escogida por el usuario
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

                    default: // Mostrar mensaje en caso de equivocaciÛn
                        system("cls");
                        cout << "\n\t\t\t\t     -OpciÛn Incorrecta - Intente Nuevamente-\n\n";
                        system("pause");
                }

                system("cls");
            } while (salir == FALSE);
        }

        // Muestra la interfaz del men˙
        void titulo()
        {
        	utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n\n";
			
			utilidad.color(3);
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
			
			utilidad.color(14);
            cout << "\t\t\t\t\t     1";
            utilidad.color(7);
			cout << ". Comenzar Juego\n\n\n";
			
			utilidad.color(14);
            cout << "\t\t\t\t\t     0";
            utilidad.color(7);
			cout << ". Salir del juego\n\n";
            
            utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n";
            
            utilidad.color(7);
            cout << "       OpciÛn (n˙mero) --> ";
        }

        // Muestra los modos de juego disponibles
        void mostrar_modos()
        {
        	utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n\n";
			
			utilidad.color(10);
            cout << "\t\t\t\t\t1"; 
			utilidad.color(7);
			cout << ". Jugador vs Jugador\n\n";
            
            
            utilidad.color(14);
            cout << "\t\t\t\t\t2";
            utilidad.color(7);
			cout << ". Jugador vs CPU\n\n";
            
            
            utilidad.color(13);
            cout << "\t\t\t\t\t3";
            utilidad.color(7);
			cout << ". CPU vs CPU\n\n";
            
            utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n";
            
            utilidad.color(7);
            cout << "      OpciÛn (n˙mero) --> ";
        }

        // Crea la partida en funciÛn del modo de juego elegido
        void crear_partida()
        {
            switch (modo)
            {
                // Se crea una partida de jugador contra jugador
                case '1':
                    Jugador_vs_Jugador partida;
                    partida.configuracion();
                    partida.juego();

                    system("pause");
                    break;
            }
        }
};

// FunciÛn principal
int main()
{
    // Posibilitar el uso de caracteres especiales del idioma espaÒol
    setlocale(LC_CTYPE, "Spanish");
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252);

    // Cambiar el tÌtulo de la ventana por el nombre del juego
    SetConsoleTitleA("Reversi");

    // Inicio formal del programa
    Menu menu;
    menu.iniciar();

    return 0;
}