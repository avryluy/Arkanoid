#include "Engine\FPSManager.h"


FPSManager::FPSManager()
{
	mStarted = false;
	mPaused = false;
	mStartTime = 0;
	mPausedTime = 0;

}

FPSManager::~FPSManager()
{

}

void FPSManager::start_time() {
	mStarted = true;
	mPaused = false;
	mStartTime = SDL_GetTicks();
	mPausedTime = 0;
}

void FPSManager::pause_time()
{
	if (mStarted && !mPaused)
	{
		mPaused = true;
		mPausedTime = SDL_GetTicks() - mStartTime;
		mStartTime = 0;
	}

}

void FPSManager::unpause_time()
{
	if (mStarted & mPaused)
	{
		mPaused = false;
		mStartTime = SDL_GetTicks() - mPausedTime;
		mPausedTime = 0;
	}

}

void FPSManager::stop_time()
{
	mPaused = false;
	mStarted = false;
	mStartTime = 0;
	mPausedTime = 0;
}

void FPSManager::frame_avg(int in_frames, int main_timer)
{
	float avgFPS = in_frames / (main_timer / 1000.f);
	//handles extreme frame counts
	if (avgFPS > 10000)
	{
		avgFPS = 0;
	}
	//SDL_Log("FPS: %f", avgFPS);
}

void FPSManager::frame_limit(Uint32 loop_frames)
{
	if (loop_frames < TICKS_PER_FRAME)
	{
		//Delay SDL to smooth frame averaging
		SDL_Delay((Uint32)(TICKS_PER_FRAME - loop_frames));
	}
}

int FPSManager::get_time()
{
	Uint32 time = 0;

	if (mStarted)
	{
		if (mPaused)
		{
			time = mPausedTime;
		}
	else
		{
			time = SDL_GetTicks() - mStartTime;
		}
		
	}

	return time;
}




bool FPSManager::isPaused()
{
	return mPaused;
}

bool FPSManager::isStarted()
{
	return mStarted;
}