//
// Created by Matq on 26/01/2025.
//

#include "Header.h"
#include "../../../structures/window_texture/WindowTexture.h"
#include "../../../components/element/TextElement.h"
#include "../../../components/element/Button.h"
#include "../../../CommonUI.h"

namespace Main {
static auto sCallbackHeader = [](TextureData* texture) {
    texture->SetColorMod(100, 190, 255);
};
TextureData* sGenerateCurrencyFrame(AssetsClass* assets) {
    auto drawing = assets->GetDrawing();

    Sheet sheet;
    WindowTexture wt(&sheet, 12, 12, 12, 12);
    sheet.Generate(assets, assets->GetTexture("window.sheet5"));
    TextureData* generated = wt.Generate(assets, Vec2f(66, 28));

    TextureData* currency_frame = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 66, 28);
    drawing->SetRenderTarget(currency_frame);
    generated->SetColorMod(254, 144, 72);
    drawing->RenderTextureFullscreen(generated->SDLTexture(), nullptr);

    return currency_frame;
}

static LinkTexture sTextureHeader("header", Main::sCallbackHeader);
static LinkTexture sTextureCoin("icons.coin");
static LinkTexture sTextureSettings("main_menu.header.settings");
static LinkTexture sTextureSettingsPressed("main_menu.header.settings_pressed");
static LinkTexture sTexturePlus("main_menu.header.plus");
static LinkTexture sTexturePlusPressed("main_menu.header.plus_pressed");
static PregenerateTexture sTextureCurrencyFrame("main_menu.header.currency_frame", sGenerateCurrencyFrame);

Header::Header(Settings* settings)
    : Frame(Vec2i(0, 0),
            Vec2i(0, 58),
            Vec2i(0, 80),
            Vec2i(0, 0),
            sTextureHeader.GetSDLTexture()) {

    auto currency_background =
        (new Frame(Vec2i(3, 0), Vec2i(66, 28), VisualTexture(sTextureCurrencyFrame.GetSDLTexture())))
            ->SetFlexInvolved(false, false)
            ->SetAlign(Align::DONT, Align::CENTER)
            ->SetName("CurrencyBackground");

    auto num_coins = (new TextElement(Vec2i(2, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "99", { 255, 255, 255, 255 })
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("NumberOfCoins");

    auto coin = (new Frame(Vec2i(0, 0), Vec2i(32, 32), VisualTexture(sTextureCoin.GetSDLTexture())))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("Coin");

    auto plus = (new Button(Vec2i(2, 0),
                            Vec2i(15, 15),
                            VisualTexture(sTexturePlus.GetSDLTexture()),
                            VisualTexture(sTexturePlusPressed.GetSDLTexture())))
        ->SetAlign(Align::DONT, Align::BOTTOM)
        ->SetName("Plus");

    auto currency_frame = (new Frame(Vec2i(10, 0), Vec2i(0, 0), DONT_DRAW))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetFlex(Flex::WIDTH, 2)
        ->SetAdaptive(true, true)
        ->SetName("CurrencyFrame")
        ->AddChildren({ currency_background, coin, num_coins, plus });

    auto logo = (new Frame(Vec2i(0, 0), Vec2i(200, 40), DRAW_RECT))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("Logo");

    auto settings_button = (Button*)(new Button(Vec2i(-10, 0),
                                Vec2i(40, 40),
                                VisualTexture(sTextureSettings.GetSDLTexture()),
                                VisualTexture(sTextureSettingsPressed.GetSDLTexture())))
        ->SetAlign(Align::RIGHT, Align::CENTER)
        ->SetName("Settings");
    settings_button->SetCallback([settings]() {
        settings->SetEnabled(true);
        settings->parent->Refresh();
    });

    SetFullyOccupy(true, false);
    SetName("Header");
    AddChildren({ currency_frame, logo, settings_button });
}

Header::~Header() {

}
}
