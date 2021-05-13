/* Mini-projet : détection de la provenance du son et contournement d'obstacle
 *
 * process_audio.h
 *
 * Authors: Léane Donzé et Alice Guntli
 */

#ifndef PROCESS_AUDIO_H_
#define PROCESS_AUDIO_H_

#include <stdint.h>

#define NUMBER_MIC	4

// gets microphone data
void processAudioData(int16_t *data, uint16_t num_samples);

// tells if the sound source is near
bool get_volume(void);

// gets table with bool that indicates direction
void get_direction(bool* direction);

#endif /* PROCESS_AUDIO_H_ */
