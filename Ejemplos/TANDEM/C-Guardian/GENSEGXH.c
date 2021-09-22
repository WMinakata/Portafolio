/*********************************************************************************/
/* Nombre del proyecto :P0133853 Optimizacion MCO Procesamiento Ordenes Incoming */
/*********************************************************************************/
/* Identificador       : GENSEGXH                                                */
/* Descripcion         : Genera archivo segundero con los datos de las           */
/*                     : operaciones recibidas de BANXICO por hora               */
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
                        LIBRERIAS DE GUARDIAN
**********************************************************************************/

#include <cextdecs(DELAY)>
#include <cextdecs(FILE_OPEN,FILE_GETINFO_,WRITEX,FILE_CLOSE_)> nolist
#include <cextdecs(COMPUTETIMESSTAMP)>
#include <cextdecs(INTERPRETETIMESTAMP)>

/*********************************************************************************
                        LIBRERIAS DE BASE DE DATOS
**********************************************************************************/

#include <sql.h>
#include <cextdecs(SQLCATOBUFFER)>
EXEC SQL include sqlca;

/*********************************************************************************
                        DEFINICIONES
**********************************************************************************/

#define  SQLNOTFOUND   100
#define  SQLEXITO        0
#define  SUCCESS         0
#define  ERROR        (-1)
#define  TRUE            1
#define  FALSE           0
#define  MICROSEGUNDOS 1000000

/*********************************************************************************
                        PROTOTIPOS
**********************************************************************************/

void vdLimpiaErrorSQL(char *strMsg);
int  iSQLStat(char *cFunc, char *cFile, long lLine, char *strMsg);
short consultaBitacoraREcibidas(long long ll_TStampIni, long long ll_TStampFin);
void vdImpFechaHora(void);
void vdObtenFechGregoriana(short *sFGregoriana);
void vdFormatT0HHMMSS(long long llTimeStamp, char *strFormat);

/*********************************************************************************
                        VARIABLES GLOBALES
**********************************************************************************/

short   sqlcode;
char    strBuffer[3550];
short   sFileIn = 0;
short   sError = 0;
char    strNombArchWRITEX[250];
char    sLenghFile = 0;

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
    char    strPathFile[250];   /* Ruta del fichero */
    short   sFinEjec = 0;       /* Estado final de la ejecucion */
    short   sStatusFunc = 0;    /* Estado de la funcion */
    short   *pFechaGrego;       /* Puntero a vector sFechGregoriana */
    short   s_error = 0;        /* Estatud de la funcion COMPUTETIMESTAMP */
    short   sFechaGregoriana[7] /* Vector que contiene la fecha y hora gregoriana */
                                /* [0] the Gregorian year (for example, 1984, 1985, ...)
                                   [1] the Gregorian month (1-12)
                                   [2] the Gregorian day of the month (1-31)
                                   [3] the hour of the day (0-23)
                                   [4] the minute of the hour (0-59)
                                   [5] the second of the minute (0-59)
                                   [6] the millisecond of the second (0-999)
                                   [7] the microsecond of the millisecond (0-999) */

    EXEC SQL BEGIN DECLARE SECTION;
        long long   ll_MaxFolio = 0;      /* Cantidad maxima de folios */
        long long   ll_TStampIni = 0;     /* Timestamp de inicio */
        long long   ll_TStampFin = 0;     /* Timestamp final */
        short       sDataset = 0;         /* Dataset que se esta usando */
        char        strB40DeviceFile[25]; /* Ruta de la tabla segun dataset */
    EXEC SQL END DECLARE SECTION;

    printf("<$ - GENSEGXH> ----- INICIA PROGRAMA GENSEGXH -----\n");
    vdImpFechaHora();

    /* Se obtiene dataset */
    sDataset = (short) atoi(argv[2]);
    memset((char *) &strB40DeviceFile, '\0', sizeof(strB40DeviceFile));

    /* Se obtiene el nombre de la tabla por dataset */
    EXEC SQL
        SELECT B41_DEVICE_FILE
        INTO :strB40DeviceFile
        FROM =B41-DEFINES
        WHERE B41_DEF_DATASET = :sDataset
        AND B41_DEFINE_NAME = "B220-SEG-OPER"
    BROWSE ACCESS;

    /* Setea vector para fecha gregoriana */
    memset(&sFechaGregoriana, 0, sizeof(sFechaGregoriana));
    pFechaGrego = sFechaGregoriana;

    /* Llenar vector con la fecha y hora */
    vdObtenFechGregoriana(pFechaGrego);

    /* Se calcula timestamp final */
    ll_TStampFin = COMPUTETIMESTAMP(pFechaGrego, &s_error);
    if(s_error != 0)
    {
        printf("ERROR en funcion COMPUTETIMESTAMP [%d]\n", s_error);
    }

    /* Se resta 3600000000 microsegundos al timestamp final para determinar el timestamp inicial */
    /* 3600000000 = 1 hr */
    ll_TStampIni = ll_TStampIni - 3600000000;

    INTERPRETETIMESTAP(ll_TStampIni, pFechaGrego);
    printf("Se procesan operaciones de %d:00 a ", sFechaGregoriana[3]);
    INTERPRETETIMESTAP(ll_TStampFin, pFechaGrego);
    printf("%d:00 hrs.\n", sFechaGregoriana[3]);

    EXEC SQL
        SELECT COUT(DISTINCT B220_OPA_CVE)
        INTO   :ll_MaxFolio
        FROM   =B220-SEG-OPER
        WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'
        AND     B220_OPA_ID_FLUJO = 1
        AND     B220_OPA_JUL_TIME_STAMP >= :ll_TStampIni
        AND     B220_OPA_JUL_TIME_STAMP <  :ll_TStampFin
        ORDER BY B220_OPA_CVE ASC
    BROWSE ACCESS;

    if(ll_MaxFolio > 0)
    {
        strcpy(strPathFile, argv[1])
        if(strPathFile == NULL)
        {
            printf("Argumeto invalido ---> \"Ruta archivo\"...\n");
            return ERROR;
        }

        printf("Generando el reporte de operaciones recibididas...\n");

        FILE_OPEN(strPathFile, (short) strlen(strPathFile), &sFileIn, 0);
        FILE_GETINFO_(sFileIn, &sError, strNombArchWRITEX, sizeof(strNombArchWRITEX), &sLenghFile);
        
        if(sError)
        {
            printf("Archivo [%s] no se pudo abrir\n", strPathFile);
            return ERROR;
        }

        memset(strBuffer, '\0', sizeof(strBuffer));

        /* Escribe header del archivo segundero */
        sprintf(strBuffer,
                "%6 Folio\tTipoPago\tMedio\tBanco\t%11 Importe\tCuentaBen\t17 Clave de Rastreo"
                "\t%7 Cuenta abono\tEstatus\t%4 T0\t%17T1\t%17T2\t%17T3\t%17T4\t%17T5\t%17T6\t%17T7"
                "\t%17T8t%17T9t%16T10t%16T11t%4 Total T9 - T1\n");

        WRITEX(sFileIn, strBuffer, (unsigned short) strlen(strBuffer));
        FILE_GETINFO_(sFileIn, &sError, strNombArchWRITEX, sizeof(strNombArchWRITEX), &sLenghFile);
        
        if(sError)
        {
            printf("Error al escribir cabecera. Error #%d\n", sError);
            return ERROR;
        }

        sStatusFunc = consultaBitacoraREcibidas(ll_TStampIni, ll_TStampFin);
    }
    else
    {
        printf("No existe informacion para %s\n", strB40DeviceFile);
        sStatusFunc = SUCCESS;
    }

    if(sStatusFunc == SUCCESS)
    {
        printf("Termina con EXITO...\n");
        sFinEjec = SUCCESS;
    }
    else
    {
        printf("Termina con ERROR...\n");
        sFinEjec =  ERROR;
    }

    FILE_CLOSE_(sFileIn);
    vdImpFechaHora();
    printf("TERMINA PROGRAMA GENSEGXH\n");
    return sFinEjec;
}

/*********************************************************************************/
/* Nombre de la funcion    : consultaBitaRecibidas                               */
/* Descripcion             : Funcion para llenado del archivo segundero en base  */
/*                           a la lectura de la B220-SEG-OPER                    */
/*                                                                               */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* long long ll_TStampIni  :: Timestap de inicio                                 */
/* long long ll_TStampFin  :: Timestamp final                                    */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* short                   :: estado de la funcion                               */
/*                                                                               */
/*********************************************************************************/
short consultaBitacoraREcibidas(long long ll_TStampIni, long long ll_TStampFin)
{
    char        strMsg[2000];       /* Mensaje que se envia en caso de error */
    long long   llContFolios = 0;   /* Contador de registros */
    long long   llTimeStamp[12];    /* Ventor que contiene los timestamp */
    long long   llTimeTotal = 0;    /* Resultado de T9 - T1 */
    short       sErrorResp = TRUE;  /* Respuesta del estatus de la funcion */
    short       sSQLCode_folio = 0; /* Estado del cursor de folios */
    double      dTStampSeg;         /* Segundos con milisegindos del sistema */

    EXEC SQL BEGIN DECLARE SECTION;
        EXEC SQL INVOKE=B220-SEG-OPER AS b220sgop_type;
        struct      b220sgop_type stB220SegOper;
        double      dMonto = 0.0;
        short       sMaxIDFlujo = 0;
        long long   llMaxFolio = 0;
        long long   llTStampIni = 0;
        long long   llTStampFin = 0;
    EXEC SQL END DECLARE SECTION;

    /* Se obtiene folio de la operacion */
    EXEC SQL DECLARE REGB220FOL CURSOR FOR
        SELECT  DISTINCT B220_OPA_CVE
        FROM   =B220-SEG-OPER
        WHERE   B220_OPA_CVE >= 1
        AND     B220_OPA_INFLUJO = 1
        AND     B220_OPA_JUL_TIME_STAMP >= :ll_TStampFin
        AND     B220_OPA_JUL_TIME_STAMP <  :ll_TStampIni
        ORDER BY B220_OPA_CVE  ASC
    BROWSE ACCESS;

    EXEC SQL OPEN REGB220FOL;

    if(sqlstat("ERROR EN OPEN CURSOR REGB220FOL",__FILE__, __LINE__, strMsg))
    {
        printf("Error en cursor REGB220FOL !!\n");
        sErrorResp = FALSE;
    }

    EXEC SQL
        SELECT  COUNT(DISTINCT B220_OPA_CVE)
        INTO    :llMaxFolio
        FROM   =B220-SEG-OPER
        WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'
        AND     B220_OPA_ID_FLUJO = 1
        AND     B220_OPA_JUL_TIME_STAMP >= :llTStampIni
        AND     B220_OPA_JUL_TIME_STAMP >= :ll_TStampFin
        ORDER BY B220_OPA_CVE ASC
    BROWSE ACCESS;

    if(llMaxFolio > 0)
    {
        llContFolios = 0;

        while(sqlcode == SQLEXITO && sErrorResp == TRUE)
        {
            memset((char *)&stB220SegOper, '\0', sizeof(stB220SegOper));

            EXEC SQL FETCH REGB220FOL INTO
                :stB220SegOper.b220_opa_cve;

            if(sqlstat("ERROR EN FETCH REGB220FOL", __FILE__, __LINE__, strMsg))
            {
                printf("Error en fetch");
                sErrorResp = FALSE;
            }

            sSQLCode_folio = sqlcode;
            dMonto = 0.0;

            if(sqlcode = SQLEXITO)
            {
                /* Se obtienen los registros del folio en base a ID_FLUJO = 3 */
                EXEC SQL
                    SELECT  B220_OPA_CVE_TPA
                           ,B220_OPA_CVE_MED
                           ,B220_OPA_CVE_INS
                           ,CAST((B220_OPA_MTO) AS FLOAT(54))
                           ,B220_OPA_NUM_CVE_BEN
                           ,B220_OPA_CVE_RASTREO
                           ,B220_OPA_DES_CTA_BEN
                    INTO    :stB220SegOper.b220_ops_cve_tpa
                           ,:stB220SegOper.b220_opa_cve_med
                           ,:stB220SegOper.b220_opa_cve_ins
                           ,:dMonto
                           ,:stB220SegOper.b220_opa_num_cve_ben
                           ,:stB220SegOper.b220_opa_cve_rastreo
                           ,:stB220SegOper.b220_opa_des_cta_ben
                    FROM   =B220-SEG-OPER
                    WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'
                    AND     B220_OPA_CVE = :stB220SegOper.b220_opa_cve
                    AND     B220_OPA_ID_FLUJO = 3
                BROWSE ACCESS;

                if(sqlcode == SQLNOTFOUND)
                {
                    printf("No se encontraron registros para ID_LUJO = 3 en folio %ld\n", stB220SegOper.b220_opa_cve);
                }

                sMaxIDFlujo = 0;
                /* Se obtiene el maximo ID_FLUJO del folio */
                EXEC SQL
                    SELECT  MAX(B220_OPA_ID_FLUJO)
                    INTO    :sMaxIDFlujo
                    FROM   =B220-SEG-OPER
                    WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'
                    AND     B220_OPA_CVE = :stB220SegOper.b220_opa_cve
                BROWSE ACCESS;

                if(sqlcode == SQLNOTFOUND)
                {
                    printf("No se encontraron registros para MAX(ID_FLUJO) del folio %ld\n", stB220SegOper.b220_opa_cve);
                }

                /* Obtiene el ultimo estado del folio */
                EXEC SQL
                    SELECT  B220_OPA_CVE_ESTADO
                    INTO    :stB220SegOper.b220_opa_cve_estado
                    FROM   =B220-SEG-OPER
                    WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'
                    AND     B220_OPA_CVE = :stB220SegOper.b220_opa_cve
                    AND     B220_OPA_ID_FLUJO = :sMaxIDFlujo
                BROWSE ACCESS;

                if(sqlcode == SQLNOTFOUND)
                {
                    printf("No se encontraron registros para el ultimo estado del folio %ld", stB220SegOper.b220_opa_cve);
                }

                /* Obtiene los timestamp del folio */
                EXEC SQL DECLARE REGB220TSTAP CURSOR FOR
                    SELECT  B220_OPA_ID_FLUJO
                           ,B220_OPA_JUL_TIME_STAMP
                    FROM   =B220-SEG-OPER
                    WHERE   B220_OPA_CVE_TIPO_ORDEN = 'R'
                    AND     B220_OPA_CVE = :stB220SegOper.b220_opa_cve
                BROWSE ACCESS;

                EXEC SQL OPEN REGB220TSTAMP;

                if(sqlstat("ERROR EN OPEN CURSOR REGB220TSTAMP", __FILE__, __LINE__, strMsg));
                {
                    sErrorResp = FALSE; 
                }

                memset(&llTimeStamp, 0, sizeof(llTimeStamp));

                while(sqlcode == SQLEXITO)
                {
                    EXEC SQL FETCH REGB220TSTAMP INTO
                        :stB220SegOper.b220_opa_id_flujo
                       ,:stB220SegOper.b220_opa_jul_time_stamp;

                       if(sqlstat("Error en FETCH REGB220TSTAMP", __FILE__, __LINE__, strMsg))
                       {
                           printf("ERROR EN FETCH REGB220TSTAMP ...!!\n");
                           sErrorResp = FALSE;
                       }

                       if(sqlcode == SQLEXITO)
                       {
                           if(stB220SegOper.b220_opa_jul_time_stamp != 0)
                           {
                               llTimeStamp[stB220SegOper.b220_opa_id_flujo] = stB220SegOper.b220_opa_jul_time_stamp;
                           }
                       }
                } /* while(sqlcode == SQLEXITO) REGB220TSTAMP*/

                EXEC SQL CLOSE REGB220TSTAMP;

                if(sqlstat("Error en close de REGB220TSTAMP",__FILE__,__LINE__, strMsg))
                {
                    printf("Error en close de cursor REGB220TSTAMP...\n");
                    sErrorResp = FALSE;
                }

                llTimeTotal = 0;
                dTStampSeg = 0.0;

                /* Se omiten registros que tengan T1, T2, T3 en cero */
                if(llTimeStamp[1] != 0  && llTimeStamp[2] != 0 && llTimeStamp[3 != 0])
                {
                    /* Calcula el campo T9 - T1 */
                    if(llTimeStamp[9 != 0])
                    {
                        llTimeTotal = llTimeStamp[9] - llTimeStamp[1];
                        dTStampSeg = (double) llTimeTotal / MICROSEGUNDOS;
                    }

                    sprintf();
                }
            }
        }
    }
}

/*********************************************************************************/
/* Nombre de la funcion    : vdImpFechaHora                                      */
/* Descripcion             : Determina la fecha del sistema en formato AAAAMMDD  */
/*                           y la hora en formato HH:MM y la imprime             */
/*                                                                               */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/*********************************************************************************/
void vdImpFechaHora(void)
{
    char    cFecha[12];
    char    cHora[7];
    time_t  rawtime =  time(0);
    struct  tm *tiempo = localtime(&rawtime);
    strftime(cFecha, 11, "%Y/%m/%d", tiempo);
    strftime(cHora, 6, "%H:%M", tiempo);
    printf("Fecha: [%s]",cFecha);
    printf("Hora: [%s]",cHora);
}

/*********************************************************************************/
/* Nombre de la funcion    : vdObtenFechGregoriana                               */
/* Descripcion             : Determina la fecha del sistema en formato AAAAMMDD  */
/*                           y la hora en formato HH:MM y la imprime             */
/*                                                                               */
/* Autor                   : (JAAM)                                              */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* char  *sFGregoriana     :: Puntero a vector que almacena la fecha y hora      */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/*********************************************************************************/
void vdObtenFechGregoriana(short *sFGregoriana)
{
    time_t  rawtime = time(0);
    struct tm *tiempo = localtime(&rawtime);
    sFGregoriana[0] = (short) tiempo->tm_year + 1900;
    sFGregoriana[1] = (short) tiempo->tm_mon + 1;
    sFGregoriana[2] = (short) tiempo->tm_mday;
    sFGregoriana[3] = (short) tiempo->tm_hour;
}