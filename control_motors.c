/*
 * control_motors.c
 *
 *  Created on: 15 Apr 2021
 *      Author: 41774
 */
#define	FREQU_MOTORS	1	//D'apr�s le cours, fr�quence thread motor 1kHz -> � checker

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
        chThdSleepMilliseconds(1); // To be determined
    }
}

void control_motors_start(void){
	chThdCreateStatic(waMotors, sizeof(Motors), NORMALPRIO, Motors, NULL);
}

