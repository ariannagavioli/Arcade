#ifndef _ADC_MANAGEMENT_H_
#define _ADC_MANAGEMENT_H_

#define DATA_NOT_READY		0xFFF

// Sets up ADC1 on channel 13, ADC3 on channel 12
void ADC1CH13_ADC3CH12_Config(void);
// Reads the value from ADC1, DATA_NOT_READY if EOC bit = 0
uint32_t getConvertedVoltageADC1();
// Reads the value from ADC3, DATA_NOT_READY if EOC bit = 0
uint32_t getConvertedVoltageADC3();

#endif
