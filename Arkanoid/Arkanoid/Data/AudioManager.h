#pragma once

#include <portaudio.h>
#include <sndfile.h>
#include <iostream>
#include <vector>

struct callback_data_s
{
	SNDFILE* sndfile;
	SF_INFO sndinfo{};
	float* buffer ;
	int buffer_size;
	sf_count_t framesRead;
	sf_count_t count;

	callback_data_s(): sndfile(nullptr), buffer(nullptr), buffer_size(512), framesRead(0), count(1)
	{
		// Initialize SF_INFO
		sndinfo.format = 0; // Initialize format to 0 (you can set this appropriately)
		sndinfo.frames = 0; // Initialize frames to 0 (or set as needed)
		sndinfo.samplerate = 48000; // Example: Set sample rate to 44100 Hz
		sndinfo.channels = 2; // Example: Set number of channels to stereo (2 channels)
		// You can initialize other fields of SF_INFO as needed

	}

};

class AudioManager {

public:
	static const int SAMPLE_RATE = 48000;
	static const int BIT_DEPTH = 24;
	static const int CHANNEL_COUNT = 2;
	static const int FRAMES_PER_BUFFER = 512;
	int numDevices{};

	AudioManager();
	//AudioManager(const char* audioFilePath);
	~AudioManager();

	static void checkerr(PaError err);
	SNDFILE* load_file(const char file_path);
	PaStreamParameters stream_param_init(int device, int channels, int samplerate);
	PaStream* open_stream();
	void close_stream(PaStream* stream);
	void play_brick_break(SNDFILE* file, PaStream* stream);

	


	void play_audio(const char* audio_file_path);

	static int PA_Callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

	//SNDFILE* loadFile(const char* fname);
	/*const char* testfile = "D:\\GameDevelopment\\Arkanoid\\Arkanoid\\Arkanoid\\Wilhelm_tk1.wav";*/
	SNDFILE* sndfile{};
	PaStream* audioStream{};
	callback_data_s data;

private:
	PaError err;
	const PaDeviceInfo* deviceInfo;
	
	
	

	


};