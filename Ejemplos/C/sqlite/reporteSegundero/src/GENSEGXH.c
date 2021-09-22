/*********************************************************************************
                                LIBRERIAS
**********************************************************************************/
#include <GENSEGXH.h>

/*********************************************************************************
                                VARIABLES GLOBALES
**********************************************************************************/

sqlite3         *db = NULL;     /* Puntero a base de datos */
char            cPathData[200]; /* Ruta de la base de datos */
char            consulta[5000]; /* Query */
sqlite3_stmt    *stmt = NULL;   /* Estructura que almacena la consulta */
stB220SegOper   st_B220SegOper; /* Estructura de la B220_SEG_OPER */

/*********************************************************************************/
/* Nombre de la funcion    : main                                                */
/* Descripcion             : Funcion principal para generar el archivo segundero */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* int argc                :: Numero de parametros                               */
/* char argv               :: Contenido de parametro                             */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* int                     :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
int main(int argc, char *argv[])
{
    FILE            *f_segReport;   /* Puntero archivo segundero */
    char            cPahtFile[200]; /* Ruta del archivo segundero */
    short           sStatusFunc = 0;
    short           sFinEjec = 0;

    strcpy(cPahtFile, "/home/wminakata/Documentos/Proyectos/DevC/segundero/src/nvoSegundero.txt");

    if((f_segReport = fopen(cPahtFile, "w")) == NULL)
    {
        printf("<$ - GENSEGXH> Error al abrir el archivo %s\n", cPahtFile);
        return ERROR;
    }

    printf("<$ - GENSEGXH> Generando el reporte de operaciones recibidas\n");

    fprintf(f_segReport, "      Folio\tTipoPago\tBanco\tMedio\t           Importe"
                         "\tCuentaBen\t                 Clabe de rastreo"
                         "\t       Cuenta de Abono\tEstatus\t    T0"
                         "\t                T1\t                T2"
                         "\t                T3\t                T4"
                         "\t                T5\t                T6"
                         "\t                T7\t                T8"
                         "\t                T9\t               T10"
                         "\t               T11\t    Total T9 - T1\n");

    sStatusFunc = generaReporteRecibidas(f_segReport);

    if(sStatusFunc == SUCCESS)
    {
        printf("<$ - GENSEGXH> Termina con exito...\n");
        sFinEjec = SUCCESS;
    }
    else
    {
        printf("<$ - GENSEGXH> Termina con error...\n");
        sFinEjec = ERROR;
    }

    fclose(f_segReport);
    printf("<$ - GENSEGXH> TERMINA PROGRAMA GENSEGXH\n");
    return sFinEjec;
}

/*********************************************************************************/
/* Nombre de la funcion    : generaReporteRecibidas                              */
/* Descripcion             :                                                     */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short generaReporteRecibidas(FILE *pf_destino)
{
    long long llMaxFolios = 0;
    long long llContFolios = 0;
    short     sMaxIDFlujo = 0;
    void *ptr_void = NULL;
    long long llArrTimeStamp[12];
    double dSegundos = 0.0;

    if(pf_destino == NULL)
    {
        return ERROR;
    }

    strcpy(cPathData, "/home/wminakata/Documentos/Proyectos/sqlite/database/DATA.db");

    if(sqlite3_open(cPathData, &db) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error al abrir la base de datos\n");
        printf("<$ - GENSEGXH> en ruta [%s]\n", cPathData);
        printf("<$ - GENSEGXH> %s", sqlite3_errmsg(db));
        return ERROR;
    }

    strcpy(consulta, "SELECT    COUNT(DISTINCT B220_OPA_CVE)\n"
                     "FROM      B220_SEG_OPER\n"
                     "WHERE     B220_OPA_CVE_TIPO_ORDEN = 'R'\n"
                     "AND       B220_OPA_CVE >= 1\n"
                     "ORDER BY B220_OPA_CVE ASC\n");

    ptr_void = consultaSimple(consulta);

    if(ptr_void != NULL)
    {
        llMaxFolios = *(long long *)ptr_void;
    }
    else
    {
        printf("<$ - GENSEGXH> Error en funcion consultaSimple para query:\n");
        printf("----------------------------------------------------------\n");
        printf("%s", consulta);
        printf("----------------------------------------------------------\n");
        return ERROR;
    }

    if(llMaxFolios > 0)
    {
        /* RECORRE LOS FOLIOS */
        strcpy(consulta, "SELECT DISTINCT B220_OPA_CVE\n"
                         "FROM   B220_SEG_OPER\n"
                         "WHERE  B220_OPA_CVE_TIPO_ORDEN = 'R'\n"
                         "AND    B220_OPA_CVE >= 1\n"
                         "ORDER BY B220_OPA_CVE ASC\n");

        if(sqlite3_prepare_v2(db, consulta, -1, &stmt, NULL) != SQLITE_OK)
        {
            printf("<$ - GENSEGXH> Error en funcion sqlite3_prepare_v2\n");
            printf("%s\n", sqlite3_errmsg(db));
            return ERROR;
        }
        
        llContFolios = 0;

        while(sqlite3_step(stmt) != SQLITE_DONE)
        {
            memset((char *) &st_B220SegOper, '\0', sizeof(st_B220SegOper));

            st_B220SegOper.b220_opa_cve = (int) sqlite3_column_int(stmt, 0);

            /* obtener los primeros registros en base al ID_flujo = 3 */
            if(getFoliosIDFlujo3(&st_B220SegOper, consulta) != SUCCESS)
            {
                printf("<$ - GENSEGXH> Error en funcion getFoliosIDFlujo3\n");
                return ERROR;
            }

            /* OBTENER EL MAXIMO ID_FLUJO */
            sprintf(consulta, "SELECT  MAX(B220_OPA_ID_FLUJO)\n"
                              "FROM    B220_SEG_OPER\n"
                              "WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'\n"
                              "AND     B220_OPA_CVE = %d \n"
                              , st_B220SegOper.b220_opa_cve);

            ptr_void = consultaSimple(consulta);
            if(ptr_void != NULL)
            {
                sMaxIDFlujo = *(short *)ptr_void;
            }
            else
            {
                printf("<$ - GENSEGXH> Error en funcion consultaSimple para query:\n");
                printf("----------------------------------------------------------\n");
                printf("%s", consulta);
                printf("----------------------------------------------------------\n");
                return ERROR;
            }

            /* OBTENER EL ULTIMO ESTADO DEL FOLIO */
            sprintf(consulta, "SELECT	B220_OPA_CVE_ESTADO\n"
                              "FROM	B220_SEG_OPER\n"
                              "WHERE	B220_OPA_CVE_TIPO_ORDEN = 'R'\n"
                              "AND 	B220_OPA_CVE = %d \n"
                              "AND 	B220_OPA_ID_FLUJO = %d \n"
                              , st_B220SegOper.b220_opa_cve
                              , sMaxIDFlujo);

            ptr_void = consultaSimple(consulta);
            if(ptr_void != NULL)
            {
                strcpy(st_B220SegOper.b220_opa_cve_estado, (char *)ptr_void);
            }
            else
            {
                printf("<$ - GENSEGXH> Error en funcion consultaSimple para query:\n");
                printf("----------------------------------------------------------\n");
                printf("%s", consulta);
                printf("----------------------------------------------------------\n");
                return ERROR;
            }

            /* OBTENER TIMESTAMP DEL FOLIO */
            memset(&llArrTimeStamp, '\0', sizeof(llArrTimeStamp));

            sprintf(consulta, "  SELECT  B220_OPA_ID_FLUJO\n"
                                ", B220_OPA_JUL_TIME_STAMP\n"
                                "  FROM    B220_SEG_OPER\n"
                                "  WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'\n"
                                "  AND     B220_OPA_CVE = %d \n"
                                , st_B220SegOper.b220_opa_cve);

            if(getAllTimeStampXFol(llArrTimeStamp, consulta) != SUCCESS)
            {
                printf("<$ - GENSEGXH> Error en funcion getAllTimeStampXFol...\n");
            }

            if(llArrTimeStamp[1] != 0 && llArrTimeStamp[2] != 0 && llArrTimeStamp [3] != 0)
            {
                if(llArrTimeStamp[9] != 0 && llArrTimeStamp[1] != 0)
                {
                    dSegundos = (double)(llArrTimeStamp[9] - llArrTimeStamp[1]);
                }

                fprintf(pf_destino, "%011d\t%8d\t%5d\t%5d\t%18.2lf\t%9d\t[%-31s]\t[%20s]\t%7s\t%06lld\t%18lld\t"
                                    "%18lld\t%18lld\t%18lld\t%18lld\t%18lld\t%18lld\t%18lld\t%18lld\t%18lld\t%18lld\t%17.3lf\n"
                                    , st_B220SegOper.b220_opa_cve, st_B220SegOper.b220_opa_cve_tpa
                                    , st_B220SegOper.b220_opa_cve_med, st_B220SegOper.b220_opa_cve_inst
                                    , st_B220SegOper.b220_opa_mto, st_B220SegOper.b220_opa_num_cve_ben
                                    , st_B220SegOper.b220_opa_cve_rastreo, st_B220SegOper.b220_opa_des_cta_ben
                                    , st_B220SegOper.b220_opa_cve_estado, llArrTimeStamp[0], llArrTimeStamp[1]
                                    , llArrTimeStamp[2], llArrTimeStamp[3], llArrTimeStamp[4], llArrTimeStamp[5]
                                    , llArrTimeStamp[6], llArrTimeStamp[7], llArrTimeStamp[8], llArrTimeStamp[9]
                                    , llArrTimeStamp[10], llArrTimeStamp[11], dSegundos);
                llContFolios++;
            }
        }

        if(sqlite3_finalize(stmt) != SQLITE_OK)
        {
            printf("<$ - GENSEGXH> Error en funcion finalize de stmt\n");
            return ERROR;
        }
    }
    else
    {
        printf("<$ - GENSEGXH> No hay registros en la B220_SEG_OPER\n");
    }

    fprintf(pf_destino, "TDR: %lld", llContFolios);

    if(sqlite3_close(db) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Falla al cerrar la base de datos\n");
        return ERROR;
    }
    else
    {
        printf("<$ - GENSEGXH> Se cierra base de datos\n");
    }

    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : getFoliosIDFlujo3                                   */
/* Descripcion             :                                                     */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short getFoliosIDFlujo3(stB220SegOper *st_B220SgOp, char *cConsuIDFluj3)
{
    sqlite3_stmt    *st_IDflujo3 = NULL;

    sprintf(cConsuIDFluj3, "SELECT  B220_OPA_CVE_TPA "
                           "      , B220_OPA_CVE_MED "
                           "      , B220_OPA_CVE_INST "
                           "      , CAST((B220_OPA_MTO) AS FLOAT (54)) "
                           "      , B220_OPA_NUM_CVE_BEN "
                           "      , B220_OPA_CVE_RASTREO "
                           "      , B220_OPA_DES_CTA_BEN "
                           "FROM    B220_SEG_OPER "
                           "WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R' "
                           "AND     B220_OPA_CVE = %d "
                           "AND     B220_OPA_ID_FLUJO = 3 "
                           , st_B220SegOper.b220_opa_cve);
                          

    if(sqlite3_prepare_v2(db, cConsuIDFluj3, -1, &st_IDflujo3, NULL) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en SELECT para ID_Flujo 3\n");
        printf("%s\n", sqlite3_errmsg(db));
        return ERROR;
    }

    while(sqlite3_step(st_IDflujo3) != SQLITE_DONE)
    {
        st_B220SgOp->b220_opa_cve_tpa     = (short)sqlite3_column_int(st_IDflujo3, 0);
        st_B220SgOp->b220_opa_cve_med     = (short)sqlite3_column_int(st_IDflujo3, 1);
        st_B220SgOp->b220_opa_cve_inst    = sqlite3_column_int(st_IDflujo3, 2);
        st_B220SgOp->b220_opa_mto         = sqlite3_column_double(st_IDflujo3, 3);
        st_B220SgOp->b220_opa_num_cve_ben = (short)sqlite3_column_int(st_IDflujo3, 4);
        strcpy(st_B220SgOp->b220_opa_cve_rastreo, (char *) sqlite3_column_text(st_IDflujo3, 5));
        strcpy(st_B220SgOp->b220_opa_des_cta_ben, (char *) sqlite3_column_text(st_IDflujo3, 6));
    }

    if(sqlite3_step(st_IDflujo3) != SQLITE_ROW)
    {
        printf("<$ - GENSEGXH> No hay registros en ID_FLUJ0 = 3 para folio %d\n", st_B220SegOper.b220_opa_cve);
    }

    if(sqlite3_finalize(st_IDflujo3) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en funcion finalize de st_IDflujo3\n");
        return ERROR;
    }

    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : getAllTimeStampXFol                                 */
/* Descripcion             :                                                     */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short getAllTimeStampXFol(long long *llArrTimeStamp, char *cConsuAllTimeStamp)
{
    sqlite3_stmt    *st_AllTimeStamp = NULL;

    if(sqlite3_prepare_v2(db, cConsuAllTimeStamp, -1, &st_AllTimeStamp, NULL) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en SELECT para AllTimeStamp...\n");
        printf("<$ - GENSEGXH> %s\n", sqlite3_errmsg(db));
        return ERROR;
    }

    while(sqlite3_step(st_AllTimeStamp) != SQLITE_DONE)
    {
        llArrTimeStamp[sqlite3_column_int(st_AllTimeStamp, 0)] = (long long) sqlite3_column_int64(st_AllTimeStamp, 1);
    }

    if(sqlite3_finalize(st_AllTimeStamp) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en funcion finalize de st_AllTimeStamp\n");
        return ERROR;
    }

    return SUCCESS;
}

/*********************************************************************************/
/* Nombre de la funcion    : consultaSimple                                      */
/* Descripcion             :                                                     */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* void *                  :: Dato respuesta de SELECT                           */
/*                            En caso error regresa puntero NULL                 */
/*                                                                               */
/*********************************************************************************/
void *consultaSimple(char *cQuery)
{
    void *prtVoid;
    double auxDouble = 0.0;
    int auxInt = 0.0;
    char auxBuffer[150];
    sqlite3_stmt    *st_Consulta = NULL;

    if(sqlite3_prepare_v2(db, cQuery, -1, &st_Consulta, NULL) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en sqlite3_prepare\n");
        printf("%s\n", sqlite3_errmsg(db));
        prtVoid = NULL;
    }

    sqlite3_step(st_Consulta);

    switch(sqlite3_column_type(st_Consulta, 0))
    {
        case SQLITE3_TEXT:
            strcpy(auxBuffer, (char *) sqlite3_column_text(st_Consulta, 0));
            prtVoid = &auxBuffer;
            break;

        case SQLITE_INTEGER:
            auxInt = sqlite3_column_int(st_Consulta, 0); 
            prtVoid = &auxInt;
            break;

        case SQLITE_FLOAT:
            auxDouble = sqlite3_column_double(st_Consulta, 0);
            prtVoid = &auxDouble;
            break;
    }

    if(sqlite3_finalize(st_Consulta) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en funcion finalize\n");
        prtVoid = NULL;
    }
    return prtVoid;
}

/*********************************************************************************/
/* Nombre de la funcion    : consultaCompuesta                                   */
/* Descripcion             :                                                     */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: Estatus de la funcion                              */
/*                                                                               */
/*********************************************************************************/
short consultaCompuesta(char *cConsuAllTimeStamp)
{
    sqlite3_stmt    *st_AllTimeStamp = NULL;

    if(sqlite3_prepare_v2(db, cConsuAllTimeStamp, -1, &st_AllTimeStamp, NULL) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en SELECT para AllTimeStamp...\n");
        printf("<$ - GENSEGXH> %s\n", sqlite3_errmsg(db));
        return ERROR;
    }

    while(sqlite3_step(st_AllTimeStamp) != SQLITE_DONE)
    {
        llArrTimeStamp[sqlite3_column_int(st_AllTimeStamp, 0)] = (long long) sqlite3_column_int64(st_AllTimeStamp, 1);
    }

    if(sqlite3_finalize(st_AllTimeStamp) != SQLITE_OK)
    {
        printf("<$ - GENSEGXH> Error en funcion finalize de st_AllTimeStamp\n");
        return ERROR;
    }

    return SUCCESS;
}