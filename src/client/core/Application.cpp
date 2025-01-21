//
// Created by Matq on 19/12/2024.
//

#include "Application.h"
#include "../ui/cursors/Cursors.h"

template<> App* Singleton<App>::instance_ = nullptr;
template<> const char* Singleton<App>::singleton_name_ = "Application";

void App::PrintVersions() {
    std::wcout << Strings::FStringColorsW(L"&8SDL %d.%d.%d\n",
                                          SDL_MAJOR_VERSION,
                                          SDL_MINOR_VERSION,
                                          SDL_MICRO_VERSION);
    std::wcout << Strings::FStringColorsW(L"&8SDLimage %u.%u.%u\n",
                                          SDL_IMAGE_MAJOR_VERSION,
                                          SDL_IMAGE_MINOR_VERSION,
                                          SDL_IMAGE_MICRO_VERSION);
    std::wcout << Strings::FStringColorsW(L"&8SDLmixer %u.%u.%u\n",
                                          SDL_MIXER_MAJOR_VERSION,
                                          SDL_MIXER_MINOR_VERSION,
                                          SDL_MIXER_MICRO_VERSION);
    std::wcout << Strings::FStringColorsW(L"&8SDLttf %u.%u.%u\n",
                                          SDL_TTF_MAJOR_VERSION,
                                          SDL_TTF_MINOR_VERSION,
                                          SDL_TTF_MICRO_VERSION);
}

App::App(const char* title,
         const char* version,
         const char* identifier,
         const Vec2i& resolution,
         double framerate,
         const char* renderer_backend)
    : clock(framerate) {
    system("");
    Strings::PrintDivider(title);

    std::wcout << Strings::FStringColorsW(L"&3%s %s\n", title, version);

    bool metadata = SDL_SetAppMetadata(title, version, identifier);
    if (!metadata) {
        std::wcout << Strings::FStringColorsW(L"[Application] &cCould not set SDL Metadata\n");
        std::wcout << Strings::FStringColorsW(L"[Application] &cReason: %s\n", SDL_GetError());
    }

    bool init_sdl = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    if (!init_sdl)
        throw std::runtime_error(Strings::FString("Error while initializing sdl %s\n", SDL_GetError()));

    bool init_ttf = TTF_Init();
    if (!init_ttf)
        throw std::runtime_error(Strings::FString("Error while initializing TTF %s\n", SDL_GetError()));

    SDL_AudioSpec desired_spec;
    SDL_zero(desired_spec);
    desired_spec.freq = 44100;
    desired_spec.format = SDL_AUDIO_S16;
    desired_spec.channels = 2;

    SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &desired_spec);
    if (device_id == 0) {
        std::wcout << Strings::FStringColorsW(L"[Application] &cCould not open audio device\n");
        std::wcout << Strings::FStringColorsW(L"[Application] &cReason: %s\n", SDL_GetError());
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0)
        throw std::runtime_error(Strings::FString("Error while initializing Mix %s\n", SDL_GetError()));

    PrintVersions();

    this->resolution = resolution;
    window = SDL_CreateWindow(title,
                              resolution.x,
                              resolution.y,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window)
        throw std::runtime_error(Strings::FString("Error while creating the window %s\n", SDL_GetError()));

    std::wcout << Strings::FStringColorsW(L"[Application] &6Available render back-ends:\n");
    int numDrivers = SDL_GetNumRenderDrivers();
    for (int i = 0; i < numDrivers; i++) {
        auto info = SDL_GetRenderDriver(i);
        std::wcout << Strings::FStringColorsW(L"[Application] &6#%i %s\n", i, info);
    }

    renderer = SDL_CreateRenderer(window, renderer_backend != nullptr ? renderer_backend : "direct3d11");
    if (!renderer)
        throw std::runtime_error(Strings::FString("Error while creating the renderer %s\n", SDL_GetError()));

    std::wcout << Strings::FStringColorsW(L"[Application] &eUsed renderer: %s\n", SDL_GetRendererName(renderer));

    bool init_audio = true;
    drawing = new Drawing(renderer);
    Assets::init(new AssetsClass(drawing, init_audio));
    Cursors::Initialize();

    Strings::PrintDivider();
}

App::~App() {
    Strings::PrintDivider("Destroying");

    Cursors::Deinitialize();

    // Application objects
    Assets::destroy();
    delete drawing;

    // SDL objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // SDL as a whole
    TTF_Quit();
//    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    Strings::PrintDivider();
}

void App::Event(const SDL_Event& sdl_event) {
    switch (sdl_event.type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            resolution.x = sdl_event.window.data1;
            resolution.y = sdl_event.window.data2;

            break;
        }
    }
}
