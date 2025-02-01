#define SDL_MAIN_HANDLED
#include "../shared/Protocol.h"

#include <iostream>
#include <thread>
#include <fcntl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "ui/menus/intermission/IntermissionMenu.h"
#include "ui/components/event/EventContext.h"
#include "ui/menus/picking/PickingMenu.h"
#include "ui/menus/ingame/IngameMenu.h"
#include "ui/menus/auth/AuthMenu.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/cursors/Cursors.h"
#include "core/Application.h"
#include "words/Words.h"
#include "ui/CommonUI.h"
#include "ui/particles/Particles.h"

static LinkTexture sTextureStar("particles.star", CommonUI::sCallbackScaleNearest);

void exit_application() {
    Application::destroy();
    std::wcout << L"Graceful exit\n";
    exit(0);
}

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    Application::init(new App(APPLICATION_NAME,
                              APPLICATION_VERSION,
                              "com.matq.draw_something",
                              Vec2i(1024, 720),
                              75.0));
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

    Words words_choice;
//    Particles particles;

    IntermissionMenu intermission_menu;
    PickingMenu picking_menu;
    AuthMenu auth_menu;
    MainMenu main_menu;
    IngameMenu guessing_menu(&words_choice);
    auto current_menu = (FullscreenMenu*)(&main_menu);

    Vec2i render_drag_from = Vec2i(0, 0);
    bool render_dragging = false;
    bool render_debug = false;
    long long frame_duration = 1;
    bool update_frame_duration = true;
    Uint64 last_fps_update = 0;
    TextureData* fps_texture = nullptr;
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

//                    if (std::rand() % 25 != 0)
//                        break;
//
//                    float mouse_x, mouse_y;
//                    SDL_GetMouseState(&mouse_x, &mouse_y);
//
//                    auto particle_texture = assets->CopyTexture(sTextureStar.GetSDLTexture(), SDL_TEXTUREACCESS_TARGET)
//                        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
//                        ->FlagForAutomaticDeletion();
//
//                    particle_texture->SetColorMod(255, 200 - rand() % 50, 125);
//                    particle_texture->SetAlphaMod(100 + rand() % 150);
//                    Particles.PlayParticle(
//                        FlyingParticle(
//                            Vec2f(mouse_x, mouse_y),
//                            Vec2f(90.0f, 90.0f),
//                            0.25f,
//                            Vec2f((float)(std::rand() % 100 - 50) / 5.0f, (float)(std::rand() % 100 - 50) / 5.0f),
//                            Vec2f(0.0f, 0.2f),
//                            0.96f,
//                            0.0f,
//                            (float)(std::rand() % 20) - 10.0f,
//                            0.99f,
//                            particle_texture
//                        )
//                    );
                    break;
                }
                case SDL_EVENT_KEY_DOWN: {
                    if (sdl_event.key.scancode == SDL_SCANCODE_9) current_menu->DebugPrint();
                    else if (sdl_event.key.scancode == SDL_SCANCODE_0)
                        Assets::Get()->SaveTextureToDisk(guessing_menu.canvas->GetCanvasTexture(), "canvas_export.png");
                    else if (sdl_event.key.scancode == SDL_SCANCODE_GRAVE) render_debug = !render_debug;
                    else if (sdl_event.key.scancode == SDL_SCANCODE_1) {
                        current_menu = &intermission_menu;
                        intermission_menu.SetTurnNumber(std::rand() % 1000);
                        intermission_menu.ShowMenuA(true);
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_2) {
                        current_menu = &intermission_menu;
                        intermission_menu.PlayScribbles();
                        intermission_menu.ShowMenuA(false);
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_3) {
                        current_menu = &picking_menu;
                        current_menu->RefreshMenu();
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_4) {
                        current_menu = &auth_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_5) {
                        current_menu = &main_menu;
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_6) {
                        current_menu = &guessing_menu;
                        ((IngameMenu*)current_menu)->PrepareGuess();
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_7) {
                        current_menu = &guessing_menu;
                        ((IngameMenu*)current_menu)->PrepareWatch();
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
                    } else if (sdl_event.key.scancode == SDL_SCANCODE_8) {
                        current_menu = &guessing_menu;
                        ((IngameMenu*)current_menu)->PrepareDraw();
                        current_menu->UpdateElement({ 0, 0 },
                                                    application->GetResolution(),
                                                    application->GetResolution());
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
            current_menu->Tick();
            Particles.Tick();

            // Drawing
            drawing->SetRenderTarget(nullptr);

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
                    drawing->DrawLine(Vec2f((float)render_drag_from.x, 0),
                                      Vec2f((float)render_drag_from.x, (float)application->GetHeight()));
                    drawing->DrawLine(Vec2f(0, (float)render_drag_from.y),
                                      Vec2f((float)application->GetWidth(), (float)render_drag_from.y));

                    auto debug_size = Strings::FString("%i, %i", abs(debug_width), abs(debug_height));
                    TextureData
                        * debug_render = Assets::Get()->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                                                          debug_size,
                                                                          { 255, 255, 255, 255 });
                    SDL_FRect text_rect = {
                        ((float)render_drag_from.x + ((float)debug_width - debug_render->GetWidth()) / 2.0f),
                        ((float)render_drag_from.y + ((float)debug_height - debug_render->GetHeight()) / 2.0f),
                        (float)(debug_render->GetWidth()),
                        (float)(debug_render->GetHeight()),
                    };
                    drawing->RenderTexture(debug_render->SDLTexture(), nullptr, text_rect);
                    delete debug_render;
                }

                drawing->SetColor(255, 0, 255, 255);
                drawing->DrawLine(Vec2f(mouse_x, 0), Vec2f(mouse_x, (float)application->GetHeight()));;
                drawing->DrawLine(Vec2f(0, mouse_y), Vec2f((float)application->GetWidth(), mouse_y));

                auto coordinates = Strings::FString("%i, %i", (int)mouse_x, (int)mouse_y);
                TextureData
                    * debug_render = Assets::Get()->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
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

            Particles.Render();

            if (last_fps_update - SDL_GetTicks() >= 1000) {
                last_fps_update = SDL_GetTicks();
                long long fps = (long long)1e9 / frame_duration;
                delete fps_texture;
                fps_texture = assets->RenderTextSolid(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                                      Strings::FString("%lld", fps),
                                                      { 0, 255, 0, 255 });
                fps_texture_rect = { 0, 0, fps_texture->GetWidth(), fps_texture->GetHeight() };
            }
            if (fps_texture != nullptr)
                drawing->RenderTexture(fps_texture->SDLTexture(), nullptr, fps_texture_rect);

            drawing->UpdateWindow();
        }
        if (update_frame_duration) {
            update_frame_duration = false;
            frame_duration = clock->GetTimeElapsedNanoNow();
        }
    }

    return 0;
}
