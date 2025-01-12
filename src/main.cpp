#define SDL_MAIN_HANDLED
#define DRAWSOMETHING_VERSION "1.1.0"

#include <iostream>
#include <thread>
#include <fcntl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "client/core/Application.h"
#include "client/ui/components/Scribbles.h"
#include "client/ui/components/event/EventContext.h"
#include "client/ui/menus/MainMenu.h"
#include "client/ui/menus/GuessingMenu.h"

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
    Application::init(new App("DrawSomething",
                              DRAWSOMETHING_VERSION,
                              "com.matq.draw_something",
                              Vec2i(1024, 720),
                              60.0));
    auto application = Application::Get();
    auto drawing = application->GetDrawing();
    auto clock = application->GetClock();

    Assets::Get()->GetTexture("header")->SetColorMod(100, 190, 255);
    Assets::Get()->GetTexture("game.guessing_bar")->SetColorMod(100, 190, 255);
    Assets::Get()->GetTexture("game.guessing_bar")->SetAlphaMod(200);
    Assets::Get()->GetTexture("game.guessing_bar")->SetBlendMode(SDL_BLENDMODE_BLEND);
    Assets::Get()->GetTexture("game.letter.slot")->SetColorMod(100, 190, 255);
    Assets::Get()->GetTexture("game.letter.normal")->SetColorMod(100, 190, 255);
    SDL_SetTextureScaleMode(Assets::Get()->GetTexture("game.finger")->SDLTexture(), SDL_SCALEMODE_NEAREST);

    auto slot = Assets::Get()->GetTexture("game.letter.slot");
    auto background = Assets::Get()->GetTexture("game.letter.slot_background");
    auto letter_slot = new Texture(SDL_CreateTexture(drawing->Renderer(),
                                                     SDL_PIXELFORMAT_RGBA8888,
                                                     SDL_TEXTUREACCESS_TARGET,
                                                     slot->GetWidth(),
                                                     slot->GetHeight()));
    letter_slot->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(letter_slot);
    drawing->RenderTextureFullscreen(background->SDLTexture(), nullptr);
    drawing->RenderTextureFullscreen(slot->SDLTexture(), nullptr);
    drawing->SetRenderTarget(nullptr);
    GuessingMenu::game_letter_slot_.SetSDLTexture(letter_slot->SDLTexture());

//    auto scribb = new Scribbles(Vec2d(0, 0), application->GetResolution() - 35, 35);
//    scribb->GenerateZigZag();
//    scribbles.push_back(scribb);

    auto scribb = new Scribbles(Vec2d(50, 450), Vec2i(200, 200), 35);
    scribb->GenerateZigZag();
    scribbles.push_back(scribb);

    auto scribb2 = new Scribbles(Vec2d(300, 450), Vec2i(250, 75), 35);
    scribb2->GenerateZigZag();
    scribbles.push_back(scribb2);

    auto scribb3 = new Scribbles(Vec2d(300, 600), Vec2i(250, 40), 35);
    scribb3->GenerateZigZag();
    scribbles.push_back(scribb3);

    MainMenu main_menu;
    GuessingMenu guessing_menu;
    Frame* current_menu = &main_menu;

    Vec2i render_drag_from = Vec2i(0, 0);
    bool render_dragging = false;
    bool render_debug = false;
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
                    else if (sdl_event.key.scancode == SDL_SCANCODE_1) {
                        current_menu = &main_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_2) {
                        current_menu = &guessing_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    }

                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    float mouse_x, mouse_y;
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    render_drag_from = { (int)mouse_x, (int)mouse_y };
                    render_dragging = true;
                    break;
                }
                case SDL_EVENT_MOUSE_BUTTON_UP: {
                    render_dragging = false;
                }
            }
        }
        if (event_context.had_mouse_motion &&
            event_context.cursor_changed == CursorChange::NO_CHANGE)
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT));

        if (clock->TimePassed()) {
            for (auto scribb : scribbles)
                scribb->Tick();

            drawing->SetColor(94, 152, 224, 255);
            drawing->Clear();

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
                    SDL_Surface* surface =
                        TTF_RenderText_Blended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                               debug_size.c_str(),
                                               debug_size.size(),
                                               { 255, 255, 255, 255 });
                    Texture* line_render = Assets::Get()->TextureFromSurface(surface);
                    SDL_DestroySurface(surface);
                    SDL_FRect text_rect = {
                        (float)(render_drag_from.x + (debug_width - line_render->GetWidth()) / 2.0f),
                        (float)(render_drag_from.y + (debug_height - line_render->GetHeight()) / 2.0f),
                        (float)(line_render->GetWidth()),
                        (float)(line_render->GetHeight()),
                    };
                    drawing->RenderTexture(line_render->SDLTexture(), nullptr, text_rect);
                }

                drawing->SetColor(255, 0, 255, 255);
                drawing->DrawLine(Vec2i(mouse_x, 0), Vec2i(mouse_x, application->GetHeight()));;
                drawing->DrawLine(Vec2i(0, mouse_y), Vec2i(application->GetWidth(), mouse_y));

                auto coordinates = Strings::FString("%i, %i", (int)mouse_x, (int)mouse_y);
                SDL_Surface* surface =
                    TTF_RenderText_Blended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                           coordinates.c_str(),
                                           coordinates.size(),
                                           { 255, 255, 255, 255 });
                Texture* line_render = Assets::Get()->TextureFromSurface(surface);
                SDL_DestroySurface(surface);
                SDL_FRect text_rect = {
                    (float)(mouse_x - line_render->GetWidth()),
                    (float)(mouse_y - line_render->GetHeight()),
                    (float)(line_render->GetWidth()),
                    (float)(line_render->GetHeight()),
                };
                drawing->RenderTexture(line_render->SDLTexture(), nullptr, text_rect);
                delete line_render;
            }

            drawing->UpdateWindow();
        }
    }

    return 0;
}
