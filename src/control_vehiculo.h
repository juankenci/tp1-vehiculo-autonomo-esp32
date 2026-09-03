// control_vehiculo.h
#ifndef CONTROL_VEHICULO_H
#define CONTROL_VEHICULO_H

#ifdef __cplusplus
extern "C" {
#endif

void iniciar_modulo(void);
void ejecutar_control(long *distancia_cm, int *lectura_adc, int *pwm,
                      const char **estado);

#ifdef __cplusplus
}
#endif

#endif