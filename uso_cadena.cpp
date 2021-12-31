/* 4730508 */

/*
  Módulo de definición de `uso_cadena_s'.

  Se definen funciones sobre cadenas.

  Laboratorio de Programación 2.
  InCo-FIng-UDELAR
 */

#include <string.h> 


#include "../include/info.h"
#include "../include/cadena.h"

/*
  Devuelve `true' si y sólo si en `cad' hay un elemento cuyo dato numérico es
  `i'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
bool pertenece(int i, cadena_t cad){

    //desde el inicio de la cadena busco un elemento(localizador) de la misma cuyo dato numerico sea i
    //luego pregunto si el localizador es valido
    //si la cadena llegasea ser vacia, no hay problema.
    return es_localizador( siguiente_clave(i,inicio_cadena(cad),cad) );
}

/*
   Devuelve la cantidad de elementos de `cad'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
nat longitud(cadena_t cad){
    //recorreremos la cadena con el siguiente localizador
    localizador_t posicion_en_cadena = inicio_cadena(cad);
    int cantidad_elementos_cadena = 0;
    //cada vez que estamos sobre un elemento valido de la cadena sumamos uno a cantidad_elementos_cadena  y avanzamos al siguiente elemento
    while(es_localizador(posicion_en_cadena)){
      posicion_en_cadena = siguiente(posicion_en_cadena, cad);
      cantidad_elementos_cadena += 1;
    }
    return cantidad_elementos_cadena;
}


/*
  Devuelve `true' si y sólo si `cad' está ordenada de forma no decreciente
  (= creciente de manera no estricta) según los datos numéricos de sus
  elementos.
  Si es_vacia_cadena (cad) devuelve `true'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena.
 */
bool esta_ordenada(cadena_t cad){
  if( es_vacia_cadena(cad) ){ 
    return true;
  }
  else{
    //este localizador va a ser usado para recorrer lacadena 
    localizador_t localizador = inicio_cadena(cad);
    while( es_localizador( siguiente(localizador,cad) ) ){
      if(numero_info( info_cadena(localizador, cad) ) > numero_info( info_cadena(siguiente(localizador, cad), cad) )){
        return false;
      }
      else{ localizador = siguiente(localizador, cad); }
    }
    //salio del while... significa que la cadena esta ordenada 
    return true;
  }
}


/*
  Devuelve una cadena_t con todos los elementos de `c1' y `c2' ordenada de
  manera  creciente segun sus datos numéricos.
  Si hay elementos cuyos datos numéricos son iguales los elementos de `c1'
  quedan antes de los de `c2'.
  La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
  Precondición: esta_ordenada (c1) y esta_ordenada (c2).
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena resultado.
 */
cadena_t mezcla(cadena_t c1, cadena_t c2){

	//creo la nueva cadena cadena_mezcla
	cadena_t cadena_mezcla = crear_cadena();
  //estos localizadores se utilizaran para recorer las respectivas cadenas
	localizador_t localizador_cadena_c1 = inicio_cadena(c1);
	localizador_t localizador_cadena_c2 = inicio_cadena(c2);
    while( es_localizador(localizador_cadena_c1) && es_localizador(localizador_cadena_c2) ){
      //comparo si el dato de la cadena c1(localizador_cadena_c1) es menor o igual a el dato de la cadena c2(localizador_cadena_c2) 
      if( numero_info( info_cadena(localizador_cadena_c1, c1) ) <= numero_info( info_cadena(localizador_cadena_c2, c2) ) ){
        //inserto el dato de c1(localizador_cadena_c1) en cadena_mezcla
        insertar_al_final( copia_info( info_cadena(localizador_cadena_c1,c1) ),cadena_mezcla);
        //avanzo en la cadena c1
        localizador_cadena_c1 = siguiente(localizador_cadena_c1,c1);
      }
      else{
        //inserto el elemento de c2(localizador_cadena_c2) en cadena_mezcla
        insertar_al_final(copia_info( info_cadena(localizador_cadena_c2,c2) ),cadena_mezcla);
        //avanzo en la cadena c2
        localizador_cadena_c2 = siguiente(localizador_cadena_c2,c2);
      }
    }
    //salir de while significa que hay almenos una de las cadenas que la recorrimos hasta el final
    //si no llege al final de la cadena c1
		if ( es_localizador(localizador_cadena_c1) ){
      //copio la cadena c1 desde donde localizador_cadena_c1 hasta el final
      cadena_t sub_cadena_de_c1 = copiar_segmento( localizador_cadena_c1,final_cadena(c1),c1 );
      //luego inserto esa sub cadena en cadena mezcla desde el final de la cadena
      insertar_segmento_despues( sub_cadena_de_c1, final_cadena(cadena_mezcla), cadena_mezcla );
      //libero la sub_cadena
      liberar_cadena(sub_cadena_de_c1);
		}
    //si no llege al final de la cadena c2
		if ( es_localizador(localizador_cadena_c2) ){
      //copio la cadena c2 desde donde localizador_cadena_c2 hasta el final
      cadena_t sub_cadena_de_c2 = copiar_segmento( localizador_cadena_c2,final_cadena(c2),c2 );
      //luego inserto esa sub cadena en cadena mezcla desde el final de la cadena
      insertar_segmento_despues( sub_cadena_de_c2, final_cadena(cadena_mezcla), cadena_mezcla );
      //libero la sub_cadena
      liberar_cadena(sub_cadena_de_c2);
		}
    return cadena_mezcla;
}

/*
  Devuelve el resultado de concatenar `c2' después de `c1'.
  La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la
  cadena resultado.
 */
cadena_t concatenar(cadena_t c1, cadena_t c2){
    // copio la cadena c1 de inicio a fin
    cadena_t c1_copia = copiar_segmento( inicio_cadena(c1) , final_cadena(c1) , c1 );
    // copio la cadena c2 de inicio a fin
    cadena_t c2_copia = copiar_segmento( inicio_cadena(c2) , final_cadena(c2) , c2 );
    //inserto la c2_copia al final de c1_copia
    insertar_segmento_despues( c2_copia, final_cadena(c1_copia), c1_copia );
    liberar_cadena(c2_copia);
    return c1_copia;
}

/*
  Se ordena `cad' de manera creciente según los datos numéricos de sus
  elementos.
  Devuelve `cad'.
  No se debe obtener ni devolver memoria de manera dinámica.
  Si es_vacia_cadena (cad) no hace nada.
  El tiempo de ejecución es O(n^2), siendo `n' la cantidad de elementos en
  la cadena resultado.
 */
cadena_t ordenar(cadena_t cad){
  if (!(es_vacia_cadena(cad))){
    //hasta que elemento de la cadena va el proceso de ordenar
    localizador_t hasta = final_cadena(cad);
    //localizador apunta a un elemento de la cadena, con el se recorre la cadena hasta donde coresponda
    localizador_t localizador;
    //mientras el proceso de ordenar no llege al primer elemento, lo cual significaria que ya no hay nada que hacer, continuamos.
    while( inicio_cadena(cad) != hasta ){
      //cada vez que comienza el primer ciclo while el proceso de ordenacion comienza desde el primer elemento
      localizador = inicio_cadena(cad) ;
      //mientras que el elemento al que apunta localizador sea distinto al elemento hasta donde tiene que ir el proceso de ordenacion continuamos
      //en cada iteracion de este while localizador toma el elemento siguiente en la cadena
      while(localizador != hasta){
        //si el dato numero del elemento(localizador) es mayor al del siguiente elemento
        if ( numero_info( info_cadena(localizador, cad) ) > numero_info( info_cadena(siguiente(localizador,cad), cad) ) ){
          //intercambio datos numericos entre ambos elementos
          intercambiar( localizador, siguiente(localizador,cad), cad );
          }
        localizador = siguiente(localizador, cad);
      }
      hasta = anterior(hasta,cad);
    }
  }
  return cad;
}

/*
  Cambia todas las ocurrencias de `original' por `nuevo' en los elementos
   de `cad'. No debe quedar memoria inaccesible.
  Devuelve `cad'.
  El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en
  la cadena resultado.
 */
cadena_t cambiar_todos(int original, int nuevo, cadena_t cad){
  if ( !es_vacia_cadena(cad) ){
    //apunta a los elementos que requieren un cambio
    //busco el primer elemento que tiene como dato numrico a original
    localizador_t localizador = siguiente_clave( original, inicio_cadena(cad), cad);
    //mientras localizador sea valido significa que hay que realizar cambios a almenos un elemento de la cadena
    while( es_localizador(localizador) ){
      //creo un string para copiar el texto del elemento que necesita cambios
      char * copia_texto = new char[strlen( frase_info( info_cadena(localizador,cad) ) ) + 1];
      //copio el string del elemento
      strcpy(copia_texto, frase_info(info_cadena(localizador,cad)) );
      //crea la nueva informacion
      info_t string_copiado = crear_info(nuevo, copia_texto);
      //elimino la info obsoleta
      info_t info_a_eliminar = info_cadena(localizador,cad);
      liberar_info(info_a_eliminar);
      //asigno la nueva info al elemento corespondiente identificado con el puntero localizador
      cambiar_en_cadena( string_copiado, localizador, cad );
      //si no hay mas elementos para buscar, pongo localizador = Null para slir del ciclo while
      if ( es_final_cadena(localizador,cad) ){
        localizador = NULL;
      }
      else{
        //busco el siguiente elemento que necesita cambios a partir del siguiente elemento del cual me encuentro ahora
        localizador = siguiente_clave( original, siguiente(localizador, cad), cad);
      }
    }
  }
  return cad;
}
