SET SERVEROUTPUT ON

DECLARE
    v_empleados empleados%ROWTYPE; -- contiene todos los tipos de datos de los campos
                                   -- de la tabla empleado funciona como una struct

BEGIN
    FOR v_empleados IN (SELECT * FROM empleados) -- recorre todos los registros de la tabla
    LOOP
        dbms_output.put_line(v_empleados.nombre || ' - ' || v_empleados.sueldo);
    END LOOP;

END;


---------------------------------------------
-- Este cursor no requiere seccion de declaracion

BEGIN
    UPDATE empleados SET sueldo = 10000
    WHERE  documeto = '23333333';

    IF (SQL%NOTFOUND)
    THEN
        dbms_output.put_line('No existe registro para modificar');
    END IF;

END;