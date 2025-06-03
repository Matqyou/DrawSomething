//
// Created by Matq on 17/02/2025.
//

#pragma once

#include "../../../../../../ui/components/element/Frame.h"

namespace Editor {
class Viewport;
class ViewportItem : public Frame {
private:
    Viewport* viewport;
    bool populated;

    Vec2i dragging_from;
    Vec2i dragging_to;
    bool dragging;

public:
    explicit ViewportItem(Viewport* viewport);
    ~ViewportItem() override;

    // Getting
    [[nodiscard]] bool IsPopulated() const { return populated; };

    // Generating
    [[nodiscard]] Rect4i GetSelection() const;
    [[nodiscard]] Rect4i GetVisualSelection() const;

    // Manipulating
    void SetSelection(const Rect4i& selection);
    void SetViewportTexture(Texture* sdl_texture);

    // Ticking
    void Tick(double elapsed_seconds) override;
    void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;
    void Render() override;

};
}
