#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Estructura de una carta del juego
struct Carta {
    char palo;          // 'C', 'E', 'D', 'T'
    int categoria;      // 1 (As) a 13 (Rey)
    int valor;          // 11 si es As, 10 si es J/Q/K, sino igual a categoria
    bool jugada;        // indica si la carta ya fue jugada en la ronda
};

// Nodo del Árbol Binario de Búsqueda
struct NodoABB {
    Carta c;
    NodoABB* izq;
    NodoABB* der;
};

// Nodo de la Lista Enlazada
struct NodoLista {
    Carta c;
    NodoLista* sig;
};

// Estado del juego en la ronda actual
struct Estado {
    int ronda_idx;              // 0=ciega pequeña, 1=ciega grande, 2=ciega jefe
    int pozo_obj;               // 250, 350, 500
    int fichas_acumuladas;      // suma de fichas en la ronda
    int manos_restantes;        // inicia en 4
    int descartes_restantes;    // inicia en 3
    int tam_mano_max;           // 8 (o 7 con El Grillete)
};

// Estructura para el resultado de detección de mano
struct ResultadoMano {
    int tipo;                   // 0=carta alta, 1=par, 2=doble par, 3=tercia, 4=escalera, 5=color, 6=poker
    int cartas_que_anotan[5];   // índices de cartas que anotan en el arreglo de cartas jugadas
    int num_cartas_anotan;      // cantidad de cartas que anotan
};

// Arreglo de 4 árboles, uno por cada palo: 0=C, 1=E, 2=D, 3=T
NodoABB* palos[4];

/*****
* int valor_por_categoria
******
* Retorna el valor de una carta según su categoría para el cálculo de puntaje
******
* Input:
*   int k : Categoría de la carta (1-13)
******
* Returns:
*   int : Valor de la carta (11 para As, 10 para J/Q/K, categoría para el resto)
*****/
int valor_por_categoria(int k) {
    if (k == 1) return 11;      // As
    if (k >= 11) return 10;     // J, Q, K
    return k;                   // 2..10
}

/*****
* char categoria_a_char
******
* Convierte una categoría numérica a su representación de caracteres para impresión
******
* Input:
*   int cat : Categoría de la carta (1-13)
******
* Returns:
*   char* : String con la representación (ej: "1", "11", "12")
*****/
const char* categoria_a_char(int cat) {
    static char buffer[3];
    if (cat == 1) return "1";       // As
    if (cat == 10) return "10";     // 10
    if (cat == 11) return "11";     // J
    if (cat == 12) return "12";     // Q
    if (cat == 13) return "13";     // K
    buffer[0] = '0' + cat;
    buffer[1] = '\0';
    return buffer;
}

/*****
* NodoABB* construir_abb_balanceado
******
* Construye un ABB balanceado a partir de un arreglo de cartas ordenado
******
* Input:
*   Carta arr[] : Arreglo de cartas ordenadas
*   int izq : Índice izquierdo del segmento
*   int der : Índice derecho del segmento
******
* Returns:
*   NodoABB* : Raíz del subárbol construido
*****/
NodoABB* construir_abb_balanceado(Carta arr[], int izq, int der) {
    if (izq > der) return nullptr;
    
    int m = (izq + der) / 2;
    NodoABB* nodo = new NodoABB;
    nodo->c = arr[m];
    nodo->izq = construir_abb_balanceado(arr, izq, m - 1);
    nodo->der = construir_abb_balanceado(arr, m + 1, der);
    
    return nodo;
}

/*****
* NodoABB* construir_palo
******
* Construye un ABB balanceado con las 13 cartas de un palo específico
******
* Input:
*   char p : Palo de las cartas ('C', 'E', 'D', 'T')
******
* Returns:
*   NodoABB* : Raíz del árbol con las 13 cartas del palo
*****/
NodoABB* construir_palo(char p) {
    Carta a[13];
    for (int i = 0; i < 13; ++i) {
        a[i].palo = p;
        a[i].categoria = i + 1;
        a[i].valor = valor_por_categoria(i + 1);
        a[i].jugada = false;
    }
    return construir_abb_balanceado(a, 0, 12);
}

/*****
* void construir_todos_los_palos
******
* Construye los 4 árboles de los palos y los almacena en el arreglo global
******
* Input: Ninguno
******
* Returns: void
*****/
void construir_todos_los_palos() {
    palos[0] = construir_palo('C');
    palos[1] = construir_palo('E');
    palos[2] = construir_palo('D');
    palos[3] = construir_palo('T');
}

/*****
* NodoABB* buscar_en_abb
******
* Busca una carta en el ABB por su categoría
******
* Input:
*   NodoABB* raiz : Raíz del árbol donde buscar
*   int cat : Categoría de la carta a buscar
******
* Returns:
*   NodoABB* : Nodo que contiene la carta, o nullptr si no se encuentra
*****/
NodoABB* buscar_en_abb(NodoABB* raiz, int cat) {
    if (!raiz) return nullptr;
    if (cat == raiz->c.categoria) return raiz;
    if (cat < raiz->c.categoria) return buscar_en_abb(raiz->izq, cat);
    return buscar_en_abb(raiz->der, cat);
}

/*****
* void liberar_abb
******
* Libera toda la memoria de un árbol recursivamente
******
* Input:
*   NodoABB* raiz : Raíz del árbol a liberar
******
* Returns: void
*****/
void liberar_abb(NodoABB* raiz) {
    if (!raiz) return;
    liberar_abb(raiz->izq);
    liberar_abb(raiz->der);
    delete raiz;
}
