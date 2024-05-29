#pragma once
#include "Data\GameConstants.h"
#include "lib\x64\SDL.h"
class FPSManager
{
public:
	FPSManager();
	~FPSManager();

	void start_time();
	void pause_time();
	void unpause_time();
	void stop_time();

	void frame_avg(int in_frames, int main_timer);
	void frame_limit(Uint32 loop_frames);

	int get_time();
	bool isPaused();
	bool isStarted();



private:
	bool mPaused;
	bool mStarted;
	int mStartTime;
	int mPausedTime;

};
