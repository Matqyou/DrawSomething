//
// Created by Matq on 19/12/2024.
//

#pragma once

#include <iostream>
#include "Singleton.h"
#include "Vec2.h"
#include "core/app/Drawing.h"
#include "core/app/Clock.h"
#include "core/app/Assets.h"

class App {
protected:
    SDL_Window* window;
    SDL_Renderer* renderer;

    NonBlockingClock clock;
    Drawing* drawing;

    Vec2i resolution;

    static void PrintVersions();

public:
    App(const char* title,
        const char* version,
        const char* identifier,
        const Vec2i& resolution,
        double framerate, double idle_framerate,
        const char* renderer_backend = nullptr);
    ~App();

    // Getting
    [[nodiscard]] SDL_Window* GetWindow() { return window; }
    [[nodiscard]] SDL_Renderer* GetRenderer() { return renderer; }
    [[nodiscard]] NonBlockingClock* GetClock() { return &clock; }
    [[nodiscard]] Drawing* GetDrawing() { return drawing; }
    [[nodiscard]] Vec2i GetResolution() const { return resolution; }
    [[nodiscard]] int GetWidth() const { return resolution.x; }
    [[nodiscard]] int GetHeight() const { return resolution.y; }

    // Ticking
    void Event(const SDL_Event& sdl_event);

};

typedef Singleton<App> Application;
