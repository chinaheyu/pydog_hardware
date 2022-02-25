#include "ADS1115.h"

// ---------------------------------------------
// ADS1115_ReadRegisters
// ---------------------------------------------
void ADS1115_ReadRegisters(uint8_t address, uint8_t *data, uint8_t size) {

    HAL_I2C_Mem_Read(&ADS1115_HI2C, ADS1115_ADDRESS << 1, address, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

// ---------------------------------------------
// ADS1115_WriteRegisters
// ---------------------------------------------
void ADS1115_WriteRegisters(uint8_t address, uint8_t *data, uint8_t size) {

    HAL_I2C_Mem_Write(&ADS1115_HI2C, ADS1115_ADDRESS << 1, address, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

uint8_t ADS1115_GetStatus(void) {

	if(HAL_I2C_IsDeviceReady(&ADS1115_HI2C, ADS1115_ADDRESS << 1, 1, 10) == HAL_OK)
        return 1;
    else
        return 0;
}

uint16_t ADS1115_ReadADC(uint8_t channel) {

	if (channel > 3) {
		return 0;
	}

	uint16_t register_data = 0;
	int16_t received_data = 0;
	uint8_t data[2] = {0};

	register_data = ADS1115_COMP_QUE;						// 11  : Disable comparator and set ALERT/RDY pin to high-impedance (default)
	register_data |= ADS1115_DR_2;							// 100 : 128 SPS (default)
	register_data |= ADS1115_MODE;							// 1   : Single-shot mode or power-down state (default)
	register_data &= ~ADS1115_PGA;							// 000 : FSR = ±6.144 V (187.5μV)
	switch (channel) {
	case 0:
		register_data |= ADS1115_MUX_2;						// 100 : AINP = AIN0 and AINN = GND
		break;
	case 1:
		register_data |= (ADS1115_MUX_2 | ADS1115_MUX_0);	// 101 : AINP = AIN1 and AINN = GND
		break;
	case 2:
		register_data |= (ADS1115_MUX_2 | ADS1115_MUX_1);	// 110 : AINP = AIN2 and AINN = GND
		break;
	case 3:
		register_data |= ADS1115_MUX;						// 111 : AINP = AIN3 and AINN = GND
		break;
	}
	register_data |= ADS1115_OS;							// 1   : Start a single conversion (when in power-down state)

	data[0] = register_data >> 8;							// msb
	data[1] = register_data & 0xFF;							// lsb
	ADS1115_WriteRegisters(ADS1115_REG_CONFIG, data, 2);

	DWT_Delay_us(10000);										// maximum time = 1 / (0.9 * 128SPS) + 30us = ~8ms

	ADS1115_ReadRegisters(ADS1115_REG_CONVERSION, data, 2);
	received_data = (int16_t)((data[0] << 8) | data[1]);

	if (received_data < 0) {
		received_data = 0;
	}

	return received_data;
}

float ADS1115_Read(uint8_t channel) {

	return (float)ADS1115_ReadADC(channel) * 187.5 / 1000000.0;		// FSR = ±6.144 V (187.5μV)
}

void ADS1115_AsyncReadADC(uint16_t mux) {

	uint16_t register_data = 0;
	int16_t received_data = 0;
	uint8_t data[2] = {0};

	register_data = ADS1115_COMP_QUE;						// 11  : Disable comparator and set ALERT/RDY pin to high-impedance (default)
	register_data |= ADS1115_DR_2;							// 100 : 128 SPS (default)
	register_data |= ADS1115_MODE;							// 1   : Single-shot mode or power-down state (default)
	register_data &= ~ADS1115_PGA;							// 000 : FSR = ±6.144 V (187.5μV)
	register_data |= mux;
	register_data |= ADS1115_OS;							// 1   : Start a single conversion (when in power-down state)

	data[0] = register_data >> 8;							// msb
	data[1] = register_data & 0xFF;							// lsb
	ADS1115_WriteRegisters(ADS1115_REG_CONFIG, data, 2);
}

float ADS1115_GetAsyncResult(void) {

    uint8_t data[2];
    int16_t received_data;
    
    ADS1115_ReadRegisters(ADS1115_REG_CONVERSION, data, 2);
    received_data = (int16_t)((data[0] << 8) | data[1]);
    
	return (float)received_data * 187.5 / 1000000.0;		// FSR = ±6.144 V (187.5μV)
}
