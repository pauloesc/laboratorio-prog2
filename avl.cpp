/* 4730508 */

#include "../include/info.h"
#include "../include/cola_avls.h"
#include "../include/avl.h"

#include <assert.h>
#include <stdio.h>  // printf
#include <stdlib.h> // NULL

#include <limits.h> // INT_MAX

// Representación de `avl_t'.
// Se debe definir en avl.cpp.
struct rep_avl{
	info_t dato;
	nat altura;
	nat cantidad;
	avl_t izq, der;
};
// Declaración del tipo `avl_t'.
typedef rep_avl *avl_t;

/*
  Devuelve un avl_t vacío (sin elementos).
  El tiempo de ejecución es O(1).
 */
avl_t crear_avl(){
	return NULL;
}

/*
  Devuelve `true' si y sólo si `avl' es vacío (no tiene elementos).
  El tiempo de ejecución es O(1).
 */
bool es_vacio_avl(avl_t avl){
	return avl == NULL;
}

/*
  Inserta `i' en `avl' respetando la propiedad de orden definida.
  Precondición: es_vacio_avl(buscar_en_avl(numero_info(i), avl).
  Precondición: numero_info(i) != INT_MAX.
  El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos
  de `avl'.
*/

static void rotarDer( avl_t &avl ){

	//creo punteros auxiliares para no perder direcciones de memoria.
	avl_t aux_nodo = avl;
	avl = avl->izq;
	aux_nodo->izq=avl->der;
	avl->der = aux_nodo;

	//recalculo de altura y cantidad de nodos del hijo izq
	aux_nodo = NULL;
	aux_nodo = avl->der;

	//altura
	if ( altura_de_avl( izq_avl(aux_nodo) ) > altura_de_avl( der_avl(aux_nodo) ) ){
		aux_nodo->altura = altura_de_avl( izq_avl(aux_nodo) ) + 1;
	}
	else{
		aux_nodo->altura = altura_de_avl( der_avl(aux_nodo) ) + 1;
	}

	//cantidad nodos
	aux_nodo->cantidad = cantidad_en_avl( izq_avl(aux_nodo) ) + cantidad_en_avl( der_avl(aux_nodo) ) + 1;


	//recalculo de altura y cantidad de nodos de avl
	//altura
	if ( altura_de_avl( izq_avl(avl) ) > altura_de_avl( der_avl(avl) ) ){
		avl->altura = altura_de_avl( izq_avl(avl) ) + 1;
	}
	else{
		avl->altura = altura_de_avl( der_avl(avl) ) + 1;
	}
	//cantidad nodos
	avl->cantidad = cantidad_en_avl( izq_avl(avl) ) + cantidad_en_avl( der_avl(avl) ) + 1;

	
}


static void rotarIzq( avl_t &avl ){

	//creo punteros auxiliares para no perder direcciones de memoria.
	avl_t aux_nodo = avl;
	avl = avl->der;
	aux_nodo->der=avl->izq;
	avl->izq = aux_nodo;

	//recalculo de altura y cantidad de nodos del hijo izq
	aux_nodo = NULL;
	aux_nodo = avl->izq;

	//altura
	if ( altura_de_avl( izq_avl(aux_nodo) ) > altura_de_avl( der_avl(aux_nodo) ) ){
		aux_nodo->altura = altura_de_avl( izq_avl(aux_nodo) ) + 1;
	}
	else{
		aux_nodo->altura = altura_de_avl( der_avl(aux_nodo) ) + 1;
	}
	//cantidad nodos
	aux_nodo->cantidad = cantidad_en_avl( izq_avl(aux_nodo) ) + cantidad_en_avl( der_avl(aux_nodo) ) + 1;


	//recalculo de altura y cantidad de nodos de avl
	//altura
	if ( altura_de_avl( izq_avl(avl) ) > altura_de_avl( der_avl(avl) ) ){
		avl->altura = altura_de_avl( izq_avl(avl) ) + 1;
	}
	else{
		avl->altura = altura_de_avl( der_avl(avl) ) + 1;
	}
	//cantidad nodos
	avl->cantidad = cantidad_en_avl( izq_avl(avl) ) + cantidad_en_avl( der_avl(avl) ) + 1;

	
}

void insertar_en_avl(info_t i, avl_t &avl){

	//si avl es null
	if ( avl == NULL ){
		avl_t nuevo = new(rep_avl);
		nuevo->dato = i;
		nuevo->altura = 1;
		nuevo->cantidad = 1;
		nuevo->der = NULL;
		nuevo->izq = NULL;
		avl = nuevo;
	}

	//si lo que quiero insertar es menor a donde estoy parado
	else if ( numero_info(i) < numero_info(avl->dato) ){

		insertar_en_avl(i, avl->izq);

		//recalculamos altura 
		if( altura_de_avl(avl->izq) > altura_de_avl(avl->der) ){
			avl->altura = altura_de_avl(avl->izq) + 1;
		}
		else{
			avl->altura = altura_de_avl(avl->der) + 1;
		}

		//recalculamos cantidad
		avl->cantidad = cantidad_en_avl(izq_avl(avl)) + cantidad_en_avl(der_avl(avl)) + 1;

		//averificamos si es necesario aplicar rotaciones
		int fb = altura_de_avl(avl->izq) - altura_de_avl(avl->der);
		if(fb>1){

			if( altura_de_avl(avl->izq->der) > altura_de_avl(avl->izq->izq) ){
				rotarIzq(avl->izq);
				rotarDer(avl);
			}else{
				rotarDer(avl);
			}

		}

	}

	else{

		//si lo que quiero insertar es mayor a donde estoy parado
		insertar_en_avl(i, avl->der);

		//recalculamos altura
		if( altura_de_avl(avl->izq) > altura_de_avl(avl->der) ){
			avl->altura = altura_de_avl(avl->izq) + 1;
		}
		else{
			avl->altura = altura_de_avl(avl->der) + 1;
		}

		//recalculamos cantidad de nodos
		avl->cantidad = cantidad_en_avl(izq_avl(avl)) + cantidad_en_avl(der_avl(avl)) + 1;

		//verificamos si es necesario aplicar rotaciones
		int fb = altura_de_avl(avl->der) - altura_de_avl(avl->izq);
		if(fb>1){

			if(  altura_de_avl(avl->der->izq) > altura_de_avl(avl->der->der) ){
				rotarDer(avl->der);
				rotarIzq(avl);
			}
			else{
				rotarIzq(avl);
			}
		}
	}
}
/*
  Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato
  numérico es `clave'.
  Si ningún nodo cumple esa condición devuelve el árbol vacío.
  El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos de
  `avl'.
 */
avl_t buscar_en_avl(int clave, avl_t avl){

	if( !es_vacio_avl(avl) ){

		int comparacion = numero_info(avl->dato)-clave;

		//si estoy parado sobre el nodo que busco
		if( comparacion == 0 ){
			return avl;
		}
		//si el nodo que busco esta a la izq
		else if( comparacion > 0 ){
			return buscar_en_avl(clave, avl->izq);
		}
		//si el nodo que busco esta a la der
		else{
			return buscar_en_avl(clave, avl->der);
		}

	}
	else{
		return NULL;
	}
}

/*
  Devuelve el elemento asociado a la raíz de `avl'.
  Precondición: ! es_vacio_avl(b).
  El tiempo de ejecución es O(1).
 */
info_t raiz_avl(avl_t avl){
	assert( !es_vacio_avl(avl) );
	return avl->dato;
}

/*
  Devuelve el subárbol izquierdo de `avl'.
  Precondición: ! es_vacio_avl(avl).
  El tiempo de ejecución es O(1).
 */
avl_t izq_avl(avl_t avl){
  assert( !es_vacio_avl(avl) );
	return avl->izq;
}

/*
  Devuelve el subárbol derecho de `avl'.
  Precondición: ! es_vacio_avl(avl).
  El tiempo de ejecución es O(1).
 */
avl_t der_avl(avl_t avl){
  assert( !es_vacio_binario(avl) );
	return avl->der;
}

/*
  Devuelve la cantidad de elementos en `avl'.
  El tiempo de ejecución es O(1).
 */
nat cantidad_en_avl(avl_t avl){
	if (avl != NULL){
		return avl->cantidad;
	}
	else{
		return 0;
	}
}

/*
  Devuelve la altura de  `avl'.
  La altura de un árbol vacío es 0.
  El tiempo de ejecución es O(1).
 */
nat altura_de_avl(avl_t avl){
	if (avl != NULL){
		return avl->altura;
	}
	else{
		return 0;
	}
}



/*
  Devuelve un arreglo con los elementos de `avl'.
  Los elementos en el arreglo deben estar en orden creciente según los datos
  numericos.
 */

void aux_en_orden_avl(avl_t arbol, info_t *arreglo, int &poss){
	if( !es_vacio_avl(arbol) ){
		aux_en_orden_avl( arbol->izq, arreglo, poss );
		//cargo en el arreglo el dato correspondiente
		arreglo[poss] = raiz_avl(arbol);
		//sumo uno a la posicion donde se cargara un nuevo elemento
		poss = poss + 1;
		aux_en_orden_avl( arbol->der, arreglo, poss );
	}
}
info_t *en_orden_avl(avl_t avl){
	info_t *res = NULL;
	if (es_vacio_avl(avl)){
		res = NULL;
	}else{
		//creo el arreglo que contendra a los elementos de avl
		res = new info_t[ cantidad_en_avl(avl) ];
		//posicion es una variable que se utiliza en la funcion auxiliar para saber en q poscion del argelo se inserta un nuevo elemento
		int posicion = 0;
		aux_en_orden_avl(avl,res,posicion);
	}
	return res;
}



/*
  Devuelve un avl_t con los `n' elementos que están en el rango [0 .. n - 1]
  del arreglo `infos'.
  Los elementos están ordenados de manera creciente estricto (creciente y sin
  repetidos) según los datos numércos.
  En cada nodo del árbol resultado la cantidad de elementos de su subárbol
  izquierdo es igual a, o 1 menos que, la cantidad de elementos de su subárbol
  derecho.
  El tiempo de ejecución es O(n).
 */

static avl_t a2avl_rec( info_t *infos, int inf, int sup ){

	if ( inf > sup ){
		return NULL;
	}

	//si solo hay un elemento que cargar
	else if(inf == sup) {
		avl_t nuevo = new(rep_avl);
		//cargo el dato correspondiente
		nuevo->dato = infos[inf-1];
		nuevo->altura = 1;
		nuevo->cantidad = 1;
		nuevo->izq = NULL;
		nuevo->der = NULL;
		return nuevo;
	}

	//si hay mas de un elemento en el rango
	else{
		int medio = ((inf + sup)/2);
		avl_t nuevo = new(rep_avl);
		//cargo el elemento correspondiente
		nuevo->dato = infos[medio-1];

		//asignamos a cada sub arbol del nodo un sub arbol generado por la funcion
		nuevo->izq = a2avl_rec(infos, inf, medio-1);
		nuevo->der = a2avl_rec(infos, medio+1, sup);

		//recalculamos la cantidad de nodos del nodo
		nuevo->cantidad = cantidad_en_avl( nuevo->izq ) + cantidad_en_avl( nuevo->der ) + 1; 

		//recalculamos altura del nodo
		if ( cantidad_en_avl(nuevo->izq) >= cantidad_en_avl(nuevo->der) ){
			nuevo->altura = cantidad_en_avl(nuevo->izq) + 1;
		}
		else{
			nuevo->altura = cantidad_en_avl(nuevo->der) + 1;
		}
		return nuevo;
	}


}

avl_t arreglo_a_avl(info_t *infos, nat n){
	return a2avl_rec( infos, 1, int(n));
}

/*
  Devuelve un avl_t de altura `h' con `n' nodos, siendo `n' la mínima cantidad
  de elementos que puede tener un avl de altura `h'.
  Los datos numéricos de los elementos van desde 1 hasta `n'.
  El dato de texto de cada elemento es la cadena vacia.
  En ningún nodo puede ocurrir que el subárbol derecho tenga más nodos que el
  subárbol izquierdo.
  El tiempo de ejecución es O(n).
  Ver ejemplos en la letra y en el caso 408.
 */


static avl_t auxiliar_avl_min(nat altura, int &valor_num_nodo){
	
	if( altura == 0){
		return NULL;
	}
	else if( altura == 1 ){
		avl_t nuevo_nodo = new(rep_avl);
		char *frase = NULL;
		nuevo_nodo->dato = crear_info(valor_num_nodo, frase);
		nuevo_nodo->cantidad = 1;
		nuevo_nodo->altura = 1;
		nuevo_nodo->der = NULL;
		nuevo_nodo->izq = NULL;
		valor_num_nodo = valor_num_nodo + 1;
		return nuevo_nodo;
	}

	else{
		//creo el nuevo nodo
		avl_t nuevo_nodo = new(rep_avl);

		//a el nodo le cargo el subarbol izq
		nuevo_nodo->izq = auxiliar_avl_min(altura-1,valor_num_nodo);

		//a el nodo le cargo la info corespondiente
		char *frase = NULL;
		nuevo_nodo->dato = crear_info(valor_num_nodo, frase);
		//modifico la variable para el proximo nodo
		valor_num_nodo = valor_num_nodo + 1;

		//a el nodo le cargo el subarbol der
		nuevo_nodo->der = auxiliar_avl_min(altura-2,valor_num_nodo);

		//le cargo la cantidad de nodos al nodo
		nuevo_nodo->cantidad = cantidad_en_avl(nuevo_nodo->izq) + cantidad_en_avl(nuevo_nodo->der) + 1;
		
		if ( ( altura_de_avl(nuevo_nodo->izq) - altura_de_avl(nuevo_nodo->der)) >=0 ){
			nuevo_nodo->altura = altura_de_avl(nuevo_nodo->izq);
		}
		else{
			nuevo_nodo->altura = altura_de_avl(nuevo_nodo->der);
		}

		return nuevo_nodo;
	}
}

avl_t avl_min(nat h){
	//numero nodo contiene el valor que se le cargara al nodo corespondiente
	int numero_nodo = 1;
	avl_t arbol = auxiliar_avl_min(h, numero_nodo);
	return arbol;
}

/*
  Imprime los datos numéricos de los nodos de cada nivel de `avl'.
  Imprime una línea por nivel, primero el más profundo y al final el que
  corresponde a la raíz.
  Cada nivel se imprime de izquierda a derecha. Se debe dejar un espacio en
  blanco después de cada número.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de nodos de `avl'.
  Ver ejemplos en la letra y en el caso 404.
 */

void imprimir_cola_avl( cola_avls_t &c, avl_t &ultimo_del_nivel ){

	if( !es_vacia_cola_avls(c) ){

		//esta cola guarda los elementos pendientes de impresion
		cola_avls_t pendientes_de_impresion = crear_cola_avls();

		//esta variable se va a utilizar en el while, indica si ya se trabajo sobre el ultimo elemento del nivel
		bool ultimo_elemento_del_nivel = false;

		//esta variable ultimo apunta a el ultimo elemento encolado en la cola pendiente de impresion
		avl_t ultimo = NULL;

		while( !es_vacia_cola_avls(c) && ( !ultimo_elemento_del_nivel ) ){

			ultimo_elemento_del_nivel = ( ultimo_del_nivel == frente(c) );

			//si el subarbol izq del frente de c es distinto de null
			if( frente(c)->izq != NULL ){
				encolar(frente(c)->izq, c);
				ultimo = frente(c)->izq;
			}

			//si el subarbol der del frente de c es distinto de null
			if( frente(c)->der != NULL ){
				encolar(frente(c)->der,c);
				ultimo = frente(c)->der;
			}

			//encolo en pendientes de impresion el frente de c
			encolar( frente(c), pendientes_de_impresion );

			desencolar(c);

		}

		imprimir_cola_avl(c, ultimo);

		//mientras la cola de impresiones pendientes dea distinta de vacia... imprimo los elementos y los desencolo
		while( !es_vacia_cola_avls(pendientes_de_impresion) ){

			int num = int( numero_info( raiz_avl( frente(pendientes_de_impresion) ) ) );
			printf("%d ", num );
			desencolar( pendientes_de_impresion );

		}
		printf("\n");

		liberar_cola_avls( pendientes_de_impresion );
		pendientes_de_impresion = NULL;
	}

}



void imprimir_avl(avl_t avl){

	if( !es_vacio_avl(avl) ){
		cola_avls_t aux_cola = crear_cola_avls();
		encolar(avl, aux_cola);
		//este puntero se va a utilizar para saber cual elemento en la cola es el ultimo de un determinado nivel
		avl_t ultimo_del_nivel = avl;
		imprimir_cola_avl(aux_cola, ultimo_del_nivel);
		liberar_cola_avls(aux_cola);
		aux_cola = NULL;
	}
}

/*
  Libera la memoria asignada a `avl' y la de sus elementos.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de
  `avl'.
 */
void liberar_avl(avl_t &avl){

	if( avl != NULL ){
		liberar_avl( avl->izq);
		liberar_avl( avl->der);
		liberar_info(avl->dato);
		delete(avl);
		avl = NULL;
	}
}