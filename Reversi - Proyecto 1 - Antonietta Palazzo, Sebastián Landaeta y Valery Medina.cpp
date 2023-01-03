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
#include <conio.h>

// == MACROS ==
#define VACIO 0
#define NEGRA 1
#define BLANCA 2
#define POSIBLE 3

#define FILAS 8
#define COLUMNAS 8

using namespace std;

// == CLASES ==

// Contiene cosas necesarias para hacer ciertas cosas
class Utilidad
{
    // == MÉTODOS ==
    public:
        // Pinta elementos en pantalla
        void color(int n)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), n);
        }

        // Mueve el cursor a una coordenada específica
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
        int n_fichas;     // Número de fichas de su color en el tablero
};

// Contiene todo lo referente a las partidas y al funcionamiento del juego
class Partida
{
    // == ATRIBUTOS ==
    protected:
        int tabla[FILAS][COLUMNAS]; // Tablero de juego
        bool turno = TRUE;          // Identifica a qué usuario le toca jugar
        Jugador j1, j2;             // Jugadores de la partida
        int n_turnos = 1;           // Cantidad de turnos
        Utilidad utilidad;          // Objeto para dar estilo al juego
        int cont_f = 0, cont_c = 0; // Coordenada fila y columna (se usan en la colocación de fichas)
        int x = 14, y = 4;          // Coordenada fila y columna (se usan para el gotoxy)
        int aux_f, aux_c;           // Coordenada fila y columna (se usan para el cambio de fichas y la colocación de posibles jugadas)
        bool op;                    // Identifica si se hará un cambio de color de fichas (FALSE) o una colocación de posible jugada en la tabla (TRUE)
        bool salir = FALSE;         // Indica si se ha terminado la partida (TRUE) o no (FALSE)

    // == MÉTODOS ==
    public:
        // Prepara la partida. El cómo lo hará dependerá del tipo de partida a jugar
        virtual void configuracion(){};

        // Comienza la partida
        void juego()
        {
            char aux;
            Jugador jugador;

            do
            {
                turno == TRUE ? jugador = j1 : jugador = j2;
                
                op = TRUE; // Se van a colocar las posibles jugadas en la tabla

                posibles_jugadas(jugador);

                mostrar_tablero();
                mostrar_jugador(jugador);

                utilidad.gotoxy(x, y);    // Se coloca el cursor en la primera posición de la tabla

                op = FALSE; // Se van a cambiar el color de fichas flanqueadas en la tabla

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
                            x += 4;
                            cont_c++;
                            utilidad.gotoxy(x, y);
                        }

                        if ((int)aux == 13)               // Enter
                        {
                            aux_f = cont_f, aux_c = cont_c;

                            if (jugada(jugador) == TRUE)
                            {
                                turno == TRUE ? j1.n_fichas++ : j2.n_fichas++;
                                break;
                            }
                        }
                    }
                }
                
                turno = !turno;    // Se cambia de turno
                n_turnos++;        // Aumenta el turno
                x = 14, y = 4;     // En el siguiente turno se volverá a colocar el cursor en la primera posición de la tabla
                borrar_posibles(); // Se borrarán las posibles jugadas para colocar las del otro jugador

            } while (salir == FALSE);
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
                        cout << "   ";  // Condición para mostrar los espacios vacíos
                    }                
                    
                    if (tabla[i][j] == NEGRA) // Condición para mostrar las fichas negras
                    {
                        utilidad.color(32);
                        cout << " O "; 
                    }

                    if (tabla[i][j] == BLANCA) // Condición para mostrar las fichas blancas
                    {
                        utilidad.color(47);
                        cout << " O ";
                    }

                    if (tabla[i][j] == POSIBLE) // Condición para mostrar las posibles jugadas
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

        // Muestra quien será el primer jugador (fichas negras)
        void primer_jugador(Jugador jugador)
        {
            utilidad.gotoxy(41,12);
            cout << jugador.nombre;
            
            utilidad.color(7);
            cout << " será el primer jugador.\n";
            
            utilidad.color(8);
            cout << "\n       ___________________________________________________________________________________________\n\n\t";
            utilidad.color(7);
            
            system("pause");
        }

        // Muestra la información del jugador en pantalla
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
            cout << "--> Número de Fichas: ";
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
        
        // Coloca en la tabla las posibles jugadas que puede realizar un jugador en su turno
        void posibles_jugadas(Jugador jugador)
        {
            for (int i = 0; i < FILAS; i++) // Buscar las fichas negras en la tabla
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    if ((jugador.color_fichas == NEGRA) ? tabla[i][j] == NEGRA : tabla[i][j] == BLANCA) // Se almacena la posición de la ficha
                    {
                        cont_f = i, cont_c = j;
                        aux_f = cont_f, aux_c = cont_c;
                        comprobaciones(jugador);
                    }
                }
            }

            cont_f = 0, cont_c = 0;
        }

        // Comprueba si puedes realizar la jugada deseada o no, y en caso de poder, la hace
        bool jugada(Jugador jugador)
        {
            if (tabla[cont_f][cont_c] == POSIBLE) // Si es posible realizar la jugada...
            {
                jugador.color_fichas == NEGRA ? tabla[cont_f][cont_c] = NEGRA : tabla[cont_f][cont_c] = BLANCA;

                comprobaciones(jugador);
                return TRUE;
            }
            else // Si no es posible realizar la jugada...
            {
                utilidad.gotoxy(0, 16);
                cout << "El movimiento que desea realizar no es válido. Por favor, intentelo de nuevo.\n";
                system("pause");
                return FALSE;
            }
        }
        
        // Enlista todas las comprobaciones de movimiento en función de la posición en la tabla seleccionada por el usuario
        void comprobaciones(Jugador jugador)
        {
            if (((cont_f >= 2) && (cont_f <= 5)) && ((cont_c >= 2) && (cont_c <= 5))) // Comprobar desde el centro
            {
                comprobar_arriba(jugador);
                comprobar_abajo(jugador);
                comprobar_derecha(jugador);
                comprobar_izquierda(jugador);
                comprobar_arriba_derecha(jugador);
                comprobar_arriba_izquierda(jugador);
                comprobar_abajo_derecha(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            if (((cont_f >= 2) && (cont_f <= 5)) && ((cont_c >= 0) && (cont_c <= 1))) // Comprobar desde el lateral izquierdo
            {
                comprobar_arriba(jugador);
                comprobar_abajo(jugador);
                comprobar_derecha(jugador);
                comprobar_arriba_derecha(jugador);
                comprobar_abajo_derecha(jugador);
                return;
            }

            if (((cont_f >= 2) && (cont_f <= 5)) && ((cont_c >= 6) && (cont_c <= 7))) // Comprobar desde el lateral derecho
            {
                comprobar_arriba(jugador);
                comprobar_abajo(jugador);
                comprobar_izquierda(jugador);
                comprobar_arriba_izquierda(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            if (((cont_f >= 0) && (cont_f <= 1)) && ((cont_c >= 2) && (cont_c <= 5))) // Comprobar desde el lateral superior
            {
                comprobar_abajo(jugador);
                comprobar_derecha(jugador);
                comprobar_izquierda(jugador);
                comprobar_abajo_derecha(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            if (((cont_f >= 6) && (cont_f <= 7)) && ((cont_c >= 2) && (cont_c <= 5))) // Comprobar desde el lateral inferior
            {
                comprobar_arriba(jugador);
                comprobar_derecha(jugador);
                comprobar_izquierda(jugador);
                comprobar_arriba_derecha(jugador);
                comprobar_arriba_izquierda(jugador);
                return;
            }

            if (((cont_f >= 0) && (cont_f <= 1)) && ((cont_c >= 0) && (cont_c <= 1))) // Comprobar desde la esquina superior izquierda
            {
                comprobar_derecha(jugador);
                comprobar_abajo(jugador);
                comprobar_abajo_derecha(jugador);
                return;
            }

            if (((cont_f >= 6) && (cont_f <= 7)) && ((cont_c >= 0) && (cont_c <= 1))) // Comprobar desde la esquina inferior izquierda
            {
                comprobar_arriba(jugador);
                comprobar_derecha(jugador);
                comprobar_arriba_derecha(jugador);
                return;
            }

            if (((cont_f >= 0) && (cont_f <= 1)) && ((cont_c >= 6) && (cont_c <= 7))) // Comprobar desde la esquina superior derecha
            {
                comprobar_abajo(jugador);
                comprobar_izquierda(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            if (((cont_f >= 6) && (cont_f <= 7)) && ((cont_c >= 6) && (cont_c <= 7))) // Comprobar desde la esquina inferior derecha
            {
                comprobar_arriba(jugador);
                comprobar_izquierda(jugador);
                comprobar_arriba_izquierda(jugador);
                return;
            }
        }
        
        // Comprueba hacia arriba para colocar posibles jugadas o cambiar fichas de color
        void comprobar_arriba(Jugador jugador)
        {
            aux_f--;

            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while ((aux_f > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f--;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_f > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_f++;

                        while (aux_f != cont_f)
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA; 
                            aux_f++;
                        }
                    }                    
                }
            }

            aux_f = cont_f;
        }

        // Comprueba hacia abajo para colocar posibles jugadas o cambiar fichas de color
        void comprobar_abajo(Jugador jugador)
        {
            aux_f++;
            
            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while ((aux_f < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f++;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_f < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_f--;

                        while (aux_f != cont_f)
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_f--;
                        }
                    }
                }
            }

            aux_f = cont_f;
        }

        // Comprueba hacia la izquierda para colocar posibles jugadas o cambiar fichas de color
        void comprobar_izquierda(Jugador jugador)
        {
            aux_c--;

            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while ((aux_c > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_c--;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_c > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_c--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_c++;

                        while (aux_c != cont_c)
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_c++;
                        }
                    }
                }
            }

            aux_c = cont_c;
        }

        // Comprueba hacia la derecha para colocar posibles jugadas o cambiar fichas de color
        void comprobar_derecha(Jugador jugador)
        {
            aux_c++;

            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while ((aux_c < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_c++;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_c < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_c++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_c--;

                        while (aux_c != cont_c)
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_c--;
                        }
                    }
                }
            }

            aux_c = cont_c;
        }
        
        // Comprueba diagonalmente hacia arriba a la derecha para colocar posibles jugadas o cambiar fichas de color
        void comprobar_arriba_derecha(Jugador jugador)
        {
            aux_f--, aux_c++;
            
            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while (((aux_f > 0) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f--, aux_c++;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f > 0) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f--, aux_c++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_f++, aux_c--;

                        while ((aux_f != cont_f) && (aux_c != cont_c))
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_f++, aux_c--;
                        }
                    }
                }
            }

            aux_f = cont_f, aux_c = cont_c;
        }

        // Comprueba diagonalmente hacia abajo a la derecha para colocar posibles jugadas o cambiar fichas de color
        void comprobar_abajo_derecha(Jugador jugador)
        {
            aux_f++, aux_c++;

            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while (((aux_f < 7) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f++, aux_c++;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f < 7) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f++, aux_c++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_f--, aux_c--;

                        while ((aux_f != cont_f) && (aux_c != cont_c))
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_f--, aux_c--;
                        }
                    }
                }
            }

            aux_f = cont_f, aux_c = cont_c;
        }

        // Comprueba diagonalmente hacia arriba a la izquierda para colocar posibles jugadas o cambiar fichas de color
        void comprobar_arriba_izquierda(Jugador jugador)
        {
            aux_f--, aux_c--;

            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while (((aux_f > 0) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f--, aux_c--;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas (debo asegurarme de que no cambie de color las fichas a no ser que haya una ficha de su color al final de la hilera)
            {
                while (((aux_f > 0) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f--, aux_c--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_f++, aux_c++;

                        while ((aux_f != cont_f) && (aux_c != cont_c))
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_f++, aux_c++;
                        }
                    }
                }
            }

            aux_f = cont_f, aux_c = cont_c;
        }

        // Comprueba diagonalmente hacia abajo a la izquierda para colocar posibles jugadas o cambiar fichas de color
        void comprobar_abajo_izquierda(Jugador jugador)
        {
            aux_f++, aux_c--;

            if (op == TRUE) // Se hará una colocación de posible jugada
            {
                while (((aux_f < 7) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f++, aux_c--;

                    if (tabla[aux_f][aux_c] == VACIO) // Si es posible colocar una posible jugada...
                    {
                        tabla[aux_f][aux_c] = POSIBLE; // Se coloca la posible jugada y se termina el ciclo
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f < 7) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar
                {
                    aux_f++, aux_c--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA)
                    {
                        aux_f--, aux_c++;

                        while ((aux_f != cont_f) && (aux_c != cont_c))
                        {
                            (jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] = NEGRA : tabla[aux_f][aux_c] = BLANCA;
                            aux_f--, aux_c++;
                        }
                    }
                }
            }

            aux_f = cont_f, aux_c = cont_c;
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

            // Elegir aleatoriamente quien será el primer jugador (fichas negras)
            int aleatorio = rand()%2;

            // Mostrar quien será el primer jugador (fichas negras)
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

// Permite interactuar y visualizar el menú del juego
class Menu
{
    // == ATRIBUTOS ==
    private:
        bool salir = FALSE;
        char opcion;
        char modo;
        Utilidad utilidad;

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
            cout << "       Opción (número) --> ";
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
                    partida.juego();

                    system("pause");
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

    // Cambiar el título de la ventana por el nombre del juego
    SetConsoleTitleA("Reversi");

    // Inicio formal del programa
    Menu menu;
    menu.iniciar();

    return 0;
}