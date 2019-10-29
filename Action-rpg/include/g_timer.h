#include "gf3d_model.h"
#include "gf3d_commands.h"
//#include "gf3d_vgraphics.h"
#include "gf3d_obj_load.h"

typedef struct LTimer
{
        //Initializes variables

        //The various clock actions



        //The clock time when the timer started
        Uint32 mStartTicks;

        //The ticks stored when the timer was paused
        Uint32 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
}LTimer;
void LTimer_init(LTimer *timer);
void start(LTimer *timer);
void stop(LTimer *timer);
void pause(LTimer *timer);
void unpause(LTimer *timer);
//Gets the timer's time
Uint32 getTicks(LTimer *timer);

//Checks the status of the timer
bool isStarted(LTimer *timer);
bool isPaused(LTimer *timer);
