#ifndef _STATE_MACHINE_H_
#define _STATE_MACHINE_H_

typedef enum state {BETWEEN_FLOORS, ON_FLOOR, STOPP, ERROR} state;
//en enum for å holde styr på hvilken tilstand heisen er i

state get_state ();
//funksjonen som oversetter heissignal til en (enum) state

void state_machine ();
//funksjonen (switchen) som forteller systemet hva som skal gjøres basert på state

void start_position();
//setter heisen i startposisjon 
#endif