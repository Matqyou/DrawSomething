//
// Created by Matq on 29/01/2025.
//

#include "LoadingScreen.h"
#include "../../CommonUI.h"
#include "../intermission/components/Scribbles.h"

namespace Loading {
TextureData* sGenerateLoadingText(AssetsClass* assets, const char* text) {
    return assets->RenderTextBlended(CommonUI::sFontBigger.GetTTFFont(), text, { 255, 255, 255, 255 });
}

static PregenerateTexture sTexturesLoadingTexts[3]{
    PregenerateTexture("loading.text1",
                       [](AssetsClass* assets) -> TextureData* { return sGenerateLoadingText(assets, "Loading."); }),
    PregenerateTexture("loading.text2",
                       [](AssetsClass* assets) -> TextureData* { return sGenerateLoadingText(assets, "Loading.."); }),
    PregenerateTexture("loading.text3",
                       [](AssetsClass* assets) -> TextureData* { return sGenerateLoadingText(assets, "Loading..."); }),
};
}

LoadingScreen::LoadingScreen()
    : FullscreenMenu() {
    last_updated_text = std::chrono::steady_clock::now();
    current_text = 0;
    screen_alpha = 150;

    loading_text = (Frame*)(new Frame(Vec2i(0, 0),
                                      Vec2i(0, 0),
                                      VisualTexture(Loading::sTexturesLoadingTexts[current_text].GetSDLTexture())))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->ResizeToTexture()
        ->SetName("LoadingText");

    auto scribbles = (new Intermission::Scribbles(Vec2i(0, 0),
                                                  Vec2i(200, 200),
                                                  25))
        ->GenerateZigZag(3, 15, 15, 15, 10, { 1.0, 1.0, 1.0, 1.0 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("LoadingScribbles");

    auto caption = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                     "Please wait while we prepare the game :o",
                     { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("LoadingCaption");

    loading_frame = (Frame*)(new Frame())
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetFlex(Flex::HEIGHT, 10)
        ->SetAdaptive(true, true)
        ->SetName("LoadingFrame")
        ->AddChildren({ scribbles, loading_text, caption });

    SetEnabled(false);
    SetColor(0, 0, 0, screen_alpha);
    SetFlexInvolved(false, false);
    SetFullyOccupy(true, true);
    SetName("LoadingScreen");
    AddChildren({ loading_frame });
}

LoadingScreen::~LoadingScreen() {

}

void LoadingScreen::StartShowing() {
    this->SetEnabled(true);
    this->parent->Refresh();
}

void LoadingScreen::HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) {
    HandleEventChildren(sdl_event, event_summary);

    // When multiple menus are stacked, the oldest one will prevent events from passing onto lower menus
    event_summary.rapid_context.event_captured = true;

    switch (sdl_event.type) {
        case SDL_EVENT_KEY_DOWN: {
            if (sdl_event.key.scancode == SDL_SCANCODE_ESCAPE)
                this->SetEnabled(false);

            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            RefreshMenu();
            break;
        }
    }
}

void LoadingScreen::Tick() {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - last_updated_text).count() >= 600) {

        if (screen_alpha < 255) screen_alpha++;
        SetColor(0, 0, 0, screen_alpha);

        this->last_updated_text = std::chrono::steady_clock::now();
        this->current_text = (current_text + 1) % 3;
        this->loading_text->SetVisualTexture(VisualTexture(Loading::sTexturesLoadingTexts[current_text].GetSDLTexture()));
        this->loading_text->UpdateVisualTexture();
        this->loading_text->ResizeToTexture();
        this->Refresh();
    }

    TickChildren();
}
