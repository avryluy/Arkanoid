#include "AudioManager.h"


AudioManager::AudioManager(const char* audioFilePath): sndfile(nullptr), audioStream(nullptr) {
	err = Pa_Initialize();
	checkerr(err);
	numDevices = Pa_GetDeviceCount();
	printf("Number of Devices : % i\n", numDevices);
	//sndfile = loadFile(audioFilePath);
	FRAMES_PER_BUFFER = paFramesPerBufferUnspecified;
	//printf("Stream Info - Samples %i \n", stream_info.sampleRate);
	data.sndfile = sf_open(audioFilePath, SFM_READ, &data.sndinfo);
	if (!data.sndfile) {
		printf("Error opening file: %s", audioFilePath);
	}
	else { printf("File opened successfully\n"); }
	openStream();
	printf("File SampleRate %d\n", data.sndinfo.samplerate);
	printf("File Channel Count %d\n", data.sndinfo.channels);
	printf("File format %i \n", data.sndinfo.format);

	if (numDevices < 0) {
		printf("Error getting device count \n");
		exit(EXIT_FAILURE);
	}
	else if (numDevices == 0) {
		printf("There are no available devices. Check your OS' device manager. \n");
		exit(EXIT_SUCCESS);
	}

	for (int i = 0; i < numDevices; i++)
	{
		// Device 1: Steinberg output 2 channels
		// Device 15: Steinberg input 2 channels
		deviceInfo = Pa_GetDeviceInfo(i);
		printf("Device #%i\n", i);
		printf("Device name: %s\n", deviceInfo->name);
		printf("Max Input Channels: %i\n", deviceInfo->maxInputChannels);
		printf("Max Outut Channels: %i\n", deviceInfo->maxOutputChannels);
		printf("Default Sample Rate: %f\n", deviceInfo->defaultSampleRate);
		
		
	}
}

AudioManager::~AudioManager() {
	closeStream();
	if (data.sndfile) {
		sf_close(data.sndfile);
	}
	err = Pa_Terminate();
	checkerr(err);
}

void AudioManager::openStream() {
	PaError err = Pa_OpenDefaultStream(&audioStream, 0, data.sndinfo.channels, paInt24, data.sndinfo.samplerate, FRAMES_PER_BUFFER, PA_Callback, &data);
	if (err != paNoError) {
		fprintf(stderr, "Error opening audio stream: %s\n", Pa_GetErrorText(err));
	}
	else {
		printf("Audio stream opened successfully.\n");
		
	}
	stream_info = Pa_GetStreamInfo(audioStream);
	printf("Stream SampleRate %f\n", stream_info->sampleRate);
	/*err = Pa_StartStream(audioStream);
	if (err != paNoError){
		fprintf(stderr, "Error opening audio stream: %s\n", Pa_GetErrorText(err));
	}*/
}

void AudioManager::closeStream() {
	if (audioStream) {
		Pa_CloseStream(audioStream);
		//audioStream = nullptr;
	}
}

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
	p_data = (callback_data_s*)userData;

	/* clear output buffer */
	memset(out, 0, sizeof(float) * frameCount * p_data->sndinfo.channels);

	/* read directly into output buffer */
	num_read = sf_read_float(p_data->sndfile, out, double(frameCount) * p_data->sndinfo.channels);

	/*  If we couldn't read a full frameCount of samples we've reached EOF */
	if (num_read < frameCount)
	{
		return paComplete;
	}

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



void AudioManager::playaudio(){
	if (!audioStream) {

		printf("Something is missing from the audioStream \n");
	}
	else if (!data.sndfile) {
		printf("Something is missing from the sound file \n");
	}
	if (Pa_IsStreamActive(audioStream) == 0) {
		openStream();
	}

	Pa_StartStream(&audioStream);
	std::vector<float> buffer(double(FRAMES_PER_BUFFER) * data.sndinfo.channels);

	while (sf_readf_float(data.sndfile, buffer.data(), FRAMES_PER_BUFFER) > 0) {
		err = Pa_WriteStream(audioStream, buffer.data(), FRAMES_PER_BUFFER);
		if (err != paNoError) {
			std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
			// Handle the error
		}
	}
	Pa_Sleep(1800);
	Pa_StopStream(audioStream);
	closeStream();
}

