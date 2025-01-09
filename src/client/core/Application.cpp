//
// Created by Matq on 19/12/2024.
//

#include "Application.h"

template<> App* Singleton<App>::instance_ = nullptr;
template<> const char* Singleton<App>::singleton_name_ = "Application";

void App::PrintVersions() {
    SDL_version Version;
    SDL_GetVersion(&Version);
    auto image_version = IMG_Linked_Version();
    auto mixer_version = Mix_Linked_Version();
    auto ttf_version = TTF_Linked_Version();
    std::wcout << Strings::FStringColorsW(L"&8SDL2 %u.%u.%u", Version.major, Version.minor, Version.patch) << std::endl;
    std::wcout << Strings::FStringColorsW(L"&8SDLimage %u.%u.%u",
                                        image_version->major,
                                        image_version->minor,
                                        image_version->patch) << std::endl;
    std::wcout << Strings::FStringColorsW(L"&8SDLmixer %u.%u.%u",
                                        mixer_version->major,
                                        mixer_version->minor,
                                        mixer_version->patch) << std::endl;
    std::wcout << Strings::FStringColorsW(L"&8SDLttf %u.%u.%u", ttf_version->major, ttf_version->minor, ttf_version->patch)
              << std::endl;
}

void App::PrintDivider(const std::string& label) {
    const int LENGTH = 72;

    auto label_length = label.size();
    if (label_length == 0) {
        std::wcout << Strings::FStringColorsW(L"&8%s", Strings::RepeatString("-", LENGTH).c_str()) << std::endl;
    } else {
        auto new_length = LENGTH - label_length;
        auto half = new_length / 2;
        auto remaining = new_length - half;
        std::wcout << Strings::FStringColorsW(L"&8%s", Strings::RepeatString("-", (int)half).c_str());
        std::wcout << Strings::FStringColorsW(L"&f%s", label.c_str());
        std::wcout << Strings::FStringColorsW(L"&8%s", Strings::RepeatString("-", (int)remaining).c_str());
        std::wcout << std::endl;
    }
}

App::App(const char* title, const Vec2i& resolution, double framerate)
    : clock(framerate) {
    system("");
    PrintDivider("Initializing");
    bool init_sdl = !SDL_Init(SDL_INIT_EVERYTHING);
    if (!init_sdl)
        throw std::runtime_error(Strings::FString("Error while initializing main %s\n", SDL_GetError()));

    int ImageFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    bool init_img = IMG_Init(ImageFlags) == ImageFlags;
    if (!init_img)
        throw std::runtime_error(Strings::FString("Error while initializing IMG %s\n", IMG_GetError()));

    bool init_ttf = !TTF_Init();
    if (!init_ttf)
        throw std::runtime_error(Strings::FString("Error while initializing TTF %s\n", TTF_GetError()));

    bool init_audio = !Mix_OpenAudio(44100 * 2, MIX_DEFAULT_FORMAT, 2, 1024 * 4);
    Mix_AllocateChannels(64);
    if (!init_audio) {
        std::wcout << Strings::FStringColorsW(L"[Application] &8Could not initialize SDL Audio") << std::endl;
        std::wcout << Strings::FStringColorsW(L"[Application] &8Reason: %s", Mix_GetError()) << std::endl;
    }

    int MixFlags = MIX_INIT_MP3 | MIX_INIT_OGG;
    bool init_mix = Mix_Init(MixFlags) == MixFlags;
    if (!init_mix)
        throw std::runtime_error(Strings::FString("Error while initializing Mix %s\n", Mix_GetError()));

    PrintVersions();

    this->resolution = resolution;
    window = SDL_CreateWindow(title,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              resolution.x,
                              resolution.y,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    if (!window)
        throw std::runtime_error(Strings::FString("Error while creating the window %s\n", SDL_GetError()));

    std::wcout << Strings::FStringColorsW(L"[Application] &6Available render back-ends:") << std::endl;
    int numDrivers = SDL_GetNumRenderDrivers();
    for (int i = 0; i < numDrivers; i++) {
        SDL_RendererInfo info;
        if (SDL_GetRenderDriverInfo(i, &info) == 0) {
            std::wcout << Strings::FStringColorsW(L"[Application] &6#%i %s", i, info.name) << std::endl;
        }
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d12");

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
        throw std::runtime_error(Strings::FString("Error while creating the renderer %s\n", SDL_GetError()));

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    std::wcout << Strings::FStringColorsW(L"[Application] &eUsed renderer: %s", info.name) << std::endl;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");

    drawing = new Drawing(renderer);
    Assets::init(new AssetsClass(renderer, init_audio));

    PrintDivider();
}

App::~App() {
    PrintDivider("Destroying");

    // Application objects
    Assets::destroy();
    delete drawing;

    // SDL objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // SDL as a whole
    TTF_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();

    PrintDivider();
}

void App::Event(const SDL_Event& sdl_event) {
    switch (sdl_event.type) {
        case SDL_WINDOWEVENT: {
            if (sdl_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                resolution.x = sdl_event.window.data1;
                resolution.y = sdl_event.window.data2;
            }

            break;
        }
    }
}
