//
// Created by Matq on 19/12/2024.
//

#pragma once

#include <iostream>
#include "app/Drawing.h"
#include "app/Clock.h"
#include "app/Assets.h"
#include "../../shared/core/Singleton.h"
#include "../../shared/core/Vec2.h"

class App {
protected:
    SDL_Window* window;
    SDL_Renderer* renderer;

    NonBlockingClock clock;
    Drawing* drawing;

    Vec2i resolution;

    static void PrintVersions();
    static void PrintDivider(const std::string& label = "");

public:
    App(const char* title,
        const char* version,
        const char* identifier,
        const Vec2i& resolution,
        double framerate,
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
