#define SDL_MAIN_HANDLED
#define DRAWSOMETHING_VERSION "1.2.0"
#define DRAWSOMETHING_NAME "DrawSomething"

#include <iostream>
#include <thread>
#include <fcntl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "core/Application.h"
#include "ui/components/Scribbles.h"
#include "ui/components/event/EventContext.h"
#include "ui/menus/MainMenu.h"
#include "ui/menus/IngameMenu.h"
#include "words/Words.h"
#include "ui/menus/AuthMenu.h"
#include "ui/cursors/Cursors.h"

std::vector<Scribbles*> scribbles;
static PreloadTexture pencil("pencil2");
static PreloadFont sFontDefault("default", "fredoka.bold", 26);
static PreloadFont sFontMinecraft24("minecraft", "minecraft", 24);
static PreloadFont sFontMath16("math", "computer_modern.cmunrm", 16);

static PreloadFont sFontDefaultBiggest("fredoka.biggest", "fredoka.bold", 46);
static PreloadFont sFontDefaultBigger("fredoka.big", "fredoka.bold", 36);
static PreloadFont sFontDefaultSmaller("fredoka.small", "fredoka.medium", 18);

Vec2d pencil_tip(222, 918);

void exit_application() {
    for (auto scribb : scribbles)
        delete scribb;

    Application::destroy();
    std::wcout << L"Graceful exit" << std::endl;
    exit(0);
}

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    Application::init(new App(DRAWSOMETHING_NAME,
                              DRAWSOMETHING_VERSION,
                              "com.matq.draw_something",
                              Vec2i(1024, 720),
                              75.0));
    auto application = Application::Get();
    auto drawing = application->GetDrawing();
    auto clock = application->GetClock();

    auto scribb = new Scribbles(Vec2d(50, 450), Vec2i(200, 200), 35);
    scribb->GenerateZigZag();
    scribbles.push_back(scribb);

    auto scribb2 = new Scribbles(Vec2d(300, 450), Vec2i(250, 75), 35);
    scribb2->GenerateZigZag();
    scribbles.push_back(scribb2);

    auto scribb3 = new Scribbles(Vec2d(300, 600), Vec2i(250, 40), 35);
    scribb3->GenerateZigZag();
    scribbles.push_back(scribb3);

    SDL_StartTextInput(Application::Get()->GetWindow());

    Words words_choice;

    AuthMenu auth_menu;
    MainMenu main_menu;
    IngameMenu guessing_menu(&words_choice);
    auto current_menu = (FullscreenMenu*)(&main_menu);

    Vec2i render_drag_from = Vec2i(0, 0);
    bool render_dragging = false;
    bool render_debug = true;
    while (true) {
        EventContext event_context;
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
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
                    if (sdl_event.key.scancode == SDL_SCANCODE_SPACE)
                        for (auto scribb : scribbles)
                            scribb->GenerateZigZag();
                    else if (sdl_event.key.scancode == SDL_SCANCODE_Q) render_debug = !render_debug;
                    else if (sdl_event.key.scancode == SDL_SCANCODE_W) current_menu->DebugPrint();
                    else if (sdl_event.key.scancode == SDL_SCANCODE_E)
                        Assets::Get()->SaveTextureToDisk(guessing_menu.canvas->GetCanvasTexture(), "canvas_export.png");
                    else if (sdl_event.key.scancode == SDL_SCANCODE_GRAVE) {
                        current_menu = &auth_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_1) {
                        current_menu = &main_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_2) {
                        current_menu = &guessing_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_3 && current_menu == (FullscreenMenu*)(&guessing_menu)) {
                        ((IngameMenu*)current_menu)->PrepareGuess();
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_4 && current_menu == (FullscreenMenu*)(&guessing_menu)) {
                        ((IngameMenu*)current_menu)->PrepareWatch();
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_5 && current_menu == (FullscreenMenu*)(&guessing_menu)) {
                        ((IngameMenu*)current_menu)->PrepareDraw();
                    }

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
        }
        if (event_context.had_mouse_motion &&
            event_context.cursor_changed == CursorChange::NO_CHANGE)
            SDL_SetCursor(SDL_GetDefaultCursor());

        if (clock->TimePassed()) {
            // Ticking
            current_menu->Tick();
            for (auto scribb : scribbles)
                scribb->Tick();

            // Drawing
            drawing->SetRenderTarget(nullptr);
            for (auto scribb : scribbles)
                scribb->Draw();
            if (render_debug) {
                for (auto scribb : scribbles)
                    scribb->DrawDebug();
            }

            auto scribble1 = scribbles[0];
            if (scribble1->IsPlaying() || true) {
                auto pen_position = scribble1->GetGlobalPen() - pencil_tip * 0.2;
                SDL_FRect pencil_rect = {
                    (float)pen_position.x,
                    (float)pen_position.y,
                    (float)(pencil.GetTexture()->GetWidth() * 0.2),
                    (float)(pencil.GetTexture()->GetHeight() * 0.2),
                };
                SDL_FPoint center = { (float)(pencil_tip.x * 0.2), (float)(pencil_tip.y * 0.2) };
                drawing->RenderTextureEx(pencil.GetTexture()->SDLTexture(),
                                         nullptr,
                                         pencil_rect,
                                         35,
                                         &center,
                                         SDL_FLIP_NONE);
            }

            current_menu->Render();
            if (render_debug) {
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
                    drawing->DrawLine(Vec2i(render_drag_from.x, 0),
                                      Vec2i(render_drag_from.x, application->GetHeight()));
                    drawing->DrawLine(Vec2i(0, render_drag_from.y), Vec2i(application->GetWidth(), render_drag_from.y));

                    auto debug_size = Strings::FString("%i, %i", abs(debug_width), abs(debug_height));
                    Texture* debug_render = Assets::Get()->RenderTextBlended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                                                             debug_size,
                                                                             { 255, 255, 255, 255 });
                    SDL_FRect text_rect = {
                        (float)(render_drag_from.x + (debug_width - debug_render->GetWidth()) / 2.0f),
                        (float)(render_drag_from.y + (debug_height - debug_render->GetHeight()) / 2.0f),
                        (float)(debug_render->GetWidth()),
                        (float)(debug_render->GetHeight()),
                    };
                    drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
                    delete debug_render;
                }

                drawing->SetColor(255, 0, 255, 255);
                drawing->DrawLine(Vec2i(mouse_x, 0), Vec2i(mouse_x, application->GetHeight()));;
                drawing->DrawLine(Vec2i(0, mouse_y), Vec2i(application->GetWidth(), mouse_y));

                auto coordinates = Strings::FString("%i, %i", (int)mouse_x, (int)mouse_y);
                Texture* debug_render = Assets::Get()->RenderTextBlended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                                                         coordinates,
                                                                         { 255, 255, 255, 255 });
                SDL_FRect text_rect = {
                    (float)(mouse_x - debug_render->GetWidth()),
                    (float)(mouse_y - debug_render->GetHeight()),
                    (float)(debug_render->GetWidth()),
                    (float)(debug_render->GetHeight()),
                };
                drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
                delete debug_render;
            }

            drawing->UpdateWindow();
        }
    }

    return 0;
}
