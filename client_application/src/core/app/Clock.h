//
// Created by T450s on 26/12/2022.
//

#pragma once

#include <chrono>

#define MeasureNanoseconds(Start, End) std::chrono::duration_cast<std::chrono::nanoseconds>(End - Start).count();
#define HNow() std::chrono::high_resolution_clock::now()
typedef std::chrono::high_resolution_clock::time_point HPoint;

class Clock {
protected:
    double Framerate;
    double InitialFramerate;
	double IdleFramerate;
	long long NanoDelay;

    long long TotalNanoElapsed;
    long long NanoElapsed;

    HPoint VeryBeginning;
    HPoint Beginning;

    unsigned long long Ticks;

public:
    Clock(double framerate, double idle_framerate);

    // Getting
    [[nodiscard]] double GetFramerate() const { return Framerate; }
    [[nodiscard]] double GetTimeElapsed() const { return (double)(NanoElapsed) / 1.0e9; }
    [[nodiscard]] double GetTotalTimeElapsed() const { return (double)(TotalNanoElapsed) / 1.0e9; }
    [[nodiscard]] long long GetTimeElapsedNano() const { return NanoElapsed; }
    [[nodiscard]] long long GetTotalTimeElapsedNano() const { return TotalNanoElapsed; }
    [[nodiscard]] unsigned long long CurrentTick() const { return Ticks; }

    // Setting
	void ResetFramerate();
	void SetIdleFramerate();
    void SetFramerate(double framerate);

    // Ticking
    void Tick();
};

class NonBlockingClock : public Clock {
private:
    using Clock::Tick;

public:
    NonBlockingClock(double framerate, double idle_framerate);

    // Getting
    [[nodiscard]] long long GetTimeElapsedNanoNow() const {
        return MeasureNanoseconds(Beginning, HNow());
    }

    // Generating
    [[nodiscard]] bool TimePassed();

};