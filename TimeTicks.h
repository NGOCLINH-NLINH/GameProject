#include <SDL.h>
#include <sstream>
#include <string>
#include <iostream>
#include <SDL.h>

using namespace std;

class TimeTicks{
    public:

        TimeTicks();

        void start();
        void stop();
        void pause();
        void unpause();

        Uint32 getTicks();

        bool isStarted();
        bool isPaused();
    private:

        Uint32 mStartTicks;

        Uint32 mPausedTicks;

        bool mPaused;
        bool mStarted;
};

