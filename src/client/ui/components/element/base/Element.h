//
// Created by Matq on 16/12/2024.
//

#pragma once

#include "SDL3/SDL.h"
#include "../../../../../shared/Protocol.h"
#include "../../../../core/app/Drawing.h"
#include "../../../../core/Rectangles.h"
#include "../../../structures/VisualTexture.h"
#include "../../event/EventContext.h"

enum class Flex {
    DONT,
    WIDTH,
    HEIGHT,
};

enum ElementDraw {
    DONT_DRAW,
    DRAW_RECT,
    DRAW_TEXTURE,
    DRAW_VISUAL_TEXTURE,
};

struct Element {
protected:
    Vec2i composition_pos;
    void UpdateComposition();

    static std::vector<Element*> sDestroyElements;

public:
    Element* parent;
    std::vector<Element*> children;

    Vec2i relative, pos, size, edge;
    Vec2i visual_size, visual_offset;
    ElementDraw draw;
    SDL_Texture* sdl_texture;
    VisualTexture visual_texture;
    std::wstring name;

    bool enabled;
    bool fullscreen_element;
    bool flex_involved_horizontal, flex_involved_vertical;
    bool occupy_width, occupy_height;
    bool occupy_fully_width, occupy_fully_height;
    bool adaptive_width, adaptive_height;
    Align align_horizontal, align_vertical;
    Flex flex;
    int flex_gap;

    bool has_focus;
    SDL_Color color;
    SDL_Color focus_color;

    bool flagged_to_destroy;

    static void DestroyElements();

private:
    void AlignHorizontal_();
    void AlignVertical_();
    int GetFlexSlice();
    void FlexChildHorizontal_(Element* child, int flex_slice, int& current_flex) const;
    void FlexChildVertical_(Element* child, int flex_slice, int& current_flex) const;

public:
    Element(const Vec2i& relative, const Vec2i& size, ElementDraw draw);
    Element(const Vec2i& relative,
            const Vec2i& size,
            const Vec2i& visual,
            const Vec2i& offset,
            SDL_Texture* sdl_texture);
    Element(const Vec2i& relative, const Vec2i& size, const VisualTexture& visual_texture);
    virtual ~Element();

    // Getting
    [[nodiscard]] SDL_Texture* SDLTexture() const { return sdl_texture; }
    [[nodiscard]] SDL_FRect GetRect() const {
        return SDL_FRect((float)pos.x,
                         (float)pos.y,
                         (float)size.x,
                         (float)size.y);
    }
    [[nodiscard]] SDL_FRect GetVisualRect() const {
        return SDL_FRect((float)(pos.x + visual_offset.x),
                         (float)(pos.y + visual_offset.y),
                         (float)visual_size.x,
                         (float)visual_size.y);
    }

    // Generating
    [[nodiscard]] bool PointCollides(int x, int y) const;

    // Options
    Element* SetChildren(const std::vector<Element*>& children);
    Element* AddChildren(const std::vector<Element*>& children);
    Element* SetEnabled(bool enabled) {
        this->enabled = enabled;
        return this;
    }
    Element* SetDraw(ElementDraw new_draw) {
        this->draw = new_draw;
        return this;
    }
    Element* SetFlexInvolved(bool horizontal, bool vertical) {
        this->flex_involved_horizontal = horizontal;
        this->flex_involved_vertical = vertical;
        return this;
    }
    Element* SetOccupy(bool horizontal, bool vertical) {
        this->occupy_width = horizontal;
        this->occupy_height = vertical;
        return this;
    }
    Element* SetFullyOccupy(bool horizontal, bool vertical) {
        this->occupy_fully_width = horizontal;
        this->occupy_fully_height = vertical;
        return this;
    }
    Element* SetFlex(Flex flex) {
        this->flex = flex;
        return this;
    }
    Element* SetFlex(Flex flex, int flex_gap) {
        this->flex = flex;
        this->flex_gap = flex_gap;
        return this;
    }
    Element* SetAlign(Align horizontal, Align vertical) {
        this->align_horizontal = horizontal;
        this->align_vertical = vertical;
        return this;
    }
    Element* SetAdaptive(bool horizontal, bool vertical) {
        this->adaptive_width = horizontal;
        this->adaptive_height = vertical;
        return this;
    }
    Element* SetName(const char* name) {
        this->name = Strings::FStringW(L"%s", name);
        return this;
    }
    Element* SetName(const wchar_t* name) {
        this->name = name;
        return this;
    }
    Element* SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        this->color = { r, g, b, a };
        return this;
    }
    Element* SetFocusColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        this->focus_color = { r, g, b, a };
        return this;
    }
    Element* SetSDLTexture(SDL_Texture* sdl_texture) {
        this->sdl_texture = sdl_texture;
        return this;
    }
    Element* SetVisualTexture(const VisualTexture& visual_texture) {
        this->visual_texture = visual_texture;
        return this;
    }
    Element* ResizeToTexture() {
        this->size = Vec2i(visual_texture.GetOriginalTextureSize());
        this->edge = pos + size;
        return this;
    }
    Element* SetFullscreenElement() {
        this->fullscreen_element = true;
        return this;
    }

    // Setting
    void SetParent(Element* parent) { this->parent = parent; };

    // Manipulating
    void UnfocusChildren();
    void SetFocus(Element* focus_element);
    void UpdateElement(const Vec2i& new_pos, const Vec2i& new_size, const Vec2i& new_visual);
    void Refresh(int child_generation = 0);
    void DebugPrint(std::vector<bool> level = { }, bool last_child = true);
    void FlagToDestroy();

    // Ticking
    virtual void Tick();
    virtual void HandleEvent(SDL_Event& event, EventContext& event_summary);
    virtual void Render();
    virtual void RenderDebug();
    virtual void PostEvent();
    virtual void PostRefresh();

    void PostEventChildren() const;
    void TickChildren() const;
    void HandleEventChildren(SDL_Event& event, EventContext& event_summary);
    void RenderChildren() const;
    void RenderDebugChildren() const;
    void UpdateVisualTexture();

};