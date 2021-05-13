/* Mini-projet : détection de la provenance du son et contournement d'obstacle
 *
 * control_motors.c
 *
 * Authors: Léane Donzé et Alice Guntli
 */

#include <stdbool.h>
#include "control_motors.h"
#include "process_proximity.h"
#include "process_audio.h"
#include "motors.h"
#include "audio\play_melody.h"


#define SLOW_SPEED		200
#define NORMAL_SPEED	600
#define FAST_SPEED		1100
#define STOP			0


static bool ir_states[NUMBER_SENSORS];
static bool no_obstacle[NUMBER_SENSORS] =	{0, 0, 0, 0, 0, 0, 0, 0};

static bool mic_states[NUMBER_MIC];
static bool go_front[NUMBER_MIC] = 			{0, 0, 0, 1};
static bool go_front_right[NUMBER_MIC] = 	{1, 0, 0, 1};
static bool go_right[NUMBER_MIC] = 			{1, 0, 0, 0};
static bool go_back[NUMBER_MIC] = 			{0, 0, 1, 0};
static bool go_left[NUMBER_MIC] = 			{0, 1, 0, 0};
static bool go_front_left[NUMBER_MIC] = 	{0, 1, 0, 1};


bool compare_tab(bool* tab1, bool* tab2, int size){
	for (int i=0; i < size; ++i){
		if((tab1[i]) != (tab2[i])){
			return false;
		}
	}
	return true;
}

void audio_control(void){

	// Idée 1 : 8 directions possibles -> moins saccadé
	if (compare_tab(go_front, mic_states, NUMBER_MIC) == true){
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}
	else if (compare_tab(go_front_right, mic_states, NUMBER_MIC) == true){
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(SLOW_SPEED);
	}
	else if (compare_tab(go_right, mic_states, NUMBER_MIC) == true){
		left_motor_set_speed(SLOW_SPEED);
		right_motor_set_speed(-SLOW_SPEED);
	}
	else if (compare_tab(go_back, mic_states, NUMBER_MIC) == true){
		left_motor_set_speed(-NORMAL_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}
	else if (compare_tab(go_left, mic_states, NUMBER_MIC) == true){
		left_motor_set_speed(-SLOW_SPEED);
		right_motor_set_speed(SLOW_SPEED);
	}
	else if (compare_tab(go_front_left, mic_states, NUMBER_MIC) == true){
		left_motor_set_speed(SLOW_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}

}

void proximity_control(void){
	if ((ir_states[IR_FRONT_RIGHT] == true) || (ir_states[IR_FRONT_LEFT] == true)){ 		//Check if obstacle on front
		if((ir_states[IR_LEFT] == false)){												//If  obstacle on the left  && (ir_states[IR_FRONT_LEFT45] == 0)
			left_motor_set_speed(-FAST_SPEED);												//If not, turn left
			right_motor_set_speed(FAST_SPEED);
		}
		else if((ir_states[IR_RIGHT] == false)){										//Check if obstacle on the right  && (ir_states[IR_FRONT_LEFT45] == 0)
			left_motor_set_speed(FAST_SPEED);												//If not, turn right
			right_motor_set_speed(-FAST_SPEED);
		}
	}
	else if ((ir_states[IR_LEFT] == true)){
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}
	else if ((ir_states[IR_RIGHT] == true)){
		left_motor_set_speed(NORMAL_SPEED);
		right_motor_set_speed(NORMAL_SPEED);
	}
	else {
		left_motor_set_speed(NORMAL_SPEED);													//If not, go straight
		right_motor_set_speed(NORMAL_SPEED);
	}

}

static THD_WORKING_AREA(waMotors, 256);	//Checker taille à réserver sur la stack
static THD_FUNCTION(Motors, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	waitMelodyHasFinished();

    	//Get IR_sensors
    	get_ir_states(ir_states);

    	//Get microphones
    	get_direction(mic_states);


    	//proximity_control();
    	if (compare_tab(no_obstacle,ir_states, NUMBER_SENSORS) == true){		//If no obstacle, follow the sound
    		audio_control();
    		chThdSleepMilliseconds(20);
    	}
    	else{																//If obstacle, direction with IR_sensors
    		proximity_control();
    		chThdSleepMilliseconds(100);
    	}


    }
}

void control_motors_start(void){
	chThdCreateStatic(waMotors, sizeof(waMotors), NORMALPRIO+1, Motors, NULL);
}

void stop_motors(void){
	left_motor_set_speed(STOP);
	right_motor_set_speed(STOP);
}
