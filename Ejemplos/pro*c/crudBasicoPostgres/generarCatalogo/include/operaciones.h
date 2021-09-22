#ifndef OPERACIONES_H
/*********************************************************************************/
/* Nombre del proyecto : CRUD sql embebido                                       */
/*********************************************************************************/
/* Identificador       : operaciones.c                                           */
/* Descripcion         :                                                         */
/*                     :                                                         */
/* Autor               : Josue Alejandro Aguilar Moreno (JAAM)                   */
/* Fecha de creacion   : 06/08/21                                                */
/* Bitacora de Modificaciones:                                                   */
/* ============================                                                  */
/*   Fecha      Version       Autor                     Descripcion              */
/*   =====      =======       =====                     ===========              */
/* 06-AGO-21      1.0       Josue Aguilar       1.- Version inicial              */
/*                              (JAAM)                                           */
/*                                                                               */
/*********************************************************************************/

/*********************************************************************************
                        LIBRERIAS ESTANDAR
**********************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

/*********************************************************************************
                        DEFINICIONES
**********************************************************************************/

#define  SUCCESS         0
#define  ERROR        (-1)
#define  TRUE            1
#define  FALSE           0

#define  ENVFILE_DEFAULT            "/home/wminakata/Documentos/Proyectos/DevC/usaPostgres/segundero/src2/src/ENVFILE-OPERS"
#define  ENV_GEN_OPERS_DIR_CONF     getenv("ENV_GEN_OPERS_DIR_CONF")
#define  CANT_OPERS                 getenv("CANT_OPERS")
#define  MAX_ID                     getenv("MAX_ID")

/*********************************************************************************
                        ESTRUCTURAS
**********************************************************************************/
typedef struct
{
    char        opa_cve_tipo_orden[0 + 1];
    long        opa_cve;
    short       opa_id_flujo;
    long long   opa_jul_time_stamp;
    char        opa_cve_estado[2 + 1];
    short       opa_cve_med;
    int         opa_cve_inst;
    double      opa_mto;
    short       opa_cve_tpa;
    char        opa_cve_rastreo[30 + 1];
    short       opa_num_cve_ben;
    char        opa_des_cta_ben[20 + 1];
} stOpers;


/*********************************************************************************
                        PROTOTIPOS
**********************************************************************************/

void  validaAmbiente(void);
short llenafichero(FILE *pf_destino);
char *_Fgets(char *cStr, int iLeng, FILE *pf);
#endif