CREATE OR REPLACE TRIGGER tr_ingresaEmpleados
BEFORE INSERT -- Activa al momento de insertar
ON empleados  -- Inserta en la tabla empleados
FOR EACH ROW -- Inserta un registro por cada insert (fila) en la tabla de control
BEGIN
    INSERT INTO control VALUES(USER, SYSDATE);
END tr_ingresaEmpleados;

--INSERTS
 INSERT INTO empleados VALUES('22333444','ACOSTA','Ana','Secretaria');
 INSERT INTO empleados VALUES('22777888','DOMINGUEZ','Daniel','Secretaria');
 INSERT INTO empleados VALUES('22999000','FUENTES','Federico','Sistemas');
 INSERT INTO empleados VALUES('22555666','CASEROS','Carlos','Contaduria');
 INSERT INTO empleados VALUES('23444555','GOMEZ','Gabriela','Sistemas');
 INSERT INTO empleados VALUES('23666777','JUAREZ','Juan','Contaduria');

 DROP TRIGGER tr_ingresaEmpleados; -- Borra el trigger;