/*********************************************************************************/
/* Nombre del proyecto : Ejemplo de cola en C                                    */
/*********************************************************************************/
/* Identificador       : cola.c                                                  */
/* Descripcion         : Funciones para manejo de cola                           */
/* Autor               : (JAAM)                                                  */
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

/*********************************************************************************/
/* Nombre de la funcion    : vdCrearCola                                         */
/* Descripcion             : Inicializa el primer y ultimo elemento de la cola   */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Cola *cola              :: Cola que va a inicializar                          */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* void                    ::                                                    */
/*********************************************************************************/
void inicializaCola(Cola *cola)
{
    cola->primer = cola->ultimo = NULL;
}

/*********************************************************************************/
/* Nombre de la funcion    : iValidaColaVacia                                    */
/* Descripcion             : Valida si la cola esta vacia                        */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Cola *cola              :: Cola que va a validar                              */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* int                     :: Estatus de la funcion donde:                       */
/*                         :: 1 = La cola esta vacia                             */
/*                         :: 2 = La cola no esta vacia                          */
/*********************************************************************************/
extern short iValidaColaVacia(Cola *cola)
{
    return (cola->primer == NULL ? VACIA : CON_ELEMENTOS);
}

/*********************************************************************************/
/* Nombre de la funcion    : crearNodo                                           */
/* Descripcion             : Crea un nodo y lo inicializa                        */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Pedido *pedido          :: Datos del pedido                                   */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* NodoPedido *            :: nodo con los datos                                 */
/*********************************************************************************/
NodoPedido *crearNodo(Pedido pedido)
{
    NodoPedido *nodo = NULL;
    nodo = (NodoPedido *) malloc(sizeof(NodoPedido));

    if(!nodo)
    {
        printf("No se pudo reservar memoria para nodo nuevo\n");
        return NULL;
    }

    nodo->pedido = pedido;
    nodo->siguente = NULL;
    return nodo;
}

/*********************************************************************************/
/* Nombre de la funcion    : insertar                                            */
/* Descripcion             : Inserta un elemento a la cola                       */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Cola *cola              :: Cola en la que va a insertar                       */
/* Pedido *pedido          :: Pedido a insertar en la cola                       */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion donde                        */
/*********************************************************************************/
short insertar(Cola *cola, Pedido pedido)
{
    NodoPedido *nodo = crearNodo(pedido);

    if (iValidaColaVacia(cola))
    {
        cola->primer = nodo;
        cola->ultimo = nodo;
    }
    else
    {
        cola->ultimo->siguente = nodo;
        cola->ultimo = nodo;
    }
    return EXITO;
}

/*********************************************************************************/
/* Nombre de la funcion    : extraer                                             */
/* Descripcion             : Extrae un elemento y lo elimina de la cola          */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Cola *cola              :: Cola de la que se va a extraer                     */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* Pedido *                :: Pedido que se extrae                               */
/*********************************************************************************/
Pedido *extraer(Cola *cola)
{
    Pedido *nPedido = NULL;
    if (!iValidaColaVacia(cola))
    {
        Pedido pedido = cola->primer->pedido;
        NodoPedido *nodoAborrar = cola->primer;

        if (cola->primer == cola->ultimo)
        {
            cola->primer = NULL;
            cola->ultimo = NULL;
        }
        else
        {
            cola->primer = cola->primer->siguente;
        }
        destruirNodo(nodoAborrar);
        nPedido = &pedido;
        return nPedido;
    }

    printf("La cola esta vacia\n");
    return NULL;
}

/*********************************************************************************/
/* Nombre de la funcion    : imprimir                                            */
/* Descripcion             : Imprime el contenido de la cola                     */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Cola *cola              :: Cola que se va a imprimir                          */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* Void                    ::                                                    */
/*********************************************************************************/
void imprimir(Cola *cola)
{
    if(cola->primer)
    {
        printf("Listado de todos los elementos de la cola.\n");

        while (cola->primer != NULL)
        {
            printf("-------------------------\n");
            printf("ID_Prodcuto: %d\n", cola->primer->pedido.id_producto);
            printf("   Cantidad: %d\n", cola->primer->pedido.cantidad);
            printf("     Precio: %.2f\n", cola->primer->pedido.precio_unidad);
            printf("-------------------------\n");
            cola->primer = cola->primer->siguente;
        }
    }
    else
    {
        printf("La cola esta vacia\n");
    }
}

/*********************************************************************************/
/* Nombre de la funcion    : liberar                                             */
/* Descripcion             : Elimina todos los elementos de la cola              */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* Cola *cola              :: Cola que se va a liberar en memoria                */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* Void                    ::                                                    */
/*********************************************************************************/
void liberar(Cola *cola)
{
    printf("Borrando todos los nodos\n");
    while (cola->primer != NULL)
    {
        NodoPedido *nodoAborrar = cola->primer;
        cola->primer = cola->primer->siguente;
        destruirNodo(nodoAborrar);
    }
}

/*********************************************************************************/
/* Nombre de la funcion    : destruirNodo                                        */
/* Descripcion             : Libera un nodo en memoria                           */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* NodoPedido *nodo        :: Nodo que se va a liberar                           */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* Void                    ::                                                    */
/*********************************************************************************/
void destruirNodo(NodoPedido *nodo)
{
    nodo->siguente = NULL;
    free(nodo);
}