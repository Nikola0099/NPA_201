#include <stdint.h>
#include <stm32f1xx_hal_conf.h>
#include <stm32f1xx_it.h>
#include <string.h>
#include <math.h>

/*  PINOUT:
 *  VCC -> 3.3V
 *  GND -> GND
 *  SLC -> B6
 *  SDA -> B7
 */

I2C_HandleTypeDef hi2c1;
uint16_t devAddress = 0x4E;  // Adresa uređaja
uint16_t devAddressRead = 0x4E | 1;

void readValue(float *Pconv, float *Tconv, float *height)
{
    float p0 = 1013.25; // Vrednost pritiska na nivou mora, izražen u mB

    uint8_t readReg = 0xAC; // Adresa registra za čitanje vrednosti

    uint8_t dataBuffer[5] = {0};

    HAL_I2C_Master_Transmit(&hi2c1, devAddress, &readReg, 1, HAL_MAX_DELAY);
    HAL_Delay(20);  // Minimalni delay od 20ms za merenje
    HAL_I2C_Master_Receive(&hi2c1, devAddressRead, dataBuffer, 5, HAL_MAX_DELAY);

    uint16_t P, T;

    P = (uint16_t)dataBuffer[1] << 8 | dataBuffer[2]; // Raw vrednosti pritiska dobijena od senzora
    
    T = (uint16_t)dataBuffer[3] << 8 | dataBuffer[4]; // Raw vrednost temperature dobijena od senzora


    *Pconv = (float)P/65535*(1260-260) + 260; // Konvertovana vrednost pritiska, izražen u mB
    *Tconv = (float)T/65535*(85+40) - 40;     // Konvertovana vrednost temperature, izražena u C


    *height = 44330 * (1 - pow((*Pconv/p0),1.0/5.255)); // stvarna visina u odnosu na nivo mora

}
