# Proyecto Balatro Balatrez quiere jugar Balatro

## Integrantes del Proyecto

   - Nombre: Isidora Cortes. 
   - Rol: 202373007-6
   - Nombre: Sofia Barraza.
   - Rol: 202473004-5

## Descripción

El presente proyecto es una implementación del juego de cartas Balatro, un juego estilo póker solitario donde el jugador debe superar tres ciegas (rondas) acumulando fichas mediante la formación de manos de póker. 

## GITHUB
https://github.com/nubeconfusa/tarea2-cortes-barraza.git 

## Características Técnicas

- *Sistema Operativo*: Linux (Ubuntu 20.04 LTS) / WSL 2 en Windows 10/11
- *Lenguaje*: C++
- *Compilador*: g++
- *Herramientas de Build*: Make
- *Bibliotecas Utilizadas*:
  - <iostream>: Entrada/salida estándar
  - <cstdlib>: Funciones de utilidad (rand(), srand())
  - <ctime>: Manejo de tiempo (time() para semilla aleatoria)

### Compilación

1) La forma estándar es: make
2) El equivalente manual es: g++ -Wall -Wextra -std=c++11 -O2 T2SEM2.cpp -o balatro


## Características Principales

### Estructuras de Datos


1. Árbol Binario de Búsqueda (ABB) Balanceado
   - Se implementaron 4 árboles, uno por cada palo (Corazones, Espadas, Diamantes, Tréboles)
   - Cada árbol almacena 13 cartas (As al Rey)
   - Construcción balanceada mediante algoritmo recursivo
   - Permite búsqueda eficiente de cartas por categoría

2. Lista Enlazada para el Mazo Barajado
   - Almacena las 52 cartas en orden aleatorio
   - Implementa algoritmo de barajado Fisher-Yates
   - Se reconstruye al inicio de cada ciega

3. Lista Enlazada para la Mano del Jugador
   - Mantiene las cartas actuales del jugador (8 cartas, 7 con El Grillete)
   - Ordenamiento automático por categoría descendente
   - Operaciones de inserción y eliminación eficientes

### Sistema

#### Ciegas (Rondas)

1. Ciega Pequeña: 250 fichas objetivo
2. Ciega Grande: 350 fichas objetivo
3. Ciega Jefe: 500 fichas objetivo + Modificador

#### Modificador de Jefe Implementado: El Grillete
- Efecto: Reduce el tamaño de la mano del jugador de 8 a 7 cartas
- Aplicación: Solo en la Ciega Jefe (ronda 3)
- Implementación: Se configura automáticamente al inicializar la ciega jefe
- Justificación: Este modificador fue elegido por su simplicidad conceptual y por afectar directamente la estrategia del jugador sin requerir lógica adicional compleja

### Tipos de Manos Implementadas

Nivel 1 - Carta más alta*
- Fichas Base: 50
- Cartas que Anotan: 1 carta

Nivel 2 - Par
- Fichas Base: 40
- Cartas que Anotan: 2 cartas

Nivel 3 - Doble Par
- Fichas Base: 80
- Cartas que Anotan: 4 cartas

Nivel 4 - Tercia
- Fichas Base: 120
- Cartas que Anotan: 3 cartas

Nivel 5 - Escalera
- Fichas Base: 150
- Cartas que Anotan: 5 cartas

Nivel 6 - Color
- Fichas Base: 200
- Cartas que Anotan: 5 cartas

Nivel 7 - Póker
- Fichas Base: 400



- Cartas que Anotan: 4 cartas

---

## Estructura del Código

El código contiene:

1. Estructuras de Datos
   - Carta: Información de cada carta
   - NodoABB: Nodo del árbol binario
   - NodoLista: Nodo de lista enlazada
   - Estado: Estado del juego
   - ResultadoMano: Resultado de detección de manos

2. Funciones de Utilidad
   - Conversión de categorías
   - Cálculo de valores

3. Funciones de ABB
   - Construcción de árboles balanceados
   - Búsqueda en árbol
   - Liberación de memoria

4. Funciones de Lista Enlazada
   - Inserción, eliminación, conteo
   - Gestión de nodos

5. Gestión de Cartas
   - Marcado de cartas jugadas
   - Recolección de cartas
   - Sistema de rastreo

6. Barajado y Mazo
   - Algoritmo Fisher-Yates
   - Entrega de cartas

7. Ordenamiento
   - Ordenamiento por categoría (As es mayor)

8. Visualización
   - Mostrar mano
   - Comando "M" (mostrar todas las cartas)

9. Detección de Manos
   - 7 funciones de detección (una por tipo)
   - Función principal de detección

10. Cálculo de Puntaje
    - Fichas base por tipo
    - Cálculo total

11. Gestión de Estado
    - Inicialización de ciegas
    - Visualización de estado

12. Flujo del Juego
    - Inicio de ronda
    - Procesamiento de jugadas y descartes
    - Verificación de victoria/derrota

13. Función Principal
    - Loop de las 3 ciegas
    - Interacción con el usuario
    - Gestión de memoria