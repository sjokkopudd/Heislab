
#include "heistilstand.h"
#include "elev.h"
#include <stdio.h>
#include <time.h>

state get_state()
{
	state return_state;

	if (elev_get_stop_signal())
	{
		return return_state = STOP;
	}

	else if (elev_get_floor_sensor_signal() == -1)
	{
		return return_state = BETWEEN_FLOORS;
	}

	else if (elev_get_floor_sensor_signal() >= 0)
	{
		return return_state = ON_FLOOR;
	}

	return return_state = error;
}

void state_machine ()
{
	switch (get_state())
	{
		case (STOP):
		{
			elev_set_motor_direction (DIRN_STOP); 
			for (int floor = 0; floor < 4; floor ++) //sletter alle ordre
			{
				reset_floor_orders(floor);
			}
			printf("Stop\n");
			break;
		}

		case (BETWEEN_FLOORS):
		{
			printf("between_floors\n");
			break;
		}

		case (ON_FLOOR):
		{
			if (check_floor_orders())// Denne sjekker om det er bestillinger i denne etasjen, og stopper i såfall heisen
			{
				int timer = time(NULL);// venter i 3 sekunder, men sjekker knappene imens. Dersom stopp, får vi en break og state machine går til stop case.
				while (time < (timer + 3))
				{
					if (elev_get_stop_signal)
					{
						break;
					}

					check_order_buttons();
				}
				elev_set_door_lamp(0);// lukker døra
				//getnext
			}
			

			printf("on_floor\n");
			break;
		}

		default :
		{
			printf("sopmething went wrong");
			break;
		}
	}
}