/*********************************************************************************
                        LIBRERIAS DEL APLICATIVO
**********************************************************************************/
#include <operaciones.h>

/*********************************************************************************/
/* Nombre de la funcion    : main                                                */
/* Descripcion             : Funcion principal para generar el archivo de        */
/*                           operaciones                                         */
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
    validaAmbiente();
    short sErrorArmado = 0;
    FILE *pFichOperaciones = NULL;

    if((pFichOperaciones = fopen(ENV_GEN_OPERS_DIR_CONF, "w")) == NULL)
    {
        printf("No se pudo abrir el archivo \n%s\n", ENV_GEN_OPERS_DIR_CONF);
        return ERROR;
    }

    sErrorArmado = llenafichero(pFichOperaciones);

    if(!sErrorArmado)
    {
        fflush(pFichOperaciones);
    }
    fclose(pFichOperaciones);

    if(sErrorArmado == SUCCESS)
    {
        printf("Operaciones termina con exito\n");
        return SUCCESS;
    }
    else
    {
        printf("Operaciones termina con error\n");
        return ERROR;
    }
}

/*********************************************************************************/
/* Nombre de la funcion    : validaAmbiente                                      */
/* Descripcion             : Funcion que carga variables del entorno             */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/*********************************************************************************/
void validaAmbiente(void)
{
    FILE    *fenv = NULL; /* Archivo de variables de ambiente */
    char    strLineas[250];
    char    strNombVar[30];
    char    strValor[220];

    if((fenv = fopen(ENVFILE_DEFAULT, "r")) != NULL)
    {
        while(!feof(fenv))
        {
            memset(strLineas, '\0' ,sizeof(strLineas));
            fgets(strLineas, sizeof(strLineas), fenv);

            if((strlen(strLineas)) < 2)
            {
                continue;
            }
            strLineas[strcspn(strLineas, "\n")] = '\0';
            if(strstr(strLineas,"#"))
            {
                continue;
            }
            /* strLineas[strcspn(strLineas, "\n")] = 0; */
            strcpy(strNombVar, strtok(strLineas, ";"));
            strcpy(strValor, strtok(NULL, ";"));
            setenv(strNombVar, strValor, 1);
        }
        fclose(fenv);
    }
    else
    {
        printf("No hay archivo de ambiente:\n");
        printf("%s",ENVFILE_DEFAULT);
    }
    
    /* Setea variables default en caso de no existir archivo de ambiente */
    if(ENV_GEN_OPERS_DIR_CONF == NULL || strlen(ENV_GEN_OPERS_DIR_CONF) == 0)
    {
        putenv("ENV_GEN_OPERS_DIR_CONF=/home/wminakata/Documentos/Proyectos/DevC/usaPostgres/segundero/src/catOpers.txt");
    }

    if(CANT_OPERS == NULL || strlen(CANT_OPERS) == 0)
    {
        putenv("CANT_OPERS=2");
    }

    if(MAX_ID == NULL || strlen(MAX_ID) == 0)
    {
        putenv("MAX_ID=2");
    }
}

/*********************************************************************************/
/* Nombre de la funcion    : llenafichero                                        */
/* Descripcion             : Funcion que carga variables del entorno             */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/*                                                                               */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* void                    ::                                                    */
/*                                                                               */
/*********************************************************************************/
short llenafichero(FILE *pf_destino)
{
    long        lFolios = 1;
    stOpers     st_operaciones;
    time_t      hora;
    short       sError = 0;
    int         iMaxOp = 0;
    short       sMaxID = 0;

    iMaxOp = atoi(getenv("CANT_OPERS"));
    sMaxID = (short)atoi(getenv("MAX_ID"));

    printf("cant_opers=%d\n",iMaxOp);
    printf("max_id=%d\n",sMaxID);

    memset((char *) &st_operaciones, '\0' ,sizeof(st_operaciones));

    if(pf_destino == NULL)
    {
        printf("Error con el archivo\n");
        sError = ERROR;
    }

    strcpy(st_operaciones.opa_cve_tipo_orden, "R");
    st_operaciones.opa_cve_tpa = 4;
    st_operaciones.opa_cve_inst = 40002;
    st_operaciones.opa_mto = 240.4;
    st_operaciones.opa_cve_med = 1;
    strcpy(st_operaciones.opa_cve_rastreo, "12340987651234567");
    st_operaciones.opa_num_cve_ben = 40;
    strcpy(st_operaciones.opa_des_cta_ben, "Pagos");

    while(lFolios <= iMaxOp)
    {
        st_operaciones.opa_cve = lFolios;

        int iFlujo;
        for(iFlujo = 0; iFlujo < sMaxID; iFlujo++)
        {
            memset((time_t *) &hora, '\0', sizeof(hora));
            st_operaciones.opa_id_flujo = iFlujo;
            hora = time(NULL);
            st_operaciones.opa_jul_time_stamp = (long long)hora;
            if(iFlujo == (sMaxID -1))
            {
                strcpy(st_operaciones.opa_cve_estado, " L");
            }
            else
            {
                strcpy(st_operaciones.opa_cve_estado, "EX");
            }
            fprintf(pf_destino, "%s;%ld;%d;%lld;%s;%d;%d;%.2lf;%d;%s;%d;%s\n",
                     st_operaciones.opa_cve_tipo_orden
                    ,st_operaciones.opa_cve
                    ,st_operaciones.opa_id_flujo
                    ,st_operaciones.opa_jul_time_stamp
                    ,st_operaciones.opa_cve_estado
                    ,st_operaciones.opa_cve_med
                    ,st_operaciones.opa_cve_inst
                    ,st_operaciones.opa_mto
                    ,st_operaciones.opa_cve_tpa
                    ,st_operaciones.opa_cve_rastreo
                    ,st_operaciones.opa_num_cve_ben
                    ,st_operaciones.opa_des_cta_ben);
        sleep(1);
        }
        lFolios++;
    }

    if(sError == ERROR)
    {
        return ERROR;
    }
    else
    {
        return SUCCESS;
    }
}

/*********************************************************************************/
/* Nombre de la funcion    : _Fgets                                              */
/* Descripcion             : Funcion que elimina el 0x0d = Retorno de linea      */
/*                           y elimina el 0x0a = Avance de linea LF al final de  */
/*                           una cadena.                                         */
/*                                                                               */
/* Autor                   : Josue Alejandro Aguilar Moreno (JAAM)               */
/*                                                                               */
/* Parametros de entarada  :                                                     */
/* char *cStr              :: Puntero a cadena que se opera                      */
/* int iLeng               :: longitud de la linea                               */
/* FILE *pf                :: Puntero a fichero de lectura                       */
/*                                                                               */
/* Parametros de salida    :                                                     */
/* char *                  :: Resultado de la operacion                          */
/*                                                                               */
/*********************************************************************************/
char *_Fgets(char *cStr, int iLeng, FILE *pf)
{
    char *p1;

    if(fgets(cStr, iLeng, pf))
    {
        return NULL;
    }

    if((p1=strchr(cStr,0x0d)) != NULL)
    {
        *p1=0;
    }

    if((p1=strchr(cStr,0x0a)) != NULL)
    {
        *p1=0;
    }
    return cStr;
}