#include "stm32f4xx.h"
#include "led_matrix.h"

/* Shifts a character value in the shift registers */
void shiftOut(uint16_t data_pin, uint16_t shift_pin, char data) {
	uint8_t i;
	/* Analyzing one bit of the char at a time */
	for (i = 0; i < 8; i++) {
		if((data & 1 << i))
			GPIO_SetBits(GPIOE, data_pin);
		else
			GPIO_ResetBits(GPIOE, data_pin);
		/* Sending the shifting signal to the shift register */
		GPIO_SetBits(GPIOE, shift_pin);
		GPIO_ResetBits(GPIOE, shift_pin);
	}

}

/* Stores the values in the shift register */
void store(uint16_t store_pin) {
	GPIO_SetBits(GPIOE, store_pin);
	GPIO_ResetBits(GPIOE, store_pin);
}

/* Displays the i-th led matrix row */
void displayRow(uint16_t data_pin, uint16_t shift_pin, uint16_t store_pin, uint8_t i, char value) {
	shiftOut(data_pin, shift_pin, ~value);			// Columns' configuration of the i-th row
	shiftOut(data_pin, shift_pin, 128 >> i);		// Activating the i-th row
	store(store_pin);
}
