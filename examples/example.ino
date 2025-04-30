#include <Arduino.h>
#include "L_AHT21.h"

float temperature, humidity;

L_AHT21 sensor;

void setup() {
    Serial.begin(115200);
    if (!sensor.begin()) {
        Serial.println("Error al inicializar el sensor AHT15");
        while (1); // Detenerse aquí
    }
}

void loop() {
    if (sensor.read(temperature, humidity)) {
        Serial.print("Temperatura: ");
        Serial.print(temperature);
        Serial.println(" °C");

        Serial.print("Humedad: ");
        Serial.print(humidity);
        Serial.println(" %");
    } else {
        Serial.println("Esperando datos del sensor AHT21...");
    }
    delay(2000); // Espera 2 segundos antes de la próxima lectura
}
