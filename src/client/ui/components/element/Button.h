//
// Created by Matq on 16/12/2024.
//

#pragma once

#include <utility>

#include "base/Element.h"

class Button : public Element {
protected:
    using Callback = std::function<void()>;
    Callback callback;
    bool clickable;

public:
    Button(const Vec2i& pos, const Vec2i& size);
    Button(const Vec2i& pos, const Vec2i& size, ElementDraw draw);
    Button(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset);
    Button(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture);
    Button(const Vec2i& pos, const Vec2i& size, Texture* texture);

    // Options
    Button* SetCallback(Callback callback) {
        this->callback = std::move(callback);
        return this;
    }
    Button* SetClickable(bool new_clickable) {
        this->clickable = new_clickable;
        return this;
    }


    // Manipulating
    void RunCallback() const;

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
//    void Render(Drawing* drawing) const override;

};