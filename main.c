#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include "leds.h"
#include <main.h>


int main(void)
{

    halInit();
    chSysInit();
    mpu_init();

    set_led(LED7, 8);
    set_led(LED1, 8);
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
