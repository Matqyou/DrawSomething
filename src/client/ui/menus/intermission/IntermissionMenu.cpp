//
// Created by Matq on 22/01/2025.
//

#include "IntermissionMenu.h"
#include "../../../core/Application.h"
#include "../../components/element/Button.h"
#include "../../CommonUI.h"

namespace Intermission {
static TextureData* sGenerateContinueButton_(AssetsClass* assets, TextureData* button_base) {
    auto drawing = assets->GetDrawing();

    auto continue_text = assets->RenderTextBlended(CommonUI::sFontSmall.GetFont()->TTFFont(),
                                                   "continue",
                                                   { 255, 255, 255, 255 });
    auto button_size = button_base->GetSize();
    auto continue_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                 SDL_TEXTUREACCESS_TARGET,
                                                 (int)button_size.x / 2,
                                                 (int)button_size.y / 2)
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(continue_button);
    drawing->RenderTextureFullscreen(button_base->SDLTexture(), nullptr);
    auto text_rect = Rectangles::CenterRelative(continue_text->GetSize(), Vec2f(193, 56));
    drawing->RenderTexture(continue_text->SDLTexture(), nullptr, text_rect);
    delete continue_text;

    return continue_button;
}
static TextureData* sGenerateContinueButton(AssetsClass* assets) {
    return sGenerateContinueButton_(assets, assets->GetTexture("button2"));
}
static TextureData* sGenerateContinueButtonPressed(AssetsClass* assets) {
    return sGenerateContinueButton_(assets, assets->GetTexture("button_pressed"));
}

static LinkTexture sTextureHeader("intermission.header.background");
static LinkTexture sTextureTurn("intermission.turn");
static LinkTexture sTextureCoin("intermission.coin");
static LinkTexture sTextureButton("button2");
static LinkTexture sTextureButtonPressed("button_pressed");
static PregenerateTexture sTextureContinueButton("intermission.continue_button", sGenerateContinueButton);
static PregenerateTexture
    sTextureContinueButtonPressed("intermission.continue_button_pressed", sGenerateContinueButtonPressed);

}

IntermissionMenu::IntermissionMenu()
    : FullscreenMenu() {
    name = L"IntermissionMenu";
    auto assets = Assets::Get();
    this->texture_turn_number = nullptr;

    // New record render
    auto new_record_text =
        assets->RenderTextBlended(CommonUI::sFontBiggest.GetFont()->TTFFont(),
                                  "NEW RECORD!",
                                  { 0, 0, 0, 255 })
            ->FlagForAutomaticDeletion();

    // New record text
    auto new_record = (Frame*)(new Frame(Vec2i(0, 0),
                                         Vec2i(new_record_text->GetSize()),
                                         new_record_text->SDLTexture()))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("NewRecordText");

    // New record
    auto bar = (new Frame(Vec2i(0, 100),
                          Vec2i(0, 68),
                          Vec2i(0, 75),
                          Vec2i(0, 0),
                          Intermission::sTextureHeader.GetSDLTexture()))
        ->SetFullyOccupy(true, false)
        ->SetFlexInvolved(false, false)
        ->SetName("NewRecordBar")
        ->AddChildren({ new_record });

    // Top Frame
    auto top_frame = (new Frame(Vec2i(0, 0),
                                Vec2i(0, 175),
                                DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetName("TopFrame")
        ->AddChildren({ bar });

    auto turn_render =
        assets->RenderTextBlended(CommonUI::sFontGiant1.GetFont()->TTFFont(),
                                  "turn",
                                  { 255, 255, 255, 255 })
            ->FlagForAutomaticDeletion();

    auto turn_text = (new Frame(Vec2i(0, 10),
                                Vec2i(turn_render->GetSize()),
                                turn_render->SDLTexture()))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnText");
    text_turn_number = (Frame*)(new Frame(Vec2i(0, -10),
                                          Vec2i(0, 0),
                                          DRAW_TEXTURE))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnNumber");

    auto turn_text_frame = (new Frame(Vec2i(0, 0),
                                      Vec2i(0, 0),
                                      DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("TurnTextFrame")
        ->AddChildren({ turn_text, text_turn_number });

    auto turn = (new Frame(Vec2i(0, 0),
                           Vec2i(295 * 0.9, 295 * 0.9),
                           Vec2i(308 * 0.9, 342 * 0.9),
                           Vec2i(0, 0),
                           Intermission::sTextureTurn.GetSDLTexture()))
        ->SetName("Turn")
        ->AddChildren({ turn_text_frame });

    auto continue_button = (new Button(Vec2i(0, 0),
                                       Vec2i(193, 56),
                                       VisualTexture(Intermission::sTextureContinueButton.GetSDLTexture(),
                                                     Vec2d(1.028423772609819, 1.3716814159292035),
                                                     Vec2d(0, 0)),
                                       VisualTexture(Intermission::sTextureContinueButtonPressed.GetSDLTexture(),
                                                     Vec2d(1.0129198966408268, 1.1150442477876106),
                                                     Vec2d(0, 0.12903225806451613))))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("ContinueButton");

    auto turn_frame = (new Frame(Vec2i(0, 0),
                                 Vec2i(0, 0),
                                 DONT_DRAW))
        ->SetFlex(Flex::HEIGHT, 85)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("TurnFrame")
        ->AddChildren({ turn, continue_button });

    auto bottom_frame = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 0),
                                   DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetName("BottomFrame")
        ->AddChildren({ turn_frame });

    menu_a = (Frame*)(new Frame(Vec2i(0, 0),
                                Vec2i(0, 0),
                                DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetFullyOccupy(true, true)
        ->SetName("MenuA")
        ->AddChildren({ top_frame, bottom_frame })
        ->SetEnabled(false);

    // B: Mini title
    auto mini_title_render = assets->RenderTextBlended(CommonUI::sFontMiniProfile.GetFont()->TTFFont(),
                                                       "The drawing was guessed correctly!",
                                                       { 255, 255, 255, 255 })
        ->FlagForAutomaticDeletion();

    // B: Mini title text
    auto mini_title_text = (Frame*)(new Frame(Vec2i(0, 0),
                                              Vec2i(mini_title_render->GetSize()),
                                              mini_title_render->SDLTexture()))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TitleText");

    // B: Title
    auto title_render = assets->RenderTextBlended(CommonUI::sFontBiggest.GetFont()->TTFFont(),
                                                  "VERI AMAZINK",
                                                  { 255, 255, 255, 255 })
        ->FlagForAutomaticDeletion();

    // B: Title text
    auto title_text = (Frame*)(new Frame(Vec2i(0, 0),
                                         Vec2i(title_render->GetSize()),
                                         title_render->SDLTexture()))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TitleText");

    // B: Title frame
    auto title = (new Frame(Vec2i(0, 0),
                            Vec2i(150, 68),
                            DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("TitleFrame")
        ->AddChildren({ mini_title_text, title_text });

    mini_profile1 = new Intermission::MiniProfile();
    mini_profile2 = new Intermission::MiniProfile();

    auto mini_profiles = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW))
        ->SetFlex(Flex::WIDTH, 75)
        ->SetAdaptive(true, true)
        ->SetName("MiniProfiles")
        ->AddChildren({ mini_profile1, mini_profile2 });

    auto coins_earned_text = (TextElement*)(new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "COINS EARNED:", { 200, 240, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("CoinsEarnedText");

    auto coin = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(36, 36), Intermission::sTextureCoin.GetSDLTexture()));
    auto coin_text =  (new TextElement(Vec2i(0, 0)))->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "+1", { 255, 255, 255, 255 })->SetAlign(Align::CENTER, Align::DONT);
    auto coin_frame = (new Frame())->SetFlex(Flex::HEIGHT)->SetAdaptive(true, true)->AddChildren({ coin, coin_text });

    auto coin2 = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(36, 36), Intermission::sTextureCoin.GetSDLTexture()));
    auto coin_text2 =  (new TextElement(Vec2i(0, 0)))->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "+1", { 255, 255, 255, 255 })->SetAlign(Align::CENTER, Align::DONT);
    auto coin_frame2 = (new Frame())->SetFlex(Flex::HEIGHT)->SetAdaptive(true, true)->AddChildren({ coin2, coin_text2 });

    auto coin3 = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(36, 36), Intermission::sTextureCoin.GetSDLTexture()));
    auto coin_text3 =  (new TextElement(Vec2i(0, 0)))->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "+1", { 255, 255, 255, 255 })->SetAlign(Align::CENTER, Align::DONT);
    auto coin_frame3 = (new Frame())->SetFlex(Flex::HEIGHT)->SetAdaptive(true, true)->AddChildren({ coin3, coin_text3 });

    auto coins_frame = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetFlex(Flex::WIDTH, 6)
        ->SetAdaptive(true, true)
        ->SetName("CoinsFrame")
        ->AddChildren({ coin_frame, coin_frame2, coin_frame3 });

    auto coins_earned_frame = (Frame*)(new Frame(Vec2i(0, 0),
                                                 Vec2i(0, 0),
                                                 DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::HEIGHT)
        ->AddChildren({ coins_earned_text, coins_frame });

    auto stats_elements = (Frame*)(new Frame(Vec2i(0, 0),
                                             Vec2i(0, 0),
                                             DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetFlex(Flex::HEIGHT, 6)
        ->SetAdaptive(true, true)
        ->SetName("StatsElements")
        ->AddChildren({ mini_profiles, coins_earned_frame });

    scribbles = (Intermission::Scribbles*)(new Intermission::Scribbles(Vec2i(0, 0),
                                                                       Vec2i(400, 400),
                                                                       35))
        ->SetAlpha(100)
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Scribbles")
        ->AddChildren({ stats_elements });

    auto stats_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DONT_DRAW))
        ->SetFlex(Flex::HEIGHT, 30)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("StatsFrame")
        ->AddChildren({ title, scribbles });

    menu_b = (Frame*)(new Frame(Vec2i(0, 0),
                                Vec2i(0, 0),
                                DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetFullyOccupy(true, true)
        ->SetName("MenuB")
        ->AddChildren({ stats_frame });

    SetTurnNumber(-1);

    SetFlex(Flex::HEIGHT);
    AddChildren({ menu_a, menu_b });
    SetName("IntermissionMenu");
    SetColor(253, 164, 62, 255);
    RefreshMenu();
}

IntermissionMenu::~IntermissionMenu() {

}

void IntermissionMenu::SetTurnNumber(int turn_number) {
    auto assets = Assets::Get();
    delete texture_turn_number;
    texture_turn_number = assets->RenderTextBlended(CommonUI::sFontGiant2.GetFont()->TTFFont(),
                                                    Strings::FString("%d", turn_number),
                                                    { 255, 255, 255, 255 });
    if (text_turn_number) {
        text_turn_number->sdl_texture = texture_turn_number->SDLTexture();
        text_turn_number->size = Vec2i(texture_turn_number->GetSize());
        text_turn_number->visual_size = Vec2i(texture_turn_number->GetSize());
    }
}

void IntermissionMenu::PlayScribbles() {
    scribbles->GenerateZigZag(3, 15, 25, 30, 50, { 0.2, 0, 0, 1.0 });
}

void IntermissionMenu::SetProfile1(const std::string& username, TextureData* profile_picture) {
    mini_profile1->SetText(username);
    mini_profile1->SetProfilePicture(profile_picture);
}

void IntermissionMenu::SetProfile2(const std::string& username, TextureData* profile_picture) {
    mini_profile2->SetText(username);
    mini_profile2->SetProfilePicture(profile_picture);
}
