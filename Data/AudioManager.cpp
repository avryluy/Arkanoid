#include "AudioManager.h"
#include "lib\x64\SDL.h"
#include <cstring>

const int AudioManager::FRAMES_PER_BUFFER;




AudioManager::AudioManager():sndfile(nullptr), stream(nullptr), deviceInfo(nullptr) {
	err = Pa_Initialize();
	

	if (err != paNoError) {
		printf("Problem Initializing PortAudio\n");
		Pa_Terminate();
	}

	numDevices = Pa_GetDeviceCount();
	if (numDevices == 0) {
		printf("No Audio Devices Found");
		Pa_Terminate();
	}

	printf("/***************/\n");
	int defaultdev = Pa_GetDefaultOutputDevice();
	deviceInfo = Pa_GetDeviceInfo(defaultdev);
	printf("Default Device Index %i\n", PaDeviceIndex(defaultdev));
	printf("Default Device name: %s\n", deviceInfo->name);
	printf("Default Max Input Channels: %i\n", deviceInfo->maxInputChannels);
	printf("Default Max Outut Channels: %i\n", deviceInfo->maxOutputChannels);
	printf("Default Sample Rate: %f\n", deviceInfo->defaultSampleRate);
	defaultParam.device = defaultdev;
	defaultParam.channelCount = 2;
	defaultParam.sampleFormat = paFloat32;
	defaultParam.suggestedLatency = deviceInfo->defaultLowOutputLatency;

	//Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, PA_Callback, this);
	Pa_OpenStream(&stream, NULL, &defaultParam, SAMPLE_RATE, FRAMES_PER_BUFFER, NULL, PA_Callback, this);

	Pa_StartStream(stream);
}

AudioManager::~AudioManager() {
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	deviceInfo = nullptr;
	stream = nullptr;
	sndfile = nullptr;
}

void AudioManager::checkerr(PaError err) {
	if (err != paNoError)
	{
		printf("Port Audio Failed to Initialize");
		exit(EXIT_FAILURE);
	}

}

bool AudioManager::load_sound(const char* file_path, soundData& soundData)
{
	SF_INFO sfinfo;
	SNDFILE* sndfile = sf_open(file_path, SFM_READ, &sfinfo);
	
	if (!sndfile)
	{
		printf("Error opening sound file: %s\n", sf_strerror(nullptr));
		return false;
	}

	soundData.filename = file_path;
	soundData.frames = sfinfo.frames;
	soundData.samplerate = sfinfo.samplerate;
	soundData.channels = sfinfo.channels;
	soundData.data = new float[sfinfo.frames * sfinfo.channels];


	if (soundData.data == nullptr)
	{
		printf("Issue with soundData object\n");
	}

	if (!sfinfo.frames)
	{
		printf("Issue with SFINFO frames\n");
	}
	if (!sfinfo.channels)
	{
		printf("Issue with SFINFO channels\n");
	}
	sf_readf_float(sndfile, soundData.data, sfinfo.frames);
	sf_close(sndfile);
	return true;
}

void AudioManager::play_sound(const soundData* soundData)
{
	for (const auto& instance: instances)
	{
		if (instance.active && instance.sound == soundData)
		{
			return;
		}
	}
	instances.emplace_back(soundData);
}

int AudioManager::PA_Callback(const void* input
	, void* output
	, unsigned long                   frameCount
	, const PaStreamCallbackTimeInfo* timeInfo
	, PaStreamCallbackFlags           statusFlags
	, void* userData)
{
	float* out = static_cast<float*>(output);
	AudioManager* audiomanager = static_cast<AudioManager*>(userData);

	//Set outbut buffer to zero
	std::fill(out, out + frameCount * 2, 0.0f);
	std::vector<PlaybackInstance*> InactiveInstances;

	for (auto& instance : audiomanager->instances) {
		if (!instance.active) continue;

		const soundData* sound = instance.sound;
		//printf("Playing Sound: %s\n", instance.sound->filename);

		//printf("Callback receiving channels: %d", instance.sound->channels);

		unsigned long framesToRead = std::min(static_cast<unsigned long>(sound->frames - instance.position), frameCount);


		// for each frame per channel, add frame data based on the sounds position + frame mult by channel count
		for (unsigned long i = 0; i < framesToRead; ++i)
		{
			if (sound->channels == 1)
			{
				float sample = sound->data[instance.position + i];
				out[i * 2] += sample; // left channel;
				out[i * 2 + 1] += sample; // right channel
			}
			else if (sound->channels >= 2 && instance.sound)
			{
				for (int ch = 0; ch < sound->channels && ch < 2; ++ch)
				{
					out[i * 2 + ch] += sound->data[(instance.position + i) * sound->channels + ch];
				}
			}
		}

		instance.position += framesToRead;
		//printf("File: %s | Frames: %I64i\n", instance.sound->filename, instance.position);
		if (instance.position >= sound->frames) {
			instance.active = false; // mark instance inactive when done
			InactiveInstances.push_back(&instance);
		}
	}
	
	// Ensure mixed audio doesn't exceed -1.0 to 1.0 range
	for (unsigned long i = 0; i < frameCount * 2; ++i)
	{
		if (out[i] > 1.0f)
		{
			out[i] = 1.0f;
		}
		if (out[i] < -1.0f)
		{
			out[i] = -1.0f;
		}
	}
	
	if (!audiomanager->instances.empty())
	{
		audiomanager->instances.erase(
		std::remove_if(audiomanager->instances.begin(), audiomanager->instances.end(),
			[](const PlaybackInstance& InactiveInstances) {return !InactiveInstances.active; }),
		audiomanager->instances.end());

	}

	return paContinue;
}

