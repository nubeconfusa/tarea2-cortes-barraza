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

/*****
* void insertar_final_lista
******
* Inserta una carta al final de una lista enlazada
******
* Input:
*   NodoLista*& cabeza : Referencia al puntero de la cabeza de la lista
*   Carta c : Carta a insertar
******
* Returns: void
*****/
void insertar_final_lista(NodoLista*& cabeza, Carta c) {
    NodoLista* nuevo = new NodoLista;
    nuevo->c = c;
    nuevo->sig = nullptr;
    
    if (!cabeza) {
        cabeza = nuevo;
        return;
    }
    
    NodoLista* actual = cabeza;
    while (actual->sig) {
        actual = actual->sig;
    }
    actual->sig = nuevo;
}

/*****
* int contar_nodos_lista
******
* Cuenta la cantidad de nodos en una lista enlazada
******
* Input:
*   NodoLista* cabeza : Cabeza de la lista
******
* Returns:
*   int : Cantidad de nodos en la lista
*****/
int contar_nodos_lista(NodoLista* cabeza) {
    int cuenta = 0;
    NodoLista* actual = cabeza;
    while (actual) {
        cuenta++;
        actual = actual->sig;
    }
    return cuenta;
}

/*****
* Carta* obtener_carta_en_indice
******
* Obtiene un puntero a la carta en el índice especificado de la lista
******
* Input:
*   NodoLista* cabeza : Cabeza de la lista
*   int indice : Índice de la carta (0-based)
******
* Returns:
*   Carta* : Puntero a la carta en el índice, o nullptr si no existe
*****/
Carta* obtener_carta_en_indice(NodoLista* cabeza, int indice) {
    NodoLista* actual = cabeza;
    int i = 0;
    while (actual) {
        if (i == indice) return &(actual->c);
        actual = actual->sig;
        i++;
    }
    return nullptr;
}

/*****
* void eliminar_nodo_en_indice
******
* Elimina el nodo en el índice especificado de la lista
******
* Input:
*   NodoLista*& cabeza : Referencia al puntero de la cabeza de la lista
*   int indice : Índice del nodo a eliminar (0-based)
******
* Returns: void
*****/
void eliminar_nodo_en_indice(NodoLista*& cabeza, int indice) {
    if (!cabeza) return;
    
    if (indice == 0) {
        NodoLista* temp = cabeza;
        cabeza = cabeza->sig;
        delete temp;
        return;
    }
    
    NodoLista* actual = cabeza;
    int i = 0;
    while (actual->sig && i < indice - 1) {
        actual = actual->sig;
        i++;
    }
    
    if (actual->sig) {
        NodoLista* temp = actual->sig;
        actual->sig = temp->sig;
        delete temp;
    }
}


/*****
* Carta remover_primera_carta
******
* Remueve y retorna la primera carta de la lista
******
* Input:
*   NodoLista*& cabeza : Referencia al puntero de la cabeza de la lista
******
* Returns:
*   Carta : La carta removida
*****/
Carta remover_primera_carta(NodoLista*& cabeza) {
    Carta c = cabeza->c;
    NodoLista* temp = cabeza;
    cabeza = cabeza->sig;
    delete temp;
    return c;
}

/*****
* void liberar_lista
******
* Libera toda la memoria de una lista enlazada
******
* Input:
*   NodoLista*& cabeza : Referencia al puntero de la cabeza de la lista
******
* Returns: void
*****/
void liberar_lista(NodoLista*& cabeza) {
    while (cabeza) {
        NodoLista* temp = cabeza;
        cabeza = cabeza->sig;
        delete temp;
    }
}

/*****
* void marcar_carta_jugada
******
* Marca una carta como jugada en el ABB correspondiente
******
* Input:
*   char palo : Palo de la carta ('C', 'E', 'D', 'T')
*   int cat : Categoría de la carta
*   bool estado : true para marcar como jugada, false para desmarcar
******
* Returns: void
*****/
void marcar_carta_jugada(char palo, int cat, bool estado) {
    int idx_palo = -1;
    if (palo == 'C') idx_palo = 0;
    else if (palo == 'E') idx_palo = 1;
    else if (palo == 'D') idx_palo = 2;
    else if (palo == 'T') idx_palo = 3;
    
    if (idx_palo == -1) return;
    
    NodoABB* nodo = buscar_en_abb(palos[idx_palo], cat);
    if (nodo) {
        nodo->c.jugada = estado;
    }
}

/*****
* void reiniciar_cartas_jugadas_recursivo
******
* Reinicia el estado de todas las cartas en un árbol (pone jugada = false)
******
* Input:
*   NodoABB* raiz : Raíz del árbol
******
* Returns: void
*****/
void reiniciar_cartas_jugadas_recursivo(NodoABB* raiz) {
    if (!raiz) return;
    raiz->c.jugada = false;
    reiniciar_cartas_jugadas_recursivo(raiz->izq);
    reiniciar_cartas_jugadas_recursivo(raiz->der);
}

/*****
* void reiniciar_todas_cartas_jugadas
******
* Reinicia el estado de todas las cartas en todos los palos
******
* Input: Ninguno
******
* Returns: void
*****/
void reiniciar_todas_cartas_jugadas() {
    for (int i = 0; i < 4; ++i) {
        reiniciar_cartas_jugadas_recursivo(palos[i]);
    }
}

/*****
* void recolectar_cartas_recursivo
******
* Recolecta todas las cartas de un árbol en un arreglo
******
* Input:
*   NodoABB* raiz : Raíz del árbol
*   Carta arr[] : Arreglo donde almacenar las cartas
*   int& idx : Índice actual en el arreglo (se incrementa)
******
* Returns: void
*****/
void recolectar_cartas_recursivo(NodoABB* raiz, Carta arr[], int& idx) {
    if (!raiz) return;
    recolectar_cartas_recursivo(raiz->izq, arr, idx);
    arr[idx++] = raiz->c;
    recolectar_cartas_recursivo(raiz->der, arr, idx);
}

/*****
* void barajar_mazo
******
* Crea una lista enlazada con las 52 cartas en orden aleatorio
******
* Input:
*   NodoLista*& mazo : Referencia al puntero del mazo
******
* Returns: void
*****/
void barajar_mazo(NodoLista*& mazo) {
    // Recolectar todas las cartas de los 4 árboles
    Carta todas[52];
    int idx = 0;
    
    for (int i = 0; i < 4; ++i) {
        recolectar_cartas_recursivo(palos[i], todas, idx);
    }
    
    // Algoritmo para barajar
    for (int i = 51; i > 0; --i) {
        int j = rand() % (i + 1);
        // Intercambiar todas[i] y todas[j]
        Carta temp = todas[i];
        todas[i] = todas[j];
        todas[j] = temp;
    }
    
    // Insertar en la lista
    for (int i = 0; i < 52; ++i) {
        insertar_final_lista(mazo, todas[i]);
    }
}



















int main(){
    srand(time(0));

    cout << "Bienvenido!" << endl;
    
    // Construir los árboles de los palos
    construir_todos_los_palos();
    
    // Jugar las 3 ciegas
    const char* nombres_ciegas[] = {"ciega pequeña", "ciega grande", "ciega jefe"};
    



}