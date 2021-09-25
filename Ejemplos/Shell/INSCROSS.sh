######################################################################
##            SISTEMA DE PAGOS ELECTRONICOS INTERBANCARIOS          ##
######################################################################
##  Identificacion  : INSCROSS.sh                                   ##
##  Descripcion: Shell de instalacion / desinstalacion de CR's      ##
##               generico                                           ##
##                                                                  ##
##     Autor:    Josue Alejandro Aguilar Moreno (JAAM)              ##
##  Creacion:    27 de Enero de 2021                                ##
##                                                                  ##
## Bitacore de modificaciones:                                      ##
## ============================                                     ##
##                                                                  ##
##   Fecha      Versión      Autor          Descripción             ##
##   =====      =======      =====          ===========             ##
## 27-ENE-21      1.0       (JAAM)    1.- Versión inicial del shell ##
##                                                                  ##
######################################################################
## Sintaxis ::                                                      ##
##           sh INSCROSS.sh   <CR> <Opcion> <PIEZA> <TIPO> donde    ##
##              <CR>     = I, D, VI, VD                             ##
##              <Opcion> = Numero de CR a 7 posiciones              ##
##              <PIEZA>  = Nombre de la pieza                       ##
##               <TIPO>  = OBJ, SHELL, CONF                         ##
##                                                                  ##
######################################################################


######################################################################
##  Nombre de la funcion : msjSintaxis                              ##
##  Descripcion          : Imprime los requisitos que se requieren  ##
##                                                                  ##
##  Autor                : (JAAM)                                   ##
##  Parametros           :                                          ##
######################################################################
msjSintaxis()
{
    rutinaPresentacion
    echo "(S622-180) PARAMETRO INVALIDO..."
    echo "(S622-180) SISTAXIS:"
    echo "(S622-180) sh INSCROSS.sh <CR> <Opcion> <PIEZA> <TIPO>"
    echo ""
    echo "(S622-180) DONDE:"
    echo ""
    echo "(S622-180) <CR> ULTIMOS 7 DIGITOS DEL NUMERO DEL CAMBIO"
    echo ""
    echo "(S622-180) <OPCION> PUEDE SER:"
    echo "(S622-180)    I = INSTALAR / RESPALDAR"
    echo "(S622-180)    D = DESINSTALAR / RESTAURAR"
    echo "(S622-180)   VI = VALIDA INSTALACION"
    echo "(S622-180)   VD = VALIDA DESINSTALACION"
    echo ""
    echo "(S622-180) <PIEZA> NOMBRE DE LA PIEZA A INSTALAR"
    echo ""
    echo "(S622-180) <TIPO> PUEDE SER:"
    echo "(S622-180)    OBJ = OBJETO TIPO BINARIO"
    echo "(S622-180)  SHELL = OBJETO TIPO SHELL"
    echo "(S622-180)   CONF = OBJETO TIPO CONF O ENV-FILE"
    rutinaPresentacion
}

######################################################################
##  Nombre de la funcion : rutinaCompilaSQL                         ##
##  Descripcion          : Rutina de compilacion de tercera face    ##
##                                                                  ##
##  Autor                : (JAAM)                                   ##
##  Parametros           :                                          ##
######################################################################
rutinaCompilaSQL()
{
    # se busca si el objeto incluye directiva SQL #
    cd $dirInst
    directivaSQL =$(grep SQL-OBJECT $pieza | wc -l)

    if [ $directivaSQL -eq 1];
    then
        echo "(S622-180) OBJETO $pieza CON DIRECTIVA SQL"
        echo "(S622-180) EFECTUANDO 3RA FASE DE COMPILACION"
        cd $dirShell
        TerceraFase3 "$pieza"
    fi
}

######################################################################
##  Nombre de la funcion : rutinaPresentacion                       ##
##  Descripcion          : Agrega linea de separacion               ##
##                                                                  ##
##  Autor                : (JAAM)                                   ##
##  Parametros           :                                          ##
######################################################################
rutinaPresentacion()
{
    echo "(S622-180) ======================================================================"
}


######################################################################
##                      PRINCIPIA PROGRAMA                          ##
######################################################################
shellName=$0
CR=$1
opcion=`echo $2 | tr '[:lower:]'  '[:upper]'`
pieza=$3
tipo=$4
dirTemp=/opt/s622/180/$CR
flagContinua=0
dirShell=/ops/s622/180/tuxedo/shell
ambiente=`uname -n`

rutinaPresentacion
date + 'FECHA DE EJECUCION = %d/%m/%y  %H:%M%S'
rutinaPresentacion
echo "$(who -mT)"
echo "$(id)"

case $ambiente in
    uat03 | uat05 | prodA | prodB)
        case $tipo in
            OBJ )
                dirInst=/opt/s622/180/tuxedo80/bin
                flagContinua=1
                ;;
            SHELL )
                dirInst=/opt/s622/180/tuxedo80/shell
                flagContinua=1
            ;;
            CONF )
                dirInst=/opt/s622/180/tuxedo80/conf
                flagContinua=1
            ;;
            *)
                flagContinua=0
            ;;
        esac
    ;;
    dev )
        case $tipo in
            OBJ )
                dirInst=/opt/s622/180/tuxedo80/bin
                flagContinua=1
                ;;
            SHELL )
                dirInst=/opt/s622/180/tuxedo80/shell
                flagContinua=1
            ;;
            CONF )
                dirInst=/opt/s622/180/tuxedo80/conf
                flagContinua=1
            ;;
            *)
                flagContinua=0
            ;;
        esac
    ;;
    * )
        flagContinua=0
        echo "ESTE EQUIPO [$ambiente] NO SE VALIDA"
        exit 0
    ;;
esac

if [ $flagContinua -eq 1 ];
then
    case $opcion in
        I )
            if [ ! -e $dirTemp/$pieza ];
            then
                rutinaPresentacion
                echo "NO EXISTE LA PIEZA A LIBERAR $pieza"
                rutinaPresentacion
            else
                # seccion para instalacion de nuevas piezas #
                rutinaPresentacion
                echo "INICIA INSTALACION DE CR $CR"
                rutinaPresentacion

                if [ -e $dirInst/$pieza ];
                then
                    echo "DETALLE DE PIEZA $pieza ACTUAL..."
                    cd $dirInst
                    ls -ltr $pieza
                    cksum $pieza

                    if [ "$opcion" -eq "OBJ"];
                    then
                        vproc ${pieza}
                    fi
                    echo "SE RESPALDA PIEZA $pieza ACTUAL..."
                    # Creando directorios de respaldo #
                    [ ! -d ${dirInst}/${CR} ] && echo "CREANDO DIRECTORIO $dirInst/$CR"
                    [ ! -d ${dirInst}/${CR} ] && echo "NO SE PUDO CREAR $dirInst/$CR"
                    chmod 777 ${dirInst}/${CR}
                    cd $dirInst
                    mv $pieza $dirInst/$CR
                fi

                echo "SE INSTALA PIEZA $pieza NUEVA"
                rutinaPresentacion
                cd $dirTemp
                chmod 777 $pieza
                mv $pieza $dirInst

                if [ "$tipo" -eq "OBJ" ];
                then
                    rutinaCompilaSQL
                fi

                cd $dirInst
                echo "DETALLE DE LA PIEZA $pieza INSTALDA..."
                rutinaPresentacion
                ls -ltr $pieza
                cksum $pieza

                if [ "$tipo" -eq "OBJ" ];
                then
                    vproc $pieza
                fi

                echo "TERMINA INSTALACION DE CR: $CR"
                rutinaPresentacion
            fi
        ;;
        D )
            rutinaPresentacion
            echo "INICIO DE DES-INSTALACION DE CR $CR"
            rutinaPresentacion
            if [-e $dirInst/$CR/$pieza ];
            then
                echo "INICIA RESTAURACION DE LA VERSION ANTERIOR"
                echo "DETALLE DE LA PIEZA $pieza EN DIRECTORIO DE RESPALDO"
                cd $dirInst/$CR
                ls -ltr $pieza
                cksum $pieza

                if [ "$tipo" -eq "OBJ" ];
                then
                    vproc $pieza
                if
                echo "REMOVIENDO VERSION LIBERADA EN $dirInst"
                cd $dirInst/$CR
                mv $pieza $dirInst

                if [ ! "$(ls $dirInst/$CR $)" ];
                then
                    cd $dirInst
                    rm -fr $CR
                    if [ $? -ne 0 ];
                    then
                        echo "NO SE PUDO ELIMINAR LA CARPETA $CR"
                    else
                        echo "SE ELININA CARPETA $CR"
                    fi
                fi
                
                if [ "$tipo" -eq "OBJ" ];
                then
                    rutinaCompilaSQL
                fi
                echo "TERMINA LA RESTAURACION DE LA VERSION ANTERIOR"
            else
                if [ -e $dirInst/$pieza ];
                then
                    echo "NO EXISTE LA PIEZA $pieza A RESTAURAR, SE REMUEVE VERSION LIBERADA"
                    cd $dirInst
                    mv $pieza $dirTemp
                fi
            fi
            rutinaPresentacion
            echo "TERMINA DES-INSTALACION DE CR $CR"
            rutinaPresentacion
        ;;
        VI )
            rutinaPresentacion
            echo "INICIA VALIDACION DE INSTALACION DEL CR $CR"
            rutinaPresentacion
            if [ -e $dirInst/$CR/$pieza ];
            then
                echo "DETALLE DE LA PIEZA $pieza RESPALDADA"
                cd $dirInst/$CR
                ls -ltr $pieza
                cksum $pieza

                if [ "$tipo" -eq  "OBJ" ];
                then
                    vproc $pieza
                fi
            fi

            if [ -e $dirInst/$pieza ];
            then
                echo "DETALLE DE LA PIEZA $pieza INSTALADA"
                cd $dirInst
                ls -ltr $pieza
                cksum $pieza

                if [ "$tipo" -eq "OBJ" ];
                then
                    vproc $pieza
                fi
            fi
            rutinaPresentacion
            echo "TERMINA VALIDACION DE INSTALACION DEL CR $CR"
            rutinaPresentacion
        ;;
        VD )
            rutinaPresentacion
            echo "INICIA VALIDACION DE DES-INSTALACION DE CR: $CR"
            rutinaPresentacion

            if [ -e $dirInst/$pieza ];
            then
                echo "DETALLE DE LA PIEZA $pieza RESTAURADA"
                cd $dirInst
                ls -ltr $pieza
                cksum $pieza

                if [ "$tipo" -eq "OBJ" ];
                then
                    vproc $pieza
                fi
            else
                echo "NO EXISTE VERSION PREVIO DE LA PIEZA $pieza, ES PRIMERA INSTALACION"
            fi
            rutinaPresentacion
            echo "TERMINA VALIDACION DE INSTALACION DEL CR: $CR"
            rutinaPresentacion
        ;;
        * )
            msjSintaxis
        ;;
    esac
else
    msjSintaxis
fi

rutinaPresentacion
echo "TERMINA SHELL DE INSTALACION / DES-INSTALACION [$shellName]"
date + 'FECHA: %d/%m/%y'
date + 'HORA: %H:%M:%S'
echo "EQUIPO: [$ambiente]"
rutinaPresentacion