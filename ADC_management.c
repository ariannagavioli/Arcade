#include "ee.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "ADC_management.h"

void ADC1CH13_ADC3CH12_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;

	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC3 and GPIO clocks ********************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC3, ENABLE);

	/* Configure ADC3 Channel12 and ADC1 Channel13 pins as analog input *******/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* GPIOD Periph clock enable **********************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Configure PD12, PD13, PD14 and PD15 in output pushpull mode ************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
									GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* ADC Common Init ********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init **************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC3 Init **************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC1 regular channel13 configuration ***********************************/
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

	/* ADC3 regular channel12 configuration ***********************************/
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles);

	/* Enable ADC1 and ADC3 */
	ADC_Cmd(ADC1, ENABLE);
	ADC_Cmd(ADC3, ENABLE);
}

uint32_t getConvertedVoltageADC1(){
	uint32_t ADCConvertedVoltage = 0;
	uint16_t ADCConvertedValue;

	/* Checking the EOC bit */
	if(!(ADC1->SR & 2))
		return DATA_NOT_READY;

	ADCConvertedValue = ADC1->DR;
	ADCConvertedVoltage = (ADCConvertedValue *3300) / 0xFFF;

	return ADCConvertedVoltage;
}

uint32_t getConvertedVoltageADC3(){
	uint32_t ADCConvertedVoltage = 0;
	uint16_t ADCConvertedValue;

	/* Checking the EOC bit */
	if(!(ADC3->SR & 2))
		return DATA_NOT_READY;

	ADCConvertedValue = ADC3->DR;
	ADCConvertedVoltage = (ADCConvertedValue *3300) / 0xFFF;

	return ADCConvertedVoltage;
}
