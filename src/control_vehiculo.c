// control_vehiculo.c
#include "control_vehiculo.h"
#include "configuracion_pines.h"
#include "Arduino.h"

void iniciar_modulo(void) {
    pinMode(POTENCIOMETRO_PIN, INPUT);
    pinMode(SENSOR_TRIG, OUTPUT);
    pinMode(SENSOR_ECHO, INPUT);
    pinMode(MOTOR_ENA, OUTPUT);
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_ENB, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);

    digitalWrite(SENSOR_TRIG, LOW);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
    analogWrite(MOTOR_ENA, 0);
    analogWrite(MOTOR_ENB, 0);
}

static long medir_distancia_cm(void) {
    unsigned long duracion;

    digitalWrite(SENSOR_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(SENSOR_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_TRIG, LOW);

    duracion = pulseIn(SENSOR_ECHO, HIGH, 30000);
    if (duracion == 0 || duracion / 58 < 2 || duracion / 58 > 400) {
        return -1;
    }
    return (long)(duracion / 58);
}

static int calcular_pwm(int lectura) {
    return (lectura * PWM_MAXIMO) / ADC_MAXIMO;
}

static void detener_motores(void) {
    analogWrite(MOTOR_ENA, 0);
    analogWrite(MOTOR_ENB, 0);
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
}

static void avanzar_motores(int pwm) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
    analogWrite(MOTOR_ENA, pwm);
    analogWrite(MOTOR_ENB, pwm);
}

static void girar_motores(int pwm) {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);
    analogWrite(MOTOR_ENA, pwm);
    analogWrite(MOTOR_ENB, pwm);
}

void ejecutar_control(long *distancia_cm, int *lectura_adc, int *pwm,
                      const char **estado) {
    long distancia = medir_distancia_cm();
    int lectura = analogRead(POTENCIOMETRO_PIN);
    int velocidad = calcular_pwm(lectura);

    if (distancia < 0) {
        detener_motores();
        *estado = "ERROR_SENSOR";
    } else if (distancia <= DISTANCIA_MINIMA_CM) {
        girar_motores(velocidad);
        *estado = "GIRO";
    } else {
        avanzar_motores(velocidad);
        *estado = "AVANCE";
    }

    *distancia_cm = distancia;
    *lectura_adc = lectura;
    *pwm = (distancia < 0) ? 0 : velocidad;
    delay(100);
}