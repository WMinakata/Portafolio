CREATE OR REPLACE PROCEDURE autorLibro(atitulo IN VARCHAR2)
AS
    v_autor VARCHAR2(20);
BEGIN
    SELECT  autor 
    INTO    v_autor
    FROM    libros
    WHERE   titulo = atitulo;

    INSERT INTO tabla1
    SELECT titulo, precio
    FROM libros
    WHERE autor = v_autor;
END autorLibro;


-- execucion de procedimiento almacenado
EXECUTE autorLibro('El quijote');

SELECT * FROM tabla1;