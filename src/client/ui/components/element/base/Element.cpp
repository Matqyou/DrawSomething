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

Element* Element::SetChildren(std::initializer_list<Element*> children) {
    if (!this->children.empty()) throw std::runtime_error("Attempted to set children with existing set of children");

    this->children = children;
    for (auto child : this->children)
        child->SetParent(this);

    return this;
}

Element* Element::SetChildren(const std::vector<Element*>& children) {
    if (!this->children.empty()) throw std::runtime_error("Attempted to set children with existing set of children");

    this->children = children;
    for (auto child : this->children)
        child->SetParent(this);

    return this;
}

void Element::UnfocusChildren() {
    for (auto child : children) {
        child->has_focus = false;
        child->UnfocusChildren();
    }
}

void Element::SetFocus(Element* focus_element) {
    if (parent) {
        parent->SetFocus(focus_element);
        return;
    }

    UnfocusChildren();
    if (focus_element != nullptr)
        focus_element->has_focus = true;
}

void Element::SetSize(const Vec2i& new_pos, const Vec2i& new_size, const Vec2i& new_visual) {
    this->pos = new_pos;
    this->size = new_size;
    this->visual_size = new_visual;
    this->edge = pos + size;

    // Updating all children
    // Step 1. calculate the *dynamic area* and count *dynamic objects*
    int flex_objects = 0;
    int flex_volume = 0;
    for (auto child : children) {
        if (flex == FLEX_WIDTH && child->flex_involved_horizontal) {
            if (child->occupy_width) flex_objects++;
            else flex_volume += child->size.x;
        } else if (flex == FLEX_HEIGHT && child->flex_involved_vertical) {
            if (child->occupy_height) flex_objects++;
            else flex_volume += child->size.y;
        }
    }
    flex_volume += (int)(children.size() - 1) * flex_gap;

    // Step 2. determine the positions and sizes of each element
    int flex_full = (flex == FLEX_WIDTH ? size.x : size.y) - flex_volume;
    int flex_slice = flex_objects > 0 ? flex_full / flex_objects : 0; // division by zero

    int flex_current = 0;
    for (auto child : children) {
        auto child_pos = pos;
        auto child_size = child->size;
        auto child_visual = child->visual_size; //

        // Children that have occupy_width, will be resized to fit the parent
        if (flex == FLEX_WIDTH) {
            if (!child->occupy_width || !child->flex_involved_horizontal) {
                child_pos.x = pos.x + flex_current;
            } else {
                child_pos.x = pos.x + flex_current;
                child_size.x = flex_slice;
                child_visual.x = flex_slice;
            }

            if (child->flex_involved_horizontal) { flex_current += child_size.x + flex_gap; }
        } // Children that have occupy_height, will be resized to fit the parent
        else if (flex == FLEX_HEIGHT) {
            if (!child->occupy_height || !child->flex_involved_vertical) {
                child_pos.y = pos.y + flex_current;
            } else {
                child_pos.y = pos.y + flex_current;
                child_size.y = flex_slice;
                child_visual.y = flex_slice;
            }

            if (child->flex_involved_vertical) { flex_current += child_size.y + flex_gap; }
        }

        // If there is a flex direction, we can use adaptive resizing
        if (child->flex != DONT_FLEX) {
            // Child's width will be resized to fit all grandchildren and descendants
            if (child->adaptive_width) {
                auto recursive_width = [](auto self, Element* child_) -> int {
                    int adaptive = 0;
                    int largest = 0;
                    for (auto grandchild : child_->children) {
                        // Ignore flex objects, because their size is dynamic
                        if (grandchild->occupy_width)
                            continue;

                        int child_width = 0;
                        if (grandchild->adaptive_width) {
                            child_width = self(self, grandchild);
                        } else if (grandchild->flex_involved_horizontal) { child_width = grandchild->size.x; }

                        adaptive += child_width;
                        if (child_width > largest)
                            largest = child_width;
                    }
                    adaptive += (int)(child_->children.size() - 1) * child_->flex_gap;
                    return child_->flex == FLEX_WIDTH ? adaptive : largest;
                };
                auto new_width = recursive_width(recursive_width, child);
                child_size.x = new_width;
                child_visual.x = new_width;
            }
            // Child's height will be resized to fit all grandchildren and descendants
            if (child->adaptive_height) {
                auto recursive_height = [](auto self, Element* child_) -> int {
                    int adaptive = 0;
                    int largest = 0;
                    for (auto grandchild : child_->children) {
                        // Ignore flex objects, because their size is dynamic
                        if (grandchild->occupy_height)
                            continue;

                        int child_height = 0;
                        if (grandchild->adaptive_height) {
                            child_height = self(self, grandchild);
                        } else if (grandchild->flex_involved_vertical) { child_height = grandchild->size.y; }

                        adaptive += child_height;
                        if (child_height > largest)
                            largest = child_height;
                    }
                    adaptive += (int)(child_->children.size() - 1) * child_->flex_gap;
                    return child_->flex == FLEX_HEIGHT ? adaptive : largest;
                };
                auto new_height = recursive_height(recursive_height, child);
                child_size.y = new_height;
                child_visual.y = new_height;
            }
        }

        // Resize child if they want to inherit the entire width
        if (child->occupy_fully_width) {
            child_pos.x = pos.x;
            child_size.x = size.x;
            child_visual.x = size.x;
        }
        // Resize child if they want to inherit the entire height
        if (child->occupy_fully_height) {
            child_pos.y = pos.y;
            child_size.y = size.y;
            child_visual.y = size.y;
        }

        if (child->align_horizontal != DONT_ALIGN) {
            switch (child->align_horizontal) {
                case ALIGN_BEHIND_LEFT: {
                    child_pos.x = pos.x - child->size.x;
                    break;
                }
                case ALIGN_LEFT: {
                    child_pos.x = pos.x;
                    break;
                }
                case ALIGN_RIGHT: {
                    child_pos.x = pos.x + size.x - child_size.x;
                    break;
                }
                case ALIGN_BEHIND_RIGHT: {
                    child_pos.x = pos.x + size.x + child->size.x;
                    break;
                }
                case ALIGN_CENTER: {
                    child_pos.x = pos.x + (size.x - child_size.x) / 2;
                    break;
                }
                case DONT_ALIGN: break;
            }
        }
        if (child->align_vertical != DONT_ALIGN) {
            switch (child->align_vertical) {
                case ALIGN_ABOVE_TOP: {
                    child_pos.y = pos.y - child->size.y;
                    break;
                }
                case ALIGN_TOP: {
                    child_pos.y = pos.y;
                    break;
                }
                case ALIGN_BOTTOM: {
                    child_pos.y = pos.y + size.y - child_size.y;
                    break;
                }
                case ALIGN_UNDER_BOTTOM: {
                    child_pos.y = pos.y + size.y + child->size.y;
                    break;
                }
                case ALIGN_CENTER: {
                    child_pos.y = pos.y + (size.y - child_size.y) / 2;
                    break;
                }
                case DONT_ALIGN: break;
            }
        }

        // Update children
        child->SetSize(
            child_pos + child->relative,
            child_size,
            child_visual
        );
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
    std::wstring width_color = flex != DONT_FLEX && adaptive_width ? L"&5" : L"&6";
    std::wstring height_color = flex != DONT_FLEX && adaptive_height ? L"&5" : L"&6";
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

void Element::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    HandleEventChildren(event, event_summary);
}

void Element::Render() const {
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