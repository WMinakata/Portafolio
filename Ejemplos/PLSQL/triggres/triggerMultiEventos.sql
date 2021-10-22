CREATE OR REPLACE TRIGGER tr_control_empleados
BEFORE INSERT OR UPDATE OR DELETE
ON empleados
    BEGIN

    IF (inserting)
    THEN
        INSERT INTO control_empleados VALUES(USERS, SYSDATE, 'ingreso');
    END IF;

    IF (deleting)
    THEN
        INSERT INTO constol_empleados VALUES(USERS, SYSDAT, 'borrado');
    END IF

    IF (updating)
    THEN
        INSERT INTO constol_empleados VALUES(USERS, SYSDAT, 'actualizacion');
    END IF

END tr_control_empleados;

--- acciones para el trigger
INSERT INTP empleados VALUES(22656698, 'Aguilar', 'Jose', 'Gerencia', 3000)
UPDATE empleados SET sueldo = 2000 WHERE documento = 23444555;
DELETE FROM empleados WHERE documento = 23666777;

--- consulta a la tabla control
SELECT * FROM control_empleados;