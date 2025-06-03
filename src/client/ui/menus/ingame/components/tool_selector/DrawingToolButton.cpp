//
// Created by Matq on 31/01/2025.
//

#include "DrawingToolButton.h"
#include "../../../../CommonUI.h"
#include "BrushSizeButton.h"
#include "ToolSelector.h"

namespace Ingame {
static LinkTexture sTextureOutlineSelected("game.tool_selector.outline_selected", CommonUI::sCallbackScaleNearest);

void DrawingToolButton::GenerateOverlayColor(const SDL_Color& sdl_color) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    Texture* overlay_texture = sTextureOutlineSelected.GetTexture();
    Vec2i overlay_size = Vec2i(overlay_texture->GetOriginalSize());

    delete generated_overlay;
    generated_overlay = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                              SDL_TEXTUREACCESS_TARGET,
                                              overlay_size.x,
                                              overlay_size.y)
        ->SetScaleMode(SDL_SCALEMODE_NEAREST);
    drawing->SetRenderTarget(generated_overlay);
    overlay_texture->SetColorMod(sdl_color);
    drawing->RenderTextureFullscreen(overlay_texture->SDLTexture(), nullptr);

    SetOverlayTexture(generated_overlay);
    UpdateOverlayTexture();
}

DrawingToolButton::DrawingToolButton(Canvas* canvas, ToolSelector* tool_selector)
    : OverlayButton(nullptr, nullptr, nullptr) {
    this->name = L"DrawingTool";
    this->SetSize(Vec2i(54, 54));
    this->SetAlign(Align::DONT, Align::CENTER);

    this->tool_selector_ = tool_selector;
    this->generated_overlay = nullptr;

    this->brush_frame = (Frame*)(new Frame())
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::DONT, Align::ABOVE_TOP)
        ->SetEnabled(false)
        ->SetName("BrushSizes");

    std::vector<Element*> brush_sizes;
    for (int i = 0; i < 4; i++) {
        auto brush_size = 65.0f - (float)i * 20.0f;
        auto brush_size_button = (new BrushSizeButton(brush_size));

        brush_size_button->SetCallback([canvas, brush_size, this, brush_size_button]() {
            canvas->SetCurrentToolSize(brush_size);
            canvas->SetCustomCursor(brush_size_button->GetBrushCursor());
            brush_frame->SetFocus(brush_size_button);
            brush_frame->SetEnabled(false);
        });

        brush_sizes.push_back(brush_size_button);
    }
    this->brush_frame->AddChildren(brush_sizes);
    this->brush_frame->SetFocus(brush_frame->children.back());

    this->AddChildren({ brush_frame });
    this->GenerateBrushesColor(SDL_Color(0, 0, 0, 255));
}

DrawingToolButton::~DrawingToolButton() {
    delete generated_overlay;
}

SDL_Cursor* DrawingToolButton::GetSelectedCursor() {
    for (auto brush_size_button : brush_frame->children) {
        if (brush_size_button->has_focus)
            return ((BrushSizeButton*)brush_size_button)->GetBrushCursor();
    }

    return nullptr;
}

void DrawingToolButton::GenerateBrushesColor(const SDL_Color& sdl_color) {
    this->GenerateOverlayColor(sdl_color);

    // Recolor brush buttons
    for (auto brush_size_button : brush_frame->children) {
        auto button = (BrushSizeButton*)brush_size_button;
        button->UpdateColor(sdl_color);
        // TODO: no idea what i did here
        button->SetOverlayTexture(generated_overlay);
//        button->UpdateOverlayTexture();
    }
}

void DrawingToolButton::HideBrushSizeButtons() {
    brush_frame->SetEnabled(false);
}

void DrawingToolButton::ToggleShowBrushSizeButtons() {
    brush_frame->SetEnabled(!brush_frame->enabled);
    if (brush_frame->enabled) tool_selector_->Refresh();
}
}
