/* Mini-projet : d�tection de la provenance du son et contournement d'obstacle
 *
 * process_audio.c
 *
 * Authors: L�ane Donz� et Alice Guntli
 */

#include "audio/microphone.h"
#include "process_audio.h"
#include "arm_const_structs.h"
#include <arm_math.h>


#define FFT_SIZE 				1024
#define MIN_VALUE_THRESHOLD		12000
#define VICTORY_THRESHOLD		400000
#define MIN_FREQ				10	//we don't analyze before this index to not use resources for nothing (156.25Hz)
#define MAX_FREQ				30	//we don't analyze after this index to not use resources for nothing (468.75Hz)
#define THRESHOLD_RL			0.3
#define THRESHOLD_FB			0.2


//2 times FFT_SIZE because these arrays contain complex numbers (real + imaginary)
static float micLeft_cmplx_input[2 * FFT_SIZE];
static float micRight_cmplx_input[2 * FFT_SIZE];
static float micFront_cmplx_input[2 * FFT_SIZE];
static float micBack_cmplx_input[2 * FFT_SIZE];
//Arrays containing the computed magnitude of the complex numbers
static float micLeft_output[FFT_SIZE];
static float micRight_output[FFT_SIZE];
static float micFront_output[FFT_SIZE];
static float micBack_output[FFT_SIZE];


static float deltaPhaseRL = 0;
static float deltaPhaseFB = 0;
static bool volume = false;


//Fast Fourier Transform
void doFFT_optimized(uint16_t size, float* complex_buffer){
	if(size == 1024)
		arm_cfft_f32(&arm_cfft_sR_f32_len1024, complex_buffer, 0, 1);

}

int16_t detect_frequency(float* data){
	float max_norm = MIN_VALUE_THRESHOLD;
	int16_t max_norm_index = -1;

	//search for the highest peak
	for(uint16_t i = MIN_FREQ ; i <= MAX_FREQ ; i++){
		if(data[i] > max_norm){
			max_norm = data[i];
			max_norm_index = i;
		}
	}
	return max_norm_index;
}


void processAudioData(int16_t *data, uint16_t num_samples){

	/*
	*
	*	We get 160 samples per mic every 10ms
	*	So we fill the samples buffers to reach
	*	1024 samples, then we compute the FFTs.
	*
	*/

	static uint16_t nb_samples = 0;

	//loop to fill the buffers
	for(uint16_t i = 0 ; i < num_samples ; i+=4){
		//construct an array of complex numbers. Put 0 to the imaginary part
		micRight_cmplx_input[nb_samples] = (float)data[i + MIC_RIGHT];
		micLeft_cmplx_input[nb_samples] = (float)data[i + MIC_LEFT];
		micBack_cmplx_input[nb_samples] = (float)data[i + MIC_BACK];
		micFront_cmplx_input[nb_samples] = (float)data[i + MIC_FRONT];

		nb_samples++;

		micRight_cmplx_input[nb_samples] = 0;
		micLeft_cmplx_input[nb_samples] = 0;
		micBack_cmplx_input[nb_samples] = 0;
		micFront_cmplx_input[nb_samples] = 0;

		nb_samples++;

		//stop when buffer is full
		if(nb_samples >= (2 * FFT_SIZE)){
			break;
		}
	}


	if(nb_samples >= (2 * FFT_SIZE)){
		/*	FFT processing
		*
		*	This FFT function stores the results in the input buffer given.
		*	This is an "In Place" function.
		*/

		doFFT_optimized(FFT_SIZE, micRight_cmplx_input);
		doFFT_optimized(FFT_SIZE, micLeft_cmplx_input);
		doFFT_optimized(FFT_SIZE, micFront_cmplx_input);
		doFFT_optimized(FFT_SIZE, micBack_cmplx_input);

		/*	Magnitude processing
		*
		*	Computes the magnitude of the complex numbers and
		*	stores them in a buffer of FFT_SIZE because it only contains
		*	real numbers.
		*
		*/
		arm_cmplx_mag_f32(micRight_cmplx_input, micRight_output, FFT_SIZE);
		arm_cmplx_mag_f32(micLeft_cmplx_input, micLeft_output, FFT_SIZE);
		arm_cmplx_mag_f32(micFront_cmplx_input, micFront_output, FFT_SIZE);
		arm_cmplx_mag_f32(micBack_cmplx_input, micBack_output, FFT_SIZE);

		//find index where max frequency is

		int16_t indexRight = detect_frequency(micRight_output);
		int16_t indexLeft = detect_frequency(micLeft_output);
		int16_t indexFront = detect_frequency(micFront_output);
		int16_t indexBack = detect_frequency(micBack_output);

		//if the amplitude is high enough, victory
		if (micFront_output[indexFront] > VICTORY_THRESHOLD){
			volume = true;
		}
		else{
			volume = false;
		}

		//find phase of sound incoming from the different mics

		float phaseRight = atan2(micRight_cmplx_input[indexRight + 1],micRight_cmplx_input[indexRight]);
		float phaseLeft = atan2(micLeft_cmplx_input[indexLeft + 1],micLeft_cmplx_input[indexLeft]);
		float phaseFront = atan2(micFront_cmplx_input[indexFront + 1],micFront_cmplx_input[indexFront]);
		float phaseBack = atan2(micBack_cmplx_input[indexBack + 1],micBack_cmplx_input[indexBack]);


		//Phase difference gives direction of the sound
		deltaPhaseRL = phaseRight - phaseLeft;
		deltaPhaseFB = phaseFront - phaseBack;


		nb_samples = 0;	//mettre commentaire

	}
}

// bool table values : Right Left Back Front
void get_direction(bool* direction){

	if (deltaPhaseRL>THRESHOLD_RL){
		direction[MIC_RIGHT] = true;
		direction[MIC_LEFT] = false;
	}
	else if (deltaPhaseRL<-THRESHOLD_RL){
		direction[MIC_RIGHT] = false;
		direction[MIC_LEFT] = true;
	}
	else {
		direction[MIC_RIGHT] = false;
		direction[MIC_LEFT] = false;
	}

	if (deltaPhaseFB>THRESHOLD_FB){
		direction[MIC_FRONT] = true;
		direction[MIC_BACK] = false;
	}
	else if (deltaPhaseFB<-THRESHOLD_FB){
		direction[MIC_FRONT] = false;
		direction[MIC_BACK] = true;
	}
	else {
		direction[MIC_FRONT] = false;
		direction[MIC_BACK] = false;
	}
}

bool get_volume(void){
	return volume;
}


