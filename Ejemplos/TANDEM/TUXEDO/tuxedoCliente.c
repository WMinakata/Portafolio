/******************************************************************************/
/* Proyecto            : Ejemplo de cliente Tuxedo                            */
/******************************************************************************/
/*                                                                            */
/* Identificacion      : tuxedoCliente.h                                      */
/* Fecha de creacion   : 28/02/2021                                           */
/* Autor               : Josue Alejandro Aguilar Moreno                       */
/* Descripcion         : Llama al servicio tuxedoServer para cargar un buffer */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*   Librerias estandar                                                       */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <atmi.h>

/******************************************************************************/
/*   Librerias SQL                                                            */
/******************************************************************************/
#include <sql.h>

/******************************************************************************/
/*   Librerias Tuxedo                                                         */
/******************************************************************************/
#include "fml32.h"
#include "atmi.h"

/******************************************************************************/
/*   Include para manejo de FMLs                                              */
/******************************************************************************/
#include "estandar.flds.h"
#include "catalogo.flds.h"

/******************************************************************************/
/*   Definiciones                                                             */
/******************************************************************************/
#define TAM_BUFFER                      1024


/******************************************************************************/
/* Nombre de la funcion: main                                                 */
/* Descripcion         : Funcion principal del cliente tuxedo                 */
/* Autor               : JAAM                                                 */
/* Parametros entrada  :                                                      */
/* int argc            :: Numero de parametros recibidos                      */
/* char *argv[]        :: Se recibe el parametro %1 que corresponde al nombre */
/*                     :: del equipo donde corre el aplicativo                */
/******************************************************************************/
int main(int argc, char argv[])
{

    if (tpinit((TPINIT *) NULL) == -1)
    {
        fprintf(stderr, "Fallo en Tpinit\n");
        exit(-1);
    }

    FBFR32 *pBufferFML = NULL;
    char    strServicio[200];
    long    lStrlen = 0L;
    char    strCve[200];
    char    strNumIndice[200];
    char    strInstNomb[200];

    memset((char *) &strServicio, '\0', sizeof(strServicio));
    memset((char *) &strCve, '\0', sizeof(strCve));
    memset((char *) &strNumIndice, '\0', sizeof(strNumIndice));
    memset((char *) &strInstNomb, '\0', sizeof(strInstNomb));

    strcpy(strServicio, argv[1]);

    pBufferFML = (FBFR32 *) tpalloc("FML32", NULL, TAM_BUFFER) ;

    if(pBufferFML == (FBFR32 *) NULL)
    {
        printf("Error al reservar memoria para pBufferFML");
        return (-1);
    }


    printf("Inicia cliente tuxedo %s", strServicio);

    if(tpcall((char *) "servicioTuxedo", (char *)pBufferFML, 0 (char **)&pBufferFML, &lStrlen, 0) == -1)
    {
        printf("Error en llamada a Serv_Atiende\n");
        tpfree(pBufferFML);
        tpterm();
    }

    Fget32(pBuffer, STD_FML_INST_CVE, 0, strCve, NULL);
    Fget32(pBuffer, STD_FML_INST_NUM_INDICE, 0, strNumIndice, NULL);
    Fget32(pBuffer, STD_FML_INST_NOM, 0, strInstNomb, NULL);

    printf("Datos recuperados del servicio");
    printf("Clave: %s", strCve);
    printf("Indice: %s", strNumIndice);
    printf("Nombre: %s", strInstNomb);

    tpfree(pBufferFML);
    tpterm();
}