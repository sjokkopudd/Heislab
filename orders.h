#ifndef _ORDERS_H_
#define _ORDERS_H_


void set_order(int floor, int button);
// Tar inn etasje, hvilken knapp, og setter bestillingen og lyset på knappen

void check_order_buttons();
//Sjekker om noen bestillingsknapper blir trykket inn, legger bestillingen i listen

void reset_floor_orders(int floor);
// Tar inn etasje og fjerner bestillingene og slår av lysene på knappene

int check_if_orders_empty();
// Sjekker om det er noen bestillinger, returnerer 1 hvis listen er tom, 0 ellers

int check_floor_orders(int floor);
//Sjekker etasjen heisen er i om noen av bestillingene matcher med noen andre
// bestillinger, returnerer 1 hvis den matcher, 0 ellers

void reset_all_orders();
//Fjerner alle bestillinger

void next_order(int floor) ;
//Finner hvilken retning den neste bestillingen er i, stopper hvis det ikke er noen 

void set_direction_after_stop( int last_floor);
//Finner hvilken retning heisen skal fortsette i etter stopp
#endif