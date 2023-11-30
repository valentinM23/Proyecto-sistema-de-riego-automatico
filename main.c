#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "dht.h"
#include <math.h>
#include "hardware/pio.h"

#define RELAY_PIN 15
#define SENSOR_PIN 20
#define ANALOG_GPIO 24 //gp18
#define ANALOG_CH 0

//#define LCD_RS_PIN 0
//#define LCD_EN_PIN 1
//#define LCD_D4_PIN 2
//#define LCD_D5_PIN 3
//#define LCD_D6_PIN 4
//#define LCD_D7_PIN 5

// DHT usado (cambiar por DHT22 si fuese el caso)
static const dht_model_t DHT_MODEL = DHT11;
// GPIO a usar para los datos
static const uint8_t DATA_PIN = 16;

float humidity, temperature_c;

dht_result_t measure_temp_and_hum(dht_t *dht);

// void lcd_send_byte(uint8_t byte, uint8_t mode) {
    // Función para enviar un byte de datos o comando a la pantalla LCD
    // Utiliza los pines GPIO definidos anteriormente
    // ...
//}
//void lcd_init() {
    // Función para inicializar la pantalla LCD
    // Utiliza las funciones lcd_send_byte() para enviar comandos a la pantalla LCD
    // ...
//}
//void lcd_print(char* str) {
    // Función para imprimir una cadena de caracteres en la pantalla LCD
    // Utiliza las funciones lcd_send_byte() para enviar datos a la pantalla LCD
    // ...
//}


int main() {
    stdio_init_all();
    //lcd_init(); //Inica la pantalla LCD
    dht_t dht;
    // Inicializacion de DHT usando el PIO0 para controlar el DATA_PIN

    dht_init(&dht, DHT_MODEL, pio0, DATA_PIN, 1);

    dht_result_t result = measure_temp_and_hum(&dht);

    //Borrar si esta mal la siguiente linea
    uint16_t moisture_data = adc_read();
    gpio_init(RELAY_PIN);
    adc_init();
    adc_gpio_init(SENSOR_PIN);
    adc_select_input(0);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    while(1) {
        uint16_t moisture_data = adc_read();
        printf("Moisture data: %d\n", moisture_data);
        if(moisture_data <= 2049) {
            gpio_put(RELAY_PIN, 1);
        } else if(moisture_data >= 2048) {
            gpio_put(RELAY_PIN, 0);
        }
        sleep_ms(1000);
    }
    //Borrar si esta mal
    //char lcd_str[16];
    //sprintf(lcd_str, "Temp: %d C", DHT_RESULT_OK);
    //lcd_print(lcd_str);
    //sprintf(lcd_str, "Humedad: %d %%", moisture_data);
    //lcd_print(lcd_str);
    return 0;
}

dht_result_t measure_temp_and_hum(dht_t *dht) {
     // Inicio comunicacion con sensor
        dht_start_measurement(dht);
        // Esperar a que la comunicacion termine y obtener los datos
        dht_result_t result = dht_finish_measurement_blocking(dht, &humidity, &temperature_c);
        // Verificar el resultado de la operacion
        if (result == DHT_RESULT_OK) {
            // Mostrar temperatura y humedad

        } else if (result == DHT_RESULT_TIMEOUT) {
            // Mensaje de error
            puts("El DHT no responde, probablemente este mal el conexionado");
        } else {
            // Error de checksum
            puts("Mal checksum");
        }
        // Demora para no saturar la consola
        sleep_ms(500);
    
    return result;
}