/*
 * control_motors.c
 *
 *  Created on: 15 Apr 2021
 *      Author: 41774
 */

#include <stdbool.h>
#include "process_proximity.h"
#include "leds.h"
#include "control_motors.h"

#define	FREQU_MOTORS	1	//D'apr�s le cours, fr�quence thread motor 1kHz -> � checker

static bool ir_states[NUMBER_SENSORS];

static const bool no_obstacle[NUMBER_SENSORS] = {0, 0, 0, 0, 0, 0, 0, 0};

void audio_control(){
	//
}

static THD_WORKING_AREA(waMotors, 256);	//Checker taille � r�server sur la stack
static THD_FUNCTION(Motors, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;


    while(1){

    	/*
    	 * Id�e de contr�le
    	 * R�up�rer la direction donn�e par les microphones
    	 * V�rifier que les capteurs de proximit� correspondant � cette direction ne voient pas d'obstacle
    	 * S'ils ne voit rien, go direction microphones
    	 * Si obstacle, �viter l'obstacle
    	 *
    	 * Questions :
    	 * - Faire des tests sur comment set_speed pour tourner � droite / � gauche
    	 * - Eviter l'obstacle de mani�re logique : garder goal microphones "en t�te"
    	 */

    	//Get IR_sensors
    	//ChSysLock();		Chercher comment l'utiliser
    	get_ir_states(ir_states);

    	//If no obstacle, follow the sound
    	audio_control();

    	//If
        chThdSleepMilliseconds(20); // To be determined
    }
}

void control_motors_start(void){
	chThdCreateStatic(waMotors, sizeof(Motors), NORMALPRIO, Motors, NULL);
}

