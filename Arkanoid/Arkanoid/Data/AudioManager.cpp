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

	for (auto& file : file_paths)
	{
		callback_data_s data;
		load_file(file, &data);
		PaStream* stream = nullptr;
		printf("Stream pointer before initialization: %p\n", (void*)stream);
		PaStreamParameters stream_param = stream_param_init(0, 2, SAMPLE_RATE);
		printf("callback Channel Data: %i\n", loaded_files[count].sndinfo.channels);
		open_stream(stream, stream_param, &loaded_files[count]);
		printf("Callback Data pointer : %p\n", &loaded_files[count]);
		printf("Callback Data Channels : %i\n", loaded_files[count].sndinfo.channels);
		//Pa_OpenStream(&stream, NULL, &stream_param, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, PA_Callback, &loaded_files[count]);
		printf("Stream pointer after initialization: %p\n", (void*)stream);
		audio_streams.push_back(stream);
		//printf(Pa_GetErrorText(Pa_IsStreamActive(stream)));
		//SDL_Log("Stream created with file: %I64i\n", data.sndinfo.frames);
		count++;
		
	}
	printf("audio_stream vector size = %I64i\n", audio_streams.size());
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
		return;
		//return nullptr;
	}

	memset(&data->sndinfo, 0, sizeof(&data->sndinfo));

	data->sndfile = sf_open(file_path, SFM_READ, &data->sndinfo);
	data->filename = file_path;

	printf("%s\n",sf_strerror(data->sndfile));
	printf("File size: %I64i\n", sizeof(data->sndfile));

	// Check for sndfile errors
	if (sf_error(data->sndfile) != SF_ERR_NO_ERROR) {
		printf("Error opening sound file: %s\n", sf_strerror(nullptr));
		//return nullptr;
	}
	printf("Filename: %s\n", data->filename);
	printf("File Channel Count: %i\n", data->sndinfo.channels);
	printf("File Samplerate %i\n", data->sndinfo.samplerate);
		
	//}
	loaded_files.push_back(*data);

	//return data.sndfile;
}

void AudioManager::open_stream(PaStream*& stream, PaStreamParameters streamParameters, callback_data_s* data)
{
	
	printf("File SR: %i | StreamParam Channel: %i\n", data->sndinfo.samplerate, streamParameters.sampleFormat);
	printf("File openeing stream: %s\n", data->filename);
	err = Pa_OpenStream(&stream
		, NULL
		, &streamParameters
		, SAMPLE_RATE
		, FRAMES_PER_BUFFER
		, paClipOff
		, AudioManager::PA_Callback
		, data);
	printf("Stream data pointer %p\n", (void*)data);
	printf("Stream Pointer %p\n", (void*)stream);
	printf("Callback Data Channels : %i\n", data->sndinfo.channels);
	printf("Callback sound format: %i\n", data->sndinfo.format);
	PaError s_err = Pa_IsFormatSupported(nullptr, &streamParameters, SAMPLE_RATE);
	if (s_err == paFormatIsSupported)
	{
		printf("\nDevice can support sample rate of %i KHz\n", SAMPLE_RATE);
	}
	// Check for stream errors
	if (err != paNoError)
	{
		printf("Problem opening Stream: %s\n", Pa_GetErrorText(PaError(err)));
		
	}
	else {
		printf("Stream opened with device: %s\n", Pa_GetDeviceInfo(streamParameters.device)->name);
		//printf("Device Host API: %i\n", Pa_GetDeviceInfo(streamParameters.device)->hostApi);
	}

}

void AudioManager::play_sound(int index)
{
	if (index >= 0 && index < audio_streams.size())
		//	// Check index against vector size
	{
		
		printf("Loading file at index %i\n", index);
		
		//callback_data_s* data = &loaded_files.at(index);
		printf("Loaded Stream Pointer: %p\n", (void*)audio_streams.at(index));
		

		if (audio_streams.at(index) == nullptr)
		{
			printf("Stream at index %i is null\n", index);
		}
		PaError err = Pa_IsStreamActive(audio_streams.at(index));
		if (err == 1)
		{
			printf("Stream at index  %i is active\n", index);
			Pa_StopStream(audio_streams.at(index));
		}
		else if (err < 0)
		{
			printf("Error checking if stream is active: %s\n", Pa_GetErrorText(err));
		}
		//PaStream* stream = streams[index];
		err = Pa_StartStream(audio_streams.at(index));
		if (err != paNoError)
		{
			printf("Error Starting Audio Stream: %s\n", Pa_GetErrorText(err));
			
		}
		else
		{
			printf("Stream Started.\n Playing file%i\n", index);
		}

	}
	else
	{
		printf("Invalid Index\n");
	}

}
//
//void AudioManager::play_sound(int index, const std::vector<PaStream*>& streams) const
//{
//	// Check index against vector size
//	if (index >= 0 && index <= streams.size())
//	{
//		
//		printf("Loading file at index %i\n", index);
//		PaStream* stream = streams.at(index);
//
//
//		if (stream == nullptr)
//		{
//			printf("Stream at index %i is null\n", index);
//		}
//		PaError err = Pa_IsStreamActive(&stream);
//		if (err == 1)
//		{
//			printf("Stream at index  %i is active\n", index);
//		}
//		else if (err < 0)
//		{
//			printf("Error checking if stream is active: %s\n", Pa_GetErrorText(err));
//		}
//		//PaStream* stream = streams[index];
//		err = Pa_StartStream(&stream);
//		if (err != paNoError)
//		{
//			printf("Error Starting Audio Stream: %s\n", Pa_GetErrorText(err));
//			
//		}
//		else
//		{
//			printf("Stream Started.\n Playing file%i\n", index+1);
//		}
//
//	}
//	else
//	{
//		printf("Invalid Index");
//	}
//
//}

std::vector<PaStream*>& AudioManager::get_streams()
{
	return audio_streams;
};
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
	/*float* out = static_cast<float*>(output);
	callback_data_s* p_data = static_cast<callback_data_s*>(userData);*/

	//// Read audio data from file
	//sf_count_t count = sf_readf_float(p_data->sndfile, p_data->buffer, frameCount * p_data->sndinfo.channels);
	//if (count < frameCount) {
	//	// If end of file reached, rewind to the beginning
	//	sf_seek(p_data->sndfile, 0, SEEK_SET);
	//	// Read remaining data to fill the buffer
	//	count += sf_readf_float(p_data->sndfile, p_data->buffer + count * p_data->sndinfo.channels, (frameCount - count) * p_data->sndinfo.channels);
	//}

	//// Copy audio data to the output buffer
	//for (unsigned long i = 0; i < frameCount * p_data->sndinfo.channels; ++i) {
	//	*out++ = p_data->buffer[i];
	//}


	//if (p_data->framesRead >= p_data->sndinfo.frames) {
	//	sf_seek(p_data->sndfile, 0, SEEK_SET);
	//	p_data->framesRead = 0;
	//	printf("Complete\n");
	//	return paComplete;
	//}

	//return paContinue;



	float* out = static_cast<float*>(output);
	callback_data_s* p_data = static_cast<callback_data_s*>(userData);
	printf("Callback file: %s", p_data->filename);
	printf("callback Channel Data: %i\n", p_data->sndinfo.channels);
	// Check if p_data or p_data->sndfile is null
	if (!p_data) {
		printf("Invalid callback data\n");
		//printf("Invalid callback data %i.\n", p_data->sndinfo.channels);
		return paAbort;
	}
	else if (sf_error(p_data->sndfile) != SF_ERR_NO_ERROR)
	{
		printf("Invalid soundfile in callback data.\n");
		printf("%s \n",sf_strerror(p_data->sndfile));
		return paAbort;
	}
	else if (p_data->sndfile == nullptr)
	{
		printf("sndfile ptr is null\n");
	}
	printf("Callback data accepted\n");
	
	size_t framesToRead = FRAMES_PER_BUFFER;

	// Allocate memory for reading data
	auto data = std::make_unique<float[]>(frameCount * p_data->sndinfo.channels);
	
	if (!data) {
		printf("Memory allocation failed.\n");
		return paAbort;
	}

	// Read data from file
	p_data->count = sf_read_float(p_data->sndfile, data.get(), framesToRead * p_data->sndinfo.channels);
	if (p_data->count < 0) {
		printf("Error reading audio data from file.\n");
		return paAbort;
	}

	// Copy data to output buffer
	for (unsigned long i = 0; i < frameCount * p_data->sndinfo.channels; ++i) {
		out[i] = data[i];
	}

	p_data->framesRead += p_data->buffer_size;
	printf("Count: %llu  | Framecount: %llu\n", p_data->framesRead, p_data->sndinfo.frames);

	if (p_data->framesRead >= p_data->sndinfo.frames) {
		sf_seek(p_data->sndfile, 0, SEEK_SET);
		p_data->framesRead = 0;
		printf("Complete\n");
		return paComplete;
	}

	return paContinue;
}

std::vector<const char*> AudioManager::prep_files()
{
	std::vector<const char*> file_list = {};
	file_list.push_back("..\\Arkanoid\\Audio\\file01.wav");
	//file_list.push_back("D:\\GameDevelopment\\Arkanoid\\Arkanoid\\Arkanoid\\Audio\\file01.wav");
	file_list.push_back("..\\Arkanoid\\Audio\\file02.wav");
	file_list.push_back("..\\Arkanoid\\Audio\\file03.wav");
	//file_list.push_back("..\\Arkanoid\\Audio\\file04.wav");
	return file_list;
}
