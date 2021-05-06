/*
 * process_audio.h
 *
 *  Created on: 15 Apr 2021
 *      Author: 41774
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

//gets angle at which robot must turn. zero on Front-Back axis
float get_direction_angle(void);

//gets table with bool that indicates direction
void get_direction(bool* direction);//RLBF

#endif /* PROCESS_AUDIO_H_ */
