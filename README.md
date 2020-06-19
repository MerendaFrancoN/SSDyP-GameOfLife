# SSDyP-GameOfLife
## Compilar SSDyP_
    $ make all
## Ejecutables /build
   * Versión Secuencial y OpenMP
   
    $ ./build/SSDyP <rows> <columns> <SimulationDays> <NumberOfExecutions>
   * Versión MPI     
    
    $ mpirun -np <numProc> ./build/SSDyP_Distributed <rows> <columns> <SimulationDays> <NumberOfExecutions>
   * Versión MPI-OpenMP
    
    $ mpirun -np <numProc> ./build/SSDyP_Hybrid <rows> <columns> <SimulationDays> <NumberOfExecutions>
    
### Ejemplo de ejecución de una grilla de 200 x 200, 120 días de simulación y 1 sola ejecución.
    $ ./build/SSDyP 200 200 120 1
    
    $ mpirun -np 8 ./build/SSDyP_Distributed 200 200 120 1
    
    $ mpirun -np 8 ./build/SSDyP_Hybrid 200 200 120 1

## Simulación de la difusión del COVID-19 en una población

Cada celda cuenta con la siguiente información:
* Estado
  * BLANCO - Celda Libre
  * AZUL - Persona susceptible a la enfermedad
  * ROJO - Enfermo sin contagio
  * AMARILLO - Persona enferma, pero aislada por atención médica
  * VERDE - Curado
  * NEGRO - Muerto
* EDAD - {Niño - Adulto - Anciano}
* ENFERMEDAD DE RIESGO - {diabetes - hipertensión - enf. respiratoria - inmunodeprimida} {SI, NO}
* PROFESIÓN DE RIESGO - {enfermero - médico - odontólogo - cajero - chofer de servicio público} {SI, NO}
* VACUNAS PREVENTIVAS - {vacunado (gripe, neumonía y meningitis)} {SI, NO}
* SEXO BIOLÓGICO - {Mujer - Hombre}

### Información adicional para las celdas
* TIEMPO DESDE CONTAGIO - tiempo desde que contrajo la enfermedad y pasa por todos los períodos.

### Reloj Virtual
 
 A cada señal de reloj, todas las celdas de la cuadrícula puede cambiar su estado de salud a uno nuevo, el cual depende de
 _edad, grupo de riesgo, sexo, acciones preventivas y estado de sus 8 vecinas adyacentes_.
 
### Reglas de cambio de estado
 
 * Susceptible __(AZUL)__ -> Enfermo sin contagio __(NARANJA)__ : si en el tiempo t, la celda en proceso C ,es __AZUL__ y,
 alguna de sus vecinas esta infectada en etapa de contagio ( t >= 4 ), pasa a __NARANJA__ con una probabilidad P, que es proporcional al número de celdas adyacentes enfermas E, la potencia de la enfermedad POT, y la susceptibilidad de la persona a la enfermedad SUSC.

   -> _P = ( (E * POT) + SUSC (EDAD, GRUPO DE RIESGO) ) / (7 * PASOS POR DIA)*_
   
   - Donde POT = 2.4, la cual es una constante.
   - Susceptibilidad por edad:
     - NIÑOS = 0.3
     - ADULTOS = 0.5
     - ANCIANOS = 0.9
     - GRUPO DE RIESGO = AGREGA 0.15
   - PASOS POR DIA - cantidad de veces que se actualiza el estado de las celdas por cada día. En este caso **= 24**.
 
 * Enfermo sin contagio __(NARANJA)__ -> Enfermo con contagio __(ROJO)__ : Si la celda está __(NARANJA)__ y t desde contagio son 4 días, pasa a __(ROJO)__.
 
 * Enfermo con contagio __(ROJO)__ -> Persona enferma, pero aislada por atención médica __(AMARILLO)__: Luego de 2 días de __ROJO__ , con P = 90%, el estado pasa a __(AMARILLO)__.
 
 * Enfermo __(ROJO)__ o __(AMARILLO)__  Recuperado __(VERDE)__ : si transcurrió 14 días, desde el contagio, la probabilidad de recuperarse se basa en la mortalidad de la enfermedad y las complicaciones. 
 - Mortalidad
   - Niños = 1%
   - Adultos = 1.3%
   - Ancianos = 14.8%
   - C/Vacuna Preventia = AGREGA 0.5%
   
 
 
 
 

