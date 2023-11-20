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
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

// == MACROS ==

// Simbolos que pueden estar en la tabla
#define VACIO 0
#define NEGRA 1
#define BLANCA 2
#define POSIBLE 3

// Dimensiones del tablero
#define FILAS 8
#define COLUMNAS 8
#define TOTAL FILAS * COLUMNAS

// Tipos de retorno del método sin_jugadas()
#define FIN 1
#define PASAR 2
#define CONTINUAR 3

// Modo de juego
#define JUGADOR_VS_JUGADOR 1
#define JUGADOR_VS_CPU 2
#define CPU_VS_CPU 3

using namespace std;

// == CLASES ==

// Permite dar estilo y realizar ciertas tareas en el programa
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

        // Aumenta en uno el tamaño de un vector dinámico de enteros
        void agrandar_vector_int(int *&n_cambios, int &longitud)
        {
            if (longitud == 0)
            {
                n_cambios = new int[1];
                longitud++;
                return;
            }

            longitud++;
            
            // Reservar espacio para el vector auxiliar
            int *aux = new int[longitud];

            // Copiar el contenido de n_cambios en aux
            for (int i = 0; i < longitud-1; i++)
            {
                aux[i] = n_cambios[i];
            }

            // Borrar contenido de n_cambios
            delete[] n_cambios;

            // Copiar contenido de aux en n_cambios (incluyendo el espacio adicional)
            n_cambios = aux;
        }

        // Aumenta en uno el tamaño de una matriz dinámica de enteros
        void agrandar_matriz_int(int **&coord_posibles, int &filas)
        {
            if (filas == 0)
            {
                coord_posibles = new int*[1], coord_posibles[0] = new int[2], filas++;
                return;
            }
            
            filas++;

            // Reservar espacio para la matriz auxiliar
            int **aux = new int*[filas];

            for (int i = 0; i < filas; i++)
            {
                aux[i] = new int[2];
            }

            // Copiar el contenido de coord_posibles en aux
            for (int i = 0; i < filas-1; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    aux[i][j] = coord_posibles[i][j];
                }
            }

            // Borrar contenido de coord_posibles
            for (int i = 0; i < filas-1; i++)
            {
                delete[] coord_posibles[i];
            }

            delete[] coord_posibles;

            // Copiar contenido de aux en coord_posibles (incluyendo el espacio adicional)
            coord_posibles = aux;
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
        int modo;                   // Modo de juego
        int *n_cambios;             // Almacena el número de fichas contrarias que se cambiarían al colocar una ficha propia en cada posible jugada
        int **coord_posibles;       // Almacena las coordenadas en las que se encuentra cada posible jugada
        int longitud = 0;           // Longitud del vector n_cambios
        int filas = 0;              // Número de filas de la matriz coord_posibles
        int tabla[FILAS][COLUMNAS]; // Tablero de juego
        int n_turnos = 1;           // Número de turnos
        int n_posibles = 0;         // Número de posibles jugadas en la tabla
        int pasadas = 0;            // Si llega a dos, significa que ya no hay más jugadas posibles, por lo que el juego terminaría
        int cont_f = 0, cont_c = 0; // Coordenada fila y columna (se usan para el cambio de fichas y la colocación de posibles jugadas)
        int aux_f, aux_c;           // Similares a cont_f y cont_c, pero estas sí van a cambiar de valores durante el turno
        bool turno = TRUE;          // Identifica a qué usuario le toca jugar
        bool op;                    // Indica si se hará un cambio de color de fichas (FALSE) o una colocación de posible jugada en la tabla (TRUE)
        Jugador j1, j2;             // Jugadores de la partida 
        Utilidad utilidad;          // Objeto para dar estilo y realizar ciertas tareas en el programa
        
    // == MÉTODOS ==
    public:
        // Prepara la partida. El cómo lo hará dependerá del tipo de partida a jugar
        virtual void configuracion(){};

        // Asigna nombres a los jugadores. Si el jugador es humano, pide su nombre por teclado 
        // Si es la máquina, se le asigna un nombre por defecto
        virtual void inicializar_nombres(){};
 
        // Comienza la partida según el modo de juego seleccionado
        virtual void juego(){};

        // Prepara la tabla para el inicio de una partida
        void inicializar_tabla()
        {
            // Inicializar tabla
            for (int i = 0; i < FILAS; i++)
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    tabla[i][j] = VACIO;
                }
            }
            
            // Colocar las fichas negras
            tabla[3][4] = NEGRA, tabla[4][3] = NEGRA;

            // Colocar las fichas blancas
            tabla[3][3] = BLANCA, tabla[4][4] = BLANCA;
        }
        
        // Elije aleatoriamente quien será el primer y segundo jugador (fichas negras y blancas, respectivamente)
        void asignar_fichas()
        {
            // Semilla para el randomizer
            srand(time(0));

            // Elección aleatoria
            int aleatorio = rand() % 2;

            // Mostrar al primer jugador y asignar los colores de cada objeto Jugador
            if (aleatorio == 0)
            {
                primer_jugador(j1, aleatorio);
                turno = TRUE;
                j1.color_fichas = NEGRA, j2.color_fichas = BLANCA;
            }
            else
            {
	            primer_jugador(j2, aleatorio);
                turno = FALSE;
                j2.color_fichas = NEGRA, j1.color_fichas = BLANCA;
            }
        }

        // Muestra quien será el primer jugador (fichas negras)
        void primer_jugador(Jugador jugador, int aleatorio)
        {
            system("cls");
            utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n";

            aleatorio == 0 ? utilidad.color(12) : utilidad.color(11);
            
            utilidad.gotoxy(41, 3);
            cout << jugador.nombre;
            
            utilidad.color(7);
            cout << " será el primer jugador.\n";
            
            utilidad.color(8);
            cout << "\n       ___________________________________________________________________________________________\n\n\t";

            utilidad.color(7);
            system("pause");
        }

        // Coloca en la tabla las posibles jugadas que puede realizar un jugador en su turno
        void posibles_jugadas(Jugador jugador)
        {
            // Se van a colocar las posibles jugadas en la tabla
            op = TRUE;

            // Busca las fichas del jugador correspondiente en la tabla
            for (int i = 0; i < FILAS; i++)
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    // Una vez encontrada una ficha, va a comprobar si es posible realizar una jugada en sus adyacencias
                    if ((jugador.color_fichas == NEGRA) ? tabla[i][j] == NEGRA : tabla[i][j] == BLANCA)
                    {
                        cont_f = i, cont_c = j;
                        aux_f = cont_f, aux_c = cont_c;
                        comprobaciones(jugador);
                    }
                }
            }

            cont_f = 0, cont_c = 0;
        }

        // Enlista todas las comprobaciones de movimiento, en función de la posición actual en la tabla
        void comprobaciones(Jugador jugador)
        {
            // Comprobar desde el centro
            if (((cont_f >= 2) && (cont_f <= 5)) && ((cont_c >= 2) && (cont_c <= 5)))
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

            // Comprobar desde el lateral izquierdo
            if (((cont_f >= 2) && (cont_f <= 5)) && ((cont_c >= 0) && (cont_c <= 1)))
            {
                comprobar_arriba(jugador);
                comprobar_abajo(jugador);
                comprobar_derecha(jugador);
                comprobar_arriba_derecha(jugador);
                comprobar_abajo_derecha(jugador);
                return;
            }

            // Comprobar desde el lateral derecho
            if (((cont_f >= 2) && (cont_f <= 5)) && ((cont_c >= 6) && (cont_c <= 7)))
            {
                comprobar_arriba(jugador);
                comprobar_abajo(jugador);
                comprobar_izquierda(jugador);
                comprobar_arriba_izquierda(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            // Comprobar desde el lateral superior
            if (((cont_f >= 0) && (cont_f <= 1)) && ((cont_c >= 2) && (cont_c <= 5)))
            {
                comprobar_abajo(jugador);
                comprobar_derecha(jugador);
                comprobar_izquierda(jugador);
                comprobar_abajo_derecha(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            // Comprobar desde el lateral inferior
            if (((cont_f >= 6) && (cont_f <= 7)) && ((cont_c >= 2) && (cont_c <= 5)))
            {
                comprobar_arriba(jugador);
                comprobar_derecha(jugador);
                comprobar_izquierda(jugador);
                comprobar_arriba_derecha(jugador);
                comprobar_arriba_izquierda(jugador);
                return;
            }

            // Comprobar desde la esquina superior izquierda
            if (((cont_f >= 0) && (cont_f <= 1)) && ((cont_c >= 0) && (cont_c <= 1)))
            {
                comprobar_derecha(jugador);
                comprobar_abajo(jugador);
                comprobar_abajo_derecha(jugador);
                return;
            }

            // Comprobar desde la esquina inferior izquierda
            if (((cont_f >= 6) && (cont_f <= 7)) && ((cont_c >= 0) && (cont_c <= 1)))
            {
                comprobar_arriba(jugador);
                comprobar_derecha(jugador);
                comprobar_arriba_derecha(jugador);
                return;
            }

            // Comprobar desde la esquina superior derecha
            if (((cont_f >= 0) && (cont_f <= 1)) && ((cont_c >= 6) && (cont_c <= 7)))
            {
                comprobar_abajo(jugador);
                comprobar_izquierda(jugador);
                comprobar_abajo_izquierda(jugador);
                return;
            }

            // Comprobar desde la esquina inferior derecha
            if (((cont_f >= 6) && (cont_f <= 7)) && ((cont_c >= 6) && (cont_c <= 7)))
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
                while ((aux_f > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_f--;
                    
                    if (insertar_posible() == TRUE) // Si se cumple esta condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_f > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while ((aux_f < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_f++;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_f < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while ((aux_c > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_c--;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_c > 0) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_c--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while ((aux_c < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_c++;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while ((aux_c < 7) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_c++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while (((aux_f > 0) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_f--, aux_c++;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f > 0) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f--, aux_c++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while (((aux_f < 7) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_f++, aux_c++;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f < 7) && (aux_c < 7)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f++, aux_c++;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while (((aux_f > 0) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_f--, aux_c--;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f > 0) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f--, aux_c--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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
                while (((aux_f < 7) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para posteriormente insertar la posible jugada
                {
                    aux_f++, aux_c--;

                    if (insertar_posible() == TRUE) // Si se cumple la condición, se inserta la posible jugada y se termina el ciclo
                    {
                        break;
                    }
                }
            }
            else // Se hará un cambio de color de fichas
            {
                while (((aux_f < 7) && (aux_c > 0)) && ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == BLANCA : tabla[aux_f][aux_c] == NEGRA)) // Comprobar si hay al menos una ficha del color contrario para flanquear
                {
                    aux_f++, aux_c--;

                    if ((jugador.color_fichas == NEGRA) ? tabla[aux_f][aux_c] == NEGRA : tabla[aux_f][aux_c] == BLANCA) // Si se cumple esta condición, se cambiarán de color todas las fichas posibles
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

        // Comprobar si se puede insertar una posible jugada en la tabla
        bool insertar_posible()
        {
            // Si hay un espacio vacío...
            if (tabla[aux_f][aux_c] == VACIO)
            {
                tabla[aux_f][aux_c] = POSIBLE; // Colocar la posible jugada
                n_posibles++;                  // Incrementar el número de posibles jugadas en la tabla

                // Si estamos en un modo de juego con máquina...
                if ((modo == JUGADOR_VS_CPU) || (modo == CPU_VS_CPU))
                {
                    // Incrementar el tamaño de n_cambios y coord_posibles
                    utilidad.agrandar_vector_int(n_cambios, longitud);
                    utilidad.agrandar_matriz_int(coord_posibles, filas);
                            
                    // Insertar las coordenadas de la posible jugada en la matriz
                    coord_posibles[filas-1][0] = aux_f, coord_posibles[filas-1][1] = aux_c;

                    // Insertar el número de fichas que se cambiarían al realizar la jugada
                    n_cambios[longitud-1] = calcular_cambios();
                }

                return TRUE;
            }

            return FALSE;
        }

        // Calcular el número de fichas que se cambiarían al realizar la jugada
        int calcular_cambios()
        {
            // Cuando el cambio de fichas es vertical o diagonal
            if (((aux_f != cont_f) && (aux_c == cont_c)) || ((aux_f != cont_f) && (aux_c != cont_c)))
            {
                return abs(aux_f - cont_f) - 1;
            }
            // Cuando el cambio de fichas es horizontal
            else if ((aux_f == cont_f) && (aux_c != cont_c))
            {
                return abs(aux_c - cont_c) - 1;
            }
        }

        // Comprueba si hay jugadas posibles en el turno y contiene una condición de fin de partida
        int sin_jugadas(Jugador jugador)
        {
            // Si no hay jugadas posibles...
            if (n_posibles == 0)
            {
                pasadas++; // Se pasa de turno

                mostrar_tablero();
                mostrar_jugador(jugador);
                
                utilidad.gotoxy(0, 16);
                utilidad.color(6);

                if (pasadas == 2) // Condición de fin de partida si ningun jugador puede realizar más movimientos
                {
                    cout << "\n\t\t\t\t     Ya no hay más jugadas disponibles\n";
                    utilidad.color(8);
                    cout << "\n       ___________________________________________________________________________________________\n\n\t";
                    utilidad.color(7);
                    system("pause");
                    
                    system("cls");

                    mostrar_tablero();
                    mostrar_ganador();
                    mostrar_resultados();
                    return FIN;
                }
                else // Si aún no se cumple la condición de fin de partida...
                {
                    cout << "\n\t No puedes realizar ninguna jugada, por lo que se le cederá el turno al siguiente jugador\n";
                    utilidad.color(8);
                    cout << "\n       ___________________________________________________________________________________________\n\n\t";
                    system("pause");
                    finalizar_turno();
                    return PASAR;
                }
            }

            return CONTINUAR;
        }

        // Muestra al ganador de la partida, o si hay un empate
        void mostrar_ganador()
        {
            for (int i = 3; i < 13; i++)
            {
                utilidad.gotoxy(54, i);
                cout << "|";
            }

            utilidad.gotoxy(68, 3);
            utilidad.color(12);
            cout << "= PARTIDA  TERMINADA =";

            if ((j1.n_fichas > j2.n_fichas) || (j1.n_fichas < j2.n_fichas)) // Si hubo un ganador...
            {
                utilidad.gotoxy(68, 6);
                utilidad.color(14);
                cout << "* +  FELICIDADES  + *";

                utilidad.gotoxy(78 - ((j1.n_fichas > j2.n_fichas ? j1.nombre.length() : j2.nombre.length())/2), 9);
                utilidad.color(7);
                cout << (j1.n_fichas > j2.n_fichas ? j1.nombre : j2.nombre);

                utilidad.gotoxy(69, 12);
                utilidad.color(14);
                cout << ".*+ HAS  GANADO +*.";

                utilidad.color(7);
            }
            else // Si hubo un empate...
            {
                utilidad.gotoxy(72, 7);
                utilidad.color(14);
                cout << "* + EMPATE + *";
                utilidad.gotoxy(69, 11);
                utilidad.color(7);
                cout << ".*+ NO HAY GANADOR +*.";
            }
            
            utilidad.gotoxy(0, 16);
        }

        // Muestra el resultado de la partida
        void mostrar_resultados()
        {
            utilidad.color(12);

            utilidad.gotoxy(28, 16);
            cout << "Jugador 1:" << endl;
            utilidad.color(7);

            utilidad.gotoxy(15, 18);
            cout << "Nombre: ";
            utilidad.color(12);
            cout<< j1.nombre << endl;
            utilidad.color(7);

            utilidad.gotoxy(15, 20);
            cout << "Color de Fichas: ";
            utilidad.color(12);
            cout << (j1.color_fichas == NEGRA ? "Negras" : "Blancas") << endl;
            utilidad.color(7);

            utilidad.gotoxy(15, 22);
            cout << "Número de Fichas: ";
            utilidad.color(12);
            cout << j1.n_fichas << endl;
            utilidad.color(8);

            for (int Y = 16; Y < 24; Y++)
            {
                utilidad.gotoxy(54, Y);
                cout << "|";
            }

            utilidad.color(11);

            utilidad.gotoxy(74, 16);
            cout << "Jugador 2: " << endl;
            utilidad.color(7);

            utilidad.gotoxy(61, 18);
            cout << "Nombre: " ;
            utilidad.color(11);
            cout<< j2.nombre << endl;
            utilidad.color(7);
            
            utilidad.gotoxy(61, 20);
            cout << "Color de Fichas: " ;
            utilidad.color(11);
            cout<< (j2.color_fichas == NEGRA ? "Negras" : "Blancas") << endl;
            utilidad.color(7);

            utilidad.gotoxy(61, 22);
            cout << "Número de Fichas: ";
            utilidad.color(11);
            cout<< j2.n_fichas << endl;

            utilidad.color(8);
            cout << "\n\n       ___________________________________________________________________________________________\n\n\t";

            utilidad.color(7);
            system("pause");
        }

        // Muestra el tablero de juego
        void mostrar_tablero()
        {
            system("cls");

            utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n\n";

            cout << "\t    ";

            for (int i = 0; i < FILAS; i++) // Imprimir letras
            {
                utilidad.color(7);
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

                    if (tabla[i][j] == VACIO) // Condición para mostrar los espacios vacíos
                    {
                        cout << "   ";
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

            utilidad.color(8);
            cout << "\n\n       ___________________________________________________________________________________________\n\n";
        }

        // Muestra la información del jugador en pantalla
        void mostrar_jugador(Jugador jugador)
        {
            utilidad.color(8);
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

        // Le permite a un humano realizar una jugada (Este método se usa en los modos Jugador vs Jugador y Jugador vs CPU)
        void jugada_humana(Jugador jugador)
        {
            char aux;
            int x = 14, y = 4; 

            // Colocar el cursor en la primera posición de la tabla
            utilidad.gotoxy(x, y);

            // Moverse por la tabla para realizar una jugada
            while (1)
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

                    if ((int)aux == 13) // Enter
                    {
                        aux_f = cont_f, aux_c = cont_c;

                        if (validar_jugada(jugador) == TRUE) // Si es valida la jugada...
                        {
                            contar_fichas(); // Se actualizan los puntos de los jugadores
                            break;
                        }
                    }
                }
            }
        }

        // Le permite a la maquina realizar una jugada (Este método se usa en los modos Jugador vs CPU y CPU vs CPU)
        void jugada_maquina(Jugador jugador)
        {
            op = FALSE; // Van a cambiar el color de fichas flanqueadas en la tabla

            // Elegir la posible jugada que más fichas le dará a la máquina
            int r = n_cambios[0];
            cont_f = coord_posibles[0][0], cont_c = coord_posibles[0][1];

            for (int i = 0; i < longitud; i++)
            {
                if (r < n_cambios[i])
                {
                    r = n_cambios[i];
                    cont_f = coord_posibles[i][0], cont_c = coord_posibles[i][1];
                }
            }

            utilidad.gotoxy(0, 19);
            utilidad.color(8);
            cout << "\n       ___________________________________________________________________________________________\n\n\t";
            
            // Espera 3 segundos para realizar la jugada
            utilidad.gotoxy(48, 17);
            utilidad.color(7);
            cout << "Pensando.";
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            cout << ".";
            Sleep(1000);
            
            // Una vez elegida la mejor jugada, el programa procede a realizarla
            jugador.color_fichas == NEGRA ? tabla[cont_f][cont_c] = NEGRA : tabla[cont_f][cont_c] = BLANCA;

            aux_f = cont_f, aux_c = cont_c;

            comprobaciones(jugador); // Hace las comprobaciones correspondientes
            contar_fichas(); // Y actualiza los puntos de los jugadores
        }

        // Comprueba si la jugada realizada es válida o no (método usado unicamente cuando la jugada es hecha por un humano)
        bool validar_jugada(Jugador jugador)
        {
            op = FALSE; // Se van a cambiar el color de fichas flanqueadas en la tabla

            if (tabla[cont_f][cont_c] == POSIBLE) // Si es posible realizar la jugada...
            {
                jugador.color_fichas == NEGRA ? tabla[cont_f][cont_c] = NEGRA : tabla[cont_f][cont_c] = BLANCA;
                
                comprobaciones(jugador);
                return TRUE;
            }
            else // Si no es posible realizar la jugada...
            {
                utilidad.gotoxy(0, 17);
                utilidad.color(6);
                cout << "\t\tEl movimiento que desea realizar no es válido. Por favor, intentelo de nuevo.";
                utilidad.color(8);
                cout << "\n\n       ___________________________________________________________________________________________\n\n";
                utilidad.gotoxy(14, 4);
                utilidad.color(7);
                return FALSE;
            }
        }
        
        // Cuenta el número de fichas blancas y negras que hay en el tablero
        void contar_fichas()
        {
            int negras = 0, blancas = 0;

            for (int i = 0; i < FILAS; i++)
            {
                for (int j = 0; j < COLUMNAS; j++)
                {
                    if (tabla[i][j] == NEGRA)
                    {
                        negras++;
                    }
                    else if (tabla[i][j] == BLANCA)
                    {
                        blancas++;
                    }
                }
            }

            (j1.color_fichas == NEGRA) ? (j1.n_fichas = negras, j2.n_fichas = blancas) : (j2.n_fichas = negras, j1.n_fichas = blancas);
        }

        // Condición de fin de partida si la tabla se llena
        bool tabla_llena()
        {
            if ((j1.n_fichas + j2.n_fichas) == TOTAL)
            {
                mostrar_tablero();
                mostrar_ganador();
                mostrar_resultados();
                return TRUE;
            }

            return FALSE;
        }

        // Hace operaciones necesarias antes de que el turno del siguiente jugador empiece
        void finalizar_turno()
        {
            turno = !turno;                   // Cambiar de turno
            longitud = 0, filas = 0;          // Reiniciar los contadores de los arreglos dinámicos
            n_turnos++;                       // Incrementa el número de turnos
            pasadas = 0;                      // La racha de pasadas se reinicia
            n_posibles = 0;                   // Se reinicia el número de posibles jugadas
            borrar_posibles();                // Se borrarán las posibles jugadas para colocar las del otro jugador

            delete[] n_cambios;               // Liberar espacio de n_cambios

            for (int i = 0; i < filas-1; i++) // Liberar espacio de coord_posibles
            {
                delete[] coord_posibles[i];
            }

            delete[] coord_posibles;
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

// Clase hija de Partida. Es el modo de juego Jugador vs Jugador
class Jugador_vs_Jugador : public Partida
{        
    public:
        // Prepara la partida para dos jugadores humanos
        void configuracion()
        {
            // Indicar el modo de juego
            modo = JUGADOR_VS_JUGADOR;
            
            // Inicializar contadores de fichas
            j1.n_fichas = 2, j2.n_fichas = 2;

            inicializar_tabla();
            inicializar_nombres();
            asignar_fichas();
        }

        // Pedir los nombres de los dos jugadores
        void inicializar_nombres()
        {
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
        }

        // Comienza la partida de Jugador vs Jugador
        void juego()
        {
            Jugador jugador;

            do
            {
                turno == TRUE ? jugador = j1 : jugador = j2;

                posibles_jugadas(jugador);

                int respuesta = sin_jugadas(jugador);

                if (respuesta == FIN)
                {
                    break;
                }
                else if (respuesta == PASAR)
                {
                    continue;
                }
                else if (respuesta == CONTINUAR)
                {
                    mostrar_tablero();
                    mostrar_jugador(jugador);
                    jugada_humana(jugador);

                    if (tabla_llena() == TRUE)
                    {
                        break;
                    }

                    finalizar_turno();
                }
            } while (1);
        }
};

// Clase hija de Partida. Es el modo de juego Jugador vs CPU
class Jugador_vs_CPU : public Partida
{
    public:
        // Prepara la partida para un jugador humano y uno controlado por la máquina
        void configuracion()
        {
            // Indicar el modo de juego
            modo = JUGADOR_VS_CPU;
            
            // Inicializar contadores de fichas
            j1.n_fichas = 2, j2.n_fichas = 2;

            inicializar_tabla();
            inicializar_nombres();
            asignar_fichas();
        }

        // Pide el nombre del jugador humano y le asigna uno por defecto a la máquina
        void inicializar_nombres()
        {
            utilidad.color(8);
            cout << "       ___________________________________________________________________________________________\n\n\n";
            
            utilidad.color(7);
	    cout << "\t\t\t\t           = INGRESE SU NOMBRE =\n\n\n";
             
            cout << "\t\t\t---> ";
            utilidad.color(12);
	    cout << "Jugador";
	    utilidad.color(7);
	    cout << ": ";
            getline(cin, j1.nombre);

            j2.nombre = "CPU";
        }

        // Comienza la partida de Jugador vs GPU
        void juego()
        {
            Jugador jugador;

            do
            {
                turno == TRUE ? jugador = j1 : jugador = j2;

                // El problema es aqui
                posibles_jugadas(jugador);

                int respuesta = sin_jugadas(jugador);

                if (respuesta == FIN)
                {
                    break;
                }
                else if (respuesta == PASAR)
                {
                    continue;
                }
                else if (respuesta == CONTINUAR)
                {
                    mostrar_tablero();
                    mostrar_jugador(jugador);
                    identificar_jugador(jugador);

                    if (tabla_llena() == TRUE)
                    {
                        break;
                    }

                    finalizar_turno();
                }
            } while (1);
        }

        // Comprueba si el turno es del humano o de la máquina
        void identificar_jugador(Jugador jugador)
        {
            if (jugador.nombre == "CPU") // Si se cumple, el jugador es la máquina
            {
                jugada_maquina(jugador);
            }
            else // Sino, el jugador es humano
            {
                jugada_humana(jugador);
            }
        }
};

// Clase hija de Partida. Es el modo de juego CPU vs CPU
class CPU_vs_CPU : public Partida
{
    public:
        // Prepara la partida para dos jugadores controlador por la máquina
        void configuracion()
        {
            // Indicar el modo de juego
            modo = CPU_VS_CPU;
            
            // Inicializar contadores de fichas
            j1.n_fichas = 2, j2.n_fichas = 2;

            inicializar_tabla();
            inicializar_nombres();
            asignar_fichas();
        }

        // Asigna nombres estandar a las máquinas
        void inicializar_nombres()
        {
            j1.nombre = "CPU 1", j2.nombre = "CPU 2";
        }

        // Comienza la partida de GPU vs GPU
        void juego()
        {
            Jugador jugador;

            do
            {
                turno == TRUE ? jugador = j1 : jugador = j2;

                posibles_jugadas(jugador);

                int respuesta = sin_jugadas(jugador);

                if (respuesta == FIN)
                {
                    break;
                }
                else if (respuesta == PASAR)
                {
                    continue;
                }
                else if (respuesta == CONTINUAR)
                {
                    mostrar_tablero();
                    mostrar_jugador(jugador);
                    jugada_maquina(jugador);

                    if (tabla_llena() == TRUE)
                    {
                        break;
                    }

                    finalizar_turno();
                }
            } while (1);
        }
};

// Permite interactuar y visualizar el menú del juego
class Menu
{
    // == ATRIBUTOS ==
    private:
        bool salir = FALSE; // Si se vuelve TRUE, se cierra el programa
        char opcion;        // Almacena la opción elegida en el menú principal
        char modo;          // Almacena el modo de juego escogido por el usuario
        Utilidad utilidad;  // Objeto para dar estilo y realizar ciertas tareas en el programa

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
                        utilidad.color(8);
                        cout << "       ___________________________________________________________________________________________\n\n";
                        utilidad.color(10);
                        cout << "\n\t\t\t\t\t  = Gracias por Jugar <(* U *)/ =\n\n";
                        utilidad.color(8);
                        cout << "       ___________________________________________________________________________________________\n\n\t";
                        utilidad.color(7);
                        system("pause");
                        salir = TRUE;
                        break;

                    default: // Mostrar mensaje en caso de equivocación
                        system("cls");
                        utilidad.color(8);
                        cout << "       ___________________________________________________________________________________________\n\n";
                        utilidad.color(12);
                        cout << "\n\t\t\t\t     -Opción Incorrecta - Intente Nuevamente-\n\n";
                        utilidad.color(8);
                        cout << "       ___________________________________________________________________________________________\n\n\t";
                        utilidad.color(7);
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
            Partida *partida;

            switch (modo)
            {
                case '1': // Se crea una partida de Jugador vs Jugador
                    partida = new Jugador_vs_Jugador();
                    partida->configuracion();
                    partida->juego();
                    break;
                case '2': // Se crea una partida de Jugador vs CPU
                    partida = new Jugador_vs_CPU();
                    partida->configuracion();
                    partida->juego();
                    break;
                case '3': // Se crea una partida de CPU vs CPU
                    partida = new CPU_vs_CPU();
                    partida->configuracion();
                    partida->juego();
                    break;

                default: // Mostrar mensaje en caso de equivocación
                    system("cls");
                    utilidad.color(8);
                    cout << "       ___________________________________________________________________________________________\n\n";
                    utilidad.color(12);
                    cout << "\n\t\t\t\t     -Opción Incorrecta - Intente Nuevamente-\n\n";
                    utilidad.color(8);
                    cout << "       ___________________________________________________________________________________________\n\n\t";
                    utilidad.color(7);
                    system("pause");
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
