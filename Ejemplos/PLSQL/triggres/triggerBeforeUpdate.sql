CREATE OR REPLACE TRIGGER tr_actualiza
BEFORE UPDATE
ON empleados
FOR EACH ROW
BEGIN
    INSERT INTO control VALUES(USERS, SYSDATE);
END tr_actualiza;


UPDATE empreados SET sueldo = sueldo + sueldo * 0.1
WHERE seccion = 'Secretaria';