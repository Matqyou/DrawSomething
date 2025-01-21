//
// Created by Matq on 16/12/2024.
//

#pragma once

#include "SDL3/SDL.h"
#include "../../../../core/app/Drawing.h"
#include "../../event/EventContext.h"
#include "../../../../core/Rectangles.h"
#include "../../../../../shared/Protocol.h"

enum ElementType {
    ELEMENT_FRAME,
    ELEMENT_BUTTON,
    ELEMENT_TEXTBOX,
    ELEMENT_CUSTOM,
    NUM_ELEMENTTYPES,
};

static const char* ELEMENTTYPE_NAMES[NUM_ELEMENTTYPES] = {
    "Frame",
    "Button",
    "Textbox",
    "Custom"
};

enum class Flex {
    DONT,
    WIDTH,
    HEIGHT,
};

enum ElementDraw {
    DONT_DRAW,
    DRAW_RECT,
    DRAW_TEXTURE,
};

struct Element {
protected:
    Vec2i composition_pos;

    void UpdateComposition();

public:
    Element* parent;
    std::vector<Element*> children;

    Vec2i relative, pos, size, edge;
    Vec2i visual_size, visual_offset;
    ElementDraw draw;
    Texture* visual_texture;
    int type;
    std::wstring name;

    bool enabled;
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

private:
    void AlignHorizontal_();
    void AlignVertical_();

public:
    Element(int type, const Vec2i& relative, const Vec2i& size, ElementDraw draw);
    Element(int type, const Vec2i& relative, const Vec2i& size, const Vec2i& visual, const Vec2i& offset);
    Element(int type,
            const Vec2i& relative,
            const Vec2i& size,
            const Vec2i& visual,
            const Vec2i& offset,
            Texture* texture);
    virtual ~Element();

    // Getting
    [[nodiscard]] Texture* GetVisualTexture() const { return visual_texture; }
    [[nodiscard]] SDL_FRect GetRect() const { return SDL_FRect(pos.x, pos.y, size.x, size.y); }
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
    Element* SetName(const char* name, bool suffix = true) {
        std::wstring new_name = Strings::FStringW(L"%s", name);
        if (suffix) new_name += Strings::FStringW(L"%s", ELEMENTTYPE_NAMES[type]);
        this->name = new_name;
        return this;
    }
    Element* SetName(const wchar_t* name, bool suffix = true) {
        std::wstring new_name = name;
        if (suffix) new_name += Strings::FStringW(L"%s", ELEMENTTYPE_NAMES[type]);
        this->name = new_name;
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
    Element* SetVisualTexture(Texture* texture) {
        this->visual_texture = texture;
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

    // Ticking
    virtual void Tick();
    virtual void HandleEvent(SDL_Event& event, EventContext& event_summary);
    virtual void Render();
    virtual void RenderDebug() const;
    virtual void PostEvent();
    virtual void PostRefresh() { };

    void PostEventChildren() const;
    void TickChildren() const;
    void HandleEventChildren(SDL_Event& event, EventContext& event_summary);
    void RenderChildren() const;
    void RenderDebugChildren() const;

};