// Controlar temperatura y humedad

#ifndef L_AHT21_H
#define L_AHT21_H

#include <Arduino.h>
#include <Wire.h>

#define L_AHT21_MAX_VALUE 1048576.0
#define L_AHT21_TEMPERATURE_SCALE 200.0
#define L_AHT21_TEMPERATURE_OFFSET 50.0
#define L_AHT21_CONVERSION_TIME 80 // Tiempo de conversión en milisegundos


class L_AHT21
{
public:
    L_AHT21(TwoWire &wire = Wire); // Constructor con puerto I2C opcional

    bool begin();                                   // Inicializar el sensor
    bool read(float &temperature, float &humidity); // Obtener temperatura y humedad

private:
    TwoWire *i2cPort;
    const uint8_t AHT21_ADDRESS = 0x38; // Dirección I2C del sensor

    bool sendCommand(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3); // Enviar comando al sensor
    bool waitForData();                                         // Esperar a que el sensor esté listo
};

#endif
