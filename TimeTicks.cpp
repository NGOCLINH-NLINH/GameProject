#include "TimeTicks.h"


TimeTicks::TimeTicks(){
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void TimeTicks::start(){
    mStarted = true;

    mPaused = false;

    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void TimeTicks::stop()
{
    mStarted = false;

    mPaused = false;

    mStartTicks = 0;
    mPausedTicks = 0;
}

void TimeTicks::pause()
{
    if( mStarted && !mPaused )
    {
        mPaused = true;

        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void TimeTicks::unpause()
{
    if( mStarted && mPaused )
    {
        mPaused = false;

        mStartTicks = SDL_GetTicks() - mPausedTicks;

        mPausedTicks = 0;
    }
}

Uint32 TimeTicks::getTicks()
{

    Uint32 time = 0;

    if( mStarted )
    {
        if( mPaused )
        {
            time = mPausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool TimeTicks::isStarted()
{
    return mStarted;
}

bool TimeTicks::isPaused()
{
    return mPaused && mStarted;
}




