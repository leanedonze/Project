/* Mini-projet : détection de la provenance du son et contournement d'obstacle
 *
 * song_selector.c
 *
 * Authors: Léane Donzé et Alice Guntli
 */

#include "selector.h"
#include "audio/play_melody.h"
#include "process_audio.h"
#include "leds.h"
#include "control_motors.h"

static uint8_t currentSong = EXTERNAL_SONG;


void launch_song (uint8_t song){
	if (currentSong!=song){
		playMelody(song, ML_FORCE_CHANGE, NULL);
		currentSong = song;
	} else {
		playMelody(song, ML_SIMPLE_PLAY, NULL);
	}
}


//chooses song according to selector state
void choose_song(void){

	switch (get_selector()) {
		case IMPOSSIBLE_MISSION:
			launch_song(IMPOSSIBLE_MISSION);
	        break;
	    case WE_ARE_THE_CHAMPIONS:
			launch_song(WE_ARE_THE_CHAMPIONS);
	        break;
	    case RUSSIA:
			launch_song(RUSSIA);
	        break;
	    case MARIO:
			launch_song(MARIO);
	        break;
	    case UNDERWORLD:
			launch_song(UNDERWORLD);
	        break;
	    case MARIO_START:
			launch_song(MARIO_START);
	        break;
	    case MARIO_DEATH:
			launch_song(MARIO_DEATH);
	        break;
	    case MARIO_FLAG:
			launch_song(MARIO_FLAG);
	        break;
	    case WALKING:
			launch_song(WALKING);
	        break;
	    case PIRATES_OF_THE_CARIBBEAN:
			launch_song(PIRATES_OF_THE_CARIBBEAN);
	        break;
	    case SIMPSON:
			launch_song(SIMPSON);
	        break;
	    case STARWARS:
			launch_song(STARWARS);
	        break;
	    case SANDSTORMS:
			launch_song(SANDSTORMS);
	        break;
	    case SEVEN_NATION_ARMY:
			launch_song(SEVEN_NATION_ARMY);
	        break;
	    default:
	    	stopCurrentMelody();
	        break;
	}

}


static THD_WORKING_AREA(waSongs, 256);	//Checker taille à réserver sur la stack
static THD_FUNCTION(Songs, arg) {

    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    /*Infinite loop*/
    while(1){
    	if (get_volume() == true){
    		stop_motors();
			//set_led(LED1,2);
			//set_led(LED3,2);
			//set_led(LED5,2);
			//set_led(LED7,2);
    		launch_song(MARIO_FLAG);
    		waitMelodyHasFinished();

    	}
    	else{
    		choose_song();
    	}

        chThdSleepMilliseconds(300); // To be determined
    }
}

void play_songs_start(void){
	chThdCreateStatic(waSongs, sizeof(waSongs), NORMALPRIO, Songs, NULL);
}

