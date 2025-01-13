//
// Created by Matq on 13/01/2025.
//

#include "IngameToolSelector.h"
#include "../../../../core/Application.h"
#include "../../../components/element/Button.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture IngameToolSelector::sTextureOutline("game.tool_selector.outline", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureOutlineSelected("game.tool_selector.outline_selected", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTexturePencil("game.tool_selector.pencil", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureEraser("game.tool_selector.eraser", sCallbackScaleNearest);
PreloadTexture IngameToolSelector::sTextureDoneButton("game.tool_selector.done_button");

auto static sPregeneratePencilButton = [](AssetsClass* assets) -> Texture* {
    auto drawing = assets->GetDrawing();
    auto texture_button = IngameToolSelector::sTextureOutline.GetTexture();
    auto texture_pencil = IngameToolSelector::sTexturePencil.GetTexture();
    Texture* pencil_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                   SDL_TEXTUREACCESS_TARGET,
                                                   (int)texture_button->GetWidth(),
                                                   (int)texture_button->GetHeight());
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
                                                   (int)texture_button->GetHeight());
    SDL_SetTextureScaleMode(eraser_button->SDLTexture(), SDL_SCALEMODE_NEAREST);
    eraser_button->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(eraser_button);
    drawing->RenderTextureFullscreen(texture_eraser->SDLTexture(), nullptr);
    drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
    return eraser_button;
};

static PregenerateTexture sTexturePencilButton("game.tool_selector.pencil_button", sPregeneratePencilButton);
static PregenerateTexture sTextureEraserButton("game.tool_selector.eraser_button", sPregenerateEraserButton);

IngameToolSelector::IngameToolSelector()
    : Frame(Vec2i(0, 0), Vec2i(0, 74), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

    // Tools
    std::vector<Element*> tools;
    auto pencil_tool = (new Button(Vec2i(0, 0),
                                  Vec2i(54, 54),
                                  sTexturePencilButton.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("PencilTool", false);
    tools.push_back(pencil_tool);

    auto eraser_tool = (new Button(Vec2i(0, 0),
                                  Vec2i(54, 54),
                                  sTextureEraserButton.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("EraserTool", false);
    tools.push_back(eraser_tool);

    auto done_button = (new Button(Vec2i(0, 0),
                                   Vec2i(192, 50),
                                   sTextureDoneButton.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("DoneButton", false);
    tools.push_back(done_button);

    // Tool selector
    SetFullyOccupy(true, false);
    SetFlex(FLEX_WIDTH);
    SetName("BrushSelector", false);
    AddChildren({ tools });
}