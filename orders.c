
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
			if (!((button == 1 && floor == 0) || (button == 0 && floor == 3))) //Sørger for at vi ikke kaller get_button på ned etg.1 eller opp etg.4
			{  
				if (elev_get_button_signal(button, floor))
				{
					set_order(floor, button);
				}
			}	
		}
	}
}

void reset_floor_orders(int floor) 
{
	for(int button = 0; button < 3; button++) 
	{
		if (!((button == 1 && floor == 0) || (button == 0 && floor == 3))) 
		{
		orders[floor][button] = 0;
		elev_set_button_lamp(button, floor, 0);
		}
	}
}


int check_if_orders_empty()
{
	int empty = 1;
	for (int floor = 0; floor < 4; floor++) 
	{
		for(int button = 0; button < 3; button++) 
		{
			if (!((button == 1 && floor == 0)) || !((button == 0 && floor == 3)))
			{
				if (orders[floor][button] != 0) 
				{
				empty = 0;
				}
			}
		}
	}
	return empty;
}


int check_floor_orders(int floor) 
{
	int direction = io_read_bit(MOTORDIR);
	if ((direction == 1) && ((orders[floor][1] == 1) || (orders[floor][2] == 1))) {
		return 1;
	}
	if ((direction == 0) && ((orders[floor][0] == 1) || (orders[floor][2] == 1))) {
		return 1;
	}
	return 0;
}

void reset_all_orders() 
{
	for(int floor = 0; floor < 4; floor++)
	{
		reset_floor_orders(floor);
	}		
}

void next_order(int floor) 
{	
	int direction = io_read_bit(MOTORDIR);
	if(direction == 0) 
	{
		for (int next_floor = floor; next_floor <= 3; next_floor++)
		{
			if ((orders[next_floor][0] == 1) || (orders[next_floor][2] == 1))
			{
				elev_set_motor_direction(DIRN_UP);
				return; //Denne løkken sjekker om flere bestillinger opp eller inni over etasjen heisen er i med retning opp.
			}
		} 
		for (int next_floor = 3; next_floor >= 0; next_floor--)
		{
			if ((orders[next_floor][1] == 1) || (orders[next_floor][2] == 1) )
			{
				if (next_floor > floor) 
				{
					elev_set_motor_direction(DIRN_UP);
					return;
				}
				else 
				{
					elev_set_motor_direction(DIRN_DOWN);
					return;
				}
			} //Sjekker om noen over skal ned hvis ingen skal opp.
				
		}
		for (int next_floor = 0; next_floor < floor; next_floor++)
		{
			if (orders[next_floor][0] == 1)
				elev_set_motor_direction(DIRN_DOWN);
				return; //Sjekker om noen under skal opp til slutt. 
		} 
	}
	else
	{
		for (int next_floor = floor; next_floor >= 0; next_floor--)
		{
			if ((orders[next_floor][1] == 1) || (orders[next_floor][2] == 1))
			{
				elev_set_motor_direction(DIRN_DOWN);
				return; //Denne løkken sjekker om flere bestillinger ned eller inni under etasjen heisen er i med retning ned.
			}
		} 
		for (int next_floor = 0; next_floor <= 3; next_floor++)
		{
			if ((orders[next_floor][0] == 1) || (orders[next_floor][2] == 1) )
			{
				if (next_floor < floor) 
				{
					elev_set_motor_direction(DIRN_DOWN);
					return;
				}
				else 
				{
					elev_set_motor_direction(DIRN_UP);
					return;
				}
			}//Sjekker om noen under skal opp hvis ingen skal ned.
				
		}
		for (int next_floor = 3; next_floor > floor; next_floor--)
		{
			if (orders[next_floor][1] == 1)
				elev_set_motor_direction(DIRN_UP);
				return; //Sjekker om noen over skal ned til slutt.
		}
	}
}

void set_direction_after_stop( int last_floor)
{
	if (io_read_bit(MOTORDIR)) //for retning ned
	{
		for (int next_floor = 0; next_floor < last_floor; next_floor++)
		{
			for (int button = 0; button < 3; button++)
			{
				if (orders[next_floor][button])
				{
					elev_set_motor_direction(DIRN_DOWN);
					return;
				}//itererer gjennom knappene for hver etasje under heisen, of dersom det er en bestilling settes retning ned
			}
		}

		elev_set_motor_direction(DIRN_UP);
		return;
	}//ellers settes retning opp

	else //for retning opp
	{
		for (int next_floor = 0; next_floor <= last_floor; next_floor++)
			{
				for (int button = 0; button < 3; button++)
				{
					if (orders[next_floor][button])
					{
						elev_set_motor_direction(DIRN_DOWN);
						return;
					}//itererer gjennom knappene for hver etasje under heisen, of dersom det er en bestilling settes retning ned
				}
			}

		elev_set_motor_direction(DIRN_UP);
		return;
	}//ellers settes retning opp
}