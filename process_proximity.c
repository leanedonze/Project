/* Mini-projet : détection de la provenance du son et contournement d'obstacle
 *
 * process_proximity.c
 *
 * Authors: Léane Donzé et Alice Guntli
 */

#include "process_proximity.h"
#include "sensors/proximity.h"
#include <stdbool.h>


#define IR_THRESHOLD		80		//tells the distance limit to an object before turning


// if sensor detects obstacle, put corresponding bool to true
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



