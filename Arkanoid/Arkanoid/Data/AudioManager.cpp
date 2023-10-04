#include "AudioManager.h"


AudioManager::AudioManager(const char* audioFilePath): sndfile(nullptr), audioStream(nullptr) {
	err = Pa_Initialize();
	checkerr(err);
	numDevices = Pa_GetDeviceCount();
	printf("Number of Devices : % i\n", numDevices);
	//sndfile = loadFile(audioFilePath);
	sndfile = sf_open(audioFilePath, SFM_READ, &sfinfo);
	if (!sndfile) {
		printf("Error opening file: %s", audioFilePath);
	}
	if (numDevices < 0) {
		printf("Error getting device count \n");
		exit(EXIT_FAILURE);
	}
	else if (numDevices == 0) {
		printf("There are no available devices. Check your OS' device manager. \n");
		exit(EXIT_SUCCESS);
	}

	for (int i = 0; i < numDevices; i++)
	{
		// Device 1: Steinberg output 2 channels
		// Device 15: Steinberg input 2 channels
		deviceInfo = Pa_GetDeviceInfo(i);
		printf("Device #%i\n", i);
		printf("Device name: %s\n", deviceInfo->name);
		printf("Max Input Channels: %i\n", deviceInfo->maxInputChannels);
		printf("Max Outut Channels: %i\n", deviceInfo->maxOutputChannels);
		printf("Default Sample Rate: %f\n", deviceInfo->defaultSampleRate);
	}
}

AudioManager::~AudioManager() {
	closeStream();
	if (sndfile) {
		sf_close(sndfile);
	}
	err = Pa_Terminate();
	checkerr(err);
}

void AudioManager::openStream() {
	PaError err = Pa_OpenDefaultStream(&audioStream, 0, 1, paFloat32, 44100, paFramesPerBufferUnspecified, PA_Callback, this);
	if (err != paNoError)
	{
		printf("Error in Stream: %s", Pa_GetErrorText(err));
	}

}
void AudioManager::closeStream() {
	if (audioStream) {
		Pa_CloseStream(audioStream);
		audioStream = nullptr;
	}
}

void AudioManager::checkerr(PaError err) {
	if (err != paNoError)
	{
		printf("Port Audio Failed to Initialize");
		exit(EXIT_FAILURE);
	}

}

//SNDFILE* AudioManager::loadFile(const char* fname) {
//	sfinfo.format = 0;
//	sndfile = sf_open(fname, SFM_READ, &sfinfo);
//	if (!sndfile) {
//		printf("Error Opening audio file: %i\n", sf_error(sndfile));
//	}
//	return sndfile;
//}


int AudioManager::PA_Callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {


	AudioManager* audioManager = static_cast<AudioManager*>(userData);
	sf_readf_float(audioManager->sndfile, static_cast<float*>(outputBuffer), framesPerBuffer);
	// Read audio data from the file and copy it to the output buffer
	//sf_readf_float(sndfile, static_cast<float*>(outputBuffer), framesPerBuffer);

	// Implement any additional audio processing here if needed

	return paContinue;



	//if (!sndfile) {
	//	return paComplete;
	//}

	//sf_count_t readcount;

	//readcount = sf_readf_float(sndfile, static_cast<float*>(output), frameCount);

	////if (readcount < frameCount) {
	////	sf_close(sndfile);
	////	sndfile = nullptr;
	////	return paComplete;
	////}
	//return paContinue;
}



//void AudioManager::playAudio(SNDFILE* sndfile, PaStream* audioStream){
//	if (!audioStream || !sndfile) {
//
//
//	}
//}

