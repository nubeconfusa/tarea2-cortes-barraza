# Proyecto Balatro Balatrez quiere jugar Balatro

## Integrantes del Proyecto

   - Nombre: Isidora Cortes.
   - Nombre: Sofia Barraza.
   - Rol: 202373007-k

## Descripción

El presente proyecto es una implementación del juego de cartas Balatro, un juego estilo póker solitario donde el jugador debe superar tres ciegas (rondas) acumulando fichas mediante la formación de manos de póker.

## Características Técnicas

- **Sistema Operativo**: Linux (Ubuntu 20.04 LTS) / WSL 2 en Windows 10/11
- **Lenguaje**: C++
- **Compilador**: g++
- **Herramientas de Build**: Make
- **Bibliotecas Utilizadas**:
  - `<iostream>`: Entrada/salida estándar
  - `<cstdlib>`: Funciones de utilidad (`rand()`, `srand()`)
  - `<ctime>`: Manejo de tiempo (`time()` para semilla aleatoria)

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