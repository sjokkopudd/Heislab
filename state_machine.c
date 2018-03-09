
#include "heistilstand.h"
#include "elev.h"
#include "orders.h"
#include "channels.h"
#include "io.h"
#include <stdio.h>
#include <time.h>
#include <math.h>

static int current_floor;
static int floor_lamp_value;

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
			elev_set_stop_lamp(1);
			reset_all_orders(); //sletter alle bestillinger

			if (current_floor >= 0) //åpner døra dersom heisen er i en etasje
			{
				elev_set_door_open_lamp(1);
			}

			if (!elev_get_stop_signal())
			{
				elev_set_stop_lamp(0);//slukker stoplampen
				if (current_floor >= 0) //hvis heisknappen slippes når heisen allerede er i en etasje, venter heisen med åpen dør i 3 sek, så går videre til neste case
				{
					int timer = time(NULL);
					while (time(NULL) < (timer + 3))
					{
						if (elev_get_stop_signal())
						{
							break;
						}

						check_order_buttons();
					}
					elev_set_door_open_lamp(0);
					break;
				}

				else
				{
					while(check_if_orders_empty())//hvis heisen er mellom to etasjer, venter den til den har fått en bestilling for å så velge hvilken retning den skal fortsette i
					{
						if (elev_get_stop_signal())
						{
							break;
						}

						check_order_buttons();
					}

					set_direction_after_stop(floor_lamp_value);
					break;
				}

			}
			break;
		}

		case (BETWEEN_FLOORS):
		{
			elev_set_stop_lamp(0);//sørger for at stoplampen ikke lyser dersom vi kommer til BETWEEN_FLOOR case (sikkerhet)
			check_order_buttons();
			break;
		}

		case (ON_FLOOR):
		{
			elev_set_floor_indicator(current_floor); // Setter etasjeindikatorlyset
			floor_lamp_value = current_floor;
			check_order_buttons();

			elev_set_stop_lamp(0);//sørger for at døren er lukket og stoplampen er slukket når vi kommer til ON_FLOOR case (sikkerhet)
			elev_set_door_open_lamp(0);

			if (check_floor_orders(current_floor))// Denne sjekker om det er bestillinger i denne etasjen
			{
				elev_set_motor_direction(DIRN_STOP);//Stopper heisen og åpner døren
				elev_set_door_open_lamp(1);

				int timer = time(NULL);// venter i 3 sekunder, men sjekker knappene imens. Dersom stopp, får vi en break og state machine går til stopp case.
				while (time(NULL) < (timer + 3))
				{
					if (elev_get_stop_signal())
					{
						break;
					}
					reset_floor_orders(current_floor);//resetter ordre fra etasjen heisen allerede er i 
					check_order_buttons();

				}
				elev_set_door_open_lamp(0);// lukker døra
			}

			if (check_if_orders_empty())
			{
				elev_set_motor_direction(DIRN_STOP);
				break;
			}		

			next_order(current_floor); //bestemmer hvilken bestilling som skal behandles neste	
			break;
		}

		default :
		{
			break;
		}
	}
}