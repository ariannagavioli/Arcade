#include "ee.h"
#include "stm32f4xx.h"
#include "joystick.h"

/* Returns -1 if it's on the left, 1 on the right, 0 in the middle */
char get_x(uint32_t convertedVoltageADC){

	if(convertedVoltageADC > 3000)		//left
		return 1;
	if(convertedVoltageADC < 300)		//right
		return -1;
	else
		return 0;
}

/* Returns -1 if it's on the bottom, 1 on the top, 0 in the middle */
char get_y(uint32_t convertedVoltageADC){

	if(convertedVoltageADC < 300)		//up
		return -1;
	if(convertedVoltageADC > 3000)		//down
		return 1;
	else
		return 0;
}
