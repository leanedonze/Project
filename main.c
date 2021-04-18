#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include "leds.h"
#include "main.h"
#include "sensors/proximity.h"
#include "process_proximity.h"
#include "audio/microphone.h"
#include "process_audio.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{

    halInit();
    chSysInit();
    mpu_init();
    messagebus_init(&bus, &bus_lock, &bus_condvar);


    //starting proximity sensors
    proximity_start();

    //calibrate proximity sensors						HERE?
    calibrate_ir();

    //start threads for processing direction and proximity
    measure_proximity_start();

    //start thread for audio
    mic_start(&processAudioData);


    /* Infinite loop. */
    while (1) {
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
