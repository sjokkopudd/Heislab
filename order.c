#include <"order.h">
#include <"elev.h">
#include <"channels.h">
#include <"io.h">
#include <"main.c">

int orders[4][3] = {{0}};

void set_order(int floor, int button, int orders) 
{
	orders[floor][button] = 1;
	elev_set_buttonlamp(elev_button_type_t button,floor, 1);

}

void check_order_buttons(int orders) 
{
	for(int floor = 0; floor < 4; floor++)
	{
		for(int button = 0; button < 3; button++)
		{
			if(elev_get_button_signal(elev_button_type_t button, floor))
			{
				set_order(floor, button, 1);
			}
		}
	}
}

void reset_floor_orders(int floor) 
{
	for(int button = 0; button < 3; button++) {
		orders[floor][button] = 0;
		elev_set_buttonlamp(elev_button_type_t button, floor, 0);
	}
}


int check_if_orders_empty(int orders)
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


int check_floor(int floor, elev_motor_direction_t dir, int orders) 
{
	if ((dir == -1) && ((orders[floor][0] == 1) || (orders[floor][2] == 1))) {
		elev_set_motor_direction(DIRN_STOP);
		//elev_set_door_open_lamp(1);
	}
	if ((dir == 1) && ((orders[floor][1] == 1) || (orders[floor][2] == 1))) {
		elev_set_motor_direction(DIRN_STOP);
		//elev_set_door_open_lamp(1);
	}

}