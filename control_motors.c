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
//#include "audio/microphone.h"			// utile pour Idée 2




#define	FREQU_MOTORS	1				//D'après le cours, fréquence thread motor 1kHz -> à checker


static bool ir_states[NUMBER_SENSORS];
static bool no_obstacle[NUMBER_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

static bool mic_states[NUMBER_MIC];
static bool go_front[NUMBER_MIC] = {0, 0, 0, 1};
static bool go_front_right[NUMBER_MIC] = {1, 0, 0, 1};
static bool go_right[NUMBER_MIC] = {1, 0, 0, 0};
//static bool go_back_right[NUMBER_MIC] = {1, 0, 1, 0};		//remove if not needed
static bool go_back[NUMBER_MIC] = {0, 0, 1, 0};
//static bool go_back_left[NUMBER_MIC] = {0, 1, 1, 0};		//remove if not needed
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

void audio_control(void){

	// Idée 1 : 8 directions possibles -> moins saccadé
	if (compare_tab(go_front, mic_states, NUMBER_MIC) == 1){
		left_motor_set_speed(600);
		right_motor_set_speed(600);
	}
	else if (compare_tab(go_front_right, mic_states, NUMBER_MIC) == 1){
		left_motor_set_speed(600);
		right_motor_set_speed(200);
	}
	else if (compare_tab(go_right, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(200);
			right_motor_set_speed(-200);
		}
	/*else if (compare_tab(go_back_right, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-600);
			right_motor_set_speed(-200);
		}*/
	else if (compare_tab(go_back, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-600);
			right_motor_set_speed(600);
		}
	/*else if (compare_tab(go_back_left, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-200);
			right_motor_set_speed(-600);
		}*/
	else if (compare_tab(go_left, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(-200);
			right_motor_set_speed(200);
		}
	else if (compare_tab(go_front_left, mic_states, NUMBER_MIC) == 1){
			left_motor_set_speed(200);
			right_motor_set_speed(600);
		}

	/*Idée 2 : soit en avant, soit en arrière, soit à gauche, soit à droite
	if (mic_states[MIC_FRONT] == 1){
		left_motor_set_speed(600);
		right_motor_set_speed(600);
		set_led(LED1,2);
		set_led(LED3,0);
		set_led(LED5,0);
		set_led(LED7,0);
	}
	else if(mic_states[MIC_BACK] == 1){
		left_motor_set_speed(-600);
		right_motor_set_speed(-600);
		set_led(LED1,0);
		set_led(LED3,0);
		set_led(LED5,2);
		set_led(LED7,0);
	}
	else if (mic_states[MIC_LEFT] == 1){
		left_motor_set_speed(-600);
		right_motor_set_speed(600);
		set_led(LED1,0);
		set_led(LED3,0);
		set_led(LED5,0);
		set_led(LED7,2);
	}
	else if (mic_states[MIC_RIGHT] == 1){
		left_motor_set_speed(600);
		right_motor_set_speed(-600);
		set_led(LED1,0);
		set_led(LED3,2);
		set_led(LED5,0);
		set_led(LED7,0);
	}
	*/
}

void proximity_control(void){
	if ((ir_states[IR_FRONT_RIGHT] == 1) || (ir_states[IR_FRONT_LEFT] == 1)){ 		//Check if obstacle on front
		//|| (ir_states[IR_FRONT_LEFT45] == 1) || (ir_states[IR_FRONT_RIGHT45] == 1)
		if((ir_states[IR_LEFT] == 0)){												//If  obstacle on the left  && (ir_states[IR_FRONT_LEFT45] == 0)
			left_motor_set_speed(-1100);												//If not, turn left
			right_motor_set_speed(1100);
		}
		else if((ir_states[IR_RIGHT] == 0)){										//Check if obstacle on the right  && (ir_states[IR_FRONT_LEFT45] == 0)
			left_motor_set_speed(1100);												//If not, turn right
			right_motor_set_speed(-1100);
		}
	} else if ((ir_states[IR_LEFT] == 1)){
		left_motor_set_speed(600);
		right_motor_set_speed(600);
	} else if ((ir_states[IR_RIGHT] == 1)){
		left_motor_set_speed(600);
		right_motor_set_speed(600);
	} /*else if ((ir_states[IR_FRONT_LEFT45] == 1) && (ir_states[IR_LEFT] == 0)){
		left_motor_set_speed(1000);
		right_motor_set_speed(-1000);
	} else if ((ir_states[IR_FRONT_RIGHT45] == 1) && (ir_states[IR_RIGHT] == 0)){
		left_motor_set_speed(-1000);
		right_motor_set_speed(1000);
	}*/ else {
		left_motor_set_speed(600);													//If not, go straight
		right_motor_set_speed(600);
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


    	//proximity_control();

    	if (compare_tab(no_obstacle,ir_states, NUMBER_SENSORS) == 1){		//If no obstacle, follow the sound
    		audio_control();
    		chThdSleepMilliseconds(20);
    	}
    	else{																//If obstacle, direction with IR_sensors
    		proximity_control();
    		chThdSleepMilliseconds(100);
    	}

//    	chThdSleepMilliseconds(100);										// To be determined
    }
}

void control_motors_start(void){
	chThdCreateStatic(waMotors, sizeof(waMotors), NORMALPRIO+1, Motors, NULL);
}

