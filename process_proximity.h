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

//starts proximity sensor thread
//void measure_proximity_start(void);

//get states of IR_sensors
void get_ir_states(bool* tab);



#endif /* PROCESS_PROXIMITY_H_ */
