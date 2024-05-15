#include "AudioManager.h"
#include "SDL.h"

const int AudioManager::FRAMES_PER_BUFFER;



AudioManager::AudioManager():sndfile(nullptr), audioStream(nullptr), deviceInfo(nullptr) {
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
	else if (numDevices > 0)
	{
		printf("Number of devices: %i \n", numDevices);
		for (int i = 0; i < numDevices; i++) {
			deviceInfo = Pa_GetDeviceInfo(i);
			printf("Device Index %i\n", PaDeviceIndex(i));
			printf("Device name: %s\n", deviceInfo->name);
			printf("Max Input Channels: %i\n", deviceInfo->maxInputChannels);
			printf("Max Outut Channels: %i\n", deviceInfo->maxOutputChannels);
			printf("Default Sample Rate: %f\n", deviceInfo->defaultSampleRate);
			
		}
	}
	else
	{
		printf("There was an error getting device count");
		exit(EXIT_FAILURE);
	}


}

AudioManager::~AudioManager() {
	err = Pa_Terminate();
	if (err == paNoError) {
		exit(EXIT_SUCCESS);
	}
}

PaStreamParameters AudioManager::stream_param_init(int device, int channels, int samplerate)
{
	PaStreamParameters m_param;
	m_param.device = device;
	m_param.channelCount = channels;
	m_param.sampleFormat = samplerate;
	m_param.suggestedLatency = Pa_GetDeviceInfo(m_param.device)->defaultLowOutputLatency;
	m_param.hostApiSpecificStreamInfo = NULL;
	return m_param;

}

void AudioManager::load_file(const char* file_path, callback_data_s& data)
{
	if (!file_path) // Check file path exists
		data.sndfile = sf_open(file_path, SFM_READ, &data.sndinfo);
	else
	{
		printf("Error with file path");
		//return nullptr;
	}
	// Check for sndfile errors
	if (sf_error(data.sndfile) != SF_ERR_NO_ERROR) {
		printf("Error Opening Sound file.");
		//return nullptr;
	}
	else {
		// Load file into memory
		::memset(&data.sndinfo, 0, sizeof(data.sndinfo));
		printf("File Channel Count: %i\n", data.sndinfo.channels);
		printf("File Samplerate %i\n", data.sndinfo.samplerate);
		//printf("File Format %s\n", data.sndinfo.format);
	}
	loaded_files.push_back(data);

	//return data.sndfile;
}

void AudioManager::open_stream(PaStreamParameters streamParameters, callback_data_s data)
{
	err = Pa_OpenStream(&audioStream
		, 0
		, &streamParameters
		, data.sndinfo.samplerate
		, FRAMES_PER_BUFFER
		, paClipOff
		, PA_Callback
		, &data);
	PaError s_err = Pa_IsFormatSupported(nullptr, &streamParameters, SAMPLE_RATE);
	if (s_err == paFormatIsSupported)
	{
		printf("Device can support sample rate of %i KHz\n", SAMPLE_RATE);
	}
	// Check for stream errors
	if (err != paNoError)
	{
		printf("Problem opening Stream: %d\n", PaError(err));
	}
	else {
		printf("Stream opened with device: %s\n", Pa_GetDeviceInfo(streamParameters.device)->name);
		printf("Device Host API: %i\n", Pa_GetDeviceInfo(streamParameters.device)->hostApi);
	}

}

void AudioManager::play_brick_break(int index, PaStream* stream)
{
	// Check index against vector size
	if (index >= 0 && index <= loaded_files.size())
	{
		callback_data_s& audio_clip = loaded_files[index];
		PaStreamParameters streamParameters = stream_param_init(0, data.sndinfo.channels, paFloat32);
		if (!audioStream)
		{
			open_stream(streamParameters, audio_clip);
		}
		else
		{
			close_stream(&audioStream);
		}
		err = Pa_StartStream(audioStream);
		if (err != paNoError)
		{
			printf("Error Starting Audio Stream\n");
			printf(Pa_GetErrorText(err));
		}
		else
		{
			printf("Stream Started.\n");
		}

	}
	else
	{
		printf("Invalid Index");
	}

}

void AudioManager::close_stream(PaStream* stream)
{
	if (stream)
	{
		Pa_StopStream(stream);
		Pa_CloseStream(stream);
		stream = nullptr;
	}
}


AudioManager::~AudioManager() {
	close_stream(&audioStream);

	for (auto& file : loaded_files)
	{
		sf_close(file.sndfile);
	}


	//if (data.sndfile) {
	//	sf_close(data.sndfile);
	//}
	err = Pa_Terminate();
	checkerr(err);
}

void AudioManager::checkerr(PaError err) {
	if (err != paNoError)
	{
		printf("Port Audio Failed to Initialize");
		exit(EXIT_FAILURE);
	}

}



int
AudioManager::PA_Callback
(const void* input
	, void* output
	, unsigned long                   frameCount
	, const PaStreamCallbackTimeInfo* timeInfo
	, PaStreamCallbackFlags           statusFlags
	, void* userData
)
{
	float* out;
	callback_data_s* p_data = (callback_data_s*)userData;
	/*sf_count_t       num_read;*/
	out = (float*)output;
	size_t framesToRead = FRAMES_PER_BUFFER;
	/*p_data = (callback_data_s*)userData;*/


	auto data = std::make_unique<float[]>(frameCount * p_data->sndinfo.channels);
	p_data->count = sf_read_float(p_data->sndfile, data.get(), framesToRead * p_data->sndinfo.channels);

	for (unsigned long i = 0; i < frameCount * p_data->sndinfo.channels; i++)
	{
		*out++ = data[i];
	}

	p_data->framesRead += p_data->buffer_size;
	printf("Count: %llu  | Framecount: %llu\n", p_data->framesRead, p_data->sndinfo.frames);

	if (p_data->framesRead == p_data->sndinfo.frames || p_data->framesRead >= p_data->sndinfo.frames)
	{
		sf_seek(p_data->sndfile, 0, SEEK_SET);
		p_data->framesRead = 0;
		printf("Complete");
		return paComplete;

	}
	else return paContinue;
}
