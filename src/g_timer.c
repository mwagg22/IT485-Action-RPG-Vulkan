#include "g_timer.h"
//#include "g_includes.h"
void LTimer_init(LTimer *timer)
{
	//Initialize the variables
	timer->mStartTicks = 0;
	timer->mPausedTicks = 0;

	timer->mPaused = false;
	timer->mStarted = false;
}
void start(LTimer *timer)
{
	//Start the timer
	timer->mStarted = true;

	//Unpause the timer
	timer->mPaused = false;

	//Get the current clock time
	timer->mStartTicks = SDL_GetTicks();
	timer->mPausedTicks = 0;
}
void stop(LTimer *timer)
{
	//Stop the timer
	timer->mStarted = false;

	//Unpause the timer
	timer->mPaused = false;

	//Clear tick variables
	timer->mStartTicks = 0;
	timer->mPausedTicks = 0;
}
void pause(LTimer *timer)
{
	//If the timer is running and isn't already paused
	if (timer->mStarted && !timer->mPaused)
	{
		//Pause the timer
		timer->mPaused = true;

		//Calculate the paused ticks
		timer->mPausedTicks = SDL_GetTicks() - timer->mStartTicks;
		timer->mStartTicks = 0;
	}
}
void unpause(LTimer *timer)
{
	//If the timer is running and paused
	if (timer->mStarted &&timer->mPaused)
	{
		//Unpause the timer
		timer->mPaused = false;

		//Reset the starting ticks
		timer->mStartTicks = SDL_GetTicks() - timer->mPausedTicks;

		//Reset the paused ticks
		timer->mPausedTicks = 0;
	}
}
Uint32 getTicks(LTimer *timer)
{
	//The actual timer time
	Uint32 time = 0;

	//If the timer is running
	if (timer->mStarted)
	{
		//If the timer is paused
		if (timer->mPaused)
		{
			//Return the number of ticks when the timer was paused
			time = timer->mPausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			time = SDL_GetTicks() - timer->mStartTicks;
		}
	}

	return time;
}
bool isStarted(LTimer *timer)
{
	//Timer is running and paused or unpaused
	return timer->mStarted;
}

bool isPaused(LTimer *timer)
{
	//Timer is running and paused
	return timer->mPaused && timer->mStarted;
}