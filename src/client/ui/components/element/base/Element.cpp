//
// Created by Matq on 16/12/2024.
//

#include "Element.h"
#include "../../../../core/Application.h"

Element::Element(int type, const Vec2i& relative, const Vec2i& size, ElementDraw draw) {
    this->parent = nullptr;
    this->relative = relative;
    this->pos = relative;
    this->size = size;
    this->draw = draw;
    this->type = type;
    this->name = Strings::FStringW(L"%s", ELEMENTTYPE_NAMES[type]);

    // Calculated
    this->edge = pos + size;

    // Default
    composition_pos = Vec2i(0, 0);
    visual_size = size;
    visual_offset = Vec2i(0, 0);
    visual_texture = nullptr;
    flex_involved_horizontal = true;
    flex_involved_vertical = true;
    occupy_width = false;
    occupy_height = false;
    occupy_fully_width = false;
    occupy_fully_height = false;
    adaptive_width = false;
    adaptive_height = false;
    align_horizontal = DONT_ALIGN;
    align_vertical = DONT_ALIGN;
    flex = DONT_FLEX;
    flex_gap = 0;
    has_focus = false;
    color = { 150, 150, 150, 255 };
    focus_color = { 255, 255, 255, 255 };
}

Element::Element(int type, const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset)
    : Element(type, pos, size, DRAW_RECT) {
    this->visual_size = visual;
    this->visual_offset = offset;
}

Element::Element(int type,
                 const Vec2i& pos,
                 const Vec2i& size,
                 const Vec2i& visual,
                 const Vec2i& offset,
                 Texture* texture)
    : Element(type, pos, size, visual, offset) {
    this->visual_texture = texture;
    this->draw = DRAW_TEXTURE;
}

Element::~Element() {
    for (auto child : children)
        delete child;
}

bool Element::PointCollides(int x, int y) const {
    return !(x < this->pos.x || x > this->edge.x ||
        y < this->pos.y || y > this->edge.y);
}

Element* Element::SetChildren(const std::vector<Element*>& children) {
    for (auto oldChild : this->children)
        oldChild->SetParent(nullptr);

    this->children.clear();
    for (auto newChild : children) {
        newChild->SetParent(this);
        this->children.push_back(newChild);
    }

    return this;
}

Element* Element::AddChildren(const std::vector<Element*>& children) {
    for (auto child : children) {
        child->SetParent(this);
        this->children.push_back(child);
    }

    return this;
}

void Element::UnfocusChildren() {
    for (auto child : children) {
        child->has_focus = false;
        child->UnfocusChildren();
    }
}

void Element::SetFocus(Element* focus_element) {
//    if (parent) {
//        parent->SetFocus(focus_element);
//        return;
//    }

//    UnfocusChildren();
    for (auto child : children)
        child->has_focus = false;

    if (focus_element != nullptr)
        focus_element->has_focus = true;
}

void Element::UpdateElement(const Vec2i& new_pos, const Vec2i& new_size, const Vec2i& new_visual) {
    // Only use case I know is for the menu element

    pos = new_pos;
    size = new_size;
    visual_size = new_visual;

    Refresh();
}

void Element::Refresh(int child_generation) {
    if (children.empty())
        return;

    { // All parents - adaptive resizing
        int adaptive_w = 0;
        int adaptive_h = 0;
        int num_children = 0;
        for (auto child : children) {
            if (child->occupy_fully_width) {
                child->size.x = size.x;
                child->visual_size.x = size.x;
            }
            if (child->occupy_fully_height) {
                child->size.y = size.y;
                child->visual_size.y = size.y;
            }
            child->Refresh(child_generation + 1);

            if (flex == FLEX_WIDTH) {
                if (!child->occupy_width && child->flex_involved_horizontal) {
                    num_children++;
                    adaptive_w += child->size.x;
                    if (child->size.y > adaptive_h)
                        adaptive_h = child->size.y;
                }
            } else if (flex == FLEX_HEIGHT && child->flex_involved_vertical) {
                num_children++;
                if (!child->occupy_height) {
                    adaptive_h += child->size.y;
                    if (child->size.x > adaptive_w)
                        adaptive_w = child->size.x;
                }
            } else {
                if (child->size.x > adaptive_w && child->flex_involved_horizontal) { adaptive_w = child->size.x; }
                if (child->size.y > adaptive_h && child->flex_involved_vertical) { adaptive_h = child->size.y; }
            }
        }

        if (adaptive_width) {
            adaptive_w += flex == FLEX_WIDTH ? (num_children - 1) * flex_gap : 0;
            size.x = adaptive_w;
            visual_size.x = adaptive_w;
        }
        if (adaptive_height) {
            adaptive_h += flex == FLEX_HEIGHT ? (num_children - 1) * flex_gap : 0;
            size.y = adaptive_h;
            visual_size.y = adaptive_h;
        }
    }
    { // Parentless parents - flex and align
        if (parent == nullptr) {
            // Flex children
            int flex_negative_space = 0;
            int flex_num_children = 0;
            int num_children = 0;
            for (auto child : children) {
                if (flex == FLEX_WIDTH && child->flex_involved_horizontal) {
                    // The number of dynamic width children
                    if (child->occupy_width) flex_num_children++;
                        // Flex volume in static children's width, count adaptive width, take the largest for height
                    else flex_negative_space += child->size.x;
                    num_children++;
                } else if (flex == FLEX_HEIGHT && child->flex_involved_vertical) {
                    if (child->occupy_height) flex_num_children++;
                    else flex_negative_space += child->size.y;
                    num_children++;
                }
            }

            int flex_space = (flex == FLEX_WIDTH ? size.x : size.y) - flex_negative_space;
            int flex_slice = flex_num_children > 0 ? (flex_space - (num_children - 1) * flex_gap) / flex_num_children : 0;

            int current_flex = 0;
            for (auto child : children) {
                // Incase we don't change some values, we can copy the previous ones
                child->composition_pos = Vec2i(0, 0);

                if (flex != DONT_FLEX) {
                    if (flex == FLEX_WIDTH && child->flex_involved_horizontal) {
                        if (child->occupy_width) {
                            child->composition_pos.x = current_flex;
                            child->size.x = flex_slice;
                            child->visual_size.x = flex_slice;
                            current_flex += flex_slice;
                        } else {
                            child->composition_pos.x = current_flex;
                            current_flex += child->size.x;
                        }
                        current_flex += flex_gap;
                    } else if (flex == FLEX_HEIGHT && child->flex_involved_vertical) {
                        if (child->occupy_height) {
                            child->composition_pos.y = current_flex;
                            child->size.y = flex_slice;
                            child->visual_size.y = flex_slice;
                            current_flex += flex_slice;
                        } else {
                            child->composition_pos.y = current_flex;
                            current_flex += child->size.y;
                        }
                        current_flex += flex_gap;
                    }
                }

                // Align children
                if (child->align_horizontal != DONT_ALIGN) {
                    switch (child->align_horizontal) {
                        case ALIGN_BEHIND_LEFT: {
                            child->composition_pos.x = -child->size.x;
                            break;
                        }
                        case ALIGN_LEFT: {
                            child->composition_pos.x = 0;
                            break;
                        }
                        case ALIGN_RIGHT: {
                            child->composition_pos.x = size.x - child->size.x;
                            break;
                        }
                        case ALIGN_BEHIND_RIGHT: {
                            child->composition_pos.x = size.x + child->size.x;
                            break;
                        }
                        case ALIGN_CENTER: {
                            child->composition_pos.x = (size.x - child->size.x) / 2;
                            break;
                        }
                        case DONT_ALIGN: break;
                    }
                }
                if (child->align_vertical != DONT_ALIGN) {
                    switch (child->align_vertical) {
                        case ALIGN_ABOVE_TOP: {
                            child->composition_pos.y = -child->size.y;
                            break;
                        }
                        case ALIGN_TOP: {
                            child->composition_pos.y = 0;
                            break;
                        }
                        case ALIGN_BOTTOM: {
                            child->composition_pos.y = size.y - child->size.y;
                            break;
                        }
                        case ALIGN_UNDER_BOTTOM: {
                            child->composition_pos.y = size.y + child->size.y;
                            break;
                        }
                        case ALIGN_CENTER: {
                            child->composition_pos.y = (size.y - child->size.y) / 2;
                            break;
                        }
                        case DONT_ALIGN: break;
                    }
                }
            }
        }
    }

    // Grandparents - flex and align
    for (auto parent : children) {
        if (parent->children.empty())
            continue;

        // Flex grandchildren
        int flex_negative_space = 0;
        int flex_num_children = 0;
        int num_children = 0;
        for (auto child : parent->children) {
            if (parent->flex == FLEX_WIDTH && child->flex_involved_horizontal) {
                // The number of dynamic width children
                if (child->occupy_width) flex_num_children++;
                    // Flex volume in static children's width, count adaptive width, take the largest for height
                else flex_negative_space += child->size.x;
                num_children++;
            } else if (flex == FLEX_HEIGHT && child->flex_involved_vertical) {
                if (child->occupy_height) flex_num_children++;
                else flex_negative_space += child->size.y;
                num_children++;
            }
        }

        int flex_space = (parent->flex == FLEX_WIDTH ? parent->size.x : parent->size.y) - flex_negative_space;
        int flex_slice = flex_num_children > 0 ? (flex_space - (num_children - 1) * parent->flex_gap) / flex_num_children : 0;

        int current_flex = 0;
        for (auto child : parent->children) {
            child->composition_pos = Vec2i(0, 0);
            if (parent->flex != DONT_FLEX) {
                if (parent->flex == FLEX_WIDTH && child->flex_involved_horizontal) {
                    if (child->occupy_width) {
                        child->composition_pos.x = current_flex;
                        child->size.x = flex_slice;
                        child->visual_size.x = flex_slice;
                        current_flex += flex_slice;
                    } else {
                        child->composition_pos.x = current_flex;
                        current_flex += child->size.x;
                    }
                    current_flex += parent->flex_gap;
                } else if (parent->flex == FLEX_HEIGHT && child->flex_involved_vertical) {
                    if (child->occupy_height) {
                        child->composition_pos.y = current_flex;
                        child->size.y = flex_slice;
                        child->visual_size.y = flex_slice;
                        current_flex += flex_slice;
                    } else {
                        child->composition_pos.y = current_flex;
                        current_flex += child->size.y;
                    }
                    current_flex += parent->flex_gap;
                }
            }

            // Align grandchildren
            if (!parent->occupy_width && child->align_horizontal != DONT_ALIGN) {
                switch (child->align_horizontal) {
                    case ALIGN_BEHIND_LEFT: {
                        child->composition_pos.x = -child->size.x;
                        break;
                    }
                    case ALIGN_LEFT: {
                        child->composition_pos.x = 0;
                        break;
                    }
                    case ALIGN_RIGHT: {
                        child->composition_pos.x = parent->size.x - child->size.x;
                        break;
                    }
                    case ALIGN_BEHIND_RIGHT: {
                        child->composition_pos.x = parent->size.x + child->size.x;
                        break;
                    }
                    case ALIGN_CENTER: {
                        child->composition_pos.x = (parent->size.x - child->size.x) / 2;
                        break;
                    }
                    case DONT_ALIGN: break;
                }
            }
            if (!parent->occupy_height && child->align_vertical != DONT_ALIGN) {
                switch (child->align_vertical) {
                    case ALIGN_ABOVE_TOP: {
                        child->composition_pos.y = -child->size.y;
                        break;
                    }
                    case ALIGN_TOP: {
                        child->composition_pos.y = 0;
                        break;
                    }
                    case ALIGN_BOTTOM: {
                        child->composition_pos.y = parent->size.y - child->size.y;
                        break;
                    }
                    case ALIGN_UNDER_BOTTOM: {
                        child->composition_pos.y = parent->size.y + child->size.y;
                        break;
                    }
                    case ALIGN_CENTER: {
                        child->composition_pos.y = (parent->size.y - child->size.y) / 2;
                        break;
                    }
                    case DONT_ALIGN: break;
                }
            }

            for (auto grandchild : child->children) {
                if (child->occupy_width && grandchild->align_horizontal != DONT_ALIGN) {
                    switch (grandchild->align_horizontal) {
                        case ALIGN_BEHIND_LEFT: {
                            grandchild->composition_pos.x = -grandchild->size.x;
                            break;
                        }
                        case ALIGN_LEFT: {
                            grandchild->composition_pos.x = 0;
                            break;
                        }
                        case ALIGN_RIGHT: {
                            grandchild->composition_pos.x = child->size.x - grandchild->size.x;
                            break;
                        }
                        case ALIGN_BEHIND_RIGHT: {
                            grandchild->composition_pos.x = child->size.x + grandchild->size.x;
                            break;
                        }
                        case ALIGN_CENTER: {
                            grandchild->composition_pos.x = (child->size.x - grandchild->size.x) / 2;
                            break;
                        }
                        case DONT_ALIGN: break;
                    }
                }
                if (child->occupy_height && grandchild->align_vertical != DONT_ALIGN) {
                    switch (grandchild->align_vertical) {
                        case ALIGN_ABOVE_TOP: {
                            grandchild->composition_pos.y = -grandchild->size.y;
                            break;
                        }
                        case ALIGN_TOP: {
                            grandchild->composition_pos.y = 0;
                            break;
                        }
                        case ALIGN_BOTTOM: {
                            grandchild->composition_pos.y = child->size.y - grandchild->size.y;
                            break;
                        }
                        case ALIGN_UNDER_BOTTOM: {
                            grandchild->composition_pos.y = child->size.y + grandchild->size.y;
                            break;
                        }
                        case ALIGN_CENTER: {
                            grandchild->composition_pos.y = (child->size.y - grandchild->size.y) / 2;
                            break;
                        }
                        case DONT_ALIGN: break;
                    }
                }
            }
        }
    }

    // Last Step - Update coordinates, since all we did was relative up to this point
    if (child_generation == 0)
        UpdateComposition();
}

void Element::UpdateComposition() {
    if (parent != nullptr) {
        pos = parent->pos + composition_pos + relative;
        edge = pos + size;
    }

    for (auto child : children) {
        child->UpdateComposition();
        child->PostRefresh();
    }
}

void Element::DebugPrint(std::vector<bool> level, bool last_child) {
    // Initialize the indentation string
    std::wstring indent;

    // Build the indentation for current level
    for (size_t i = 0; i < level.size(); ++i) {
        if (i < level.size() - 1) {
            indent += level[i] && !level[i + 1] ? L" │  " : L"    "; // Draw vertical line or space
        } else {
            indent += last_child ? L" └─>" : L" ├─>"; // Draw branch or last branch
        }
    }

    // Add the current level's status to the stack
    level.push_back(last_child);

    // Print the current element
    std::wstring output_element = Strings::FStringColorsW(L"%ls\"%ls&r\"", indent.c_str(), name.c_str());

    if (relative.x != 0 || relative.y != 0) {
        const wchar_t* x_sign = relative.x > 0 ? L"+" : L"";
        const wchar_t* y_sign = relative.y > 0 ? L"+" : L"";
        output_element += Strings::FStringColorsW(L" (&d%ls%ix&r,&d%ls%iy&r)", x_sign, relative.x, y_sign, relative.y);
    }
    const wchar_t* x_color = parent != nullptr && parent->flex == FLEX_WIDTH ? L"&5" : L"&6";
    const wchar_t* y_color = parent != nullptr && parent->flex == FLEX_HEIGHT ? L"&5" : L"&6";
    const wchar_t* x_suffix = align_horizontal == DONT_ALIGN ? L"x" :
                              align_horizontal == ALIGN_LEFT ? L"←" :
                              align_horizontal == ALIGN_RIGHT ? L"→" :
                              L"⇆";
    const wchar_t* y_suffix = align_vertical == DONT_ALIGN ? L"y" :
                              align_vertical == ALIGN_TOP ? L"↑" :
                              align_vertical == ALIGN_BOTTOM ? L"↓" :
                              L"⇅";
    output_element +=
        Strings::FStringColorsW(L" (%ls%i%ls&r,%ls%i%ls&r)", x_color, pos.x, x_suffix, y_color, pos.y, y_suffix);
    if (flex == FLEX_WIDTH) output_element += L"←→";
    else if (flex == FLEX_HEIGHT) output_element += L" ↕ ";
    else output_element += L" ";
    std::wstring width_color =
        flex != DONT_FLEX && adaptive_width ? L"&d" : occupy_width ? L"&5" : occupy_fully_width ? L"&c" : L"&6";
    std::wstring height_color =
        flex != DONT_FLEX && adaptive_height ? L"&d" : occupy_height ? L"&5" : occupy_fully_height ? L"&c" : L"&6";
    output_element +=
        Strings::FStringColorsW(L"(%ls%iw&r,%ls%ih&r)", width_color.c_str(), size.x, height_color.c_str(), size.y);

    const wchar_t* output_draw = draw == DRAW_RECT ? L"&a□" :
                                 draw == DRAW_TEXTURE ? L"&a■" :
                                 L"&c□";
    output_element += Strings::FStringColorsW(L" %ls", output_draw);

    std::wcout << output_element << std::endl;

    // Recurse into children
    int index = 0;
    int num_children = (int)children.size();
    for (auto child : children) {
        child->DebugPrint(level, ++index == num_children);
    }
}

void Element::Tick() {
    TickChildren();
}

void Element::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    HandleEventChildren(event, event_summary);
}

void Element::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Application::Get()->GetDrawing();
        auto& fill_color = has_focus ? focus_color : color;

        if (draw == ElementDraw::DRAW_RECT) {
            drawing->SetColor(fill_color);
            drawing->FillRect(GetRect());
        } else if (draw == ElementDraw::DRAW_TEXTURE) {
            drawing->RenderTexture(visual_texture->SDLTexture(), nullptr, GetVisualRect());
        }
    }

    RenderChildren();
}

void Element::RenderDebug() const {
    auto drawing = Application::Get()->GetDrawing();

    drawing->SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetColor(0, 255, 0, 255);
    drawing->DrawRect(GetVisualRect());

    SDL_Color debug_color;
    if (occupy_width || occupy_height) debug_color = { 128, 0, 255 };
    else debug_color = { 255, 0, 0 };
    drawing->SetColor(debug_color.r, debug_color.g, debug_color.b, 10);
    drawing->FillRect(GetRect());

    drawing->SetColor(debug_color.r, debug_color.g, debug_color.b, 255);
    drawing->DrawRect(GetRect());

    RenderDebugChildren();
}

void Element::TickChildren() const {
    for (auto child : children)
        child->Tick();
}

void Element::HandleEventChildren(SDL_Event& event, EventContext& event_summary) {
    for (auto child : children)
        child->HandleEvent(event, event_summary);
}

void Element::RenderChildren() const {
    for (auto child : children)
        child->Render();
}

void Element::RenderDebugChildren() const {
    for (auto child : children)
        child->RenderDebug();
}
