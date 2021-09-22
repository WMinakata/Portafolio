/*********************************************************************************/
/* Nombre del proyecto : PRACTICA DE SEGUNDERO CON SQLITE                        */
/*********************************************************************************/
/* Identificador       : GENSEGXH.h                                              */
/* Descripcion         : Simula la generacion del segundero del SPEI             */
/*                     :                                                         */
/* Autor               : Josue Alejandro Aguilar Moreno (JAAM)                   */
/* Fecha de creacion   : 28/06/21                                                */
/* Bitacora de Modificaciones:                                                   */
/* ============================                                                  */
/*   Fecha      Version       Autor                     Descripcion              */
/*   =====      =======       =====                     ===========              */
/* 28-JUN-21      1.0       Josue Aguilar       1.- Version inicial              */
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

#include <sqlite3.h>

/*********************************************************************************
                        DEFINICIONES
**********************************************************************************/

#define  SUCCESS         0
#define  ERROR        (-1)
#define  TRUE            1
#define  FALSE           0

/*********************************************************************************
                        ESTRUCTURAS
**********************************************************************************/
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

/*********************************************************************************
                        PROTOTIPOS
**********************************************************************************/

short generaReporteRecibidas(FILE *);
short cuentaRegistrosTotales(long long *, char *);
short getFoliosIDFlujo3(stB220SegOper *, char *);
short getMaxID(stB220SegOper *, char *);
void *consultaSimple(char *);
short getAllTimeStampXFol(long long *, char *);
/* void vdFormatT0HHMMSS(long long llTimeStamp, char *strFormat); */