/* 4730508 */

/*
  Módulo de definición de `cadena_t'.

  Lista doblemente enlazada de elementos de tipo `info_t', con cabezal con
  punteros al inicio y al final.

  Laboratorio de Programación 2.
  InCo-FIng-UDELAR
 */

#include "../include/cadena.h"
#include "../include/info.h"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

struct nodo {
    info_t dato;
    nodo *anterior;
    nodo *siguiente;
};

struct rep_cadena {
    nodo *inicio;
    nodo *final;
};

/*
  Devuelve la cadena_t vacía (sin elementos).
  El tiempo de ejecución es O(1).
 */
cadena_t crear_cadena(){
	cadena_t res = new(rep_cadena);
	res->inicio = NULL;
	res->final = NULL;
	return res;
}

/*
  Se inserta `i' como último elemento de `cad'.
  Devuelve `cad'.
  Si es_vacia_cadena (cad) `i' se inserta como único elemento de `cad'.
  El tiempo de ejecución es O(1).
 */
cadena_t insertar_al_final(info_t i, cadena_t cad){
	nodo *nuevo = new(nodo);
	nuevo->dato = i;
	nuevo->siguiente = NULL;
	nuevo->anterior = cad->final;
	if(cad->final == NULL) {
		//assert(cad->inicio == NULL);
		cad->inicio = nuevo;
	}
	else{
		//assert(cad->inicio != NULL);
		cad->final->siguiente = nuevo;
	}
	cad->final = nuevo;
	return cad;
}

/*
  Se inserta `i' como un nuevo elemento inmediatamente antes de `loc'.
  Devuelve `cad'.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
cadena_t insertar_antes(info_t i, localizador_t loc, cadena_t cad){
	//assert(localizador_en_cadena(loc, cad));
	//creo nuevo elemento
	localizador_t nuevo = new(nodo);
	nuevo->dato = i;
	if(es_inicio_cadena(loc,cad)){
		cad->inicio = nuevo;
		nuevo->siguiente = loc;
		nuevo->anterior = NULL;
		loc->anterior = nuevo;
	}
	else{
		//reasigno punteros para que la cadena quede consistente
		localizador_t anterior_a_loc = anterior(loc,cad);
		anterior_a_loc->siguiente = nuevo;
		nuevo->anterior = anterior_a_loc;
		nuevo->siguiente = loc;
		loc->anterior = nuevo;
	}
	return cad;
}


/*
  Se inserta la cadena_t `sgm' inmediatamente después de `loc' en `cad',
  manteniendo los elementos originales y el orden relativo entre ellos.
  Devuelve `cad'.
  No se debe obtener memoria (los nodos de `sgm' pasan a ser parte de `cad').
  Al terminar, `sgm' queda vacía.
  Si es_vacia_cadena(cad) `loc' es ignorado y el segmento queda insertado.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
cadena_t insertar_segmento_despues(cadena_t &sgm, localizador_t loc, cadena_t cad){
	//assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
	if(es_vacia_cadena(cad)){
		cad->inicio = sgm->inicio;
		cad->final = sgm->final;
	}
	else{
		if(!es_vacia_cadena(sgm)){
			sgm->inicio->anterior = loc;
			sgm->final->siguiente = loc->siguiente;
			if(es_final_cadena(loc,cad)){
				cad->final = sgm->final;
			}
			else{
				loc->siguiente->anterior = sgm->final;
			}
			loc->siguiente = sgm->inicio;
		}
	}
	sgm->inicio = sgm->final = NULL;
	return cad;
}


/*
  Devuelve una cadena_t con los elementos de `cad' que se encuentran entre
  `desde' y `hasta', incluidos.
  La cadena_t resultado no comparte memoria con `cad'.
  Si es_vacia_cadena(cad) devuelve la cadena_t vacia.
  Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena resultado.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena resultado.
 */
cadena_t copiar_segmento(localizador_t desde, localizador_t hasta, cadena_t cad){
	//assert(es_vacia_cadena(cad) || precede_en_cadena(desde, hasta, cad));
	cadena_t res = crear_cadena();
	if(!es_vacia_cadena(cad)){
		localizador_t loc = desde;
		while (loc != siguiente(hasta, cad)){
			info_t info = copia_info(loc->dato);
			insertar_al_final( info , res);
			loc = siguiente(loc, cad);
		}
	}
	return res;
}

/*
  Remueve de `cad' los elementos que se encuentran  entre `desde' y `hasta',
  incluidos y libera la memoria que tenían asignada y la de sus nodos.
  Devuelve `cad'.
  Si es_vacia_cadena(cad) devuelve la cadena_t vacía.
  Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena resultado.
 */
cadena_t cortar_segmento(localizador_t desde, localizador_t hasta, cadena_t cad){
	//assert( es_vacia_cadena(cad) || precede_en_cadena(desde, hasta, cad) );
	if(es_vacia_cadena(cad)){
		return cad;
	}
	else{
		//creo una nueva cadena que se corresponde con los elementos que quiero eliminar
		cadena_t eliminar = crear_cadena();
		eliminar->inicio = desde;
		eliminar->final = hasta;

		localizador_t anterior_a_desde = anterior(desde,cad);
		localizador_t siguiente_a_hasta = siguiente(hasta,cad);

		if ( es_inicio_cadena(desde,cad) ){
			//el nuevo inicio de la cadena es el elemento siguente a hasta
			cad->inicio = siguiente_a_hasta;
		}
		else{
			//el siguiente elemento al que s accede desde el nodo anterior a desde es el siguiente elemento de hasta
			anterior_a_desde->siguiente = siguiente_a_hasta;
		}

		if ( es_final_cadena(hasta,cad) ){
			//el nuevo final de la cadena es el elemento anterior a desde
			cad->final = anterior_a_desde;
		}
		else{
			//el anterior elemento al que s accede desde el nodo siguiente a hasta es el anterior elemento de desde
			siguiente_a_hasta->anterior = anterior_a_desde;
		}
		inicio_cadena(eliminar)->anterior = NULL;
		final_cadena(eliminar)->siguiente = NULL;
		eliminar = liberar_cadena(eliminar);
		return cad;
	}
}


/*
  Se quita el elemento al que se accede desde `loc' y se libera la memoria
  asignada al mismo y al nodo apuntado por el localizador.
  Devuelve `cad'.
  El valor de `loc' queda indeterminado.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
cadena_t remover_de_cadena(localizador_t &loc, cadena_t cad){
	//assert( localizador_en_cadena(loc, cad) );
	localizador_t anterior_loc = anterior(loc,cad);
	localizador_t siguiente_loc = siguiente(loc,cad);
	if ( es_inicio_cadena(loc,cad) ){
		//entonces el nuevo inicio de la cadena es...
		cad->inicio = siguiente_loc;
	}
	//restructuro la cadena
	else{
		anterior_loc->siguiente = siguiente_loc;
	}

	if ( es_final_cadena(loc,cad) ){
		//entonces el nuevo final de la cadena es...
		cad->final = anterior_loc;
	}
	///restructuro la cadena
	else{
		siguiente_loc->anterior = anterior_loc;
	}

	//elimino info
	liberar_info(loc->dato);

	delete loc;
	loc = NULL;
	return cad;
}


/*
  Libera la memoria asignada a `cad' y la de todos sus elementos.
  Devuelve `cad'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
cadena_t liberar_cadena(cadena_t cad){
	nodo * a_borrar;
	while( cad->inicio != NULL ){
		a_borrar = cad->inicio;
		cad->inicio = cad->inicio->siguiente;
		liberar_info(a_borrar->dato);
		delete a_borrar;
	}
	delete cad;
	return cad;
}

/*
  Devuelve `true' si y sólo si `loc' es un localizador_t válido.
  En cadenas enlazadas un localizador_t no es válido si es `NULL'.
  El tiempo de ejecución es O(1).
*/
bool es_localizador(localizador_t loc){
	return loc != NULL;
}

/*
  Devuelve `true' si y sólo si `cad' es vacía (no tiene elementos).
  El tiempo de ejecución es O(1).
 */
bool es_vacia_cadena(cadena_t cad){
	return (cad->inicio == NULL) && (cad->final == NULL);
}

/*
  Devuelve `true' si y sólo si con `loc' se accede al último elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
  El tiempo de ejecución es O(1).
 */
bool es_final_cadena(localizador_t loc, cadena_t cad){
	return es_localizador(loc) && (loc == final_cadena(cad));
}

/*
  Devuelve `true' si y sólo si con `loc' se accede al primer elemento de `cad'.
  Si es_vacia_cadena (cad) devuelve `false'.
  El tiempo de ejecución es O(1).
 */
bool es_inicio_cadena(localizador_t loc, cadena_t cad){
	return es_localizador(loc) && (loc == inicio_cadena(cad));
}


/*
  Devuelve `true' si y sólo si con `loc' se accede a un elemento de `cad',
  (o sea, si apunta a un nodo de `cad').
  Si es_vacia_cadena (cad) devuelve `false'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
bool localizador_en_cadena(localizador_t loc, cadena_t cad){
	localizador_t cursor = inicio_cadena(cad);
	while (es_localizador(cursor) && (cursor != loc)){
		cursor = siguiente(cursor, cad);
	}
	return es_localizador(cursor);
}

/*
  Devuelve `true' si y sólo si `loc1' es igual o precede a `loc2` en `cad`.
  Si es_vacia_cadena (cad) devuelve `false'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
bool precede_en_cadena(localizador_t loc1, localizador_t loc2, cadena_t cad){
	if ( !es_vacia_cadena(cad) && localizador_en_cadena(loc1,cad) && localizador_en_cadena(loc2,cad) ){
		//como loc1 esta pasado por copia lo voy a utilizar para recorrer la cadena
		//si loc1 es valido y loc1 es distinto de loc2 entonces sigo avanzando en la cadena
		//avanzo hasta que loc1 sea nulo o loc1 sea igual a loc2
		while ( es_localizador(loc1) && (loc1 != loc2) ){
			loc1 = siguiente(loc1, cad);
		}
		return ( loc1 == loc2 );
	}
	else{
		return false;
	}
}

/*
  Devuelve el localizador_t con el que se accede al inicio de `cad`.
  Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
  El tiempo de ejecución es O(1).
 */
localizador_t inicio_cadena(cadena_t cad){
	if( es_vacia_cadena(cad) ){
		return NULL;
	} else{
		return cad->inicio;
	}
}

/*
  Devuelve el localizador_t con el que se accede al final de `cad'.
  Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
  El tiempo de ejecución es O(1).
 */
localizador_t final_cadena(cadena_t cad){
	if( es_vacia_cadena(cad) ){
		return NULL;
	}
	else{
		return cad->final;
	}
}


/*
  Devuelve el localizador_t con el que se accede al k-esimo elemento de `cad'.
  Si `k' es 0 o mayor a la cantidad de elementos de `cad' devuelve un localizdor
  no válido.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
localizador_t kesimo(nat k, cadena_t cad){
	if ( (k == 0) || es_vacia_cadena(cad) ){
		return NULL;
	}
	else{
		//localizador nesimo.. identifica el nesimo elemento de la cadena
		localizador_t localizador_nesima = inicio_cadena(cad);
		//posicion_nesima... contabiliza cuantos elementos de la cadena llevo recoridos
		int posicion_nesima = 1;
		//reccoro la cadena buscando el kesimo elemento
		while( (posicion_nesima < int(k) ) && (es_localizador(siguiente(localizador_nesima,cad))) ){
			localizador_nesima = siguiente(localizador_nesima,cad);
			posicion_nesima += 1;			
		}
		//si posicion_nesima es igual a k entonces significa que localizador_nesima apunta al kesimo elemento de la cadena
		if ( posicion_nesima == int(k) ){
			return localizador_nesima;
		}
		else{
			return NULL;
		}
	}
}

/*
  Devuelve el localizador_t con el que se accede al elemento de `cad'
  inmediatamente siguiente a `loc'.
  Si es_final_cadena(loc, cad) devuelve un localizador_t no válido.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
localizador_t siguiente(localizador_t loc, cadena_t cad){
	//assert( localizador_en_cadena(loc, cad) );
	if ( es_final_cadena(loc,cad) ){
		return NULL;
	}
	else{
		return loc->siguiente;
	}
}

/*
  Devuelve el localizador_t con el que se accede al elemento de `cad'
  inmediatamente anterior a `loc'.
  Si es_inicio_cadena(loc, cad) devuelve un localizador_t no válido.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
localizador_t anterior(localizador_t loc, cadena_t cad){
	//assert( localizador_en_cadena(loc, cad) );
	if ( es_inicio_cadena(loc,cad) ){
		return NULL;
	}
	else{
		return loc->anterior;
	}
}


/*
  Devuelve el localizador_t con el que se accede al elemento cuyo dato numérico
  es el menor en el segmento que va desde `loc' hasta el final_cadena(cad).
  Si hay más de un elemento cuyo valor es el menor el resultado accede al que
  precede a los otros.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
localizador_t menor_en_cadena(localizador_t loc, cadena_t cad){
	//assert( localizador_en_cadena(loc, cad) );
	localizador_t res = loc;
	while (es_localizador(siguiente(loc, cad))){
		loc = siguiente(loc, cad);
		if (numero_info(info_cadena(loc, cad)) < numero_info(info_cadena(res, cad))){
			res = loc;
		}
	}
	return res;
}

/*
  Devuelve el primer localizador_t con el que se accede a un elemento cuyo dato
  numérico es igual a `clave', buscando desde `loc' (inclusive) hacia el final
  de `cad'.
  Si no se encuentra o `cad' es vacía devuelve un localizador_t no válido.
  Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
localizador_t siguiente_clave(int clave, localizador_t loc, cadena_t cad){
	//assert( es_vacia_cadena(cad) || localizador_en_cadena(loc, cad) );
	localizador_t res = loc;
	if(es_vacia_cadena(cad)){
		res = NULL;
	}
	else{
		while ( es_localizador(res) && (numero_info(info_cadena(res, cad)) != clave) ){
			res = siguiente(res, cad);
		}
	}
	return res;
}

/*
  Devuelve el elemento de `cad' al que se accede con `loc'.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
info_t info_cadena(localizador_t loc, cadena_t cad){
	//assert( localizador_en_cadena(loc, cad) );
	return loc->dato;
}

/*
  Sustituye con `i' el elemento de `cad' al que se accede con `loc'.
  Devuelve `cad'.
  No destruye el elemento al que antes se accedía con `loc'.
  Precondición: localizador_en_cadena(loc, cad).
  El tiempo de ejecución es O(1).
 */
cadena_t cambiar_en_cadena(info_t i, localizador_t loc, cadena_t cad){
	//assert( localizador_en_cadena(loc, cad) );
	loc->dato = i;
	return cad;
}


/*
  Intercambia los elementos a los que se accede con `loc1' y `loc2'.
  `loc1' y `loc2' mantienen su relación de precedencia.
  Devuelve `cad'.
  Precondición:
    localizador_en_cadena (loc1, cad)
  y localizador_en_cadena (loc2, cad).
  El tiempo de ejecución es O(1).
 */
cadena_t intercambiar(localizador_t loc1, localizador_t loc2, cadena_t cad){
	//creo una variable que apunte a la info de loc1
	info_t info_de_loc1 = info_cadena(loc1,cad);
	//cambio datos
	loc1->dato = info_cadena(loc2, cad);
	loc2->dato = info_de_loc1;
	return cad;
}

/*
  Imprime los elementos de `cad` de la siguiente forma:
  (dn de pos1,fr de pos1)(dn de pos2,fr de pos2) ...
  donce `dn` es el dato numérico y `fr` es la frase.
  Antes de terminar, se debe imprimir un fin de linea.
  Si es_vacia_cadena(cad) sólo se imprime el fin de línea.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
void imprimir_cadena(cadena_t cad){
	if (!es_vacia_cadena(cad)){
		//crearemos un localizador para recorer la cadena
		localizador_t localizador = inicio_cadena(cad);
		//mientras el localizador apunte a un elemento ed la cadena, imprimimos su info
		while( es_localizador(localizador) ){
			printf("(%d,%s)",numero_info( info_cadena(localizador,cad) ), frase_info( info_cadena(localizador,cad) ) );
			localizador = siguiente(localizador, cad);
		}
	}
	printf("\n");
}


