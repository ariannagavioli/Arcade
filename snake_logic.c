#include <stdlib.h>
#include <time.h>
#include "snake_logic.h"

/* MAX_X and MAX_Y set to a default value */
#define MAX_X 8
#define MAX_Y 8

char snake_matrix[8][8] =
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

int previous_x, previous_y, previous_vx, previous_vy, tail_x, tail_y;

void relocateApple(){
	int next_position_x, next_position_y;
	int valid = 0;
	while(!valid){
		next_position_x = rand() % MAX_X;
		next_position_y = rand() % MAX_Y;

		if(snake_matrix[next_position_y][next_position_x] == '0'){
			snake_matrix[next_position_y][next_position_x] = '*';
			valid = 1;
		}
	}
}

void initializeSnake(){

	for(uint8_t i = 0; i < MAX_Y; i++){
		for(uint8_t j = 0; j < MAX_X; j++){
			snake_matrix[i][j] = '0';
		}
	}

	snake_matrix[4][4] = '-';
	previous_x = 4;
	previous_y = 4;
	tail_x = 4;
	tail_y = 4;
	previous_vx = -1;
	previous_vy = 0;

	srand(time(NULL));

	relocateApple();
}

void updateSnake(int8_t vx, int8_t vy) {

	int8_t x, y;
	x = previous_x;
	y = previous_y;

	if( (vx == 0 && vy == 0) ||
		(vx == -(previous_vx)) ||
		(vy == -(previous_vy))) {
		vx = previous_vx;
		vy = previous_vy;
	}

	// If user tries to go into diagonal
	if(vx != 0 && vy != 0)
		vy = 0;

	x += vx;
	y += vy;

	x = (x + MAX_X) % MAX_X;
	y = (y + MAX_Y) % MAX_Y;

	if(snake_matrix[y][x] != '0' && snake_matrix[y][x] != '*') {
		initializeSnake();
		return;
	}

	/* Advancing the head of the snake */
	if(vx == -1)
		snake_matrix[previous_y][previous_x] = 'l';
	else if(vx == 1)
		snake_matrix[previous_y][previous_x] = 'r';
	else if(vy == -1)
		snake_matrix[previous_y][previous_x] = 'd';
	else if(vy == 1)
		snake_matrix[previous_y][previous_x] = 'u';

	/* Updating the tail if the snake didn't eat the apple */
	if(snake_matrix[y][x] == '0'){
		int8_t prev_tail_x = tail_x;
		int8_t prev_tail_y = tail_y;
		char tail = snake_matrix[tail_y][tail_x];
		switch(tail) {
			case 'l':
				tail_x--;
				tail_x = (tail_x + MAX_X) % MAX_X;
				break;
			case 'r':
				tail_x++;
				tail_x %= MAX_X;
				break;
			case 'd':
				tail_y--;
				tail_y = (tail_y + MAX_Y) % MAX_Y;
				break;
			case 'u':
				tail_y++;
				tail_y %= MAX_Y;
				break;
			default:
				break;
		}
		snake_matrix[prev_tail_y][prev_tail_x] = '0';
	}
	/* Or else, the snake ate the apple, so let's relocate the fruit */
	else
		relocateApple();

	snake_matrix[y][x] = '-';

	previous_x = x;
	previous_y = y;

	previous_vx = vx;
	previous_vy = vy;
}
