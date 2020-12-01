#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

/* Returns -1 if it's on the left, 1 on the right, 0 in the middle */
char get_x(uint32_t convertedVoltageADC);
/* Returns -1 if it's on the bottom, 1 on the top, 0 in the middle */
char get_y(uint32_t convertedVoltageADC);

#endif
