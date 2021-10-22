CREATE OR REPLACE FUNCTION f_costo(valor NUMBER)
RETURN VARCHAR
IS
    costo := '';
    IF (valor <= 500)
    THEN
        costo := 'economico';
    ELSE
        costo := 'costoso';
    END IF;
    RETURN costo;
END;

SELECT titulo, autor, precio, f_costo(precio) FROM libros;