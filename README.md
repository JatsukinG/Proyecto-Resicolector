# Proyecto-Resicolector
Proyecto resicolector

------------------------------------------------------------------------------------------------------------------

El archivo Proyecto_Recolector.ino corresponde al  arhivo que debe ser cargado  en el Arduino:

¡¡ Es imporatnte tener en cuenta que a la hora de subir el codigo se debe configuar el IDE Arduino en el aparatado de herramientas de la siguiente manera:

Herramientas > Placa -- Arduino Mega 2560.
Herramientas > Puerto -- Puerto correspondiente al Arduino.

-------------------------------------------------------------------------------------------------------------------

Cuando se inicie el sistema No debe haber ningun peso en los Resicolectores, de manera que se pueda iniciar de manera correcta la lectura del peso.

Leer los comentarios del codigo de Arduino para entender la logica de funcionamiento.

El esquema de este circuito se puede observar en Esquema_recicolector_bb.png


https://raw.githubusercontent.com/JatsukinG/Proyecto-Resicolector/main/Esquema_Resicolector_bb.png

--------------------------------------------------------------------------------------------------------------------

Para realizar la calibracion de cada uno de los sensores se debe hacer uso del archivo adjunto Calibracion_celdas_de_carga.ino, este debe ser cargado al arduino
y se deben identificar los pines de conexion para celada de carga en conjunto con los modulos HX711. (Leer los comentarios que se encuentran el codigo 
anteriormente mencionado).

Tener en cuenta que en el codigo se debe cambiar el valor de knowWeight por un peso conocido, esto para que se pueda hacer correctamente la calibracion del sensor.

Una de vez hecho lo anterior, se procede a ejecutar el sistema. Abrir el monitor serial del IDE de arduino para ver la informacion y seguir los pasos indicados.

Finalmente, cuando se terminan los pasos indicados por el monitor serial, podras observar el peso y el valor del factor de calibracion, este valor
es el que se debe modificar en el codigo Proyecto_Resicolector.ino, Teniendo en cuenta los pines de conexion de cada uno de los sensores.

--------------------------------------------------------------------------------------------------------------------

El archivo Resicolector_Recepsion.ino corresponde al codigo para el circuito que se encraga de recibir los mensajes provenientes de cada Resicolector y enviando
la informacion a la API

El esquema de este circuito se puede observar en Esquema_Resicolector_receptor.png

¡¡Importante tener en cuenta que a la hora de subir el codigo se debe configuar el IDE Arduino en el aparatado de herramientas de la siguiente manera:

Herramientas > Placa > ESP32 Arduino -- DOIT ESP32 DEVKIT V1 0.
Herramientas > Puerto -- Puerto correspondiente al ESP32.


