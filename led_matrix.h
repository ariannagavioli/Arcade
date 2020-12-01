#ifndef _LED_MATRIX_H_
#define _LED_MATRIX_H_

#define N_ROWS 8
#define N_COLS 8

/* Displays the i-th led matrix row */
void displayRow(uint16_t data_pin, uint16_t shift_pin, uint16_t store_pin, uint8_t i, char value);

#endif
