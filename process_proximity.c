#include "process_proximity.h"
#include "sensors/proximity.h"
#include "leds.h"
#include <stdbool.h>



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


void ir_state(bool* tab, int sensor){
	if (get_calibrated_prox(sensor)>IR_THRESHOLD){
		tab[sensor] = true;

	}
	else{
		tab[sensor] = false;

	}
}

void get_ir_states(bool* tab){
	for (int i=0; i < NUMBER_SENSORS; ++i){
		ir_state(tab, i);
	}
}

/*
static THD_WORKING_AREA(waProximity, 256);
static THD_FUNCTION(Proximity, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	ir_state(IR_FRONT_RIGHT,LED1);
    	//ir_state(IR_FRONT_RIGHT45,LED3);
    	ir_state(IR_RIGHT,LED3);
    	//ir_state(IR_BACK_RIGHT,LED5);
    	ir_state(IR_BACK_LEFT,LED5);
    	ir_state(IR_LEFT,LED7);
    	//ir_state(IR_FRONT_LEFT45,LED7);
    	//ir_state(IR_FRONT_LEFT,LED1);

    	if (get_calibrated_prox(IR_FRONT_RIGHT)>IR_THRESHOLD){
    		ir_states[IR_FRONT_RIGHT] = true;
    		set_led(LED1,2);									//Temporary (for test)
    	}
    	else{
    		ir_states[IR_FRONT_RIGHT] = false;
    		set_led(LED1,0);
    	}

    	if (get_calibrated_prox(IR_FRONT_RIGHT45)>IR_THRESHOLD){
    		ir_states[IR_FRONT_RIGHT45] = true;
    		set_led(LED2,2);									//Temporary (for test)
    	}
    	else{
    		ir_states[IR_FRONT_RIGHT45] = false;
    		set_led(LED2,0);
    	}

    	if (get_calibrated_prox(IR_RIGHT)>IR_THRESHOLD){
    		ir_states[IR_RIGHT] = true;
    		set_led(LED3,2);									//Temporary (for test)
    	}
    	else{
    		ir_states[IR_RIGHT] = false;
    		set_led(LED3,0);
    	}

    	if (get_calibrated_prox(IR_BACK_RIGHT)>IR_THRESHOLD){
    		ir_states[IR_BACK_RIGHT] = true;
    		set_led(LED5,2);									//Temporary (for test)
    	}
    	else{
    		ir_states[IR_BACK_RIGHT] = false;
    		set_led(LED5,0);
    	}

    	if (get_calibrated_prox(IR_BACK_LEFT)>IR_THRESHOLD){
    		ir_states[IR_BACK_LEFT] = true;
    		set_led(LED6,2);									//Temporary (for test)
    	}
    	else{
    		ir_states[IR_BACK_LEFT] = false;
    		set_led(LED6,0);
    	}

    	if (get_calibrated_prox(IR_LEFT)>IR_THRESHOLD){
    		ir_states[IR_LEFT] = true;
    		set_led(LED7,2);
    	}
    	else{
    		ir_states[IR_LEFT] = false;
    		set_led(LED7,0);
    	}


        chThdSleepMilliseconds(SLEEP50HZ); // To be determined
    }
}



void measure_proximity_start(void){
	chThdCreateStatic(waProximity, sizeof(waProximity), NORMALPRIO, Proximity, NULL);
}
*/


