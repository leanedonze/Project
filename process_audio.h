/* Mini-projet : détection de la provenance du son et contournement d'obstacle
 *
 * process_audio.h
 *
 * Authors: Léane Donzé et Alice Guntli
 */

#ifndef PROCESS_AUDIO_H_
#define PROCESS_AUDIO_H_

typedef enum {
	//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
	LEFT_CMPLX_INPUT = 0,
	RIGHT_CMPLX_INPUT,
	FRONT_CMPLX_INPUT,
	BACK_CMPLX_INPUT,
	//Arrays containing the computed magnitude of the complex numbers
	LEFT_OUTPUT,
	RIGHT_OUTPUT,
	FRONT_OUTPUT,
	BACK_OUTPUT
} BUFFER_NAME_t;

#define NUMBER_MIC	4

void processAudioData(int16_t *data, uint16_t num_samples);

//gets table with bool that indicates direction
//order of the bool table values Right Left Back Front
void get_direction(bool* direction);

#endif /* PROCESS_AUDIO_H_ */
