#define SDL_MAIN_HANDLED

#include <iostream>
#include <thread>
#include <fcntl.h>

#include "client/core/Application.h"
#include "client/ui/components/Scribbles.h"
#include "client/ui/components/event/EventContext.h"
#include "client/ui/menus/MainMenu.h"
#include "client/ui/menus/GuessingMenu.h"

std::vector<Scribbles*> scribbles;
LoadedTexture pencil("pencil2");
LoadedFont sDefaultFont("minecraft", 24);

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
    Application::init(new App("DrawSomething", Vec2i(1024, 720), 60.0));
    auto application = Application::Get();
    auto drawing = application->GetDrawing();
    auto clock = application->GetClock();

    Assets::Get()->GetTexture("header")->SetColorMod(100, 190, 255);
    Assets::Get()->GetTexture("game.guessing_bar")->SetColorMod(100, 190, 255);
    Assets::Get()->GetTexture("game.guessing_bar")->SetAlphaMod(150);
    Assets::Get()->GetTexture("game.guessing_bar")->SetBlendMode(SDL_BLENDMODE_BLEND);
    Assets::Get()->GetTexture("game.letter.slot")->SetColorMod(100, 190, 255);
    Assets::Get()->GetTexture("game.letter.normal")->SetColorMod(100, 190, 255);

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

    bool render_debug = false;
    while (true) {
        EventContext event_context;
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event)) {
            application->Event(sdl_event);
            current_menu->HandleEvent(sdl_event, event_context);

            switch (sdl_event.type) {
                case SDL_QUIT: {
                    exit_application();
                    break;
                }
                case SDL_MOUSEMOTION: {
                    event_context.had_mouse_motion = true;
                    break;
                }
                case SDL_KEYDOWN: {
                    if (sdl_event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                        for (auto scribb : scribbles)
                            scribb->GenerateZigZag();
                    else if (sdl_event.key.keysym.scancode == SDL_SCANCODE_Q) render_debug = !render_debug;
                    else if (sdl_event.key.keysym.scancode == SDL_SCANCODE_W) current_menu->DebugPrint();
                    else if (sdl_event.key.keysym.scancode == SDL_SCANCODE_1) current_menu = &main_menu;
                    else if (sdl_event.key.keysym.scancode == SDL_SCANCODE_2) current_menu = &guessing_menu;

                    break;
                }

            }
        }
        if (event_context.had_mouse_motion &&
            event_context.cursor_changed == CursorChange::NO_CHANGE)
            SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW));

        if (clock->TimePassed()) {
            for (auto scribb : scribbles)
                scribb->Tick();

            drawing->SetColor(94, 152, 224, 255);
            drawing->Clear();

//        drawing->SetColor(0, 0, 0, 100);
//        drawing->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
//        drawing->FillRect({ 50, (int)(80 * 0.722) + 50, application->GetWidth() - 100, application->GetHeight() - 100 - (int)(80 * 0.722) });

            for (auto scribb : scribbles)
                scribb->Draw();
            if (render_debug) {
                for (auto scribb : scribbles)
                    scribb->DrawDebug();
            }

            auto scribble1 = scribbles[0];
            if (scribble1->IsPlaying() || true) {
                auto pen_position = scribble1->GetGlobalPen() - pencil_tip * 0.2;
                SDL_Rect pencil_rect = {
                    (int)pen_position.x,
                    (int)pen_position.y,
                    (int)(pencil.GetTexture()->GetWidth() * 0.2),
                    (int)(pencil.GetTexture()->GetHeight() * 0.2),
                };
                SDL_Point center = { (int)(pencil_tip.x * 0.2), (int)(pencil_tip.y * 0.2) };
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
                int mouse_x, mouse_y;
                SDL_GetMouseState(&mouse_x, &mouse_y);
                drawing->SetColor(255, 255, 0, 255);
                drawing->DrawLine(Vec2i(mouse_x, 0), Vec2i(mouse_x, application->GetHeight()));
                drawing->SetColor(255, 0, 255, 255);
                drawing->DrawLine(Vec2i(0, mouse_y), Vec2i(application->GetWidth(), mouse_y));

                auto coordinates = Strings::FString("%i, %i", mouse_x, mouse_y);
                SDL_Surface* surface =
                    TTF_RenderText_Blended(sDefaultFont.GetFont()->TTFFont(),
                                           coordinates.c_str(),
                                           { 255, 255, 255, 255 });
                Texture* line_render = Assets::Get()->TextureFromSurface(surface);
                SDL_FreeSurface(surface);
                SDL_Rect text_rect = {
                    mouse_x - line_render->GetWidth(),
                    mouse_y - line_render->GetHeight(),
                    line_render->GetWidth(),
                    line_render->GetHeight(),
                };
                drawing->RenderTexture(line_render->SDLTexture(), nullptr, text_rect);
                delete line_render;
            }

            drawing->UpdateWindow();
        }
    }

    return 0;
}
