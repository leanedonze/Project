#include "process_proximity.h"
#include "sensors/proximity.h"
#include "leds.h"


#define SLEEP50HZ			20
#define IR_THRESHOLD		50		//tells the distance limit to an object before turning

#define IR_FRONT_RIGHT		0
#define IR_FRONT_RIGHT45	1
#define IR_RIGHT			2
#define IR_BACK_RIGHT		3
#define IR_BACK_LEFT		4
#define IR_LEFT				5
#define IR_FRONT_LEFT45		6
#define IR_FRONT_LEFT		7


static THD_WORKING_AREA(waProximity, 256);
static THD_FUNCTION(Proximity, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	if (get_calibrated_prox(IR_RIGHT)>IR_THRESHOLD){
    		set_led(LED3,2);									//Temporary (for test)
    	}
    	else{
    		set_led(LED3,0);
    	}
    	if (get_calibrated_prox(IR_LEFT)>IR_THRESHOLD){
    		set_led(LED7,2);
    	}
    	else{
    		set_led(LED7,0);
    	}


        chThdSleepMilliseconds(SLEEP50HZ); // To be determined
    }
}



void measure_proximity_start(void){
	chThdCreateStatic(waProximity, sizeof(waProximity), NORMALPRIO, Proximity, NULL);
}
