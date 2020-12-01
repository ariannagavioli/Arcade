/* ###*B*###
 * ERIKA Enterprise - a tiny RTOS for small microcontrollers
 *
 * Copyright (C) 2002-2013  Evidence Srl
 *
 * This file is part of ERIKA Enterprise.
 *
 * ERIKA Enterprise is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation,
 * (with a special exception described below).
 *
 * Linking this code statically or dynamically with other modules is
 * making a combined work based on this code.  Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * As a special exception, the copyright holders of this library give you
 * permission to link this code with independent modules to produce an
 * executable, regardless of the license terms of these independent
 * modules, and to copy and distribute the resulting executable under
 * terms of your choice, provided that you also meet, for each linked
 * independent module, the terms and conditions of the license of that
 * module.  An independent module is a module which is not derived from
 * or based on this library.  If you modify this code, you may extend
 * this exception to your version of the code, but you are not
 * obligated to do so.  If you do not wish to do so, delete this
 * exception statement from your version.
 *
 * ERIKA Enterprise is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License version 2 for more details.
 *
 * You should have received a copy of the GNU General Public License
 * version 2 along with ERIKA Enterprise; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA.
 * ###*E*### */

#include "ee.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "stm32f4_discovery_lcd.h"
#include "STMPE811QTR.h"
//#include "LCD_Touch_Calibration.h"

#include "led_matrix.h"
#include "snake_logic.h"
#include "arkanoid_logic.h"
#include "joystick.h"
#include "ADC_management.h"
#include "pictures.h"
#include "touch.h"

#define SHIFT GPIO_Pin_6		//orange
#define STORE GPIO_Pin_5		//purple
#define DATA GPIO_Pin_4			//gray

GPIO_InitTypeDef  GPIO_InitStructure;

const unsigned char* bg_image = background_Image;
const unsigned char* snake_image = snake_Image;
const unsigned char* arkanoid_image = arkanoid_Image;
const unsigned char* snake_pressed_image = snake_pressed_Image;
const unsigned char* arkanoid_pressed_image = arkanoid_pressed_Image;

char current_game = 's';

/*
 * SysTick ISR2
 */
ISR2(systick_handler)
{
	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

/* Updates one led matrix row at a time following multiplexing logic */
TASK(TaskMatrix)
{
	static EE_UINT8 i = 0;
	uint8_t row_value = 0;
	/* Checking the matrix row's value and updating row_value accordingly
	*  If the j-th column is not empty, let's light up the corresponfing led */
	for(uint8_t j = 0; j < N_COLS; j++) {
		if(current_game == 's'){
			if(snake_matrix[i][j] != '0')
				row_value += (1 << j);
		} else {
			if(arkanoid_matrix[i][j] != '0')
				row_value += (1 << j);
		}
	}
	displayRow(DATA, SHIFT, STORE, i, row_value);
	i++;
	i %= N_ROWS;
}

/* Checks data from ADCs and updates game status */
TASK(TaskJoystick)
{
	uint32_t convertedVoltageADC1 = getConvertedVoltageADC1();
	uint32_t convertedVoltageADC3 = getConvertedVoltageADC3();

	int8_t vx = 0, vy = 0;

	if(convertedVoltageADC1 != DATA_NOT_READY)
		vx = get_x(convertedVoltageADC1);

	/*If current game is arkanoid, there is enough information
	 * to update the game */
	if(current_game == 'a'){
		updateArkanoid(vx);
		return;
	}

	if(convertedVoltageADC1 != DATA_NOT_READY)
		vy = get_y(convertedVoltageADC3);
	updateSnake(vx, vy);
}


TASK(TaskLCD)
{

	unsigned int x, y;
	/* If there was a touch */
	if(GetTouch_SC_Sync( &x, &y)){
		/* If a button was pressed */
		if(y >= 88){
			if(x > 160 && current_game == 's') {	// The arkanoid button was pressed and the current game is snake
				current_game = 'a';
				initializeArkanoid();
				LCD_DrawPicture(0, 120, 160, 123, (uint8_t*) snake_image);
				LCD_DrawPicture(159, 120, 160, 123, (uint8_t*) arkanoid_pressed_image);
			}
			else if(x < 140 && current_game == 'a') {
				current_game = 's';
				initializeSnake();
				LCD_DrawPicture(0, 120, 160, 123, (uint8_t*) snake_pressed_image);
				LCD_DrawPicture(159, 120, 160, 123, (uint8_t*) arkanoid_image);
			}
		}
	}
}

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 * Setup the microcontroller system.
	 * Initialize the Embedded Flash Interface, the PLL and update the
	 * SystemFrequency variable.
	 * For default settings look at:
	 * pkg/mcu/st_stm32_stm32f4xx/src/system_stm32f4xx.c
	 */
	SystemInit();

	/*Initialize Erika related stuffs*/
	EE_system_init();

	/*Initialize systick */
	EE_systick_set_period(MILLISECONDS_TO_TICKS(1, SystemCoreClock));
	EE_systick_enable_int();
	EE_systick_start();

	/* GPIOE Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/* Configure PD12, PD13 and PD14 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOE, STORE);
	GPIO_ResetBits(GPIOE, DATA);
	GPIO_ResetBits(GPIOE, SHIFT);

	ADC1CH13_ADC3CH12_Config();
	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC3);

	initializeSnake();

	/* LCD set-up */
	IOE_Config();
	/* Initialize the LCD */
	STM32f4_Discovery_LCD_Init();
	/* Clear the LCD */
	LCD_Clear(White);
	/* Set the LCD Text size */
	LCD_SetFont(&Font8x12);
	/* Initializing touch-screen scaling values and offsets */
	//LCD_Touch_Calibration();
	InitTouch(-0.091488, 0.063837, -356, 11);
	LCD_DrawPicture(0, 0, 320, 240, (uint8_t*) bg_image);
	LCD_DrawPicture(0, 120, 160, 123, (uint8_t*) snake_pressed_image);
	LCD_DrawPicture(159, 120, 160, 123, (uint8_t*) arkanoid_image);


	SetRelAlarm(AlarmMatrix, 10, 3);
	SetRelAlarm(AlarmJoystick, 15, 300);
	SetRelAlarm(AlarmLCD, 30, 100);

	/* Forever loop: background activities (if any) should go here */
	for (;;);

}

