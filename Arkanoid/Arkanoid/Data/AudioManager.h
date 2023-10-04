#pragma once

#include <portaudio.h>
#include <sndfile.h>
#include <iostream>

#define SAMPLE_RATE = 44100;
#define BIT_DEPTH = 24;
#define CHANNEL_COUNT = 2;
#define FRAMES_PER_BUFFER = 512;

class AudioManager {

public:

	AudioManager(const char* audioFilePath);
	~AudioManager();

	static void checkerr(PaError err);
	void openStream();
	void closeStream();

	static int PA_Callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

	//SNDFILE* loadFile(const char* fname);
	const char* testfile = "C:\\Users\\avryl\\Documents\\CODE\\Arkanoid\\Arkanoid\\Arkanoid\\Audio\\bow_shot_02.wav";
	

private:
	PaError err;
	int numDevices;
	int device;
	const PaDeviceInfo* deviceInfo;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	PaStream* audioStream;
	

	


};