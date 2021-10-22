SET SERVEROUTPUT ON;

--- CURSOR IMPLICITO ---
DECLARE
    filas NUMBER(2);

BEGIN
    UPDATE empleados
    SET sueldo = sueldo + 500
    WHERE sueldo >= 9000;

    IF(SQL%NOTFOUND)
    THEN
        dbms_out.put_line('No hay empleados dispobles');
    ELSIF(SQL%FOUND)
    THEN
        filas := SQL%ROWCOUNT; -- CUENTA LAS FILAS ENCONTRADAS
        dbms_out.put_line(filas || ': Empleados actualizados');
    END IF
END;