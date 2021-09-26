/******************************************************************************/
/* Proyecto            : Ejemplo de servidor Tuxedo                           */
/******************************************************************************/
/*                                                                            */
/* Identificacion      : tuxedoServer.h                                       */
/* Fecha de creacion   : 28/02/2021                                           */
/* Autor               : Josue Alejandro Aguilar Moreno                       */
/* Descripcion         : Servicio tuxedo devuelve un buferFML de la consulta  */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/*   Librerias estandar                                                       */
/******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <userlog.h>
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
/*                            Variables globales                              */
/******************************************************************************/
char strServicio[200];
EXEC SQL INVOKE =B07_INSTITUCIONES AS b07int_type
typedef struct b07int_type b07ins;

EXEC SQL BEGIN DECLARE SECTION;
    b07ins b07instOut;
EXEC SQL END DECLARE SECTION;

/******************************************************************************/
/* Nombre de la funcion: tpsvrinit                                            */
/* Descripcion         : Funcion de iniciaizacion de servidor tuxedo          */
/*                       inicializa recursos a utilizar                       */
/* Autor               : JAAM                                                 */
/* Parametros entrada  :                                                      */
/* int argc            :: Numero de parametros recibidos                      */
/* char *argv[]        :: Se recibe el parametro %1 que corresponde al nombre */
/*                     :: del equipo donde corre el aplicativo                */
/******************************************************************************/
tpsvrinit(int argc, char *argv[])
{
    userlog("Se inicia servidor tuxedo");
    return 0;
}

/******************************************************************************/
/* Nombre de la funcion: main                                                 */
/* Descripcion         : Funcion principal del cliente tuxedo                 */
/* Autor               : JAAM                                                 */
/* Parametros entrada  :                                                      */
/* int argc            :: Numero de parametros recibidos                      */
/* char *argv[]        :: Se recibe el parametro %1 que corresponde al nombre */
/*                     :: del equipo donde corre el aplicativo                */
/******************************************************************************/
void servicioTuxedo(TPSVCINFO *rqst)
{
    FBFR32 *pBuffer = NULL;

    pBuffer = (FBFR32 *) rqst->data;

    memset((char *) &strServicio, '\0', sizeof(strServicio));
    strcpy(strServicio, rqst->name);

    printf("Nombre se servicio %s", strServicio);

    if(Fpres32(pBuffer, STDN_NOMBRE_CAMPO_FML, 0))
    {
        EXEC SQL DECLARE c_tabla CURSOR FOR
            SELECT  B07_INS_CVE
                   ,B07_INT_NUM_INDICE
                   ,B07_INS_NOM
            FROM   =B07_INSTITUCIONES
            WHERE   B07_INS_NUM_INDICE >= 100
            AND     B07_INS_NUM_INDICE <= 110
        BROWSE ACCESS;

        EXEC SQL OPEN c_tabla;

        while(sqlcode != SQLNOTFOUND)
        {
            memset((char *) &b07instOut, '\0', sizeof(b07instOut));

            EXEC SQL FETCH c_tabla INTO
                :b07instOut.b07_ints_cve
               ,:b07instOut.b07_ints_num_indice
               ,:b07instOut.b07_ints_nom;

            if(sqlcode != SQLNOTFOUND)
            {
                printf("Se cargan campos en buffer FML32\n");
                Fadd32(pBuffer, STD_FML_INST_CVE, (char *)b07instOut.b07_ints_cve, 0);
                Fadd32(pBuffer, STD_FML_INST_NUM_INDICE, (char *)b07instOut.b07_ints_num_indice, 0);
                Fadd32(pBuffer, STD_FML_INST_NOM, (char *)b07instOut.b07_ints_nom, 0);
            }
        }

        EXEC SQL CLOSE c_tabla;

        if(sqlcode != EXITO)
        {
            printf("Error al cerrar cursor\n");
        }

        printf("Exito al realizar la consulta\n");
        
        tpreturn(TPSUCCESS, 0, (char *) pBuffer, 0, 0);
    }
}