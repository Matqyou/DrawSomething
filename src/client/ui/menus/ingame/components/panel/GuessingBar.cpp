//
// Created by Matq on 21/01/2025.
//

#include "GuessingBar.h"
#include "../../../../CommonUI.h"
#include "Letter.h"
#include <ranges>

namespace Ingame {
TextureData* sGenerateCompleteText(AssetsClass* assets) {
    auto drawing = assets->GetDrawing();

    const Vec2i text_size = Vec2i(275, 50);
    auto text_render = assets->RenderTextBlended(CommonUI::sFontBiggest.GetTTFFont(),
                                                 "correct!",
                                                 { 255, 255, 255, 255 });
    auto text_rect = Rectangles::CenterRelative(Vec2i(text_render->GetSize()), text_size);
    auto complete_text = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                               SDL_TEXTUREACCESS_TARGET,
                                               text_size.x,
                                               text_size.y)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(complete_text);
    drawing->SetColor(50, 200, 50, 170);
    drawing->Clear();
    drawing->RenderTexture(text_render->SDLTexture(), nullptr, text_rect);
    return complete_text;
}
TextureData* sGenerateGuessingBar(AssetsClass* assets, SDL_Color sdl_color) {
    auto drawing = assets->GetDrawing();

    auto original_guessing_bar = assets->GetTexture("game.ingame_panel.guessing_bar");
    auto guessing_bar = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                              SDL_TEXTUREACCESS_TARGET,
                                              (int)original_guessing_bar->GetWidth(),
                                              (int)original_guessing_bar->GetHeight())
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(guessing_bar);
    original_guessing_bar->SetColorMod(sdl_color);
    original_guessing_bar->SetAlphaMod(170);
    drawing->RenderTextureFullscreen(original_guessing_bar->SDLTexture(), nullptr);
    return guessing_bar;
}
TextureData* sGenerateGuessingBarBlue(AssetsClass* assets) { return sGenerateGuessingBar(assets, { 100, 190, 255 }); }
TextureData* sGenerateGuessingBarGreen(AssetsClass* assets) { return sGenerateGuessingBar(assets, { 75, 200, 75 }); }

PregenerateTexture sTextureGuessingBarBlue("game.ingame_panel.guesing_bar.blue", sGenerateGuessingBarBlue);
PregenerateTexture sTextureGuessingBarGreen("game.ingame_panel.guesing_bar.green", sGenerateGuessingBarGreen);
PregenerateTexture sTextureCompleteText("game.ingame_panel.complete_text", sGenerateCompleteText);

GuessingBar::GuessingBar()
    : Frame(Vec2i(0, 0),
            Vec2i(0, 90),
            VisualTexture(sTextureGuessingBarBlue.GetSDLTexture())) {
    guess_word = "not set";

    complete_text = (Frame*)(new Frame(Vec2i(0, 0),
                                    Vec2i(275, 50),
                                    VisualTexture(sTextureCompleteText.GetSDLTexture())))
        ->SetFlexInvolved(false, false)
        ->SetAlign(Align::CENTER, Align::ABOVE_TOP)
        ->SetEnabled(false)
        ->SetName("CompleteText");

    // Guessing letter frame
    guessing_frame = (Frame*)(new Frame(Vec2i(0, 0),
                                        Vec2i(0, 0),
                                        DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::WIDTH, 8);

    SetFullyOccupy(true, false);
    SetFlexInvolved(true, false);
    SetAlign(Align::DONT, Align::ABOVE_TOP);
    SetName("Guessing");
    AddChildren({ complete_text, guessing_frame });
}

GuessingBar::~GuessingBar() {

}

bool GuessingBar::IsAnsweredCorrectly() {
    std::string user_answer;
    for (auto slot : letter_slots) {
        auto occupy_letter = slot->GetOccupyLetter();
        if (occupy_letter != nullptr)
            user_answer += occupy_letter->GetLetter();
    }

    return user_answer == guess_word;
}

void GuessingBar::GenerateForWord(const std::string& word) {
    this->complete_text->SetEnabled(false);
    SetVisualTexture(VisualTexture(sTextureGuessingBarBlue.GetSDLTexture()));
    UpdateVisualTexture();

    for (auto slot : letter_slots)
        delete slot;
    letter_slots.clear();

    guess_word = word;
    auto num_slots = word.length();
    for (int i = 0; i < num_slots; i++) {
        auto new_slot = new LetterSlot();
        new_slot->UpdateRender(LetterSlot::sColorBlue);
        new_slot->SetCallback([new_slot]() { new_slot->RemoveLetter(); });
        letter_slots.push_back(new_slot);
    }

    std::vector<Element*> letter_slots_as_elements(letter_slots.begin(), letter_slots.end());
    guessing_frame->SetChildren(letter_slots_as_elements);
}

bool GuessingBar::AddLetter(Letter* new_letter) {
    size_t used_slots = 0;
    bool find_available = true;
    for (auto slot : letter_slots) {
        if (slot->GetOccupyLetter() == nullptr) {
            if (find_available) {
                find_available = false;
                new_letter->SetOccupySlot(slot);
                slot->OccupyLetter(new_letter);
                slot->SetClickable(true);
            }
        } else { used_slots++; }
    }

    if (used_slots == letter_slots.size() - 1 &&
        IsAnsweredCorrectly()) {

        this->complete_text->SetEnabled(true);
        this->parent->Refresh();
        this->SetVisualTexture(VisualTexture(sTextureGuessingBarGreen.GetSDLTexture()));
        this->UpdateVisualTexture();

        for (auto letter_slot : letter_slots) {
            Letter* OccupyLetter = letter_slot->GetOccupyLetter();
            if (OccupyLetter != nullptr)
                OccupyLetter->SetAnsweredCorrectly();
        }

        RunCallback();
    }

    return !find_available;
}

void GuessingBar::RemoveLastLetter() {
    for (auto letter_slot : std::ranges::reverse_view(letter_slots)) {
        if (letter_slot->GetOccupyLetter() != nullptr) {
            letter_slot->RemoveLetter();
            break;
        }
    }
}

void GuessingBar::RunCallback() const {
    if (callback != nullptr)
        callback();
}

void GuessingBar::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    HandleEventChildren(event, event_summary);

    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            if (event.key.scancode == SDL_SCANCODE_BACKSPACE)
                RemoveLastLetter();

            break;
        }
    }
}
}
