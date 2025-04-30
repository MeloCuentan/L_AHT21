#include "L_AHT21.h"

/**
 * @brief Constructor
 * @param wire Puerto I2C
 */
L_AHT21::L_AHT21(TwoWire &wire)
{
    i2cPort = &wire; // Asignar puerto I2C
}

/**
 * @brief Inicializar el sensor
 * @return true si hay un error
 */
bool L_AHT21::begin()
{
    i2cPort->begin();                   // Inicializar puerto I2C
    delay(10);                          // Esperar un poco para estabilización
    if (!sendCommand(0xE1, 0x08, 0x00)) // Enviar comando de inicialización
        return false;                   // Retorna false si hay un error
    delay(10);                          // Dar tiempo para que el sensor se configure
    return true;                        // Retorna false si no hay error
}

/**
 * @brief Enviar comando al sensor
 * @param cmd1 Comando 1
 * @param cmd2 Comando 2
 * @param cmd3 Comando 3
 * @return true si hay un error
 */
bool L_AHT21::sendCommand(uint8_t cmd1, uint8_t cmd2, uint8_t cmd3)
{
    i2cPort->beginTransmission(AHT21_ADDRESS); // Iniciar transmisión I2C a la dirección del sensor
    i2cPort->write(cmd1);                      // Enviar comando 1
    i2cPort->write(cmd2);                      // Enviar comando 2
    i2cPort->write(cmd3);                      // Enviar comando 3
    return (i2cPort->endTransmission() == 0);  // Retorna false si hay un error
}

/**
 * @brief Esperar a que el sensor esté listo
 * @return true si el sensor está listo
 */
bool L_AHT21::waitForData()
{
    uint32_t start = millis();
    while (millis() - start < 100) // Esperar hasta 100ms
    {
        i2cPort->requestFrom(AHT21_ADDRESS, (uint8_t)1); // Leer 1 byte
        if (i2cPort->available())                        // Verificar si hay datos disponibles
        {
            uint8_t status = i2cPort->read(); // Leer el byte de estado
            if ((status & 0x80) == 0)         // Si el bit BUSY está en 0, la medición ha terminado
                return true;                  // Retorna true si el sensor está listo
        }
        delayMicroseconds(500); // Reducimos el impacto del delay
    }
    return false; // Si después de 100ms sigue ocupado, retorna error
}

/**
 * @brief Obtener temperatura y humedad
 * @param temperature Temperatura en grados Celsius
 * @param humidity Humedad relativa en porcentaje
 * @return true si hay error
 */
bool L_AHT21::read(float &temperature, float &humidity)
{
    if (!sendCommand(0xAC, 0x33, 0x00)) // Enviar comando de lectura
        return false;                   // Retorna false si hay un error

    if (!waitForData()) // Esperar a que el sensor esté listo
        return false;   // Retorna false si hay un error

    i2cPort->requestFrom(AHT21_ADDRESS, (uint8_t)6); // Leer 6 bytes
    if (i2cPort->available() < 6)                    // Verificar si se recibieron los 6 bytes
        return false;                                // Retorna false si hay un error

    uint8_t data[6];               // Buffer para almacenar los datos
    for (int i = 0; i < 6; i++)    // Bucle para recorrer los 6 bytes
        data[i] = i2cPort->read(); // Leer byte

    // Verificar si el sensor sigue ocupado o tiene error
    if (data[0] & 0x80) // Si el bit BUSY está en 1, el sensor está ocupado
        return false;   // Retorna false si hay un error

    // Obtener humedad (20 bits)
    uint32_t rawHumidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | ((data[3] & 0xF0) >> 4); // Obtener los 20 bits de humedad
    humidity = (rawHumidity * 100.0) / 1048576.0;                                                          // Calcular la humedad relativa

    // Obtener temperatura (20 bits)
    uint32_t rawTemperature = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5]; // Obtener los 20 bits de temperatura
    temperature = ((rawTemperature * 200.0) / 1048576.0) - 50.0;                                       // Calcular la temperatura en grados Celsius

    return true; // Retorna true si no hay error
}
