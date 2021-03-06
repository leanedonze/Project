/* Mini-projet : d?tection de la provenance du son et contournement d'obstacle
 *
 * main.c
 *
 * Authors: L?ane Donz? et Alice Guntli
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include "main.h"
#include "sensors/proximity.h"
#include "audio/microphone.h"
#include "process_audio.h"
#include "song_selector.h"
#include "audio/play_melody.h"
#include "audio/audio_thread.h"
#include "motors.h"
#include "control_motors.h"


messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);


int main(void)
{
	//initializations
    halInit();
    chSysInit();
    mpu_init();
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    motors_init();

    //starts speakers
    dac_start();

    //starts melody thread
    playMelodyStart();

    //starts thread that choose which/when a song is playing
    play_songs_start();

    //starts proximity sensors
    proximity_start();

    //calibrates proximity sensors
    calibrate_ir();

    //starts thread for audio
    mic_start(&processAudioData);

    //starts thread that controls motors
    control_motors_start();


    // Infinite loop
    while (1) {
        chThdSleepMilliseconds(1000);
    }
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
