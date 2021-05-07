/* Mini-projet : détection de la provenance du son et contournement d'obstacle
 *
 * main.c
 *
 * Authors: Léane Donzé et Alice Guntli
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include "main.h"
#include "sensors/proximity.h"
#include "process_proximity.h"
#include "audio/microphone.h"
#include "process_audio.h"
#include "song_selector.h"
#include "audio/play_melody.h"
#include "audio/audio_thread.h"
#include "motors.h"
#include "control_motors.h"

static void serial_start(void)
{
	static SerialConfig ser_cfg = {
	    115200,
	    0,
	    0,
	    0,
	};

	sdStart(&SD3, &ser_cfg); // UART3.
}

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{
	//initialise message bus, motors
    halInit();
    chSysInit();
    mpu_init();
    serial_start();
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    motors_init();

    //start speakers
    dac_start();

    //starts melody thread
    playMelodyStart();

    //starts thread that choose which/when a song is playing
    play_songs_start();

    //starting proximity sensors
    proximity_start();

    //calibrate proximity sensors
    calibrate_ir();

    //start thread for audio
    mic_start(&processAudioData);

    //starts thread that controls motor depending on proximity or audio
    control_motors_start();


    /* Infinite loop. */
    while (1) {
    	//waits 1 second
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
