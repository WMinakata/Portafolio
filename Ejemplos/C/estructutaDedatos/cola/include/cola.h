/*******************************************************************************/
/* Identificador        : cola.h                                               */
/* Fecha de creacion    : 06/09/2021                                           */
/* Autor                : JAAM                                                 */
/* Descripcion          : cabecera que define la estructura de colas           */
/*******************************************************************************/
#ifndef  COLA_H

/*********************************************************************************
                        LIBRERIAS DEL APLICATIVO
**********************************************************************************/

#include <pedido.h>

/*********************************************************************************
                        DEFINICIONES
**********************************************************************************/

#define VACIA 1
#define CON_ELEMENTOS 0

#define ERROR (-1)
#define SUCCESS 0
#define EXITO 0

typedef struct Cola
{
    NodoPedido   *primer;
    NodoPedido   *ultimo;
}Cola;

/*********************************************************************************
                        PROTOTIPOS
**********************************************************************************/

extern short iValidaColaVacia(Cola *cola);
NodoPedido *crearNodo(Pedido  pedido);
void    inicializaCola(Cola *cola);
short   insertar(Cola *cola, Pedido pedido);
Pedido  *extraer(Cola *cola);
void    imprimir(Cola *cola);
void    liberar(Cola *cola);
void    destruirNodo(NodoPedido *nodo);

#endif /* COLA_H */
