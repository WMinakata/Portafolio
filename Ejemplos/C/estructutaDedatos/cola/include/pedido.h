/*******************************************************************************/
/* Identificador        : pedido.h                                             */
/* Fecha de creacion    : 06/09/2021                                           */
/* Autor                : JAAM                                                 */
/* Descripcion          : cabecera que define la estructura de pedidos         */
/*******************************************************************************/
#ifndef PEDIDO_H

/*********************************************************************************
                        LIBRERIAS ESTANDAR
**********************************************************************************/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*********************************************************************************
                        DEFINICIONES
**********************************************************************************/

typedef struct Pedido
{
    int     id_producto;
    int     cantidad;
    float   precio_unidad;
}Pedido;

typedef struct NodoPedido
{
    Pedido              pedido;
    struct NodoPedido  *siguente;
}NodoPedido;

#endif /* PEDIDO_H */