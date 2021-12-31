/* 4730508 */

#include "../include/info.h"
#include "../include/iterador.h"
#include "../include/avl.h"
#include "../include/conjunto.h"

#include <assert.h>
#include <stdlib.h> // NULL



// Declaración del tipo `conjunto_t'
// struct rep_conjunto es la representación de `conjunto_t',
// definida en conjunto.cpp

struct rep_conjunto {
    avl_t datos;
};

typedef struct rep_conjunto *conjunto_t;

/* Constructoras */

/*
  Devuelve un conjunto_t vacío (sin elementos).
  El tiempo de ejecución es O(1).
 */
conjunto_t crear_conjunto(){
	conjunto_t nuevo = new(rep_conjunto);
	nuevo->datos = NULL;
	return nuevo;
}

/*
  Devuelve un conjunto_t cuyo único elemento es `i'.
  El tiempo de ejecución es O(1).
 */
conjunto_t singleton(info_t i){
	conjunto_t nuevo = crear_conjunto();
	//inserto la info en el conjunto
	insertar_en_avl(i, nuevo->datos);
	return nuevo;
}

/*
  Devuelve un conjunto_t con los elementos que pertenecen a  `c1' o `c2'.
  Si en ambos conjuntos hay un elemento con el mismo dato  numérico en el
  conjunto_t devuelto se debe incluir el que pertenece a `c1'.
  El tiempo de ejecucion es O(n1 + n2 + n.log n), siendo `n1' y `n2' la
  cantidad de elementos de `c1' y `c2' respectivamente y `n' la del conjunto_t
  resultado.
  El conjunto_t devuelto no comparte memoria ni con `c1' no con `c2'.
 */
conjunto_t union_conjunto(conjunto_t c1, conjunto_t c2){
	
	//creo el conjunto a devolver
	conjunto_t conjunto_union = crear_conjunto();

	iterador_t iter_de_c1 = iterador_conjunto(c1);
	bloquear_iterador(iter_de_c1);
	reiniciar_iterador(iter_de_c1);

	iterador_t iter_de_c2 = iterador_conjunto(c2);
	bloquear_iterador(iter_de_c2);
	reiniciar_iterador(iter_de_c2);

	//mientras en amobos iteradores existan elementos por recorer
	while( esta_definida_actual(iter_de_c1) && esta_definida_actual(iter_de_c2) ){

		int comparacion = numero_info(actual_en_iterador(iter_de_c1)) - numero_info(actual_en_iterador(iter_de_c2) );

		if ( comparacion == 0 ){
			//inserto la info del iterador de c1
			insertar_en_avl(copia_info(actual_en_iterador(iter_de_c1)), conjunto_union->datos);
			//avanzo ambos iteradores
			avanzar_iterador(iter_de_c1);
			avanzar_iterador(iter_de_c2);
		}


		else if( comparacion > 0){
			//cargo la info del iterador de c2
			insertar_en_avl(copia_info(actual_en_iterador(iter_de_c2)), conjunto_union->datos);
			//avanzo el iterador de c2
			avanzar_iterador(iter_de_c2);

		}

		else{
			//cargo la info del iterador de c1
			insertar_en_avl(copia_info(actual_en_iterador(iter_de_c1)), conjunto_union->datos);
			//avanzo el iterador de c1
			avanzar_iterador(iter_de_c1);
		}

	}

	//si quedaron elementos por cargar del iterador de c1
	while( esta_definida_actual(iter_de_c1) ){
		insertar_en_avl(copia_info(actual_en_iterador(iter_de_c1)), conjunto_union->datos);
		avanzar_iterador(iter_de_c1);
	}

	//si quedaron elementos por cargar del iterador de c2
	while( esta_definida_actual(iter_de_c2) ){
		insertar_en_avl(copia_info(actual_en_iterador(iter_de_c2)), conjunto_union->datos);
		avanzar_iterador(iter_de_c2);
	}

	reiniciar_iterador(iter_de_c1);
	reiniciar_iterador(iter_de_c2);
	liberar_iterador(iter_de_c1);
	liberar_iterador(iter_de_c2);

	return conjunto_union;

}

/*
  Devuelve un conjunto_t con los elementos de `c1' cuyos datos numéricos no
  pertenecen a `c2'.
  El tiempo de ejecucion es O(n1 + n2 + n.log n), siendo `n1' y `n2' la
  cantidad de elementos de `c1' y `c2' respectivamente y `n' la del conjunto_t
  resultado.
  El conjunto_t devuelto no comparte memoria ni con `c1' no con `c2'.
 */
conjunto_t diferencia(conjunto_t c1, conjunto_t c2){
	
	//creo el conjunto a devolver
	conjunto_t conjunto_diferencia = crear_conjunto();

	iterador_t iter_de_c1 = iterador_conjunto(c1);
	bloquear_iterador(iter_de_c1);
	reiniciar_iterador(iter_de_c1);

	iterador_t iter_de_c2 = iterador_conjunto(c2);
	bloquear_iterador(iter_de_c2);
	reiniciar_iterador(iter_de_c2);


	//mientras en amobos iteradores existan elementos por recorer
	while( esta_definida_actual(iter_de_c1) && esta_definida_actual(iter_de_c2) ){

		int comparacion = numero_info(actual_en_iterador(iter_de_c1)) - numero_info(actual_en_iterador(iter_de_c2) );

		if ( comparacion < 0 ){
			insertar_en_avl(copia_info(actual_en_iterador(iter_de_c1)), conjunto_diferencia->datos);
			avanzar_iterador(iter_de_c1);
		}

		else if( comparacion > 0){
			avanzar_iterador(iter_de_c2);
		}

		else{
			avanzar_iterador(iter_de_c2);
			avanzar_iterador(iter_de_c1);
		}

	}

	//si iter de c1 no llego al final cargo el resto de los elementos
	while( esta_definida_actual(iter_de_c1) ){
		insertar_en_avl(copia_info(actual_en_iterador(iter_de_c1)), conjunto_diferencia->datos);
		avanzar_iterador(iter_de_c1);
	}

	reiniciar_iterador(iter_de_c1);
	reiniciar_iterador(iter_de_c2);
	liberar_iterador(iter_de_c1);
	liberar_iterador(iter_de_c2);

	return conjunto_diferencia;

}

/*
  Libera la memoria asignada a `c' y la de todos sus elementos.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos de `c'.
 */
void liberar_conjunto(conjunto_t &c){
	liberar_avl(c->datos);
	delete(c);
}

/*
  Devuelve `true' si y sólo si `info' es un elemento de `c'.
  El tiempo de ejecución es O(log n), siendo `n' es la cantidad de
  elementos de `c'.
 */
bool pertenece_conjunto(info_t info, conjunto_t s){
	avl_t elemento =  buscar_en_avl( numero_info(info) ,s->datos );
	return ( (!es_vacio_avl(elemento)) && son_iguales(raiz_avl(elemento),info) );
}

/*
  Devuelve `true' si y sólo `c' es vacío (no tiene elementos).
  El tiempo de ejecución es O(1).
 */
bool es_vacio_conjunto(conjunto_t c){
	return (c->datos == NULL );
}

/*
  Devuelve un conjunto_t con los `n' elementos que están en en el rango
  [0 .. n - 1] del arreglo `infos'.
  Los elementos están ordenados de manera creciente estricto (creciente y sin
  repetidos) según los datos numércos.
  El tiempo de ejecución es O(n).
 */
conjunto_t arreglo_a_conjunto(info_t *infos, nat n){
	conjunto_t nuevo = new(rep_conjunto);
	nuevo->datos = arreglo_a_avl(infos, n);
	return nuevo;
}

/*
  Devuelve un iterador_t de los elementos de `c'.
  En la recorrida del iterador devuelto los datos numéricos aparecerán en orden
  creciente.
  El tiempo de ejecución es O(n), siendo `n' es la cantidad de elementos de `c'.
  El iterador_t resultado NO comparte memoria con `c'.
 */
iterador_t iterador_conjunto(conjunto_t c){

	iterador_t iter = crear_iterador();

	if ( !es_vacio_conjunto(c) ){

		//transformo el conjunto en un arreglo
		info_t *datos = en_orden_avl(c->datos);

		int i = 0;
		//cantidad de elementos del conjunto
		int cantidad = cantidad_en_avl(c->datos);

		//agrego al iterador los datos del arreglo
		for( i=0; i < cantidad; i++ ){
			agregar_a_iterador( datos[i] ,iter);
		}
		bloquear_iterador(iter);
		delete[] datos;
	}
	return iter;
}
