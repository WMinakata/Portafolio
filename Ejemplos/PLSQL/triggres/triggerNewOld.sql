CREATE OR REPLACE TRIGGER tr_ingresaLibros
BEFORE INSERT
ON libros
FOR EACH ROW -- Para cada fila
BEGIN
    IF (:new.precio <= 30)
    THEN
        INSERT INTO ofertas VALUES(:new.codigo, :new.precio,, USERS, SYSDATE);
    END IF

END tr_;

CREATE OR REPLACE TRIGGER tr_actualizaLibros
BEFORE UPDATE OF precio -- se activa al de actualizar el campo precio
ON libros
FOR EACH ROW
BEGIN
    IF (:old.precio <= 30) AND (:new.precio > 30)
    THEN
        DELETE FROM ofertas WHERE codigo = :old.codigo;
    END IF;

    IF (:old.precio > 30) AND (:new.precio <= 30)
    THEN
        INSERT INTO ofertas VALUES(:new.codigo, :new.precio, USER, SYSDATE);
    END IF;

END tr_actualizaLibros;

UPDATE libros SET precio =  25 WHERE codigo = 120; -- se valida segunda condicion

UPDATE libros SET precio =  60 WHERE codigo = 120; -- se valida primera condicion
