#include "AudioManager.h"
#include "SDL.h"

const int AudioManager::FRAMES_PER_BUFFER;



AudioManager::AudioManager():sndfile(nullptr), audioStream(nullptr) {
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
			deviceInfo = new PaDeviceInfo;
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

void AudioManager::play_audio(const char* audio_file_path) {
	//Load Sound File
	::memset(&data.sndinfo, 0, sizeof(data.sndinfo));
	data.sndfile = sf_open(audio_file_path, SFM_READ, &data.sndinfo);
	// Check for sndfile errors
	if (sf_error(data.sndfile) != SF_ERR_NO_ERROR) {
		printf("Error Opening Sound file.");
	}
	else {
		printf("File Channel Count: %i\n", data.sndinfo.channels);
		printf("File Samplerate %i\n", data.sndinfo.samplerate);
		//printf("File Format %s\n", data.sndinfo.format);
	}

	//Allocate buffer for audio data
	//data.buffer = (float*)malloc(FRAMES_PER_BUFFER * data.sndinfo.channels * sizeof(float));
	//Load Default Audio Stream

	PaStreamParameters streamParameters = stream_param_init(0, data.sndinfo.channels, paFloat32);

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
		printf("Device can support sample rate of {} KHz\n", SAMPLE_RATE);
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
	// Let callback process stream
	//while (data.count > 0) {}

	//while (Pa_IsStreamActive(audioStream))
	//{

	//	Pa_Sleep(1);
	//	if (data.framesRead == data.sndinfo.frames) {
	//		paComplete;
	//		break;
	//		
	//	}
	//}
	//if (Pa_IsStreamStopped)
	//{
	//	sf_close(data.sndfile);
	//err = Pa_StopStream(audioStream);
	//if (err != paNoError)
	//	std::cerr << "PAError: " << Pa_GetErrorText(err) << std::endl;
	//// Close Audio Stream
	//err = Pa_CloseStream(audioStream);
	//if (err != paNoError) {
	//	printf("Problem Closing Stream\n");
	//}
//	}
	// Close Sound File
	
	//free(data.buffer);
}	

//AudioManager::~AudioManager() {
//	closeStream();
//	if (data.sndfile) {
//		sf_close(data.sndfile);
//	}
//	err = Pa_Terminate();
//	checkerr(err);
//}

void AudioManager::checkerr(PaError err) {
	if (err != paNoError)
	{
		printf("Port Audio Failed to Initialize");
		exit(EXIT_FAILURE);
	}

}

//SNDFILE* AudioManager::loadFile(const char* fname) {
//	sfinfo.format = 0;
//	sndfile = sf_open(fname, SFM_READ, &sfinfo);
//	if (!sndfile) {
//		printf("Error Opening audio file: %i\n", sf_error(sndfile));
//	}
//	return sndfile;
//}



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


	////If the frames read plus the buffer is greater than the frames in the sound file
	//if (p_data->framesRead + FRAMES_PER_BUFFER > p_data->sndinfo.frames)
	//{
	//	//Update framesToRead to the file total frames minus what's already read
	//	framesToRead = p_data->sndinfo.frames - p_data->framesRead;
	//}

	//sf_readf_float(p_data->sndfile, out, framesToRead);
	//p_data->framesRead += framesToRead;
	//SDL_Log("Read: %i | Length: %i", p_data->framesRead, p_data->sndinfo.frames);

	//if (p_data->framesRead == p_data->sndinfo.frames)
	//{
	//	sf_seek(p_data->sndfile, 0, SEEK_SET);
	//	p_data->framesRead = 0;
	//	return paComplete;
	//	printf("Stopping");
	//}
	//else
	//{
	//	return paContinue;
	//}

	///* clear output buffer */
	//memset(out, 0, sizeof(float) * frameCount * p_data->sndinfo.channels);

	/////* read directly into output buffer */
	//num_read = sf_read_float(p_data->sndfile, out, frameCount * p_data->sndinfo.channels);

	///*  If we couldn't read a full frameCount of samples we've reached EOF */


	
}
//
//int AudioManager::PA_Callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
//	const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
//
//
//	AudioManager* audioManager = static_cast<AudioManager*>(userData);
//
//	printf("Callback called. Frames per buffer: %lu\n", framesPerBuffer);
//	// Read audio data from the file and copy it to the output buffer
//	//sf_readf_float(sndfile, static_cast<float*>(outputBuffer), framesPerBuffer);
//
//	// Implement any additional audio processing here if needed
//
//
//	sf_count_t readCount = sf_readf_float(audioManager->sndfile, static_cast<float*>(outputBuffer), framesPerBuffer);
//	if (readCount < framesPerBuffer) {
//		if (readCount == 0) {
//			// End of file reached
//			// Optionally handle end-of-file behavior
//			return paComplete;
//		}
//		else {
//			fprintf(stderr, "Error reading from audio file: %s\n", sf_strerror(audioManager->sndfile));
//			// Handle the error (e.g., stop the stream, close the file)
//		}
//	}
//	return paContinue;
//}



//void AudioManager::playaudio(){
//	if (!audioStream) {
//
//		printf("Something is missing from the audioStream \n");
//	}
//	else if (!data.sndfile) {
//		printf("Something is missing from the sound file \n");
//	}
//	if (Pa_IsStreamActive(audioStream) == 0) {
//		openStream();
//	}
//
//	Pa_StartStream(&audioStream);
//	std::vector<float> buffer(double(FRAMES_PER_BUFFER) * data.sndinfo.channels);
//
//	while (sf_readf_float(data.sndfile, buffer.data(), FRAMES_PER_BUFFER) > 0) {
//		err = Pa_WriteStream(audioStream, buffer.data(), FRAMES_PER_BUFFER);
//		if (err != paNoError) {
//			std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
//			// Handle the error
//		}
//	}
//	Pa_Sleep(1800);
//	Pa_StopStream(audioStream);
//	closeStream();
//}
