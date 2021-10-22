SET SERVEROUTPUT ON;

DECLARE
    v_nom productos.nombre_producto%TYPE;
    v_pre productos.precio%TYPE;

    CURSOR c_productos (idprod productos.id_producto%TYPE)
        IS
        SELECT  nombre_producto
               ,precio 
        FROM    productos
        WHERE   id_productos = idprod;

BEGIN
    OPEN c_productos(2); -- Se abre cursor y pasa el parametro que es el "idprod"

    LOOP
        FETCH c_productos
        INTO  v_nom
             ,v_pre;

        EXIT WHEN c_productos%NOTFOUND; -- Termina cuando ya no encuentra mas tuplas
        dbms_output.put_line('Articulo: ' || v_nom || ', precio: ' || v_pre);
    END LOOP;
    CLOSE c_productos

END;