//
// Created by T450s on 26/12/2022.
//

#include "Clock.h"
#include <stdexcept>

Clock::Clock(double framerate, double idle_framerate) {
    Framerate = 1.0;
	InitialFramerate = framerate;
	IdleFramerate = idle_framerate;
	NanoDelay = 1e9;
    TotalNanoElapsed = 0;
    NanoElapsed = 0;
    VeryBeginning = HNow();
    Beginning = VeryBeginning;
    Ticks = 0;

    SetFramerate(framerate);
}

void Clock::Tick() {
    HPoint Ending = HNow();
    NanoElapsed = MeasureNanoseconds(Beginning, Ending);
    while (NanoElapsed < NanoDelay) {
        Ending = HNow();
        NanoElapsed = MeasureNanoseconds(Beginning, Ending);
    }
    TotalNanoElapsed = MeasureNanoseconds(VeryBeginning, Ending);
    Beginning = HNow();
    Ticks++;
}

void Clock::ResetFramerate()
{
	SetFramerate(InitialFramerate);
}

void Clock::SetIdleFramerate()
{
	SetFramerate(IdleFramerate);
}

void Clock::SetFramerate(double framerate) {
    if (framerate <= 0.0)
        throw std::invalid_argument("Invalid clock framerate was set");

    Framerate = framerate;
    NanoDelay = (long long)(1e9 / framerate);
}

NonBlockingClock::NonBlockingClock(double framerate, double idle_framerate)
    : Clock(framerate, idle_framerate) {

}

bool NonBlockingClock::TimePassed() {
    HPoint Ending = HNow();
    NanoElapsed = MeasureNanoseconds(Beginning, Ending);
    bool passed = (NanoElapsed >= NanoDelay);
    if (passed) {
        TotalNanoElapsed = MeasureNanoseconds(VeryBeginning, Ending);
        Beginning = HNow();
        Ticks++;
    }
    return passed;
}