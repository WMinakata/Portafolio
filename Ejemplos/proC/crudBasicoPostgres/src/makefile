############################################################################
#  Identificacion           : makefile                                     #
#  Version                  : 2.0                                          #
#  Numero de proyecto       :                                              #
#  Descripcion              : makefile para aplicaciones standar           #
#                                                                          #
#  Autor                    : Josue Alejandro Aguilar Moreno (JAAM)        #
#  Fecha de creacion        : 15/06/21                                     #
#                                                                          #
############################################################################

############################################################################
#  RUTAS LIBRERIAS DEL APLICATIVO                                          #
############################################################################

BIN_DIR := ../bin
INC_DIR := ../include
SRC_DIR := ../src
INC_POSTGRE := /usr/include/postgresql

############################################################################
#  BANDERAS DEL COMPILADOR                                                 #
############################################################################

CFLAGS01 := -I$(INC_DIR) -I$(INC_POSTGRE) -g -Wall
CFLAGSQL := -lecpg -lpq

############################################################################
#  COMPONENTES DEL APLICATIVO                                              #
############################################################################

FASE3     := $(wildcard *.pgc) # Consigue todos .pgc en el directorio actual
SOURECPG   := $(subst .pgc,.c,$(FASE3)) # Sustituye los .pgc por .c

SOURCES   := $(wildcard *.c) # Consigue todos .c en el directorio actual
OBJS      := $(subst .c,.o,$(SOURCES)) # Sustituye los .c por .o
PATH_OBJS := $(addprefix $(BIN_DIR)/,$(OBJS)) # Se agrega ruta a objetos
TARGET    := $(BIN_DIR)/EJECUTABLE # Nombre de ejecutable

############################################################################
#  REGLAS                                                                  #
############################################################################

all : servidor

servidor: $(TARGET)
#@echo >/dev/null
$(TARGET): $(PATH_OBJS)
	@echo -----------------------------------------------------
	@echo Construyendo aplicativo $(@F)
	c89 -o $(@) $(^) $(CFLAGSQL)
	@echo -----------------------------------------------------
	@echo $(@F) Listo para usarse !!!
	@echo -----------------------------------------------------

%.o: %.c
$(BIN_DIR)/%.o : %.c
	@echo ---------------------------------------------
	@echo Compilando $(@F) por que cambiaron $(?F)
	@echo ---------------------------------------------
	c89 -o $(@) -c $(^) $(CFLAGS01)
	@echo Ok!

fs3: $(SOURECPG)

%.c: %.pgc 
	@echo ----------------------------------------------------
	@echo Construyendo $(@F) por que cambiaron $(?F)
	@echo ----------------------------------------------------
	ecpg  $(^)
	@echo Ok!

clean:
	@echo ---------------------------------------------
	@echo "Borrando todo del directorio $(BIN_DIR)"
	rm -f $(BIN_DIR)/*
	@echo "Se eliminaron todos los objetos del directorio $(BIN_DIR)"
	@echo ---------------------------------------------

info:
	$(info $(FASE3))
	$(info $(SOURECPG))
	$(info $(SOURCES))
	$(info $(OBJS))
	$(info $(PATH_OBJS))
