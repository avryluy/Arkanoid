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
		printf("There was an error getting device count\n");
		exit(EXIT_FAILURE);
	}
	init();
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

int AudioManager::init()
{
	auto file_paths = prep_files();
	int count = 0;

	for (auto file : file_paths)
	{
		callback_data_s data;
		load_file(file, &data);
		
		PaStreamParameters stream_param = stream_param_init(0, 2, SAMPLE_RATE);
		open_stream(&audioStream, stream_param, loaded_files[count]);
		//audio_streams.push_back(audioStream);
		//SDL_Log("Stream created with file: %I64i\n", data.sndinfo.frames);
		//count++;
	}

	if (err)
	{
		return -1;
	}

	return 0;
}

PaStreamParameters AudioManager::stream_param_init(int device, int channels, int samplerate)
{
	PaStreamParameters m_param;
	m_param.device = device;
	m_param.channelCount = channels;
	m_param.sampleFormat = paFloat32;
	m_param.suggestedLatency = Pa_GetDeviceInfo(m_param.device)->defaultLowOutputLatency;
	m_param.hostApiSpecificStreamInfo = NULL;
	return m_param;

}

void AudioManager::load_file(const char* file_path, callback_data_s* data)
{
	printf("%s \n",file_path);
	if (!file_path) // Check file path exists
	{
		printf("Error with file path\n");
		//return nullptr;
	}
	else
	{
		data->sndfile = sf_open(file_path, SFM_READ, &data->sndinfo);
	}
	// Check for sndfile errors
	if (sf_error(data->sndfile) != SF_ERR_NO_ERROR) {
		printf("Error Opening Sound file.\n");
		//return nullptr;
	}
	else {
		// Load file into memory
		::memset(&data->sndfile, 0, sizeof(data->sndfile));
		printf("File Channel Count: %i\n", data->sndinfo.channels);
		printf("File Samplerate %i\n", data->sndinfo.samplerate);
		//printf("File Format %s\n", data.sndinfo.format);
	}
	loaded_files.push_back((callback_data_s) *data);

	//return data.sndfile;
}

void AudioManager::open_stream(PaStream* stream, PaStreamParameters streamParameters, callback_data_s data)
{
	printf("File SR: %i | StreamParam SR: %i\n", data.sndinfo.samplerate, streamParameters.sampleFormat);
	err = Pa_OpenStream(&stream
		, 0
		, &streamParameters
		, SAMPLE_RATE
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
		printf("Problem opening Stream: %s\n", Pa_GetErrorText(PaError(err)));
		
	}
	else {
		printf("Stream opened with device: %s\n", Pa_GetDeviceInfo(streamParameters.device)->name);
		printf("Device Host API: %i\n", Pa_GetDeviceInfo(streamParameters.device)->hostApi);
	}

}

void AudioManager::play_sound(int index, std::vector<PaStream*> &streams)
{
	// Check index against vector size
	if (index >= 0 && index <= loaded_files.size())
	{
		
		PaStream* stream = streams[index];
		err = Pa_StartStream(stream);
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

void AudioManager::checkerr(PaError err) {
	if (err != paNoError)
	{
		printf("Port Audio Failed to Initialize");
		exit(EXIT_FAILURE);
	}

}

int AudioManager::PA_Callback(const void* input
	, void* output
	, unsigned long                   frameCount
	, const PaStreamCallbackTimeInfo* timeInfo
	, PaStreamCallbackFlags           statusFlags
	, void* userData)
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

std::vector<const char*> AudioManager::prep_files()
{
	std::vector<const char*> file_list = {};
	file_list.push_back("..\\Arkanoid\\Audio\\file01.wav");
	file_list.push_back("..\\Arkanoid\\Audio\\file02.wav");
	file_list.push_back("..\\Arkanoid\\Audio\\file03.wav");
	file_list.push_back("..\\Arkanoid\\Audio\\file04.wav");
	return file_list;
}
