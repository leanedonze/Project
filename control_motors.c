/*
 * control_motors.c
 *
 *  Created on: 15 Apr 2021
 *      Author: 41774
 */

#include <stdbool.h>
#include "process_proximity.h"
#include "leds.h"
#include "motors.h"
#include "control_motors.h"

#define	FREQU_MOTORS	1	//D'après le cours, fréquence thread motor 1kHz -> à checker

static bool ir_states[NUMBER_SENSORS];

static bool no_obstacle[NUMBER_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};


void audio_control(){
	left_motor_set_speed(600);
	right_motor_set_speed(600);
}

void proximity_control(){
	if ((ir_states[IR_FRONT_RIGHT] == 1) || (ir_states[IR_FRONT_LEFT] == 1)){		//Check if obstacle on front
		if(ir_states[IR_LEFT] == 0){												//If if obstacle on the left
			left_motor_set_speed(-200);												//If not, turn left
			right_motor_set_speed(200);
		}
		else if(ir_states[IR_RIGHT] == 0){											//Check if obstacle on the right
			left_motor_set_speed(200);												//If not, turn right
			right_motor_set_speed(-200);
		}
	}
}

bool compare_tab(bool* tab1, bool* tab2){
	for (int i=0; i < NUMBER_SENSORS; ++i){
		if((tab1[i]) != (tab2[i])){
			return false;
		}
	}
	return true;
}

static THD_WORKING_AREA(waMotors, 256);	//Checker taille à réserver sur la stack
static THD_FUNCTION(Motors, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	/*
    	 * Idée de contrôle
    	 * Réupérer la direction donnée par les microphones
    	 * Vérifier que les capteurs de proximité correspondant à cette direction ne voient pas d'obstacle
    	 * S'ils ne voit rien, go direction microphones
    	 * Si obstacle, éviter l'obstacle
    	 *
    	 * Questions :
    	 * - Faire des tests sur comment set_speed pour tourner à droite / à gauche
    	 * - Eviter l'obstacle de manière logique : garder goal microphones "en tête"
    	 */

    	//Get IR_sensors
    	get_ir_states(ir_states);


    	if (compare_tab(no_obstacle,ir_states) == 1){								//If no obstacle, follow the sound
    		audio_control();
    	}
    	else{																		//If obstacle, direction with IR_sensors
    		proximity_control();
    	}

        chThdSleepMilliseconds(1); // To be determined
    }
}

void control_motors_start(void){
	chThdCreateStatic(waMotors, sizeof(waMotors), NORMALPRIO, Motors, NULL);
}

