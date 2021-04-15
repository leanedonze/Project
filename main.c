#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include "leds.h"
#include <sensors/proximity.h>
#include <main.h>

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

#define THRESHOLD 	5


int main(void)
{


    halInit();
    chSysInit();
    mpu_init();
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    proximity_start();
    calibrate_ir();


    /* Infinite loop. */
    while (1) {

    	if (get_calibrated_prox(6) > THRESHOLD){
    		set_led(LED7, 2);
    	}
    	else{
    		set_led(LED7, 0);
    	}
    	if (get_calibrated_prox(3) > THRESHOLD){
    		set_led(LED3, 2);

    	}
    	else{
    		set_led(LED3, 0);
    	}
    	//waits 1 second
    	//chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
