#include "AudioManager.h"
#define FRAMES_PER_BUFFER 256
#define SAMPLE_RATE 48000

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


void AudioManager::play_audio(const char* audio_file_path) {
	//Load Sound File
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
	data.buffer = (float*)malloc(FRAMES_PER_BUFFER * data.sndinfo.channels * sizeof(float));
	//Load Default Audio Stream
	PaStreamParameters streamParameters;
	streamParameters.device = 0;
	streamParameters.channelCount = data.sndinfo.channels;
	streamParameters.sampleFormat = paFloat32;
	streamParameters.suggestedLatency = Pa_GetDeviceInfo(streamParameters.device)->defaultLowOutputLatency;
	streamParameters.hostApiSpecificStreamInfo = NULL;
	err = Pa_OpenStream(&audioStream
		, 0
		, &streamParameters
		, data.sndinfo.samplerate
		, FRAMES_PER_BUFFER
		, paClipOff
		, PA_Callback
		, &data);
	PaError s_err = Pa_IsFormatSupported(nullptr, &streamParameters, 48000);
	if (s_err == paFormatIsSupported)
	{
		printf("Device can support sample rate of 48KHz\n");
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
	}
	else
	{
		printf("Stream Started.\n");
	}
	// Let callback process stream
	while (Pa_IsStreamActive(audioStream))
	{
		if (data.framesRead == data.sndinfo.frames) {
			paComplete;
		}
	}

	// Close Sound File
	sf_close(data.sndfile);
	// Close Audio Stream
	err = Pa_CloseStream(audioStream);
	if (err != paNoError) {
		printf("Problem Closing Stream\n");
	}
	free(data.buffer);
}	
//AudioManager::AudioManager(const char* audioFilePath): sndfile(nullptr), audioStream(nullptr) {
//	err = Pa_Initialize();
//	checkerr(err);
//	numDevices = Pa_GetDeviceCount();
//	printf("Number of Devices : % i\n", numDevices);
//	//sndfile = loadFile(audioFilePath);
//	FRAMES_PER_BUFFER = paFramesPerBufferUnspecified;
//	//printf("Stream Info - Samples %i \n", stream_info.sampleRate);
//	data.sndfile = sf_open(audioFilePath, SFM_READ, &data.sndinfo);
//	if (!data.sndfile) {
//		printf("Error opening file: %s", audioFilePath);
//	}
//	else { printf("File opened successfully\n"); }
//	openStream();
//	printf("File SampleRate %d\n", data.sndinfo.samplerate);
//	printf("File Channel Count %d\n", data.sndinfo.channels);
//	printf("File format %i \n", data.sndinfo.format);
//
//	if (numDevices < 0) {
//		printf("Error getting device count \n");
//		exit(EXIT_FAILURE);
//	}
//	else if (numDevices == 0) {
//		printf("There are no available devices. Check your OS' device manager. \n");
//		exit(EXIT_SUCCESS);
//	}
//
//	for (int i = 0; i < numDevices; i++)
//	{
//		// Device 1: Steinberg output 2 channels
//		// Device 15: Steinberg input 2 channels
//		deviceInfo = Pa_GetDeviceInfo(i);
//		printf("Device #%i\n", i);
//		printf("Device name: %s\n", deviceInfo->name);
//		printf("Max Input Channels: %i\n", deviceInfo->maxInputChannels);
//		printf("Max Outut Channels: %i\n", deviceInfo->maxOutputChannels);
//		printf("Default Sample Rate: %f\n", deviceInfo->defaultSampleRate);
//		
//		
//	}
//}

//AudioManager::~AudioManager() {
//	closeStream();
//	if (data.sndfile) {
//		sf_close(data.sndfile);
//	}
//	err = Pa_Terminate();
//	checkerr(err);
//}

//void AudioManager::openStream() {
//	PaError err = Pa_OpenDefaultStream(&audioStream, 0, data.sndinfo.channels, paInt24, data.sndinfo.samplerate, FRAMES_PER_BUFFER, PA_Callback, &data);
//	if (err != paNoError) {
//		fprintf(stderr, "Error opening audio stream: %s\n", Pa_GetErrorText(err));
//	}
//	else {
//		printf("Audio stream opened successfully.\n");
//		
//	}
//	stream_info = Pa_GetStreamInfo(audioStream);
//	printf("Stream SampleRate %f\n", stream_info->sampleRate);
//	/*err = Pa_StartStream(audioStream);
//	if (err != paNoError){
//		fprintf(stderr, "Error opening audio stream: %s\n", Pa_GetErrorText(err));
//	}*/
//}

//void AudioManager::closeStream() {
//	if (audioStream) {
//		Pa_CloseStream(audioStream);
//		//audioStream = nullptr;
//	}
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
	sf_count_t       num_read;
	out = (float*)output;
	size_t framesToRead = FRAMES_PER_BUFFER;
	/*p_data = (callback_data_s*)userData;*/


	//If the frames read plus the buffer is greater than the frames in the sound file
	if (p_data->framesRead + FRAMES_PER_BUFFER > p_data->sndinfo.frames)
	{
		//Update framesToRead to the file total frames minus what's already read
		framesToRead = p_data->sndinfo.frames - p_data->framesRead;
	}

	sf_readf_float(p_data->sndfile, out, framesToRead);
	p_data->framesRead += framesToRead;

	if (p_data->framesRead >= p_data->sndinfo.frames)
	{
		sf_seek(p_data->sndfile, 0, SEEK_SET);
		p_data->framesRead = 0;
	}


	///* clear output buffer */
	//memset(out, 0, sizeof(float) * frameCount * p_data->sndinfo.channels);

	/////* read directly into output buffer */
	//num_read = sf_read_float(p_data->sndfile, out, frameCount * p_data->sndinfo.channels);

	///*  If we couldn't read a full frameCount of samples we've reached EOF */
	/*if (num_read < frameCount)
	{
		return paComplete;
	}*/

	return paContinue;
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
