//
// Created by Matq on 13/01/2025.
//

#include "ToolSelector.h"

#include "../../../../core/Application.h"
#include "components/BrushSizeButton.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture sTextureOutline("game.tool_selector.outline", sCallbackScaleNearest);
PreloadTexture sTextureOutlineSelected("game.tool_selector.outline_selected", sCallbackScaleNearest);
PreloadTexture sTexturePencil("game.tool_selector.pencil", sCallbackScaleNearest);
PreloadTexture sTextureEraser("game.tool_selector.eraser", sCallbackScaleNearest);
PreloadTexture sTextureTrash("game.tool_selector.trash", sCallbackScaleNearest);
PreloadTexture sTextureGear("game.tool_selector.gear", sCallbackScaleNearest);
PreloadTexture sTextureDoneButton("game.tool_selector.done_button");

auto static sPregeneratePencilButton = [](AssetsClass* assets) -> Texture* {
    auto drawing = assets->GetDrawing();
    auto texture_button = sTextureOutline.GetTexture();
    auto texture_pencil = sTexturePencil.GetTexture();
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
    auto texture_button = sTextureOutline.GetTexture();
    auto texture_eraser = sTextureEraser.GetTexture();
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
    auto button_base = sTextureDoneButton.GetTexture();
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

namespace Ingame {
ToolSelector::ToolSelector(Canvas* canvas)
    : Frame(Vec2i(0, 0), Vec2i(0, 74), DONT_DRAW) {
    // Tools
    std::vector<Element*> tools;

    // ==============================================================================================================
    std::vector<Element*> pencil_sizes, eraser_sizes;
    pencil_brush_frame = (Frame*)(new Frame(Vec2i(0, 0),
                                            Vec2i(0, 0),
                                            DONT_DRAW))
        ->SetAdaptive(true, true)
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(DONT_ALIGN, ALIGN_ABOVE_TOP)
        ->SetEnabled(false)
        ->SetName("PencilSizes", false);
    eraser_brush_frame = (Frame*)(new Frame(Vec2i(0, 0),
                                            Vec2i(0, 0),
                                            DONT_DRAW))
        ->SetAdaptive(true, true)
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(DONT_ALIGN, ALIGN_ABOVE_TOP)
        ->SetEnabled(false)
        ->SetName("EraserSizes", false);

    for (int i = 0; i < 4; i++) {
        auto brush_size = 35.0f - (float)i * 10.0f;
        auto brush_size_button = (new BrushSizeButton(canvas, pencil_brush_frame, brush_size));
        auto eraser_size_button = (new BrushSizeButton(canvas, eraser_brush_frame, brush_size));

        brush_size_button->SetCallback([canvas, brush_size, this, brush_size_button]() {
            canvas->SetBrushSize(brush_size);
            canvas->SetCustomCursor(brush_size_button->GetBrushCursor());
            pencil_brush_frame->SetFocus(brush_size_button);
            pencil_brush_frame->SetEnabled(false);
        });
        eraser_size_button->SetCallback([canvas, brush_size, this, eraser_size_button]() {
            canvas->SetEraserSize(brush_size);
            canvas->SetCustomCursor(eraser_size_button->GetBrushCursor());
            eraser_brush_frame->SetFocus(eraser_size_button);
            eraser_brush_frame->SetEnabled(false);
        });

        pencil_sizes.push_back(brush_size_button);
        eraser_sizes.push_back(eraser_size_button);
    }
    pencil_brush_frame->AddChildren(pencil_sizes);
    eraser_brush_frame->AddChildren(eraser_sizes);
    pencil_brush_frame->SetFocus(pencil_brush_frame->children.back());
    eraser_brush_frame->SetFocus(eraser_brush_frame->children.back());
    canvas->SetCustomCursor(((BrushSizeButton*)pencil_sizes.back())->GetBrushCursor());
    // ==============================================================================================================

    pencil_tool = (OverlayButton*)(new OverlayButton(Vec2i(50, 0),
                                                     Vec2i(54, 54),
                                                     sTexturePencilButton.GetTexture(),
                                                     sTextureOutlineSelected.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("PencilTool", false)
        ->AddChildren({ pencil_brush_frame });
    tools.push_back(pencil_tool);

    eraser_tool = (OverlayButton*)(new OverlayButton(Vec2i(150, 0),
                                                     Vec2i(54, 54),
                                                     sTextureEraserButton.GetTexture(),
                                                     sTextureOutlineSelected.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("EraserTool", false)
        ->AddChildren({ eraser_brush_frame });
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

    // Callbacks
    trash_button->SetCallback([canvas]() { canvas->ClearCanvas(); });
    pencil_tool->SetCallback([this, canvas]() {
        eraser_brush_frame->SetEnabled(false);
        canvas->SetTool(TOOL_PENCIL);
        if (!pencil_tool->has_focus) {
            SetFocus(pencil_tool);
        } else {
            pencil_brush_frame->SetEnabled(!pencil_brush_frame->enabled);
            Refresh();
        }
    });
    eraser_tool->SetCallback([this, canvas]() {
        pencil_brush_frame->SetEnabled(false);
        canvas->SetTool(TOOL_ERASER);
        if (!eraser_tool->has_focus) {
            SetFocus(eraser_tool);
        } else {
            eraser_brush_frame->SetEnabled(!eraser_brush_frame->enabled);
            Refresh();
        }
    });

    // Tool selector
    SetFullyOccupy(true, false);
    SetName("ToolSelector", false);
    AddChildren(tools);
    SetFocus(pencil_tool);
}
}
