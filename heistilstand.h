#ifndef _HEISTILSTAND_H_
#define _HEISTILSTAND_H_

//en enum for å holde styr på hvilken tilstand heisen er i
typedef enum state {BETWEEN_FLOORS, ON_FLOOR, STOPP, ERROR} state;

//funksjonen som oversetter heissignal til en (enum) state
state get_state ();

//funksjonen (switchen) som forteller systemet hva som skal gjøres basert på state
void state_machine ();

#endif