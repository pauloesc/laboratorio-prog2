/* 4730508 */

/*
  Módulo de definición de `binario_t'.

  Se definen los árboles binarios de búsqueda de elementos de tipo `info_t'.
  La propiedad de orden de los árboles es definida por la frase de sus
  elementos.

  Laboratorio de Programación 2.
  InCo-FIng-UDELAR
 */

#include "../include/binario.h"
#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/uso_cadena.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rep_binario{
	info_t dato;
	rep_binario *izq;
	rep_binario * der;
};

typedef rep_binario *binario_t;

static int absoluto(int n){
	return (n >= 0) ? (n) : (-n);
}

static nat maximo(nat n1, nat n2){ 
	return (n1 >= n2) ? n1 :n2;
}

/*
  Devuelve un binario_t vacío (sin elementos).
  El tiempo de ejecución es O(1).
 	
 funcion dada	
 */
binario_t crear_binario(){
	return NULL;
}

/*
  Inserta `i' en `b' respetando la propiedad de orden definida.
  Devuelve `b'.
  Precondición: es_vacio_binario(buscar_subarbol(frase_info(i), b).
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */
binario_t insertar_en_binario(info_t i, binario_t b){

  //assert( es_vacio_binario(buscar_subarbol(frase_info(i), b)) );

  if( b == NULL ){
    b = new(rep_binario);
    b->dato = i;
    b->izq = NULL;
    b->der = NULL;
    return b;
  }

  else{
    //si esta a la izquierda el nodo que busco
    if ( 0 > strcmp( frase_info(i) , frase_info(b->dato) ) ){
      b->izq =  insertar_en_binario(i,b->izq);
      return b;
    }

    //si esta a la derecha el nodo que busco
    else if ( 0 < strcmp( frase_info(i) , frase_info(b->dato) ) ){
      b->der =  insertar_en_binario(i,b->der);
      return b;
    }

    //estoy parado sobre el 
    else{
      return b;
    }
  }
}

/*
  Devuelve el elemento mayor (según la propiedad de orden definida) de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */
info_t mayor(binario_t b){
	
	//assert( !es_vacio_binario(b) );

	if (b->der == NULL){
		return b->dato;
	}
	else{
		return mayor(b->der);
	}
}

/*
  Remueve el nodo en el que se localiza el elemento mayor de `b'
  (libera la memoria  asignada al nodo, pero no la del elemento).
  Devuelve `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.

  funcion dada
 */
binario_t remover_mayor(binario_t b){
	//assert( !es_vacio_binario(b) );
	if (b->der == NULL){
		binario_t izq = b->izq;
		delete(b);
		b = izq;
	}
  else {
		b->der = remover_mayor(b->der);
	}
	return b;
}

/*
  Remueve de `b' el nodo en el que el dato de texto de su elemento es `t'.
  Si los dos subárboles del nodo a remover son no vacíos, en sustitución del
  elemento removido debe quedar el que es el mayor (segun la propiedad de orden
  definida) en el subárbol izquierdo.
  Devuelve `b'.
  Precondición: !es_vacio_binario(buscar_subarbol(frase_info(i), b).
  Libera la memoria del nodo y del elemento.
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */
binario_t remover_de_binario(const char *t, binario_t b){

  //assert( !es_vacio_binario(buscar_subarbol(frase_info(i), b) ) );

	//compara el string de el nodo donde estoy parado y con el string de t
	int comp = strcmp(t,frase_info(raiz(b)));
	
	//si el nodo que busco esta a la izquierda
	if (comp < 0 ){
		b->izq = remover_de_binario(t, izquierdo(b)); 
		return b;

	}
	//si el nodo que busco esta ala derecha
	else if (comp > 0){
		b->der = remover_de_binario(t, derecho(b));
		return b;
	}

	//si estoy sobre el
	else {
		//si no tiene sub arboles
		if ((b->der == NULL) && (b->izq == NULL)){
			liberar_info(b->dato);
      delete(b);
			return NULL;
		}

		//si tine sub arbol solo a la izquierda
		else if ( (b->der == NULL) && (b->izq != NULL) ){
      binario_t retorno = b->izq;
      liberar_info(b->dato);
			delete(b);
			return retorno;
		}

		//si tiene sub arbol solo a la derecha
		else if ( (b->izq == NULL) && (b->der != NULL) ){
			binario_t retorno = b->der;
      liberar_info(b->dato);
			delete(b);
			return retorno;
		}

		//si tiene sub arboles a ambos lados
		else{
      //creo el nodo que remplaza al eliminado
			binario_t nodo_remplazante = new(rep_binario);

      //borro la info del nodo que quiero eliminar
			liberar_info(b->dato);
      
      //le carlo la informacion a el nodo remplazante
      nodo_remplazante->dato = mayor(b->izq);
      nodo_remplazante->der = b->der;

      if ( (b->der->izq == NULL) && (b->der->der == NULL) ){
        nodo_remplazante->izq = NULL;
      }
      else{
        nodo_remplazante->izq = b->izq;
      }

			remover_mayor(b->izq);
      delete(b);
			return nodo_remplazante;
		}
	}
}

/*
  Libera la memoria asociada a `b' y todos sus elementos.
  Devuelve `b'.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.

  funcion dada
 */
binario_t liberar_binario(binario_t b){
	if (b != NULL){
		b->izq = liberar_binario(b->izq);
		b->der = liberar_binario(b->der);
		liberar_info(b->dato);
		delete b;
		b = NULL;
	}
	return b;
}

/*
  Devuelve `true' si y sólo si `b' es vacío (no tiene elementos).
  El tiempo de ejecución es O(1).
 */
bool es_vacio_binario(binario_t b){
	return b == NULL;
}

/*
  Devuelve `true' si y sólo si cada nodo de `b' cumple la condición de balanceo
  AVL. El árbol vacío cumple la condición.
  Un nodo cumple la condición de balanceo AVL si el valor absoluto de la
  diferencia de las alturas de sus subárboles izquierdo y derecho en menor o
  igual a 1.
  Cada nodo se puede visitar una sola vez.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */

int auxiliar_avl( binario_t b, bool &cumple_avl ){

  if( b != NULL  ){
    //altura del subarbol izquierdo
    int altura_subarbol_izquierdo = auxiliar_avl(b->izq, cumple_avl);
    //altura del subarbol derecho
    int altura_subarbol_derecho = auxiliar_avl(b->der, cumple_avl);

    //si los subarboles previamente inspeccionados estan valanceados
    if(cumple_avl){

      //si el nodo deonde estoy parado cumple la condicion de valanceo
      if( absoluto(altura_subarbol_izquierdo - altura_subarbol_derecho) <= 1 ){

        //retorno la altura del subarbol mas uno por el nivel del nodo donde estoy parado
        return maximo(altura_subarbol_izquierdo, altura_subarbol_derecho) + 1;
      }    
      //no se cumple la condicion de valanceo
      else{
        cumple_avl = false;
        return 0;
      }
    }
    //subarbol esta desvalanceado
    else{
      return 0;
    }
  }
  //b es null
  else{
    return 0;
  }
}

bool es_AVL(binario_t b){
  //cumple_avl es true si solo si solo si se cumple la condicion de avl
  bool cumple_avl = true;
  auxiliar_avl(b, cumple_avl);
  return cumple_avl;
}


/*
  Devuelve el elemento asociado a la raíz de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(1).
 */
info_t raiz(binario_t b){
 //assert( !es_vacio_binario(b) );
	return b->dato;
}

/*
  Devuelve el subárbol izquierdo de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(1).
 */
binario_t izquierdo(binario_t b){
  //assert( !es_vacio_binario(b) );
	return b->izq;
}

/*
  Devuelve el subárbol derecho de `b'.
  Precondición: ! es_vacio_binario(b).
  El tiempo de ejecución es O(1).
 */
binario_t derecho(binario_t b){
  //assert( !es_vacio_binario(b) );
	return b->der;
}

/*
  Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato de
  texto es `t'.
  Si `t' no pertenece a `b', devuelve el árbol vacío.
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.

  funcion dada
 */
binario_t buscar_subarbol(const char *t, binario_t b){
	binario_t res;
	if (es_vacio_binario(b)){
		res = crear_binario();
	} else {
		int comp = strcmp(t,frase_info(raiz(b)));
		if (comp < 0 ){
			res = buscar_subarbol(t, izquierdo(b));
		}
		else if (comp > 0){
			res = buscar_subarbol(t, derecho(b));
		}
		else {
			res = b;
		}
	}
	return res;
}

/*
  Devuelve la altura de `b'.
  La altura de un árbol vacío es 0.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
nat altura_binario(binario_t b){

  if (b != NULL){
    //retorno la altura del subarbol mas grande mas uno por contar el nodo en el que me encuentro
    return 1 + maximo( altura_binario(b->izq), altura_binario(b->der) );  
  }
  else{
    return 0;
  }
}

/*
  Devuelve la cantidad de elementos de `b'.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
nat cantidad_binario(binario_t b){
	
	//si el arbol es vacio
	if (b != NULL){
    return  1 + cantidad_binario(b->izq) + cantidad_binario(b->der);
	}
	//si no es vacio
	else{
		return 0;
	}
}


/*
  Devuelve la suma de los datos numéricos de los últimos `i' elementos
  (considerados según la propiedad de orden de los árboles binario_t)
  de `b' cuyos datos numéericos sean pares.
  Si en `b' hay menos de `i' elementos con dato numérico par devuelve la
  suma de todos los datos numéricos pares de `b'.
  No se deben crear estructuras auxiliares.
  No se deben visitar nuevos nodos después que se hayan encontrado los
  `i' elementos.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
*/

void auxiliar_suma_ultimos_pares(nat i, binario_t b, int &cant_sumas, int &valor_sumas){

	if (b == NULL){
		return;
	}
	else{
    //sumo los elementos del subarbol derecho
		auxiliar_suma_ultimos_pares(i, b->der, cant_sumas, valor_sumas);

    //compruebo que la cantidad de sumas sea menor o igual a las q se piden
		if ( cant_sumas < int(i) ){

      //si el dato numero del nodo es par
			if ( (numero_info(b->dato) % 2)  == 0 ){
				valor_sumas = valor_sumas + numero_info(b->dato);
				cant_sumas += 1;
			}
      //compruebo que la cantidad de sumas sea menor o igual a las q se piden
      //si lo es accedo al subarbol izquierdo
			if ( cant_sumas < int(i) ){
				auxiliar_suma_ultimos_pares(i, b->izq, cant_sumas, valor_sumas);
			}
		}
	}
}

int suma_ultimos_pares(nat i, binario_t b){
	//valor_de_suma tiene el valor de la suma de los elementos del arbol que cumplen la condicion
  int valor_de_suma = 0;
  //cantidad_de_sumas, es un contador, cuenta cuantos elementos fueron sumados
	int cantidad_de_sumas = 0;
	auxiliar_suma_ultimos_pares(i,b,cantidad_de_sumas,valor_de_suma);
	return valor_de_suma;
}

/*
  Devuelve una cadena_t con los elementos de `b' en orden lexicográfico
  creciente según sus datos de texto.
  La cadena_t devuelta no comparte memoria con `b'.
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */

void auxiliar_linealizacion(binario_t b, cadena_t cad){

  if( b != NULL ){
    auxiliar_linealizacion(b->izq,cad);
    //inserto el dato del elemento donde estoy
    insertar_al_final( copia_info(b->dato), cad);
    auxiliar_linealizacion(b->der,cad);
  }
}
cadena_t linealizacion(binario_t b){
  cadena_t cadena = crear_cadena();
  auxiliar_linealizacion(b,cadena);
  return cadena;
}

/*
  Devuelve un árbol balanceado cuyos elementos son los que están contenidos en
  `cad'.
  En esta función se dice que un árbol está balanceado si
  a) es vacío;
  o
  b)
    b1) el sub-árbol izquierdo tiene la misma cantidad de elementos o uno más
      que el  subárbol derecho;
    y
    b2) los subárboles izquierdo y derecho están balanceados.

  DEFINICIÓN ALTERNATIVA DE ÁRBOL BALANCEADO
  En esta función se dice que un árbol esta balanceado si en cada nodo, la
  cantidad de elementos de su subárbol izquierdo es igual a, o 1 más que, la
  cantidad de elementos de su subárbol derecho.

  Precondición: los elementos de `cad' están en orden lexicográfico creciente
  estricto según sus datos de texto.
  El árbol devuelto no comparte memoria con `cad'.
  El tiempo de ejecución es O(n . log n), siendo `n` la cantidad de elementos
  de `cad'.
 */

binario_t auxiliar_cadena_a_binario(cadena_t cad, localizador_t desde, localizador_t hasta){
  
  //si largo de la cadena es 1
  if ( desde == hasta ){
    binario_t nodo = new(rep_binario);
    nodo->dato = copia_info(info_cadena(desde,cad));
    nodo->der = NULL;
    nodo->izq = NULL;
    return nodo;
  }

  //si la cadena tiene 2 elemento
  else if ( siguiente(desde,cad) == hasta ) {
    binario_t nodo = new(rep_binario);
    nodo->dato = copia_info(info_cadena(hasta,cad));
    nodo->izq = auxiliar_cadena_a_binario( cad, desde, desde );
    nodo->der = NULL;
    return nodo;

  }

  else{

    //este bool indica si el largo de la cadena hasta el momento reccorida es par o impar 
    // 0 para impar 1 para par.
    bool  qe_es = 0;

    //registra la mitad de la cadeno o la mitad mas 1
    localizador_t mitad_de_cadena = desde;

    localizador_t aux = desde;

    while (aux != hasta){
      aux = siguiente(aux,cad);
      qe_es = !qe_es;

      if(qe_es){
        mitad_de_cadena = siguiente(mitad_de_cadena,cad);
      }
    }

    //nuevo nodo
    binario_t nodo = new(rep_binario);
    nodo->dato = copia_info(info_cadena(mitad_de_cadena,cad));
    nodo->izq = auxiliar_cadena_a_binario( cad, desde, anterior(mitad_de_cadena,cad) );
    nodo->der = auxiliar_cadena_a_binario( cad, siguiente(mitad_de_cadena,cad), hasta );
    return nodo;
  }

}

binario_t cadena_a_binario(cadena_t cad){

  if (es_vacia_cadena(cad)){
    return crear_binario();
  }
  else{
    return auxiliar_cadena_a_binario(cad, inicio_cadena(cad), final_cadena(cad) );
  }
}

/*
  Devuelve un árbol con copias de los elementos de `b' que cumplen la condición
  "numero_info(elemento) < clave".
  La estructura del árbol resultado debe ser análoga a la de `b'. Esto
  significa que dados dos nodos `U' y `V' de `b' en los que se cumple la
  condición y tal que `U' es ancestro de `V', en el árbol resultado la copia de
  `U' debe ser ancestro de la copia de `V' cuando sea posible. Esto no siempre
  se puede lograr y al mismo tiempo mantener la propiedad de orden del árbol
  en el caso en el que en un nodo `V' no se cumple la condición y en sus dos
  subárboles hay nodos en los que se cumple. En este caso, la copia del nodo
  cuyo elemento es el mayor (según la propiedad de orden definida) de los que
  cumplen la condición en el subárbol izquierdo de `V' deberá ser ancestro de
  las copias de todos los descendientes de `V' que cumplen la condición.
  (Ver ejemplos en LetraTarea3.pdf).
  El árbol resultado no comparte memoria con `b'. *)
  El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
 */
binario_t menores(int clave, binario_t b){

  if( b == NULL ){
    return NULL;
  }

  else{
    //subarbol izquierdo al nodo en el que me encuentro con elementos que cumple la condicion numero_info(elemento) < clave 
    binario_t sub_arbol_izquierdo = menores(clave, b->izq);
    //subarbol derecho al nodo en el que me encuentro con elementos que cumple la condicion numero_info(elemento) < clave 
    binario_t sub_arbol_derecho = menores(clave, b->der);

    // numero_info(elemento) < clave
    if ( numero_info(b->dato) < clave ){
      binario_t nodo = new(rep_binario);
      nodo->dato = copia_info(b->dato);
      nodo->izq = sub_arbol_izquierdo;
      nodo->der = sub_arbol_derecho;
      return nodo;

    }

    //si no cumple numero_info(elemento) < clave"
    else{

      if ( (sub_arbol_izquierdo == NULL)  && (sub_arbol_derecho == NULL) ){
        return NULL;
      }

      else if ( (sub_arbol_izquierdo == NULL)  && (sub_arbol_derecho != NULL) ){
        return sub_arbol_derecho;
      }

      else if ( (sub_arbol_izquierdo != NULL)  && (sub_arbol_derecho == NULL) ){
        return sub_arbol_izquierdo;
      }

      //
      else{
        binario_t nodo = new(rep_binario);
        //a nodo le asigno la info del mayor elemento del subarbol izquierdo
        nodo->dato = mayor( sub_arbol_izquierdo );
        //elimino el nodo mayor del subarbol izquierdo
        sub_arbol_izquierdo = remover_mayor( sub_arbol_izquierdo );
        nodo->izq = sub_arbol_izquierdo;
        nodo->der = sub_arbol_derecho;
        return nodo;
      }

    }

  }

}


/*
  Devuelve `true' si y solo si en `b' hay un camino desde la raiz hasta una
  hoja que sea compatible con `c'.
  Decimos que una cadena y un camino son compatibles si tienen la misma
  longitud y al recorrer la cadena desde el inicio hasta el final y el camino
  desde la raíz hasta la hoja las frases de los respectivos elementos son
  iguales.
  El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de
  elementos de `b'.
 */

bool es_camino_mas_localizador(cadena_t c,localizador_t loc, binario_t b){

  //si la cadena es vacia y el arbol es vacio
  if ( (b == NULL) && !es_localizador(loc) ){
    return true;
  }

  //si el arbol no es vacio y la cadena tampoco
  else if ( (b != NULL) && es_localizador(loc) ){

    //compara la frase del nodo del arbol con la frase del elemento de la cadena
    int comp = strcmp( frase_info(info_cadena(loc,c)), frase_info(b->dato) );

    if (comp == 0){

      //pregunto si hay siguiente elemento en la lista
      if( es_localizador( siguiente(loc,c) ) ){

        //compara la frase del nodo del arbol con la frase del elemento siguiente de la cadena
        //esta comparacion la hago para saber si tengo que bajar por el subarbol derecho o izquierdo
        int comp = strcmp( frase_info(info_cadena(siguiente(loc,c),c)), frase_info(b->dato) );

        //reasigno b con el subarbol por el cual debo bajar 

        //bajr por subarbol izquierdo
        if (0 > comp)
          { b = b->izq; }
        //bajar por subarbol derecho
        else
          { b = b->der; }

        return es_camino_mas_localizador(c, siguiente(loc,c), b);
      }

      //si no hay siguiente elemento en la cadena
      else{
        //pregunto si el nodo es hoja

        //si lo es
        if( (b->izq == NULL) && (b->der == NULL) ){
          return true;
        }
        //si no lo es 
        else{
          return false;
        }
      }

    }
    //si no son iguales 
    else{
      return false;
    }
  }

  //si una de las dos parte es invalida
  else{
    return false;
  }
}

bool es_camino(cadena_t c, binario_t b){
    return es_camino_mas_localizador(c, inicio_cadena(c), b);
}

/*
  Devuelve una cadena_t con los elementos del nivel `l' de 'b'.
  La raíz es el nivel 1.
  La cadena_t resultado debe estar ordenada de manera creciente según las
  frases de sus elementos.
  La cadena_t devuelta no comparte memoria con `b'.
 */
cadena_t nivel_en_binario(nat l, binario_t b){

	if(b == NULL){
		return crear_cadena();
	}

	else if ( int(l) == 1 ){
		cadena_t cadena_nueva = crear_cadena();
		insertar_al_final( copia_info(b->dato), cadena_nueva);
		return cadena_nueva;
	}
	else{
		cadena_t cadena_izquierda = nivel_en_binario( l-1, b->izq );
		cadena_t cadena_derecha = nivel_en_binario( l-1, b->der );

		//creo la cadena nueva
		cadena_t cadena_nueva = crear_cadena();

		//inserto cadena_izquierda al final de cadena nueva
		insertar_segmento_despues(cadena_izquierda, final_cadena(cadena_nueva), cadena_nueva);

		//inserto cadena_derecha al final de la cadena a devolver
		insertar_segmento_despues(cadena_derecha, final_cadena(cadena_nueva), cadena_nueva);

		liberar_cadena(cadena_izquierda);
		liberar_cadena(cadena_derecha);

		return cadena_nueva;
	}

}

/*
  Imprime los elementos de `b', uno por línea, en orden descendente de la
  propiedad de orden de los árboles `binario_t'.
  Antes del elemento imprime una cantidad de guiones igual a su nivel.
  El elemento se imprime según la especificación dada en `info_a_texto'.
  El nivel de la raíz es 0.
  Antes de terminar, se debe imprimir un fin de linea.
  Si es_vacio_binario(b) sólo se imprime el fin de línea.
  El tiempo de ejecución es O(n . log n) en promedio, donde `n' es la cantidad
  de elementos de `b'.
 */

void auxiliar_imprimir_binario(binario_t b, int altura){
  if(b != NULL){
    auxiliar_imprimir_binario(b->der, altura + 1);
    //imprimo espacios en blanco
    //la cantidad de espacios depende de la altura
    for(int i = 1; i <= altura; i=i+1){
      printf("-");
    }
    printf("(%d,%s)", numero_info(b->dato), frase_info(b->dato) );    
    printf("\n");
    auxiliar_imprimir_binario(b->izq, altura + 1);
  }
}

void imprimir_binario(binario_t b){
  printf("\n");
  if (b != NULL){
    //altura solo se usa para pasar la altura iniciar del arbol que como empezamos con el nodo la altura es 0
    //ademas es indispensable para la funcion auxiliar
    int altura = 0;
    auxiliar_imprimir_binario(b, altura);
  }
}
