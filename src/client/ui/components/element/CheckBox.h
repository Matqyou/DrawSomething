//
// Created by Matq on 17/02/2025.
//

#pragma once

#include "base/Element.h"

class CheckBox : public Element {
private:
    using Callback = std::function<void()>;
    Callback activated_callback;
    Callback deactivated_callback;
    bool clickable;

    VisualTextureInstance activated_visual_texture;
    bool activated;

public:
    CheckBox();
    CheckBox(Texture* texture, Texture* activated_texture);
    ~CheckBox() override;

    // Getting
    [[nodiscard]] bool Activated() const { return activated; }

    // Options
    CheckBox* SetActivatedCallback(Callback callback) {
        this->activated_callback = std::move(callback);
        return this;
    }
    CheckBox* SetDeactivatedCallback(Callback callback) {
        this->deactivated_callback = std::move(callback);
        return this;
    }
    CheckBox* SetClickable(bool new_clickable) {
        this->clickable = new_clickable;
        return this;
    }
    CheckBox* SetActivatedVisualTexture(Texture* texture) {
        this->activated_visual_texture.ChangeTexture(texture);
        return this;
    }

    // Manipulating
    void SetActivated(bool activated);
    void RunActivatedCallback() const;
    void RunDeactivatedCallback() const;

    // Ticking
    void PostRefresh() override;
    void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;
    void Render() override;
    void UpdateActivatedTexture();

};
