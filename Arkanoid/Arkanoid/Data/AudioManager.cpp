#include "AudioManager.h"


AudioManager::AudioManager() {
	err = Pa_Initialize();
	checkerr(err);
	SNDFILE* sndfile = loadFile(testfile);
	numDevices = Pa_GetDeviceCount();
	printf("Number of Devices : % i\n", numDevices);

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
	err = Pa_Terminate();
	checkerr(err);
}

//void AudioManager::openStream() {
//	err = Pa_OpenStream(&audioStream,NULL,)
//}

void AudioManager::checkerr(PaError err) {
	if (err != paNoError)
	{
		printf("Port Audio Failed to Initialize");
		exit(EXIT_FAILURE);
	}

}

SNDFILE* AudioManager::loadFile(const char* fname) {
	sfinfo.format = 0;
	sndfile = sf_open(fname, SFM_READ, &sfinfo);
	if (!sndfile) {
		printf("Error Opening audio file: %i\n", sf_error(sndfile));
	}
	return sndfile;
}

void playAudio(SNDFILE* sndfile, PaStream* audioStream){
	if (!audioStream || !sndfile) {


	}
}
