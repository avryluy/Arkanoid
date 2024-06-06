#pragma once

#include <lib\x64\portaudio.h>
#include <lib\x64\sndfile.h>
#include <iostream>
#include <vector>

struct soundData {
	const char* filename;
	float* data;
	sf_count_t frames;
	int samplerate;
	int channels;

	soundData() : data(nullptr), frames(0), samplerate(0), channels(0) {}
	~soundData() { delete[] data; }
};

struct PlaybackInstance {

	const soundData* sound;
	sf_count_t position;
	bool active;

	PlaybackInstance(const soundData* soundData) : sound(soundData), position(0), active(true){}
};


class AudioManager {

public:

	int numDevices{};


	AudioManager();
	~AudioManager();

	bool load_sound(const char* file_path, soundData& soundData);
	void play_sound(const soundData* soundData);

	static int PA_Callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
		const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
	static void checkerr(PaError err);
		

private:
	PaError err;
	const PaDeviceInfo* deviceInfo;
	PaStream* stream;
	std::vector<PlaybackInstance> instances;
	PaStreamParameters defaultParam;
	SNDFILE* sndfile{};
	static const int SAMPLE_RATE = 48000;
	static const int BIT_DEPTH = 24;
	static const int CHANNEL_COUNT = 2;
	static const int FRAMES_PER_BUFFER = 512;

};