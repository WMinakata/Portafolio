-- ACTIVA O DESACTIVA UN TRIGGER
-- ejemplos tomados de triggerWhenIF.sql

---- IMPLEMENTACION ---- SE OBTIENEN LOS CAMPOS DE LOS TRIGGERS VINCULADOS A LA TABLA EMPLEADOS ----
SELECT trigger_name, triggering_event, table_name, STATUS
FROM user_triggers
WHERE table_name = 'EMPLEADOS';

--- DESACTIVA EL TRIGGER ----
ALTER TRIGGER tr_aumentaSualdo DISABLE;

--- ACTIIVA EL TRIGGER ----
ALTER TRIGGER tr_aumentaSualdo ENABLE;


--- Ya no se activa el trigger tr_aumentaSueldo ---
  UPDATE empleados SET sueldo = 600 WHERE documento = 22333444;