#include "arkanoid_logic.h"
#include <stdlib.h>

#include <stdio.h>

#define MAX_X 8
#define MAX_Y 8

uint8_t is_active	= 0;
int8_t left_bar_x	= 0;
int8_t right_bar_x	= 0;
int8_t ball_x		= 3;
int8_t ball_y		= 6;
int8_t ball_vx		= 0;
int8_t ball_vy		= 0;
int8_t next_ball_x	= 0;
int8_t next_ball_y	= 0;

char arkanoid_matrix[8][8] =
{
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'},
	{'0', '0', '0', '0', '0', '0', '0', '0'}
};

void initializeArkanoid() {
	for(uint8_t i = 0; i < MAX_Y; i++) {
		for(uint8_t j = 0; j < MAX_X; j++) {
			/* First five rows contain bricks */
			if(i < 4)
				arkanoid_matrix[i][j] = '-';
			/* Following row is completely free */
			else if(i < 6)
				arkanoid_matrix[i][j] = '0';
			/* Second to last row contains the ball in the middle */
			else if(j == 3 && i == 6)
				arkanoid_matrix[i][j] = '*';
			else if(i == 6)
				arkanoid_matrix[i][j] = '0';
			/* Last row contains the playing bar */
			else if(j >= 2 && j <= 4)
				arkanoid_matrix[i][j] = 'b';
			else
				arkanoid_matrix[i][j] = '0';
		}
	}

	is_active	= 0;
	left_bar_x	= 2;
	right_bar_x = 4;
	ball_x = 3;
	ball_y = 6;

}

void updateArkanoid(int vx) {
	if(is_active) {
		/* Updating bar position */

		if(vx < 0 && left_bar_x > 0) {						// Moving to the left
			arkanoid_matrix[MAX_Y - 1][right_bar_x] = '0';
			right_bar_x--;
			left_bar_x--;
			arkanoid_matrix[MAX_Y - 1][left_bar_x] = 'b';
		}

		else if(vx > 0 && right_bar_x < (MAX_X - 1)) {		// Moving to the right
			arkanoid_matrix[MAX_Y - 1][left_bar_x] = '0';
			right_bar_x++;
			left_bar_x++;
			arkanoid_matrix[MAX_Y - 1][right_bar_x] = 'b';
		}

		/* Updating ball position */
		arkanoid_matrix[ball_y][ball_x] = '0';
		if(ball_x == (MAX_X - 1) || ball_x == 0)	// Bouncing laterally
			ball_vx = -ball_vx;
		if(ball_y == (MAX_Y - 1)){					// Lost
			initializeArkanoid();
			return;
		}
		if(ball_y == 0)								// Touched the ceiling
			ball_vy = - ball_vy;
		next_ball_x = ball_x + ball_vx;
		next_ball_y = ball_y + ball_vy;

		/* Checking next ball position */
		char next_ball_pos = arkanoid_matrix[next_ball_y][next_ball_x];
		switch(next_ball_pos){
			case '-':			// Hitting a brick
				if(arkanoid_matrix[next_ball_y][ball_x] != '0'){
					arkanoid_matrix[next_ball_y][ball_x] = '0';
					ball_vy = -ball_vy;
				}
				else if(arkanoid_matrix[ball_y][next_ball_x] != '0'){
					arkanoid_matrix[ball_y][next_ball_x] = '0';
					ball_vx = -ball_vx;
				}
				else{
					arkanoid_matrix[next_ball_y][next_ball_x] = '0';
					ball_vy = -ball_vy;
				}
				break;
			case 'b':			// Hitting the bar
				ball_vy = -ball_vy;
				ball_vx += vx;
				ball_vx %= 2;
				/*
				if(vx == -ball_vx)
					ball_vx = 0;
				if(ball_vx == 0){
					if(ball_x == right_bar_x)
						ball_vx = 1;
					else if(ball_x == left_bar_x)
						ball_vx = -1;
				}*/
			default:
				break;
		}

		ball_x += ball_vx;
		ball_y += ball_vy;

		if(ball_x < 0)
			ball_x = 0;
		else if (ball_x == MAX_X)
			ball_x = MAX_X -1;

		arkanoid_matrix[ball_y][ball_x] = '*';
	}
	/* If the game is not active and vx != 0, the user is starting to play */
	else if(vx != 0) {
		is_active = 1;
		ball_vx = vx;
		ball_vy = -1;
		/* Updating bar position */
		if(vx < 0) {					// Moving to the left
			arkanoid_matrix[MAX_Y - 1][right_bar_x] = '0';
			right_bar_x--;
			left_bar_x--;
			arkanoid_matrix[MAX_Y - 1][left_bar_x] = 'b';
		}
		else if(vx > 0) {				// Moving to the right
			arkanoid_matrix[MAX_Y - 1][left_bar_x] = '0';
			right_bar_x++;
			left_bar_x++;
			arkanoid_matrix[MAX_Y - 1][right_bar_x] = 'b';
		}
	}
}
