SET SERVEROUTPUT ON

DECLARE
    v_docu empleados.documento%TYPE; -- prepara la variable para recibir el tipo de dato del campo
    v_nom  empleados.nombre%TYPE;
    v_ape  empleados.apellido%TYPE;
    v_suel empleados.sueldo%TYPE;

    CURSOR c_cursor2 IS -- se declara un cursor
        SELECT  documento
               ,nombre
               ,apellido
               ,sueldo
        FROM    empleados
        WHERE   documento = '2222222';

BEGIN
    OPEN    c_cursor2;

    FETCH   c_cursor2 
    INTO    v_docu
           ,v_nom
           ,v_ape
           ,v_suel;

    CLOSE c_cursor2

    dbms_output.put_line('Documentos: ' || v_docu);
    dbms_output.put_line('Nombre: ' || v_nom);
    dbms_output.put_line('Apellidos: ' || v_ape);
    dbms_output.put_line('Sueldo: ' || v_suel);
END;