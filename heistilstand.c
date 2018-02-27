
#include "heistilstand.h"
#include "elev.h"
#include "orders.h"
#include "channels.h"
#include "io.h"
#include <stdio.h>
#include <time.h>

static int current_floor;

void initialize()
{
	while(elev_get_floor_sensor_signal() == -1)
	{
		elev_set_motor_direction(DIRN_DOWN);
	}
	elev_set_motor_direction(DIRN_STOP);
}	

state get_state()
{
	state return_state;

	if (elev_get_stop_signal())
	{
		return return_state = STOPP;
	}

	else if (current_floor == -1)
	{
		return return_state = BETWEEN_FLOORS;
	}

	else if (current_floor >= 0)
	{
		return return_state = ON_FLOOR;
	}

	return return_state = ERROR;
}

void state_machine ()
{
	current_floor = elev_get_floor_sensor_signal();
	switch (get_state())
	{
		case (STOPP):
		{
			elev_set_motor_direction (DIRN_STOP); 
			reset_all_orders();
			printf("Stop\n");
			break;
		}

		case (BETWEEN_FLOORS):
		{
			check_order_buttons();
			break;
		}

		case (ON_FLOOR):
		{
			elev_set_floor_indicator(current_floor); // Setter etasjeindikatorlyset
			check_order_buttons();

			if (current_floor == N_FLOORS - 1) // Bytter etasjeretning dersom heisen når etasje 1 eller 4
			{
            	elev_set_motor_direction(DIRN_DOWN);
        	} 
        	else if (current_floor == 0) 
        	{
            	elev_set_motor_direction(DIRN_UP);
        	}

			if (check_floor_orders(current_floor))// Denne sjekker om det er bestillinger i denne etasjen, og stopper i såfall heisen
			{
				reset_floor_orders(current_floor);
				int timer = time(NULL);// venter i 3 sekunder, men sjekker knappene imens. Dersom stopp, får vi en break og state machine går til stop case.
				while (time(NULL) < (timer + 3))
				{
					if (elev_get_stop_signal())
					{
						break;
					}

					check_order_buttons();
				}
				printf("checked \n");
				elev_set_door_open_lamp(0);// lukker døra
			}

			next_order(current_floor);
			
			//printf("on_floor\n");
			break;
		}

		default :
		{
			printf("something went wrong");
			break;
		}
	}
}