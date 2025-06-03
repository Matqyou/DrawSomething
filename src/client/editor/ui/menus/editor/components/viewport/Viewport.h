//
// Created by Matq on 17/02/2025.
//

#pragma once

#include "../../../../../../ui/components/element/CheckBox.h"
#include "../../../../../../ui/components/element/Frame.h"
#include "../texture_button/TextureButton.h"
#include "ViewportItem.h"

namespace Editor {
class Viewport : public Frame {
private:
    ViewportItem* viewport_item;
    TextureButton* viewport_texture_button;
    TextElement* selection_size;
    TextElement* indicator_size;
    TextElement* indicator_key;
    CheckBox* check_box;

    double zoom;
    Vec2f offset;
    Vec2f dragging_from;
    bool dragging;

    bool visual_editing;

    std::chrono::steady_clock::time_point started;

public:
    Viewport();
    ~Viewport() override;

    // Getting
    [[nodiscard]] bool VisualEditing() const { return visual_editing; }

    // Manipulation
    void OpenByTextureButton(TextureButton* texture_button);
    void UpdateSelectionSizeText();

    // Ticking
    void Tick(double elapsed_seconds) override;
    void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;
    void Render() override;

};
}
