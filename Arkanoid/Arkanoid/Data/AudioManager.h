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

	AudioManager();
	~AudioManager();

	static int PA_Callback(const void* input,void* output,unsigned long frameCount,
		const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags,void* userData);
	static void checkerr(PaError err);
	void openStream();
	void closeStream();
	SNDFILE* loadFile(const char* fname);

private:
	PaError err;
	int numDevices;
	int device;
	const PaDeviceInfo* deviceInfo;
	SNDFILE* sndfile = nullptr;
	SF_INFO sfinfo;
	PaStream* audioStream = nullptr;
	const char* testfile = "C:\\Users\\avryl\\Documents\\CODE\\Arkanoid\\Arkanoid\\Arkanoid\\Audio\\bow_shot_02.wav";

	


};