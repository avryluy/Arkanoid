#pragma once

#include <portaudio.h>
#include <sndfile.h>
#include <iostream>
#include <vector>

#define SAMPLE_RATE (48000)
#define BIT_DEPTH (24)
#define CHANNEL_COUNT (2)

typedef struct
{
	SNDFILE* sndfile;
	SF_INFO sndinfo;
	float* buffer;
	size_t framesRead;
} callback_data_s;

class AudioManager {

public:

	AudioManager();
	//AudioManager(const char* audioFilePath);
	~AudioManager();

	static void checkerr(PaError err);
	/*void openStream();
	void closeStream();*/
	//void playaudio();
	void play_audio(const char* audio_file_path);

	static int PA_Callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

	//SNDFILE* loadFile(const char* fname);
	/*const char* testfile = "D:\\GameDevelopment\\Arkanoid\\Arkanoid\\Arkanoid\\Wilhelm_tk1.wav";*/
	SNDFILE* sndfile;
	PaStream* audioStream;
	int FRAMES_PER_BUFFER;
	SF_INFO sfinfo;
	callback_data_s data;

private:
	PaError err;
	int numDevices;
	int device;
	const PaDeviceInfo* deviceInfo;
	const PaStreamInfo* stream_info;
	
	
	
	

	


};