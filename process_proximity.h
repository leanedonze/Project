/*
 * process_proximity.h
 *
 *  Created on: 15 Apr 2021
 *      Author: 41774
 */

#ifndef PROCESS_PROXIMITY_H_
#define PROCESS_PROXIMITY_H_

#include <stdbool.h>

#define NUMBER_SENSORS		8

#define IR_FRONT_RIGHT		0
#define IR_FRONT_RIGHT45	1
#define IR_RIGHT			2
#define IR_BACK_RIGHT		3
#define IR_BACK_LEFT		4
#define IR_LEFT				5
#define IR_FRONT_LEFT45		6
#define IR_FRONT_LEFT		7

//starts proximity sensor thread
//void measure_proximity_start(void);

//get states of IR_sensors
void get_ir_states(bool* tab);



#endif /* PROCESS_PROXIMITY_H_ */
