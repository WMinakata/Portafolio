--- FUNCIONALIDADES DE UN TRIGGER ---
-- Conserva la integridad referencial y la coherencia entre los datos entre distitas tablas
-- Registra los cambios que se efectuan sobre las tablas y la identidad de quien los realizo
-- Realizar cualquier accion cuando una tabla es modificada
-- Insertar, actualizar o eliminar datos de una tabla asociada en forma automatica

--- REGLAS ---
-- No puden ser invocados directamente por ejecucion de codigo
-- Al intentar modificar, los datos de una tabla asociada, el trigger se ejecuta automaticamente
-- No reciben ni retornan parametros
-- No generan resultados de consultas de SQL

--- CLASIFICACION ---
-- El momento en que se dispara: si se ejecutan antes (before) o despues (after) de la sentencia.
-- El evento que los dispara: insert, update o delete, segun se ejecute una de estas sentencias 
--                            sobre la tabla
-- Nivel: Dependiendo si se ejecuta para cada fila afectada en la sentencia (por cada fila) 
--        o bien una unica vez por sentencia independientemente de las filas afectadas 
--        (nivel de sentencia)

-- TRIGGER DE CONTROL DE INSERCION DE DATOS
CREATE OR REPLACE TRIGGER tr_ingresoLibros
BEFORE INSERT
ON libros
BEGIN
    INSERT INTO control VALUES(USER, SYSDATE);
END tr_ingresoLibros;