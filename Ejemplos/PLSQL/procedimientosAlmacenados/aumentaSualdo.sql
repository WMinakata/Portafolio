CREATE OR REPLACE PROCEDURE aumentaSueldo(anio IN NUMBER, porcentaje IN NUMBER)
AS
BEGIN
    UPDATE  empleados 
    SET     sueldo = sueldo + (sueldo * porcentaje / 100)
    WHERE   (extract(YEAR FROM current_date)) - extract(YEAR FROM fechaIngreso)) > anio;
END aumentaSueldo;

-- ejecucion del procedimiento
EXECUTE aumentaSueldo(10,20);