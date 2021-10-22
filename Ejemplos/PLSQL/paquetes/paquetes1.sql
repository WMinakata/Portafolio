SET SERVEROUTPUT ON:

--- CREACION DE ESPECIFICACION DE PAQUETE --- 
CREATE OR REPLACE PACKAGE los_productos AS
    PROCEDURE caracteristicas(v_codigo productos.codigo%TYPE);

    FUNCTION  f_precio(v_codigo INT)
    RETURN NUMBER;

END los_productos;

--- CREACION DE CUERPO DE PAQUETE ---
CREATE OR REPLACE PACKAGE BODY los_productos AS
    PROCEDURE caracteristicas(v_codigo productos.codigo%TYPE) IS
        v_producto productos.nombre%TYPE;
        v_precio productos.precio%TYPE;
BEGIN
    SELECT  nombre
           ,precio
    INTO    v_producto
           ,v_precio
    WHERE   codigo = v_codigo;

    dbms_output.put_line('Articulo ' || v_producto);
    dbms_output.put_line('Precio ' || v_precio);
END caracteristicas;
--------------------------------------------
CREATE OR REPLACE FUNCTION f_precio(v_codigo INT)
RETURN NUMBER
AS
    v_precio NUMBER;
BEGIN
    SELECT  precio INTO v_precio
    FROM    productos 
    WHERE   codigo = v_codigo;

    RETURN v_precio;
END f_precio

END los_productos;


-- EJECUCION ---
BEGIN
    los_productos.caracteristicas(3);
    los_productos.f_precio(4) AS Precio FROM DUAL;

END;