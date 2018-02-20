

void set_order(int floor, int button);
// Tar inn etasje, hvilken knapp, og setter bestillingen og lyset på knappen

void check_order_buttons();
//Sjekker om noen bestillingsknapper blir trykket inn, legger bestillingen i listen

void reset_floor_orders(int floor);
// Tar inn etasje og fjerner bestillingene og slår av lysene på knappene

int check_if_orders_empty();
// Sjekker om det er noen bestillinger, returnerer 1 hvis listen er tom

int check_floor_orders();
//Sjekker etasjen heisen er i om noen av bestillingene matcher med noen andre bestillinger

void reset_orders();
//Fjerner alle bestillinger