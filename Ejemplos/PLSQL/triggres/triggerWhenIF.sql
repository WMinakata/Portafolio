CREATE OR REPLACE TRIGGER tr_aumentaSualdo
BEFORE UPDATE OF sueldo
ON empleados
FOR EACH ROW WHEN(new.sueldo > old.sueldo) -- en cada fila cuando el sueldo nuevo sea mayor al
                                           -- sueldo anterior
BEGIN
    INSERT INTO control 
    VALUES
    (
        user
       ,SYSDATE
       ,:old.documento
       ,:old.sueldo
       ,:new sueldo
    );

END tr_aumentaSualdo;

    UPDATE * FROM empleados;

    UPDATE empleados SET sueldo = 600 WHERE documento = 22333444;

----------------------------------------------------------------------
CREATE OR REPLACE TRIGGER tr_actualiza_datos
BEFORE INSERT
ON empleados
FOR EACH ROW
BEGIN
    :new.apellido := UPPER(:new.apellido);
    IF (:new.sueldo IS NULL)
    THEN
        :new.sueldo := 0;
    END IF;
END tr_actualiza_datos;

-- Prueba
INSERT INTO empleados VALUES('2566777', 'Lopez', 'Luisa', 'Secretaria', null);