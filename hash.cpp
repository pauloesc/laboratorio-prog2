/* 4730508 */

/*
  Módulo de definición de `hash_t'.

  Se define un hash de asociaciones de número a texto.
  La cantidad máxima de asociaciones queda determinada por el parámetro
  `tamanio' pasado en `crear_tabla'.

  Laboratorio de Programación 2.
  InCo-FIng-UDELAR
 */

#include "../include/cadena.h"

#include <assert.h>
#include <stdlib.h> // NULL
#include <stdio.h>  // sprintf, scanf, ungetc, stdin


// Representación de `hash_t'.
// Se debe definir en hash.cpp.
struct rep_hash{
	cadena_t *datos;
	int cant_insertados;
	int max_capacidad;
};
// Declaración del tipo `hash_t'.
typedef rep_hash *hash_t;

/*
 Crea un hash_t de asociaciones numero->texto.
 Podrá haber hasta `tamanio' asociaciones.
 */
hash_t crear_hash(nat tamanio){
	hash_t nuevo = new(rep_hash);
	
	nuevo->datos = NULL;
	cadena_t *arreglo = new cadena_t[int(tamanio)];
	nuevo->datos = arreglo;

	for(int indice = 0; indice < int(tamanio); indice++){
		nuevo->datos[indice] = crear_cadena();
	}

	nuevo->cant_insertados = 0;
	nuevo->max_capacidad = int(tamanio);

	return nuevo;
}

/*
  Inserta en `h' la asociación entre `clave' y `valor'.
  Precondición: !esta_lleno_hash(t) y !existe_asociacion(clave, h).
  El tiempo de ejecución es O(1).
 */
void asociar_en_hash(int clave, char *valor, hash_t &h){

	int posicion = (clave % h->max_capacidad);	
	if (posicion < 0){
		posicion= posicion + h->max_capacidad;
	}

	assert( posicion < 0 );

	//creo la info
	info_t nueva_info = crear_info(clave, valor);

	cadena_t cadena = h->datos[posicion];

	if ( es_vacia_cadena(cadena) ){
		insertar_al_final(nueva_info, cadena);
	}
	else{
		//funcion de cadena
		insertar_antes( nueva_info, inicio_cadena( cadena ), cadena );
	}

	h->cant_insertados++;

}

/*
  Sustituye en `h' el anterior valor asociado a `clave' por `valor'.
  Precondición: existe_asociacion(clave, h).
  El tiempo de ejecución es O(1).
 */
void actualizar_hash(int clave, char *valor, hash_t &h){
	int posicion = (clave % h->max_capacidad);	
	if (posicion < 0){
		posicion= posicion + h->max_capacidad;
	}
	cadena_t cadena = h->datos[posicion];
	info_t nueva = crear_info(clave, valor);
	insertar_antes( nueva, inicio_cadena( cadena ), cadena );
}

/*
  Elimina de `h' la asociación entre `clave' y algún valor y libera la menoria
  asignada a mantener esa asociación.
  Precondición: existe_asociacion(clave, h).
  El tiempo de ejecución es O(1) en promedio.
 */
void eliminar_de_hash(int clave, hash_t &h){

	int posicion = (clave % h->max_capacidad);	
	if (posicion < 0){
		posicion= posicion + h->max_capacidad;
	}
	cadena_t cadena = h->datos[posicion];

	localizador_t elemento = siguiente_clave(clave, inicio_cadena(cadena), cadena);

	while( es_localizador(elemento) ){

		localizador_t siguiente_a_elemento = siguiente( elemento, cadena );
		remover_de_cadena(elemento, cadena);
		elemento = siguiente_clave(clave, siguiente_a_elemento, cadena);
	} 

	h->cant_insertados--;

}

/*
  Libera la memoria asignada a `h' y todos sus elementos.
 */
void liberar_hash(hash_t &h){

	for(int i = 0; i < h->max_capacidad; i++){
		cadena_t cadena = h->datos[i];
		liberar_cadena(cadena);
	}

	delete[] h->datos;
	delete h;
}

/*
  Devuelve `true' si y sólo si en `h' hay una asociación entre `clave'
  y algún valor.
  El tiempo de ejecución es O(1) en promedio.
 */
bool existe_asociacion(int clave, hash_t h){

	int posicion = (clave % h->max_capacidad);	
	if (posicion < 0){
		posicion= posicion + h->max_capacidad;
	}

	cadena_t cadena =  h->datos[posicion];

	if ( es_vacia_cadena(cadena) ){

		bool hola = false;
		return hola;
	}
	else{

		localizador_t elemento = siguiente_clave(clave, inicio_cadena(cadena), cadena);
		return es_localizador(elemento);
	}
	
	
}

/*
  Devuelve el valor correspondiente a la asociacion de `clave' en `h'.
  Precondición: existe_asociacion(clave, h).
  El tiempo de ejecución es O(1) en promedio.
 */
char *valor_en_hash(int clave, hash_t h){
	
		int posicion = (clave % h->max_capacidad);	
	if (posicion < 0){
		posicion= posicion + h->max_capacidad;
	}
	cadena_t cadena =  h->datos[posicion];
	localizador_t elemento = siguiente_clave(clave, inicio_cadena(cadena), cadena);
	return frase_info( info_cadena(elemento,cadena) );


}

/*
  Devuelve `true' si y sólo si `h' tiene `tamanio' elementos.
  El tiempo de ejecución es O(1).
 */
bool esta_lleno_hash(hash_t h){
	return (h->cant_insertados == h->max_capacidad);
}
