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
#include "process_audio.h"

#define	FREQU_MOTORS	1	//D'après le cours, fréquence thread motor 1kHz -> à checker

static bool ir_states[NUMBER_SENSORS];
static bool no_obstacle[NUMBER_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

static bool mic_states[NUMBER_MIC];
static bool go_front[NUMBER_MIC] = {0, 0, 0, 1};
static bool go_front_right[NUMBER_MIC] = {1, 0, 0, 1};
static bool go_right[NUMBER_MIC] = {1, 0, 0, 0};
static bool go_back_right[NUMBER_MIC] = {1, 0, 1, 0};
static bool go_back[NUMBER_MIC] = {0, 0, 1, 0};
static bool go_back_left[NUMBER_MIC] = {0, 1, 1, 0};
static bool go_left[NUMBER_MIC] = {0, 1, 0, 0};
static bool go_front_left[NUMBER_MIC] = {0, 1, 0, 1};

bool compare_tab(bool* tab1, bool* tab2, int size){
	for (int i=0; i < size; ++i){
		if((tab1[i]) != (tab2[i])){
			return false;
		}
	}
	return true;
}

void audio_control(){
	if (compare_tab(go_front, mic_states, NUMBER_MIC) == 1){
		left_motor_set_speed(600);
		right_motor_set_speed(600);
	}
	else if (compare_tab(go_front_right, mic_states, NUMBER_MIC) == 1){
		left_motor_set_speed(600);
		right_motor_set_speed(200);
	}
	else if (compare_tab(go_right, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(600);
			right_motor_set_speed(-600);
		}
	else if (compare_tab(go_back_right, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-600);
			right_motor_set_speed(-200);
		}
	else if (compare_tab(go_back, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-600);
			right_motor_set_speed(-600);
		}
	else if (compare_tab(go_back_left, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-200);
			right_motor_set_speed(-600);
		}
	else if (compare_tab(go_left, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-600);
			right_motor_set_speed(600);
		}
	else if (compare_tab(go_front_left, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(200);
			right_motor_set_speed(600);
		}
}

void proximity_control(){
	if ((ir_states[IR_FRONT_RIGHT] == 1) || (ir_states[IR_FRONT_LEFT] == 1)){		//Check if obstacle on front
		if(ir_states[IR_LEFT] == 0){												//If if obstacle on the left
			left_motor_set_speed(-600);												//If not, turn left
			right_motor_set_speed(600);
		}
		else if(ir_states[IR_RIGHT] == 0){											//Check if obstacle on the right
			left_motor_set_speed(600);												//If not, turn right
			right_motor_set_speed(-600);
		}
	}
}

static THD_WORKING_AREA(waMotors, 256);	//Checker taille à réserver sur la stack
static THD_FUNCTION(Motors, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	//Get IR_sensors
    	get_ir_states(ir_states);

    	//Get microphones
    	get_direction(mic_states);

    	if (compare_tab(no_obstacle,ir_states, NUMBER_SENSORS) == 1){		//If no obstacle, follow the sound
    		audio_control();
    	}
    	else{																//If obstacle, direction with IR_sensors
    		proximity_control();
    	}

        chThdSleepMilliseconds(1); // To be determined
    }
}

void control_motors_start(void){
	chThdCreateStatic(waMotors, sizeof(waMotors), NORMALPRIO, Motors, NULL);
}

