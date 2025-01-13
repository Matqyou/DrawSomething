//
// Created by Matq on 13/01/2025.
//

#include "IngameToolSelector.h"

#include <utility>
#include "../../../../core/Application.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture IngameToolSelector::sTextureOutline("game.tool_selector.outline", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureOutlineSelected("game.tool_selector.outline_selected", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTexturePencil("game.tool_selector.pencil", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureEraser("game.tool_selector.eraser", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureTrash("game.tool_selector.trash", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureGear("game.tool_selector.gear", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureDoneButton("game.tool_selector.done_button");

auto static sPregeneratePencilButton = [](AssetsClass* assets) -> Texture* {
    auto drawing = assets->GetDrawing();
    auto texture_button = IngameToolSelector::sTextureOutline.GetTexture();
    auto texture_pencil = IngameToolSelector::sTexturePencil.GetTexture();
    Texture* pencil_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                   SDL_TEXTUREACCESS_TARGET,
                                                   (int)texture_button->GetWidth(),
                                                   (int)texture_button->GetHeight())
        ->FlagForAutomaticDeletion();
    SDL_SetTextureScaleMode(pencil_button->SDLTexture(), SDL_SCALEMODE_NEAREST);
    pencil_button->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(pencil_button);
    drawing->RenderTextureFullscreen(texture_pencil->SDLTexture(), nullptr);
    drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
    return pencil_button;
};
auto static sPregenerateEraserButton = [](AssetsClass* assets) -> Texture* {
    auto drawing = assets->GetDrawing();
    auto texture_button = IngameToolSelector::sTextureOutline.GetTexture();
    auto texture_eraser = IngameToolSelector::sTextureEraser.GetTexture();
    Texture* eraser_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                   SDL_TEXTUREACCESS_TARGET,
                                                   (int)texture_button->GetWidth(),
                                                   (int)texture_button->GetHeight())
        ->FlagForAutomaticDeletion();
    SDL_SetTextureScaleMode(eraser_button->SDLTexture(), SDL_SCALEMODE_NEAREST);
    eraser_button->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(eraser_button);
    drawing->RenderTextureFullscreen(texture_eraser->SDLTexture(), nullptr);
    drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
    return eraser_button;
};
auto static sPregenerateDoneButton = [](AssetsClass* assets) -> Texture* {
    auto drawing = assets->GetDrawing();
    auto button_base = IngameToolSelector::sTextureDoneButton.GetTexture();
    auto button_text = assets->RenderTextBlended(assets->GetFont("fredoka.big")->TTFFont(),
                                                 "D O N E",
                                                 { 230, 230, 230, 255 });
    SDL_FRect text_rect = Rectangles::CenterRelative(button_text->GetSize() * 1.5, button_base->GetSize());

    Texture* done_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                 SDL_TEXTUREACCESS_TARGET,
                                                 (int)button_base->GetWidth(),
                                                 (int)button_base->GetHeight())
        ->FlagForAutomaticDeletion();
    SDL_SetTextureScaleMode(done_button->SDLTexture(), SDL_SCALEMODE_NEAREST);
    done_button->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(done_button);
    drawing->RenderTextureFullscreen(button_base->SDLTexture(), nullptr);
    drawing->RenderTexture(button_text->SDLTexture(), nullptr, text_rect);
    return done_button;
};

static PregenerateTexture sTexturePencilButton("game.tool_selector.pencil_button", sPregeneratePencilButton);
static PregenerateTexture sTextureEraserButton("game.tool_selector.eraser_button", sPregenerateEraserButton);
static PregenerateTexture sTextureDoneButton_("game.tool_selector.done_button_", sPregenerateDoneButton);

IngameToolSelector::IngameToolSelector()
    : Frame(Vec2i(0, 0), Vec2i(0, 74), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

    // Tools
    std::vector<Element*> tools;
    pencil_tool = (OverlayButton*)(new OverlayButton(Vec2i(50, 0),
                                                          Vec2i(54, 54),
                                                          sTexturePencilButton.GetTexture(),
                                                          sTextureOutlineSelected.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("PencilTool", false);
    tools.push_back(pencil_tool);

    eraser_tool = (OverlayButton*)(new OverlayButton(Vec2i(150, 0),
                                                          Vec2i(54, 54),
                                                          sTextureEraserButton.GetTexture(),
                                                          sTextureOutlineSelected.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("EraserTool", false);
    tools.push_back(eraser_tool);

    trash_button = (Button*)(new Button(Vec2i(250, 0),
                                  Vec2i(54, 54),
                                  sTextureTrash.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("ClearCanvas", false);
    tools.push_back(trash_button);

    auto done_button = (new Button(Vec2i(350, 0),
                                   Vec2i(192, 50),
                                   sTextureDoneButton_.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("DoneButton", false);
    tools.push_back(done_button);

    auto settings_button = (new Button(Vec2i(592, 0),
                                  Vec2i(54, 54),
                                  sTextureGear.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("Settings", false);
    tools.push_back(settings_button);

    // Tool selector
    SetFullyOccupy(true, false);
    SetName("BrushSelector", false);
    AddChildren({ tools });
    SetFocus(pencil_tool);
}
