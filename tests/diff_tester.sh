#!/bin/bash

# -z DEVUELVE TRUE SI EL VALOR ES NULL
# -n DEVUELVE TRUE SI EL VALOR NO ES NULL

# $1 CONTIENE EL VALOR DEL PRIMER ARGUMENTO DEL PROGRAMA
TEST_FILE=$1

# PARA COMPROBAR SI HAN PASADO EL NOMBRE DEL ARCHIVO DE TESTS COMO ARGUMENTO
if [ -z "$TEST_FILE" ]
then
echo -e "Error\nIncluir nombre de archivo de tests como argumento."
exit 1
fi

# LOOP PARA EJECUTAR LOS COMANDOS DEL ARCHIVO DE TESTS LÍNEA POR LÍNEA
# COMPARANDO LOS RESULTADOS ENTRE bash Y minishell
LINE_NUM=0
while read COMMAND;
do
LINE_NUM=$(($LINE_NUM + 1))
#ELIMINAR MINI_COMMAND CUANDO SE PUEDA EJECUTAR ./minishell
MINI_COMMAND="hola"
# ./minishell "$COMMAND" > mini.txt
echo "$MINI_COMMAND" > mini.txt
#exit
$COMMAND > original.txt
RESULT=`diff mini.txt original.txt`
if [ -n "$RESULT" ]
then
	echo -e "Command: $COMMAND\nAt line number $LINE_NUM of $TEST_FILE failed."
	echo -e "Difference/s with original:\n$RESULT\n\n"
fi
done < $TEST_FILE

# BORRAR ARCHIVOS DE COMPARACIÓN DE OUTPUTS
rm -rf original.txt mini.txt
