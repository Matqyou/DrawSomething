#include "../shared/Protocol.h"

#define EDITOR_NAME (APPLICATION_NAME " Editor")
#define SDL_MAIN_HANDLED

#include <iostream>
#include <string>
#include <fcntl.h>
#include <windows.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../shared/core/Strings.h"
#include "ui/cursors/Cursors.h"
#include "core/Application.h"
#include "ui/CommonUI.h"
#include "ui/components/event/EventContext.h"
#include "ui/components/element/base/Element.h"
#include "editor/ui/menus/editor/Interface.h"

void exit_application() {
    Application::destroy();
    std::wcout << L"Graceful exit\n";
    exit(0);
}

void render_debugging(FullscreenMenu* current_menu, bool render_dragging, Vec2i& render_drag_from) {
    auto application = Application::Get();
    auto drawing = Assets::Get()->GetDrawing();

    current_menu->RenderDebug();
    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (render_dragging) {
        int debug_width = (int)mouse_x - render_drag_from.x;
        int debug_height = (int)mouse_y - render_drag_from.y;
        SDL_FRect debug_rect = {
            (float)render_drag_from.x,
            (float)render_drag_from.y,
            (float)debug_width,
            (float)debug_height,
        };
        drawing->SetColor(255, 0, 255, 100);
        drawing->FillRect(debug_rect);

        drawing->SetColor(255, 0, 255, 255);
        drawing->DrawLine(Vec2f((float)render_drag_from.x, 0),
                          Vec2f((float)render_drag_from.x, (float)application->GetHeight()));
        drawing->DrawLine(Vec2f(0, (float)render_drag_from.y),
                          Vec2f((float)application->GetWidth(), (float)render_drag_from.y));

        auto debug_size = Strings::FString("%i, %i", abs(debug_width), abs(debug_height));
        Texture* debug_render = Assets::Get()->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                                                     debug_size,
                                                                     { 255, 255, 255, 255 });
        Vec2f debug_render_size = debug_render->GetOriginalSize();
        SDL_FRect text_rect = {
            (float)render_drag_from.x + ((float)debug_width - debug_render_size.x) / 2.0f,
            (float)render_drag_from.y + ((float)debug_height - debug_render_size.y) / 2.0f,
            debug_render_size.x,
            debug_render_size.y,
        };
        drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
        delete debug_render;
    }

    drawing->SetColor(255, 0, 255, 255);
    drawing->DrawLine(Vec2f(mouse_x, 0), Vec2f(mouse_x, (float)application->GetHeight()));;
    drawing->DrawLine(Vec2f(0, mouse_y), Vec2f((float)application->GetWidth(), mouse_y));

    auto coordinates = Strings::FString("%i, %i", (int)mouse_x, (int)mouse_y);
    Texture* debug_render = Assets::Get()->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                                          coordinates,
                                                          { 255, 255, 255, 255 });
    Vec2f debug_render_size = debug_render->GetOriginalSize();
    SDL_FRect text_rect = {
        mouse_x - debug_render_size.x,
        mouse_y - debug_render_size.y,
        debug_render_size.x,
        debug_render_size.y,
    };
    drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
    delete debug_render;
}

void render_fps_counter(Uint64& last_fps_update, Texture** fps_texture, SDL_FRect& fps_texture_rect, long long frame_duration) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    if (last_fps_update - SDL_GetTicks() >= 1000) {
        last_fps_update = SDL_GetTicks();
        long long fps = (long long)1e9 / frame_duration;
        delete *fps_texture;
        *fps_texture = assets->RenderTextSolid(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                               Strings::FString("%lld", fps),
                                               { 0, 255, 0, 255 });
        Vec2f fps_render_size = (*fps_texture)->GetOriginalSize();
        fps_texture_rect = { 0, 0, fps_render_size.x, fps_render_size.y };
    }
    if (fps_texture != nullptr)
        drawing->RenderTexture((*fps_texture)->SDLTexture(), nullptr, fps_texture_rect);
}

int main() {
//    _setmode(_fileno(stdout), _O_U16TEXT);
    // Set the console output to UTF-8
    SetConsoleOutputCP(CP_UTF8);

    // Enable UTF-8 processing in the console
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);


    Application::init(new App(EDITOR_NAME,
                              APPLICATION_VERSION,
                              "com.matq.draw_something",
                              Vec2i(1024, 720),
                              75.0, 10.0));
    auto application = Application::Get();
    auto assets = Assets::Get();
    auto drawing = application->GetDrawing();
    auto clock = application->GetClock();

    // Loading loop
    SDL_Event event;
    while (AssetsClass::IsLoading()) {
        // Events
        while (SDL_PollEvent(&event)) {
            application->Event(event);
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    exit_application();
                    break;
                }
            }
        }

        // Ticking
        assets->ThreadedLoading();

        // Drawing
        if (clock->TimePassed()) {
            drawing->SetRenderTarget(nullptr);
            drawing->SetColor(0, 0, 0, 255);
            drawing->Clear();

            SDL_FRect progress_rect = {
                ((float)application->GetWidth() - 800.0f) / 2.0f,
                ((float)application->GetHeight() - 50.0f) / 2.0f,
                800.0f * ((float)AssetsClass::GetWorkDone() / (float)AssetsClass::GetTotalWork()),
                50.0f
            };
            drawing->SetColor(104, 195, 235, 255);
            drawing->FillRect(progress_rect);

            progress_rect.w = 800.0f;
            progress_rect.h = 50.0f;
            drawing->DrawRect(progress_rect);

            drawing->UpdateWindow();
        }
    }

    SDL_StartTextInput(Application::Get()->GetWindow());

    Editor::Interface interface_menu;
    FullscreenMenu* current_menu = &interface_menu;

    Vec2i render_drag_from = Vec2i(0, 0);
    bool render_dragging = false;
    bool render_debug = false;
    long long frame_duration = 1;
    bool update_frame_duration = true;
    Uint64 last_fps_update = 0;
    Texture* fps_texture = nullptr;
    SDL_FRect fps_texture_rect;

    SDL_Event sdl_event;
    EventContext event_context;
    drawing->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    while (true) {
        event_context.ResetContext();
        while (SDL_PollEvent(&sdl_event)) {
            event_context.ResetRapidContext();
            application->Event(sdl_event);
            current_menu->HandleEvent(sdl_event, event_context);

            switch (sdl_event.type) {
                case SDL_EVENT_QUIT: {
                    exit_application();
                    break;
                }
                case SDL_EVENT_MOUSE_MOTION: {
                    event_context.had_mouse_motion = true;
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    if (sdl_event.key.scancode == SDL_SCANCODE_9) current_menu->DebugPrint();
                    else if (sdl_event.key.scancode == SDL_SCANCODE_GRAVE) render_debug = !render_debug;
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    if (sdl_event.button.button == SDL_BUTTON_RIGHT) {
                        float mouse_x, mouse_y;
                        SDL_GetMouseState(&mouse_x, &mouse_y);
                        render_drag_from = { (int)mouse_x, (int)mouse_y };
                        render_dragging = true;
                    }
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    if (sdl_event.button.button == SDL_BUTTON_RIGHT)
                        render_dragging = false;
                    break;
                }
            }

            Element::DestroyElements(); // todo: might be expensive
        }
        if (event_context.had_mouse_motion &&
            event_context.cursor_changed == CursorChange::NO_CHANGE)
            SDL_SetCursor(SDL_GetDefaultCursor());

        if (clock->TimePassed()) {
            update_frame_duration = true;

            // Post-Event
            current_menu->PostEvent();

            // Ticking
            current_menu->Tick((double)(frame_duration) / 1e9);

            // Drawing
            drawing->SetRenderTarget(nullptr);
            current_menu->Render();

            if (render_debug)
                render_debugging(current_menu, render_dragging, render_drag_from);

            render_fps_counter(last_fps_update, &fps_texture, fps_texture_rect, frame_duration);

            drawing->UpdateWindow();
        }
        if (update_frame_duration) {
            update_frame_duration = false;
            frame_duration = clock->GetTimeElapsedNanoNow();
        }
    }

    return 0;
}
