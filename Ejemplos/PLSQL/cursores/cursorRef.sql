CREATE OR REPLACE FUNCTION f_deatoEmpleados(v_documento IN NUMBER)
RETURN sys_refcursor -- RETORNA TIPO REFERENCIA
IS
    v_ref sys_refcursor;
BEGIN
    OPEN v_ref FOR SELECT * FROM empleados
    WHERE documeto = v_documento;
    RETURN v_ref;
END;

--- Ejecucion ---
var rc1 refcursor
EXEC :rc1 := f_deatoEmpleados(22222222);
print rc1; -- imprime todos los datos de consulta devueltos por la funcion

------------------------------------------------------------------
---- AQUI SE REGREAN DOS REGISTROS ----
CREATE OR REPLACE FUNCTION f_deatoEmpleados(v_valor1 IN NUMBER,
                                            v_valor2 IN NUMBER)
RETURN sys_refcursor -- RETORNA TIPO REFERENCIA
IS
    v_ref sys_refcursor;
BEGIN
    OPEN v_ref FOR SELECT * FROM empleados
    WHERE documeto IN (v_valor1, v_valor2);
    RETURN v_ref;
END;

--- Ejecucion ---
var rc1 refcursor
EXEC :rc1 := f_deatoEmpleados(22222222, 23333333);
print rc1; -- imprime todos los datos de consulta devueltos por la funcion