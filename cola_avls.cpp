/* 4730508 */

#include "../include/avl.h"
#include "../include/cola_avls.h"

#include <assert.h>
#include <stdlib.h> // NULL


// Representación de `cola_avls_t'.
// Se debe definir en cola_avls.cpp
struct nodo_cola{
	avl_t dato;
	nodo_cola *siguiente;
};

struct rep_cola_avls{
	nodo_cola *inicio;
	nodo_cola *final;
};
// Declaración del tipo `cola_avls_t'
typedef struct rep_cola_avls *cola_avls_t;

/*  Devuelve una cola_avls_t vacía (sin elementos). */
cola_avls_t crear_cola_avls(){

	//creacion
	cola_avls_t nueva_cola = new(rep_cola_avls);
	nueva_cola->inicio = NULL;
	nueva_cola->final = NULL;
	return nueva_cola;
}

/* Encola `avl' en `c'. */
void encolar(avl_t b, cola_avls_t &c){

	nodo_cola *nuevo = new(nodo_cola);
	//cargo el dato en el nodo
	nuevo->dato = b;
	nuevo->siguiente=NULL;

	if (!es_vacia_cola_avls(c)){
		c->final->siguiente = nuevo;
		c->final=nuevo;
	}
	else{
		c->inicio = nuevo;
		c->final = nuevo;
	}

}

/*
  Remueve de `c' el elemento que está en el frente.
  NO libera la memoria del elemento removido.
  Si es_vacia_cola_binarios(c) no hace nada.
 */
void desencolar(cola_avls_t &c){
	if(!es_vacia_cola_avls(c)){

		//elemento a borrar
		nodo_cola *borrar = c->inicio;

		if (c->inicio == c->final){
			c->inicio = NULL; 
			c->final = NULL;
		}else{
			c->inicio = c->inicio->siguiente;
		}
		delete(borrar);
		borrar = NULL;
	}
}

/* Libera la memoria asignada a `c', pero NO la de sus elementos. */
void liberar_cola_avls(cola_avls_t &c){

	while( !es_vacia_cola_avls(c) ){
		desencolar(c);
	}
	delete(c);
	c = NULL;
}

/* Devuelve `true' si y sólo si `c' es vacía (no tiene elementos). */
bool es_vacia_cola_avls(cola_avls_t c){
	return ( (c->final == NULL) && (c->inicio == NULL) );
}

/*
  Devuelve el elemento que está en el frente de `c'.
  Precondición: ! es_vacia_cola_binarios(c);
 */
avl_t frente(cola_avls_t c){
	return c->inicio->dato;
}