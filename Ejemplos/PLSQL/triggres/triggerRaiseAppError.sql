CREATE OR REPLACE TRIGGER tr_controlEmpleados
BEFORE INSERT OR UPDATE OR DELETE
ON empleados
FOR EACH ROW
BEGIN
    IF (:new.sueldo > 5000)
    THEN
        raise_application_error(-20000, 'SUELDO NO PUEDE SUPERAR LOS $5000.00');
    END IF;

    INSERT INTO control VALUES(user, SYSDATE, 'INSERCION');

    IF (:old.seccion = 'Gerencia')
    THEN
        raise_application_error(-20000, 'NO SE PUEDE ELIMINAR PUESTO DE GERENCIA');
    END IF;

    INSERT INTO control VALUES(USER, SYSDATE, 'BORRADO');

    IF(UPDATING('documento'))
    THEN
        raise_application_error(-20000, 'NO SE PUEDE ACTUALIZAR NUMERO DE DOCUMENTO')
    END IF;
END tr_controlEmpleados;