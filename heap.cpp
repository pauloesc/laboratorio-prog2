/* 4730508 */

/*
  Módulo de definición de `heap_t`.

  Se definen los heap en los que los elementos son de tipo `info_t'.

  La cantidad máxima de elementos que puede contener el heap es `tamanio',
  que es un parámetro pasado al crearlo.

  El valor de los elementos que determina el orden es su dato numerico,
  que debe pertenecer al rango 0 .. `max_valor',
  siendo `max_valor' un parámetro de `crear_heap'.

  No puede haber dos elementos con el mismo dato numerico.

  Laboratorio de Programación 2.
  InCo-FIng-UDELAR
 */


#include "../include/info.h"
#include "../include/cadena.h"

#include <assert.h>
#include <stdlib.h> // NULL


// Representación de `heap_t'
// Se debe definir en heap.cpp
struct rep_heap{
  info_t *datos;
  //maxima cantidad elementos
  int tamanio;
  //cantidad de elementos cargados 
  int cargados;
  //maximo valor numerico que puede tomar un elemento
  int max_valor;
  //array de enteros para localizar elementos
  int *loc;
};
// Declaración del tipo `heap_t_t'
typedef rep_heap *heap_t;

/*
  Devuelve un `heap' que puede contener hasta `tamanio' elementos.
  Los datos numéricos pueden estar en el rango 0 .. max_valor.
 */
heap_t crear_heap(nat tamanio, nat max_valor){
  heap_t nuevoHeap = new(rep_heap);
  nuevoHeap->datos = new info_t[int(tamanio)+1];
  nuevoHeap->tamanio = int(tamanio);
  nuevoHeap->cargados = 0;
  nuevoHeap->max_valor = max_valor;
  nuevoHeap->loc = new int[max_valor+1];

  //inicializo los valores de loc en 0 ya que ningun elemento puede estar en la posicion 0.
  int i = 0;
  for(i=0; i < int(max_valor)+1; i++ ){
  	nuevoHeap->loc[i] = 0;
  }

  i = 0;
  //inicializo
  for(i=0; i < int(tamanio)+1; i++ ){
  	nuevoHeap->datos[i] = NULL;
  }

  return nuevoHeap;
}


void filtrado_ase_insertar(heap_t h, int indice){
  
  //indice indica sobre que elemento estamos parados

  //si el indice es uno significaria que ya no podria subir mas el elemento.
  if( indice > 1){

    int padre = indice/2;
    int hijo = indice;

    info_t padre_t = h->datos[padre];
    info_t hijo_t = h->datos[hijo];

    //si el padre es mayor al hijo
    if( numero_info( padre_t ) > numero_info( hijo_t ) ){
      
      //intercambio los elementos de lugar entre ellos mismos
      h->datos[padre] = hijo_t;
      h->datos[hijo] = padre_t;

      //hago un cambio en loc
      h->loc[numero_info( hijo_t )] = padre;
      h->loc[numero_info( padre_t )] = hijo;

      filtrado_ase_insertar( h, padre);
    }
  }
}

/*
  Si inserta el elemento `i'.
  Precondición: !es_lleno_heap(h) y !hay_valor(v,h) y v <= max_valor(h),
  donde v = numero_info(i).
  El tiempo de ejecución es O(log tamanio).
 */
void insertar_en_heap(info_t i, heap_t &h){

  //donde se inserta el nuevo elemento
  int donde_insertar = h->cargados + 1;

  //la cantidad de elementos en el heap aumenta 1
  h->cargados++;
  //inserto el elemento
  h->datos[ donde_insertar ] = i;

  //hago el localizador del elemento a insertar
  int loc_indice = numero_info(i);
  h->loc[loc_indice] = donde_insertar;

  //restructuro el heap
  filtrado_ase_insertar(h, donde_insertar);
}

/*
  Reduce a v/2 el valor del elemento cuyo valor actual es `v'.
  Precondición: hay_valor(v, h) y !hay_valor(v/2, h).
  No debe quedar memoria inaccesible.
  El tiempo de ejecución es O(log tamanio).
 */
#include <string.h> // strlen, strcpy, strcmp, strcat

void reducir(nat v, heap_t &h){

  //encuentro el indice donde esta el elemento
	int indice = h->loc[int(v)];

	//v es un valor que va a dejar de existir por lo que en loc, en la posicion v cargo 0... ya que nada puede estar en 0
	h->loc[int(v)] = 0;

	info_t info_a_modificar = h->datos[indice];
	
	//creo una copia de la frase
	char *frasecopia = new char[strlen( frase_info(info_a_modificar) ) + 1];
  strcpy(frasecopia, frase_info(info_a_modificar)  );
  //creo nueva info
	info_t nuevaInfo =  crear_info( int(v)/2, frasecopia );

	//libero info vieja
	liberar_info(info_a_modificar);

	//asigno nueva info
	h->datos[ indice ] = nuevaInfo;

	//hago localizador del elemento v/2
  h->loc[ int(v)/2 ] = indice;

	filtrado_ase_insertar(h, indice);
}

void filtrado_descendente( heap_t h, int indice, int ultimo_elemento_indice){

  //dato numerico de la informacion a reacomodar
  //dato numerico del ultimo elemento del heap... este elemento es al q hay q reacomodar
	int numero_de_info_reacomodar = numero_info(h->datos[ultimo_elemento_indice]);

  	int hijoIzq = 2*indice;
  	int hijoDer = 2*indice + 1;

  //compruebo que esten los valores dentro del rango para no generar errores
  if( (hijoIzq <= ultimo_elemento_indice) && ( hijoDer <= ultimo_elemento_indice ) ){

    //puedo meter el nodo a reacomodar en el indice donde estoy
    // esta condicion tiene un >= porque, el = contempla el caso en que el elemento hijo es el elemento que quiero reacomodar
    if ( (numero_info(h->datos[hijoIzq]) >= numero_de_info_reacomodar) && (numero_info(h->datos[hijoDer]) >= numero_de_info_reacomodar)){
      
      //pongo el elemento en el indice donde estoy
      h->datos[indice] = h->datos[ultimo_elemento_indice];
      //hago el localizador para ese elemento
      h->loc[numero_info(h->datos[ultimo_elemento_indice])] =  indice;

      h->datos[ultimo_elemento_indice] = NULL;

    }
    else{

      //si es el hijo izq es menor al hijo derecho
      if( numero_info( h->datos[hijoIzq] ) < numero_info( h->datos[hijoDer] ) ){
        
        //subo el hijo izq
        h->datos[indice] = h->datos[hijoIzq];

        //la posicion donde estaba el hijo izq queda en nul
        h->datos[hijoIzq] = NULL;

        //hago el localizador del elemento que subio
        h->loc[numero_info( h->datos[indice] )] = indice;

        filtrado_descendente( h, hijoIzq, ultimo_elemento_indice );

      }
      //si es el hijo der es menor al hijo izq
      else{
        //subo el hijo der
        h->datos[indice] = h->datos[hijoDer];

        //la posicion donde estaba el hijo der queda en nul
        h->datos[hijoDer] = NULL;

        //hago el localizador del elemento que subio
        h->loc[numero_info( h->datos[indice] )] = indice;

        filtrado_descendente( h, hijoDer, ultimo_elemento_indice );
      }

    }
 
  }
  //no hay mas niveles hacia abajo, entonces meto el nodo a reacomodar en este indice
  else{
    h->datos[indice] = h->datos[ultimo_elemento_indice];
    h->loc[numero_info( h->datos[indice] )] = indice;

    //cargo nul a la posicion donde estaba el elemento a cambiar de lugar
    h->datos[ultimo_elemento_indice] = NULL;
  }
}

/*
  Elimina de 'h' el elemento de menor valor y libera la memoria que tiene
  asignada.
  Precondición: ! es_vacia_heap(h).
  El tiempo de ejecución es O(log tamanio).
 */
void eliminar_menor(heap_t &h){

  int ultima_posicion_valida = h->cargados;

  //restructuro loc
  int valor_info = numero_info( h->datos[1] );
  h->loc[valor_info] = 0;

  //libero la info
  liberar_info( h->datos[1] );
  h->datos[1] = NULL;

  //la cantidad de elementos del heap disminuye en 1
  h->cargados--;

  //si hay elementos en el heap
  if (h->cargados > 0){
    filtrado_descendente(h ,1, ultima_posicion_valida);
  }

}

/*  Libera la menoria asignada a `h' y a sus elementos. */
void liberar_heap(heap_t &h){

  //libero todos los info_t
  int i = 0;
  for( i=0; i < h->tamanio + 1; i++ ){

    info_t informacion = h->datos[i];
    if( informacion != NULL ){
      liberar_info( informacion );
    }

  }
  //libero todos los array
  delete [] h->datos;
  delete [] h->loc;

  delete h;
  h = NULL;
}

/*
  Devuelve `true' si y sólo si en `h' no hay elementos.
  El tiempo de ejecución es O(1).
 */
bool es_vacio_heap(heap_t h){
  return ( h->cargados == 0 );
}

/*
  Devuelve `true' si y sólo si en `h' hay `tamanio' elementos.
  El tiempo de ejecución es O(1).
 */
bool es_lleno_heap(heap_t h){
  return ( h->cargados == h->tamanio );
}

/*
  Devuelve `true' si y sólo si en 'h' hay un elemento con valor `v'.
  El tiempo de ejecución es O(1).
 */
bool hay_valor(nat v, heap_t h){

  //si indice donde buscar es distinto de 0, entonces significa que existe el elemento que estoy buscando
  //pensar que ningun elemento puede estar en la posicion 0 porque esa posicion decidimos no usarla
  //en realizad todo elemento que no existe en heap y que por lo tanto no se puede localizar en loc tiene cargado un 0
	int indice_donde_buscar = h->loc[int(v)];

	if( indice_donde_buscar == 0 ){
		return false;
	}
	else{
		return true;
	}
}

/*
  Devuelve el elemento de 'h' con menor valor.
  Precondición: ! es_vacio_h(h).
  El tiempo de ejecución es O(1).
 */
info_t menor(heap_t h){
  return h->datos[1];
}

/*
  Devuelve el máximo valor del rango para los datos numericos, establecido en
  crear_h.
 */
nat max_valor(heap_t h){
  return h->max_valor;
}
