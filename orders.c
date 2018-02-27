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
	print_orders();
}

void check_order_buttons() 
{
	for(int floor = 0; floor < 4; floor++)
	{
		for(int button = 0; button < 3; button++)
		{
			if (!(button == 0 && floor == 0) || !(button == 1 && floor == 3))// sørger for at vi ikke kaller ned etg 1 og opp etg 4.
			{
				if(elev_get_button_signal(button, floor))
				{
					set_order(floor, button);
				}
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


int check_floor_orders(int floor) 
{	
	int direction = io_read_bit(MOTORDIR);
	if ( direction && ((orders[floor][0]) || orders[floor][2])) //Hvis retning ned og (bestilling ned eller bestilling til denne etasjen)
	{
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		reset_floor_orders(floor);
		return 1;
	}
	if ( !direction && (orders[floor][1] || orders[floor][2])) //Hvis retning opp og (bestilling opp eller bestilling til denne etasjen)
	{
		elev_set_motor_direction(DIRN_STOP);
		elev_set_door_open_lamp(1);
		reset_floor_orders(floor);
		return 1;
	}
	//printf("no orders \n");
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