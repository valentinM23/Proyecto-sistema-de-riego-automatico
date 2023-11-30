#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Pin al que está conectado el sensor de humedad (ADC0)
#define PIN_SENSOR_HUMEDAD 26

// Pin al que está conectada la bomba de agua (PWM)
#define PIN_BOMBA_AGUA 0

// Frecuencia de PWM en Hz
#define PWM_FREQ 1000

void configurar_pwm() {
    // Configurar el pin de la bomba de agua como PWM
    gpio_set_function(PIN_BOMBA_AGUA, GPIO_FUNC_PWM);
    
    // Configurar PWM
    pwm_set_wrap(pwm_gpio_to_slice_num(PIN_BOMBA_AGUA), 255);  // Rango de PWM de 0 a 255
    pwm_set_clkdiv(pwm_gpio_to_slice_num(PIN_BOMBA_AGUA), 125); // Frecuencia = 125MHz / 125 = 1MHz
    pwm_set_enabled(pwm_gpio_to_slice_num(PIN_BOMBA_AGUA), true);
}

void activar_bomba_agua(int velocidad) {
    // Configurar la velocidad de la bomba de agua usando PWM
    pwm_set_gpio_level(PIN_BOMBA_AGUA, velocidad);
}

int main() {
    stdio_init_all();

    // Configurar el ADC para leer el sensor de humedad
    adc_init();
    adc_gpio_init(PIN_SENSOR_HUMEDAD);
	adc_select_input(0);

    // Configurar el PWM
    configurar_pwm();

    while (1) {
        // Leer el valor del sensor de humedad
        uint16_t valor_adc = adc_read();
        // Puedes ajustar estos valores según las especificaciones de tu sensor
        int valor_humedad = (valor_adc * -100) / 4095 +100;

        printf("Valor de humedad: %d\n", valor_humedad);

        // Ajustar la velocidad de la bomba de agua según el valor de humedad
        int velocidad_pwm = (valor_humedad < 30) ? 255 : 0; // Encender completamente si la humedad es baja, apagar si no

        // Activar la bomba de agua con la velocidad calculada
        activar_bomba_agua(velocidad_pwm);

        sleep_ms(10000); // Esperar 10 segundos antes de la próxima lectura (ajusta según tus necesidades)
    }

    return 0;
}