CREATE OR REPLACE FUNCTION f_prueba(valor NUMBER)
RETURN NUMBER -- Tipo de valor que regresa
IS
BEGIN
    RETURN valor * 2;
END;

-- Ejecucion
SELECT f_prueba(2) AS total FROM dual;
                   -- campo  -- no hay tablas ejecutandoce se llama a uno por defecto
                             -- temporal