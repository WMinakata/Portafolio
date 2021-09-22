/*********************************************************************************/
/* Nombre del proyecto : Ejemplo de cola en C                                    */
/*********************************************************************************/
/* Identificador       : main.c                                                  */
/* Descripcion         : Programa para poner a prueba la logica de colas         */
/* Autor               : Josue Alejandro Aguilar Moreno (JAAM)                   */
/* Fecha de creacion   : 06/09/21                                                */
/*                                                                               */
/* Bitacora de Modificaciones:                                                   */
/* ============================                                                  */
/*   Fecha      Version       Autor                     Descripcion              */
/*   =====      =======       =====                     ===========              */
/* 06-SEP-21      1.0         (JAAM)            1.- Version inicial              */
/*                                                                               */
/*********************************************************************************/

/*********************************************************************************
                        LIBRERIAS DEL APLICATIVO
**********************************************************************************/
#include <cola.h>

/*********************************************************************************
                        VARIABLES GLOBALES
**********************************************************************************/

Cola cola;

/*********************************************************************************/
/* Nombre de la funcion    : main                                                */
/* Descripcion             : Funcion principal para implementar la cola          */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* int argc                :: Numero de parametros                               */
/* char *argv              :: Contenido de parametro                             */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* int                     :: Estatus de la funcion                              */
/*********************************************************************************/
int main(int argc, char *argv[])
{
    inicializaCola(&cola);

    Pedido st_pedido;
    /* Pedido *st_p = NULL; */

    memset((char *) &st_pedido, '\0', sizeof(st_pedido));

    st_pedido.id_producto = 1;
    st_pedido.cantidad = 3;
    st_pedido.precio_unidad = 3.50;

    if(insertar(&cola, st_pedido))
    {
        printf("Error al insertar en cola\n");
        return ERROR;
    }

    st_pedido.id_producto = 2;
    st_pedido.cantidad = 5;
    st_pedido.precio_unidad = 207.50;

    if(insertar(&cola, st_pedido))
    {
        printf("Error al insertar en cola\n");
        return ERROR;
    }
    
    st_p = extraer(&cola);
    if(st_p)
    {
        printf("ID_Prodcuto: %d\n", st_p->id_producto);
        printf("   Cantidad: %d\n", st_p->cantidad);
        printf("     Precio: %.2f\n\n", st_p->precio_unidad);
    }

    imprimir(&cola);

    st_pedido.id_producto = 3;
    st_pedido.cantidad = 16;
    st_pedido.precio_unidad = 7.40;

    if(insertar(&cola, st_pedido))
    {
        printf("Error al insertar en cola\n");
        return ERROR;
    }

    imprimir(&cola);

    liberar(&cola);

    return SUCCESS;
}
