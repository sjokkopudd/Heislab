
#include "state_machine.h"
#include "elev.h"
#include "orders.h"
#include "channels.h"
#include "io.h"
#include <stdio.h>
#include <time.h>

static int current_floor;

void start_position()
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
			elev_set_motor_direction (DIRN_STOP); //stopper heisen
			reset_all_orders(); //sletter alle bestillinger
			printf("Stopp\n");

			if (current_floor >= 0) //åpner døra dersom heisen er i en etasje
			{
				elev_set_door_open_lamp(1);
			}

			if (!elev_get_stop_signal())
			{
				if (current_floor >= 0) //hvis heiskanppen slippes når heisen allerede er i en etasje, venter heisen med åpen dør i 3 sek, så går videre til neste case
				{
					int timer = time(NULL);
					while (time(NULL) < (timer + 3))
					{
						printf("Waiting\n");
						check_order_buttons();
					}
					elev_set_door_open_lamp(0);
					break;
				}

				else
				{
					while(check_if_orders_empty())
					{
						check_order_buttons();
						printf("Waiting for order \n");
					}
					elev_set_motor_direction(DIRN_DOWN);
					break;
				}

			}
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

			if (check_floor_orders(current_floor))// Denne sjekker om det er bestillinger i denne etasjen, og stopper i såfall heisen
			{
				reset_floor_orders(current_floor);
				int timer = time(NULL);// venter i 3 sekunder, men sjekker knappene imens. Dersom stopp, får vi en break og state machine går til stopp case.
				while (time(NULL) < (timer + 3))
				{
					if (elev_get_stop_signal())
					{
						break;
					}

					check_order_buttons();
				}
				printf("door opened \n");
				elev_set_door_open_lamp(0);// lukker døra
			}

			if (check_if_orders_empty())
			{
				printf("empty \n");
				elev_set_motor_direction(DIRN_STOP);
				break;
			}		

			next_order(current_floor); //bestemmer hvilken bestilling som skal behandles neste
			printf("Checked next order \n");	
			break;
		}

		default :
		{
			printf("something went wrong");
			break;
		}
	}
}