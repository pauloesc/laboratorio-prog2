/* 4730508 */

#include "../include/pila.h"

#include <assert.h>
#include <stdlib.h> // NULL

// Representación de `pila_t'.
// Se debe definir en pila.cpp.
struct elemento_pila {
	int dato;
	elemento_pila *antecesor;
	
};

struct rep_pila {
	elemento_pila *tope_de_pila;
  //cantidad de elementos q tiene la pila
	int cantidad;
  //maxima capacidad de la pila
	int max_capacidad;
};


// Declaración del tipo `pila_t'
typedef struct rep_pila *pila_t;

/*
  Devuelve una pila_t vacía (sin elementos) que puede tener hasta `tamanio'
  elementos.
 */
pila_t crear_pila(int tamanio) {
  pila_t p = new (rep_pila);
  p->tope_de_pila = NULL;
  p->cantidad = 0;
  p->max_capacidad = tamanio;
  return p;
}

/*
  Apila `num' en `p'.
  Si es_llena_pila(p) no hace nada.
 */
void apilar(int num, pila_t &p){

  if (!es_llena_pila(p)){
    elemento_pila *nuevo = new(elemento_pila);
    //cargo numero
    nuevo->dato = num;
    //reasigno punteros
    nuevo->antecesor = p->tope_de_pila;
    //umonto la cantidad de elementos de la pila
    p->cantidad = p->cantidad + 1;
    p->tope_de_pila = nuevo; 
  }
}

/*
  Remueve de `p' el elemento que está en la cima.
  Si es_vacia_pila(p) no hace nada.
 */
void desapilar(pila_t &p){

  if ( !es_vacia_pila(p) ){
    //borrar apunta a el elemento a desapilar
    elemento_pila *borrar = p->tope_de_pila;
    //reasigno punteros
    p->tope_de_pila = borrar->antecesor;
    //borro 
    delete(borrar);
    borrar = NULL;
    p->cantidad = p->cantidad - 1;

  }

}

/* Libera la memoria asignada a `p'. */
void liberar_pila(pila_t &p){

  while( !es_vacia_pila(p) ){
    desapilar(p);
  }
  delete(p);
  p = NULL;

}

/* Devuelve `true' si y sólo si `p' es vacía (no tiene elementos). */
bool es_vacia_pila(pila_t p){
  return (p->cantidad == 0);
}

/*
  Devuelve `true' si y sólo si la cantidad de elementos en `p' es `tamanio'
  (siendo `tamanio' el valor del parámetro con que fue creada `p').
 */
bool es_llena_pila(pila_t p){
  return ( (p->cantidad) == (p->max_capacidad) );
}

/*
  Devuelve el elemento que está en la cima de `p'.
  Precondición: ! es_vacia_pila(p);
 */
int cima(pila_t p){
  return p->tope_de_pila->dato;
}
