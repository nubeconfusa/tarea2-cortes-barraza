# Proyecto Balatro Balatrez quiere jugar Balatro

## Integrantes del Proyecto

   - Nombre: Isidora Cortes.
   - Nombre: Sofia Barraza.
   - Rol: 202373007-k

## Descripción

El presente proyecto es una implementación del juego de cartas Balatro, un juego estilo póker solitario donde el jugador debe superar tres ciegas (rondas) acumulando fichas mediante la formación de manos de póker.

## Características Técnicas

## Entorno de Desarrollo

## Compilación

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
