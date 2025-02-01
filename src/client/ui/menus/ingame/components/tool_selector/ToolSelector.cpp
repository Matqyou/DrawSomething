//
// Created by Matq on 13/01/2025.
//

#include "ToolSelector.h"

#include "../../../../../core/Application.h"
#include "../../../../CommonUI.h"
#include "BrushSizeButton.h"

namespace Ingame {
static LinkTexture sTextureOutline("game.tool_selector.outline", CommonUI::sCallbackScaleNearest);
static LinkTexture sTexturePencil("game.tool_selector.pencil", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureEraser("game.tool_selector.eraser", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureTrash("game.tool_selector.trash", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureGear("game.tool_selector.gear", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureDoneButton("game.tool_selector.done_button");

auto static sPregeneratePencilButton = [](AssetsClass* assets) -> TextureData* {
    auto drawing = assets->GetDrawing();
    auto texture_button = sTextureOutline.GetTexture();
    auto texture_pencil = sTexturePencil.GetTexture();
    TextureData* pencil_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                       SDL_TEXTUREACCESS_TARGET,
                                                       (int)texture_button->GetWidth(),
                                                       (int)texture_button->GetHeight())
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->SetBlendMode(SDL_BLENDMODE_BLEND)
        ->FlagForAutomaticDeletion();
    SDL_SetTextureScaleMode(pencil_button->SDLTexture(), SDL_SCALEMODE_NEAREST);
    drawing->SetRenderTarget(pencil_button);
    drawing->RenderTextureFullscreen(texture_pencil->SDLTexture(), nullptr);
    drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
    return pencil_button;
};
auto static sPregenerateEraserButton = [](AssetsClass* assets) -> TextureData* {
    auto drawing = assets->GetDrawing();
    auto texture_button = sTextureOutline.GetTexture();
    auto texture_eraser = sTextureEraser.GetTexture();
    TextureData* eraser_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                       SDL_TEXTUREACCESS_TARGET,
                                                       (int)texture_button->GetWidth(),
                                                       (int)texture_button->GetHeight())
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->SetBlendMode(SDL_BLENDMODE_BLEND)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(eraser_button);
    drawing->RenderTextureFullscreen(texture_eraser->SDLTexture(), nullptr);
    drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
    return eraser_button;
};
auto static sPregenerateDoneButton = [](AssetsClass* assets) -> TextureData* {
    auto drawing = assets->GetDrawing();
    auto button_base = sTextureDoneButton.GetTexture();
    auto button_text = assets->RenderTextBlended(assets->GetFont("fredoka.big")->TTFFont(),
                                                 "D O N E",
                                                 { 230, 230, 230, 255 });
    SDL_FRect text_rect = Rectangles::CenterRelative(button_text->GetSize() * 1.5, button_base->GetSize());

    TextureData* done_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                     SDL_TEXTUREACCESS_TARGET,
                                                     (int)button_base->GetWidth(),
                                                     (int)button_base->GetHeight())
        ->SetBlendMode(SDL_BLENDMODE_BLEND)
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(done_button);
    drawing->RenderTextureFullscreen(button_base->SDLTexture(), nullptr);
    drawing->RenderTexture(button_text->SDLTexture(), nullptr, text_rect);
    return done_button;
};

static PregenerateTexture sTexturePencilButton("game.tool_selector.pencil_button", sPregeneratePencilButton);
static PregenerateTexture sTextureEraserButton("game.tool_selector.eraser_button", sPregenerateEraserButton);
static PregenerateTexture sTextureDoneButton_("game.tool_selector.done_button_", sPregenerateDoneButton);

ToolSelector::ToolSelector(Canvas* canvas)
    : Frame(Vec2i(0, 0), Vec2i(0, 74), DONT_DRAW) {
    // Tools
    std::vector<Element*> tools;

    pencil_tool = (DrawingToolButton*)(new DrawingToolButton(canvas,
                                                             this,
                                                             Vec2i(50, 0),
                                                             VisualTexture(sTexturePencilButton.GetSDLTexture())))->SetName(
        "PencilTool");
    eraser_tool = (DrawingToolButton*)(new DrawingToolButton(canvas,
                                                             this,
                                                             Vec2i(150, 0),
                                                             VisualTexture(sTextureEraserButton.GetSDLTexture())))->SetName(
        "EraserTool");
    trash_button = (Button*)(new Button(Vec2i(250, 0),
                                        Vec2i(54, 54),
                                        VisualTexture(sTextureTrash.GetSDLTexture(), Vec2d(1.0, 1.0), Vec2d(0.0, 0.0)),
                                        VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("ClearCanvas");

    tools.push_back(pencil_tool);
    tools.push_back(eraser_tool);
    tools.push_back(trash_button);

    canvas->SetCustomCursor(pencil_tool->GetSelectedCursor());

    auto done_button = (new Button(Vec2i(350, 0),
                                   Vec2i(192, 50),
                                   VisualTexture(sTextureDoneButton_.GetSDLTexture(), Vec2d(1.0, 1.0), Vec2d(0.0, 0.0)),
                                   VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("DoneButton");
    tools.push_back(done_button);

    auto settings_button = (new Button(Vec2i(592, 0),
                                       Vec2i(54, 54),
                                       VisualTexture(sTextureGear.GetSDLTexture(), Vec2d(1.0, 1.0), Vec2d(0.0, 0.0)),
                                       VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("Settings");
    tools.push_back(settings_button);

    // Callbacks
    trash_button->SetCallback([canvas]() { canvas->ClearCanvas(); });
    pencil_tool->SetCallback([this, canvas]() {
        eraser_tool->HideBrushSizeButtons();
        canvas->SetTool(TOOL_PENCIL);
        if (!pencil_tool->has_focus) {
            SetFocus(pencil_tool);
            canvas->SetCustomCursor(pencil_tool->GetSelectedCursor());
        } else {
            pencil_tool->ToggleShowBrushSizeButtons();
//            Refresh();
        }
    });
    eraser_tool->SetCallback([this, canvas]() {
        pencil_tool->HideBrushSizeButtons();
        canvas->SetTool(TOOL_ERASER);
        if (!eraser_tool->has_focus) {
            SetFocus(eraser_tool);
            canvas->SetCustomCursor(eraser_tool->GetSelectedCursor());
        } else {
            eraser_tool->ToggleShowBrushSizeButtons();
//            Refresh();
        }
    });

    // Tool selector
    SetFullyOccupy(true, false);
    SetName("ToolSelector");
    AddChildren(tools);
    SetFocus(pencil_tool);
}
}
