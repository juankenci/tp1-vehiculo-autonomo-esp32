# Guia sencilla para armar y probar el vehiculo

Segui los pasos en orden. No conectes los motores hasta haber probado el sensor y el potenciometro.

## 1. Potenciometro

Mirando el potenciometro de frente:

- Una pata lateral -> `3.3V` del ESP32.
- La otra pata lateral -> `GND` del ESP32.
- La pata del medio -> `GPIO 34`.

El orden de las dos patas laterales puede invertirse. Solo cambiara el sentido en que aumenta la velocidad al girar la perilla.

Nunca conectes el potenciometro a `5V`.

## 2. Sensor HC-SR04

- `VCC` -> `5V` del ESP32.
- `GND` -> `GND` del ESP32.
- `TRIG` -> `GPIO 5`.
- `ECHO` -> `GPIO 18`, pasando antes por el divisor de resistencias indicado abajo.

### Proteccion del cable ECHO

El sensor puede enviar 5 V y el ESP32 acepta como maximo aproximadamente 3.3 V. Coloca dos resistencias entre el sensor y el ESP32:

```text
ECHO del sensor ---- resistencia de 1 kOhm ----+---- GPIO 18
                                               |
                                       resistencia de 2 kOhm
                                               |
                                              GND
```

El resto de los cables se conecta directamente. Esta proteccion es importante para no danar el ESP32.

## 3. Driver L298N y motores

Usaremos un motor por canal:

- Motor izquierdo: `OUT1` y `OUT2`.
- Motor derecho: `OUT3` y `OUT4`.

Conecta los cables de control asi:

| L298N | ESP32 |
| --- | --- |
| `ENA` | `GPIO 23` |
| `IN1` | `GPIO 19` |
| `IN2` | `GPIO 21` |
| `ENB` | `GPIO 22` |
| `IN3` | `GPIO 17` |
| `IN4` | `GPIO 16` |
| `GND` | `GND` |

Conecta tambien:

- `+12V` o `Vs` del L298N -> positivo de la bateria o fuente de los motores.
- `GND` del L298N -> negativo de esa fuente.
- `GND` de esa fuente -> `GND` del ESP32.

El positivo de la fuente no se conecta al ESP32. No alimentes los motores desde el pin `5V` del ESP32.

Si tu modulo tiene un jumper colocado en `ENA` o `ENB`, retiralo. Esos pines deben quedar libres para recibir el control de velocidad del ESP32.

## 4. Orden de prueba

### Prueba 1: potenciometro

Conecta solamente el potenciometro y carga el programa de prueba del ADC. Al girar la perilla, el monitor serial debe mostrar valores que cambian entre aproximadamente `0` y `4095`.

### Prueba 2: sensor

Conecta el HC-SR04 y su divisor de resistencias. Coloca la mano delante del sensor y verifica que la distancia mostrada por el monitor serial cambie.

### Prueba 3: motor izquierdo

Conecta la fuente del L298N y un solo motor en `OUT1/OUT2`. Prueba el giro durante un instante. Si gira en sentido contrario, intercambia sus dos cables.

### Prueba 4: motor derecho

Conecta el segundo motor en `OUT3/OUT4` y prueba nuevamente. Los dos motores deben girar en el mismo sentido de avance.

### Prueba 5: programa completo

Con todos los componentes conectados, el vehiculo debe:

- avanzar cuando la distancia sea mayor a `15 cm`;
- girar cuando la distancia sea de `15 cm` o menor;
- detenerse si el sensor no responde;
- cambiar la velocidad al girar el potenciometro.

El monitor serial mostrara la distancia, el potenciometro, la velocidad y el estado del vehiculo.

## 5. Importante antes de encender

- Todas las tierras (`GND`) deben estar conectadas entre si.
- No conectes ni desconectes motores con la fuente encendida.
- Verifica dos veces la polaridad de la bateria.
- Levanta las ruedas del suelo durante la primera prueba.
- Usa una fuente capaz de entregar la corriente que necesitan los motores.

## Archivos del proyecto

- `src/configuracion_pines.h`: lista de pines usados.
- `src/control_vehiculo.c`: control del sensor y los motores.
- `src/control_vehiculo.h`: funciones del control.
- `src/main.cpp`: inicio del programa y mensajes del monitor serial.