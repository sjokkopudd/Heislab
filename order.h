

void set_order(int floor, int button);
// Tar inn etasje, hvilken knapp, og setter bestillingen og lyset på knappen


void reset_floor_orders(int floor);
// Tar inn etasje og fjerner bestillingene og slår av lysene på knappene

int check_if_orders_empty(int orders);
// Sjekker om noen har gjort en bestilling, returnerer 1 hvis listen er tom

int check_floor(int floor, elev_motor_direction_t dir, int orders);