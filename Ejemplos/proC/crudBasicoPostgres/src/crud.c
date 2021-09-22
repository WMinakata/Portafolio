/*********************************************************************************/
/* Nombre del proyecto : CRUD sql embebido                                       */
/*********************************************************************************/
/* Identificador       : crud.c                                                  */
/* Descripcion         :                                                         */
/*                     :                                                         */
/* Autor               : Josue Alejandro Aguilar Moreno (JAAM)                   */
/* Fecha de creacion   : 02/12/20                                                */
/* Bitacora de Modificaciones:                                                   */
/* ============================                                                  */
/*   Fecha      Version       Autor                     Descripcion              */
/*   =====      =======       =====                     ===========              */
/* 02-DIC-20      1.0       Josue Aguilar       1.- Version inicial              */
/*                              (JAAM)                                           */
/*                                                                               */
/*********************************************************************************/

/*********************************************************************************
                        LIBRERIAS ESTANDAR
**********************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*********************************************************************************
                        LIBRERIAS DE BASE DE DATOS
**********************************************************************************/

EXEC SQL INCLUDE sqlca;

/*********************************************************************************
                        DEFINICIONES
**********************************************************************************/

#define  SQLNOTFOUND   100
#define  SQLEXITO        0
#define  SUCCESS         0
#define  ERROR        (-1)
#define  TRUE            1
#define  FALSE           0

/*********************************************************************************
                        PROTOTIPOS
**********************************************************************************/
void  gestionaError(void);
short createTable(void);
short readTable(void);
short updateField();
short deleteField();
short insertTable(void);

/*********************************************************************************
                        VARIABLES GLOBALES
**********************************************************************************/
EXEC SQL WHENEVER SQLERROR DO gestionaError();

EXEC SQL BEGIN DECLARE SECTION;
    typedef struct stB220SegOper
    {
        char        b220_opa_cve_tipo_orden[0 + 1];
        int         b220_opa_cve;
        short       b220_opa_id_flujo;
        long long   b220_opa_jul_time_stamp;
        char        b220_opa_cve_estado[2 + 1];
        short       b220_opa_cve_med;
        int         b220_opa_cve_inst;
        double      b220_opa_mto;
        short       b220_opa_cve_tpa;
        char        b220_opa_cve_rastreo[30 + 1];
        short       b220_opa_num_cve_ben;
        char        b220_opa_des_cta_ben[20 + 1];
    }stB220SegOper;
EXEC SQL END DECLARE SECTION;

/*********************************************************************************/
/* Nombre de la funcion    : main                                                */
/* Descripcion             : Funcion principal para generar el archivo segundero */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* int argc                :: Numero de parametros                               */
/* char *argv              :: Contenido de parametro                             */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* int                     :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
int main(int args, char *argv[])
{
    /* EXEC SQL CONNECT TO "UNIX: postgreSQL: //127.0.0.1:5432/operaciones" AS server1 USER postgres; */
    /* EXEC SQL CONNECT "postgres" IDENTIFIED BY "root"; */
    EXEC SQL CONNECT TO "operaciones" AS myConnection;

    if(sqlca.sqlcode == ERROR)
    {
        printf("Error al conectarse a la base de datos\n");
        return ERROR;
    }
    else
    {
        printf("Se conecta a la base de datos\n");
    }

    if(createTable() != SUCCESS)
    {
        printf("No fue posible crear la tabla B220_SEG_OPER\n");
        printf("SQLSTATE = %s", sqlca.sqlstate);
        return  ERROR;
    }

    if(insertTable() != SUCCESS)
    {
        printf("Fallo en funcion insertTable\n");
    }

    if(updateField() != SUCCESS)
    {
        printf("Fallo en funcion updateField\n");
    }

    if(deleteField() != SUCCESS)
    {
        printf("Fallo en funcion deleteField\n");
    }

    EXEC SQL BEGIN DECLARE SECTION;
        long long   ll_MaxFolio = 0;      /* Cantidad maxima de folios */
    EXEC SQL END DECLARE SECTION;

    EXEC SQL
        SELECT  COUNT(DISTINCT B220_OPA_CVE)
        INTO    :ll_MaxFolio
        FROM    B220_SEG_OPER
        WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R';

    printf("sqlstate = %s\n", sqlca.sqlstate);
    printf("sqlcode = %ld\n", sqlca.sqlcode);
    printf("Cantidad de folios %lld \n", ll_MaxFolio);

    EXEC SQL DISCONNECT myConnection;
    return 0;
}

/*********************************************************************************/
/* Nombre de la funcion    : createTable                                         */
/* Descripcion             : Funcion para crear tabla                            */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* int                     :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short createTable(void)
{
    EXEC SQL
        CREATE TABLE IF NOT EXISTS B220_SEG_OPER
        (
              B220_OPA_CVE_TIPO_ORDEN     CHAR(1) NOT NULL
            , B220_OPA_CVE                INT NOT NULL
            , B220_OPA_ID_FLUJO           SMALLINT NOT NULL
            , B220_OPA_JUL_TIME_STAMP     BIGINT NOT NULL
            , B220_OPA_CVE_ESTADO         CHAR(2) NULL
            , B220_OPA_CVE_MED            SMALLINT NULL
            , B220_OPA_CVE_INST           INT NULL
            , B220_OPA_MTO                NUMERIC(18, 2) NULL
            , B220_OPA_CVE_TPA            SMALLINT NULL
            , B220_OPA_CVE_RASTREO        CHAR(30) NULL
            , B220_OPA_NUM_CVE_BEN        SMALLINT NULL
            , B220_OPA_DES_CTA_BEN        CHAR(20) NULL
        );

    if(!strcmp(sqlca.sqlstate, "42P07"))
    {
        printf("B220_SEG_OPER existe, NO es necesario crearla\n");
    }
    else
    {
        printf("Se crea tabla B220_SEG_OPER\n");
    }

    if(sqlca.sqlcode != SQLEXITO)
    {
        printf("sqlstate = %s\n", sqlca.sqlstate);
        printf("sqlcode = %ld\n", sqlca.sqlcode);
        EXEC SQL ROLLBACK;
        return ERROR;
    }

    EXEC SQL COMMIT;
    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : deleteField                                         */
/* Descripcion             : Funcion para eliminar un registro                   */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short deleteField(void)
{
    EXEC SQL
        DELETE FROM B220_SEG_OPER
        WHERE B220_OPA_CVE = 3;

    if(sqlca.sqlcode != SQLEXITO)
    {
        printf("sqlstate = %s\n", sqlca.sqlstate);
        printf("sqlcode = %ld\n", sqlca.sqlcode);
        EXEC SQL ROLLBACK;
        return ERROR;
    }

    EXEC SQL COMMIT;
    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : updateField                                         */
/* Descripcion             : Funcion para actualizar un registro                 */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short updateField(void)
{
    EXEC SQL 
        UPDATE B220_SEG_OPER
        SET B220_OPA_CVE_INST = 62345
        WHERE B220_OPA_CVE_TIPO_ORDEN = 'R'
        AND B220_OPA_CVE = 6;

    if(sqlca.sqlcode != SQLEXITO)
    {
        printf("sqlstate = %s\n", sqlca.sqlstate);
        printf("sqlcode = %ld\n", sqlca.sqlcode);
        EXEC SQL ROLLBACK;
        return ERROR;
    }

    EXEC SQL COMMIT;
    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : readTable                                           */
/* Descripcion             : Funcion para eliminar todos los registros           */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short readTable(void)
{
    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : insertTable                                         */
/* Descripcion             : Funcion para llenar tabla                           */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short insertTable(void)
{
    FILE    *pFicheroOP = NULL;
    char    strLineas[250];

    EXEC SQL BEGIN DECLARE SECTION;
        struct stB220SegOper   st_B220SegOper;
    EXEC SQL END DECLARE SECTION;

    pFicheroOP = fopen("/home/wminakata/Documentos/Proyectos/DevC/usaPostgres/segundero/src/catOpers.txt", "r");

    if(pFicheroOP != NULL)
    {
        while (!feof(pFicheroOP))
        {
            memset((char *) &st_B220SegOper, '\0' ,sizeof(st_B220SegOper));
            memset(strLineas, '\0' ,sizeof(strLineas));

            fgets(strLineas, sizeof(strLineas), pFicheroOP);

            if((strlen(strLineas)) < 1)
            {
                continue;
            }

            strcpy(st_B220SegOper.b220_opa_cve_tipo_orden, strtok(strLineas, ";"));
            st_B220SegOper.b220_opa_cve = atoi(strtok(NULL, ";"));
            st_B220SegOper.b220_opa_id_flujo = (short)atoi(strtok(NULL, ";"));
            st_B220SegOper.b220_opa_jul_time_stamp = atoll(strtok(NULL, ";"));
            strcpy(st_B220SegOper.b220_opa_cve_estado, strtok(NULL, ";"));
            st_B220SegOper.b220_opa_cve_med = (short)atoi(strtok(NULL, ";"));
            st_B220SegOper.b220_opa_cve_inst = atoi(strtok(NULL, ";"));
            st_B220SegOper.b220_opa_mto = strtod(strtok(NULL, ";"), NULL);
            st_B220SegOper.b220_opa_cve_tpa = (short)atoi(strtok(NULL, ";"));
            strcpy(st_B220SegOper.b220_opa_cve_rastreo, strtok(NULL, ";"));
            st_B220SegOper.b220_opa_num_cve_ben = (short)atoi(strtok(NULL, ";"));
            strcpy(st_B220SegOper.b220_opa_des_cta_ben, strtok(NULL, ";"));

            EXEC SQL
                INSERT INTO B220_SEG_OPER
                (
                      B220_OPA_CVE_TIPO_ORDEN
                    , B220_OPA_CVE           
                    , B220_OPA_ID_FLUJO      
                    , B220_OPA_JUL_TIME_STAMP
                    , B220_OPA_CVE_ESTADO    
                    , B220_OPA_CVE_MED       
                    , B220_OPA_CVE_INST      
                    , B220_OPA_MTO           
                    , B220_OPA_CVE_TPA       
                    , B220_OPA_CVE_RASTREO   
                    , B220_OPA_NUM_CVE_BEN   
                    , B220_OPA_DES_CTA_BEN   
                )
                VALUES
                (
                      :st_B220SegOper.b220_opa_cve_tipo_orden
                    , :st_B220SegOper.b220_opa_cve
                    , :st_B220SegOper.b220_opa_id_flujo
                    , :st_B220SegOper.b220_opa_jul_time_stamp
                    , :st_B220SegOper.b220_opa_cve_estado
                    , :st_B220SegOper.b220_opa_cve_med
                    , :st_B220SegOper.b220_opa_cve_inst
                    , :st_B220SegOper.b220_opa_mto
                    , :st_B220SegOper.b220_opa_cve_tpa
                    , :st_B220SegOper.b220_opa_cve_rastreo
                    , :st_B220SegOper.b220_opa_num_cve_ben
                    , :st_B220SegOper.b220_opa_des_cta_ben
                );

                if(sqlca.sqlcode != SQLEXITO)
                {
                    printf("Falla INSERT en folio %d\n", st_B220SegOper.b220_opa_cve);
                    EXEC SQL ROLLBACK;
                }
                else
                {
                    EXEC SQL COMMIT;
                }
        }
    }
    else
    {
        printf("No se pudo abrir el archivo con operaciones\n");
        return ERROR;
    }

    return SUCCESS;
}

void gestionaError(void)
{
    EXEC SQL WHENEVER SQLERROR CONTINUE;
    printf("sqlstate = %s\n", sqlca.sqlstate);
    printf("sqlcode = %ld\n", sqlca.sqlcode);
    EXEC SQL ROLLBACK;
    exit(-1);
}