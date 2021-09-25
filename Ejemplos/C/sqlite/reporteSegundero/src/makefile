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

############################################################################
#  BANDERAS DEL COMPILADOR                                                 #
############################################################################

CFLAGS01 := -I$(INC_DIR) -g -Wall

############################################################################
#  COMPONENTES DEL APLICATIVO                                              #
############################################################################

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
	c89 -o $(@) $(^) -l sqlite3
	@echo -----------------------------------------------------
	@echo $(@F) Listo para usarse !!!
	@echo -----------------------------------------------------

%.o: %.c
$(BIN_DIR)/%.o : %.c
	@echo ---------------------------------------------
	@echo Construyendo objeto $(@F)
	@echo Compilando $(@F) por que cambiaron $(?F)
	@echo ---------------------------------------------
	c89 $(CFLAGS01) -c $(^) -o $(@)
	@echo Ok!

clean:
	@echo ---------------------------------------------
	@echo "Borrando todo del directorio $(BIN_DIR)"
	rm -f $(BIN_DIR)/*
	@echo "Se eliminaron todos los objetos del directorio $(BIN_DIR)"
	@echo ---------------------------------------------

clean.o:
	@echo ---------------------------------------------
	@echo "Borrando objetos del directorio $(BIN_DIR)"
	rm -f $(BIN_DIR)/*.o
	@echo "Se eliminaron todos los objetos del directorio $(BIN_DIR)"
	@echo ---------------------------------------------

info:
	$(info $(SOURCES))
	$(info $(OBJS))
	$(info $(PATH_OBJS))
