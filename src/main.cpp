/* NO EDITAR ESTE BLOQUE DE COMENTARIOS
    Este es el código principal del proyecto.

    Vehiculo autonomo evasor de obstaculos
    Sensor: HC-SR04
    Puente H: L298N
    Potenciometro: control de velocidad por ADC
*/

#include "Arduino.h"
#include "control_vehiculo.h"

void setup() {
    Serial.begin(115200);
    iniciar_modulo();
}

void loop() {
    long distancia_cm;
    int lectura_adc;
    int pwm;
    const char *estado;

    ejecutar_control(&distancia_cm, &lectura_adc, &pwm, &estado);

    Serial.print("Distancia: ");
    if (distancia_cm < 0) {
        Serial.print("INVALIDA");
    } else {
        Serial.print(distancia_cm);
    }
    Serial.print(" cm | ADC: ");
    Serial.print(lectura_adc);
    Serial.print(" | PWM: ");
    Serial.print(pwm);
    Serial.print(" | Estado: ");
    Serial.println(estado);
}