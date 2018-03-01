#include "elev.h"
#include "heistilstand.h"
#include "io.h"
#include "channels.h"
#include <stdio.h>


int main() {
    if (!elev_init()) //initialiserer hardware
    {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");

    start_position(); // setter heisen til startposisjon

    while (1) 
    {
        if (io_read_bit(OBSTRUCTION)) 
        {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }

        state_machine();
    }

    return 0;
}
