#include "orders.h"
#include "elev.h"
#include "channels.h"
#include "io.h"
#include <stdio.h>

static int orders[4][3] = {{0}};

void set_order(int floor, int button) 
{
	orders[floor][button] = 1;
	elev_set_button_lamp(button,floor, 1);
}

void check_order_buttons() 
{
	for(int floor = 0; floor < 4; floor++)
	{
		for(int button = 0; button < 3; button++)
		{
			if(elev_get_button_signal(button, floor))
			{
				set_order(floor, button);
			}
		}
	}
}

void reset_floor_orders(int floor) 
{
	for(int button = 0; button < 3; button++) {
		orders[floor][button] = 0;
		elev_set_button_lamp(button, floor, 0);
	}
}


int check_if_orders_empty()
{
	int empty = 1;
	for (int floor = 0; floor < 4; floor++) 
	{
		for(int button = 0; button < 3; button++) 
		{
			if (button != 0) 
			{
				empty = 0;
			}
		}
	}
	return empty;
}


int check_floor_orders() 
{
	int floor = elev_get_floor_sensor_signal();	
	int direction = io_read_bit(MOTORDIR);
	if ((direction == 1) && ((orders[floor][0] == 1) || (orders[floor][2] == 1))) {
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		reset_floor_orders(floor);
		return 1;
	}
	if ((direction == 0) && ((orders[floor][1] == 1) || (orders[floor][2] == 1))) {
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		reset_floor_orders(floor);
		return 1;
	}
	return 0;
}

void reset_orders() 
{
	for(int floor = 0; floor < 4; floor++)
	{
		for(int button = 0; button < 3; button++)
		{
			orders[floor][button] = 0;
		}
	}		
}

void print_orders()
{
	int row, column;
	for (row = 0; row < 4; row ++)
	{
		for (column = 0; column < 3; column ++)
		{
			printf("%d    ", orders[row][column]);
		}
		printf("\n");
	}
}

void next_order() 
{
	int floor = elev_get_floor_sensor_signal();	
	int direction = io_read_bit(MOTORDIR);
	if((direction == 0) && (floor != 3)) 
	{
		for (int next_floor = floor + 1; next_floor <= 3; next_floor++) 
		{
			if((orders[next_floor][0] == 1) || (orders[next_floor][1] == 1) || (orders[next_floor][2] == 1)) 
			{
				io_clear_bit(MOTORDIR);
				return;
			}
			
	}
		if(check_if_orders_empty() == 0) 
		{
				io_set_bit(MOTORDIR);
				return;
		}
	else if((direction == 1) && (floor != 0 )) 
	{
		for (int next_floor = floor - 1; next_floor >= 0; next_floor--) 
		{
			if((orders[next_floor][0] == 1) || (orders[next_floor][1] == 1) || (orders[next_floor][2] == 1))  
			{
				io_set_bit(MOTORDIR);
				return;
			}
			

		}
		if(check_if_orders_empty() == 0) 
		{
			io_clear_bit(MOTORDIR);
			return;
		}
}

