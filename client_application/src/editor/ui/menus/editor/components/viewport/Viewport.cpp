//
// Created by Matq on 17/02/2025.
//

#include "Viewport.h"
#include "ui/components/element/Button.h"
#include "ui/CommonUI.h"

namespace Editor {
Viewport::Viewport()
    : Frame() {
    this->SetOccupy(true, false);
    this->SetFullyOccupy(false, true);
    this->SetName("Viewport");

    this->zoom = 1.0;
    this->offset = Vec2f(0.0f, 0.0f);
    this->dragging_from = Vec2f(0.0f, 0.0f);
    this->dragging = false;
    this->visual_editing = false;
    this->started = std::chrono::steady_clock::now();

    viewport_item = (ViewportItem*)(new ViewportItem(this))
        ->SetDraw(DRAW_TEXTURE)
        ->SetName("Texture");
    viewport_texture_button = nullptr;

    selection_size = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Selection: NaNx, NaNy, NaNw, NaNh", { 255, 255, 255, 255 })
        ->SetName("SelectionSize");

    auto selection_info = (new Frame())
        ->SetName("SelectionInfo")
        ->SetFlexInvolved(false, true)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::WIDTH)
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->AddChildren({ selection_size });

    indicator_size = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "NaNx, NaNy", { 255, 255, 255, 255 })
        ->SetName("Dimensions");

    indicator_key = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), " no texture selected", { 255, 200, 255, 255 })
        ->SetName("TextureKey");

    auto texture_info = (new Frame())
        ->SetName("TextureInfo")
        ->SetFlexInvolved(false, true)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::WIDTH)
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->AddChildren({ indicator_size, indicator_key });

    auto bottom_right = (new Frame())
        ->SetName("BottomRightIndicator")
        ->SetFlexInvolved(false, false)
        ->SetAlign(Align::RIGHT, Align::BOTTOM)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::HEIGHT)
        ->AddChildren({ selection_info, texture_info });

    auto check_box_label = (new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "Visual Texture", { 255, 255, 255, 255 })
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("Label");

    check_box = (CheckBox*)(new CheckBox())
        ->SetSize(Vec2i(24, 24))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("EnableVisual");
    check_box->SetActivatedCallback([this]() {
        this->visual_editing = true;
    });
    check_box->SetDeactivatedCallback([this]() {
        this->visual_editing = false;
    });

    auto visual_toggle = (new Frame())
        ->SetFlex(Flex::WIDTH, 3)
        ->SetAdaptive(true, true)
        ->SetName("VisualFrame")
        ->AddChildren({ check_box_label, check_box });

    auto save_label = (new TextElement())
        ->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "Save", { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("SaveLabel");

    auto save_button = (Button*)(new Button())
        ->SetSize(Vec2i(60, 24))
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetName("SaveButton")
        ->AddChildren({ save_label });
    save_button->SetCallback([this]() {
        if (viewport_item->IsPopulated() && viewport_texture_button) {
            auto& hitbox_file = viewport_texture_button->GetHitboxFile();
            hitbox_file.SetHitbox(viewport_item->GetSelection(), visual_editing);
            hitbox_file.SaveFile(R"(.\assets\images\)", viewport_texture_button->GetTextureKey());

            viewport_texture_button->GetStateIndicator()->SetTextureState(visual_editing);
            viewport_texture_button->UpdateTextColor();
        } else { std::wcout << "No texture is selected.\n"; }
    });

    auto top_right = (new Frame())
        ->SetDraw(DRAW_RECT)
        ->SetColor(255, 255, 255, 50)
        ->SetRelative(Vec2i(-5, 5))
        ->SetFlexInvolved(false, false)
        ->SetAlign(Align::RIGHT, Align::TOP)
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::HEIGHT, 3)
        ->SetName("TopRight")
        ->AddChildren({ visual_toggle, save_button });

    this->AddChildren({ viewport_item, bottom_right, top_right });
}

Viewport::~Viewport() {

}

void Viewport::OpenByTextureButton(TextureButton* texture_button) {
    auto texture = texture_button->GetAssignedTexture()
        ->SetScaleMode(SDL_SCALEMODE_NEAREST);
    auto centered_texture = Rectangles::CenterRelative(texture->GetOriginalSize(), Vec2f(size));

    this->zoom = 1.0;
    this->offset = Vec2f(centered_texture.x, centered_texture.y);
    this->viewport_item->SetRelative(Vec2i(offset));
    this->viewport_item->SetViewportTexture(texture);
    this->viewport_texture_button = texture_button;

    bool uses_hitbox = viewport_texture_button->GetHitboxFile().UsesHitbox();
    this->check_box->SetActivated(uses_hitbox);
    this->visual_editing = uses_hitbox;
    if (texture->UsesHitbox()) {
        this->viewport_item->SetSelection(Rect4i(((VisualTexture*)texture)->GetHitbox()));
    } else {
        Vec2f texture_size = texture->GetOriginalSize();
        this->viewport_item->SetSelection(Rect4i(0, 0, (int)texture_size.x - 1, (int)texture_size.y - 1));
    }

    auto texture_dimensions = Vec2i(viewport_item->texture_instance.GetTexture()->GetOriginalSize());
    indicator_size->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                               Strings::FString("%dx, %dy ",
                                                texture_dimensions.x,
                                                texture_dimensions.y).c_str(),
                               { 255, 255, 255, 255 });
    indicator_key->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                              texture_button->GetTextureKey().c_str(),
                              { 255, 200, 255, 255 });
    this->Refresh();
}

void Viewport::UpdateSelectionSizeText() {
    auto selection = viewport_item->GetSelection();
    selection_size->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                               Strings::FString("Selection: %dx, %dy, %dw, %dh",
                                                selection.x, selection.y, selection.w, selection.h).c_str(),
                               { 255, 255, 255, 255 });
    this->parent->Refresh();
}

void Viewport::Tick(double elapsed_seconds) {
    if (dragging) {
        Vec2f current_mouse;
        SDL_GetMouseState(&current_mouse.x, &current_mouse.y);

        offset += current_mouse - dragging_from;
        dragging_from = current_mouse;

        viewport_item->SetRelative(Vec2i(offset));
        this->Refresh();
    }

    TickChildren(elapsed_seconds);
}

void Viewport::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) {
    this->HandleEventChildren(sdl_event, event_summary);

    switch (sdl_event.type) {
        case SDL_EVENT_MOUSE_WHEEL: {
            if (!event_summary.rapid_context.event_captured &&
                PointCollides((int)sdl_event.wheel.mouse_x, (int)sdl_event.wheel.mouse_y)) {
                event_summary.rapid_context.event_captured = true;

                auto new_zoom = zoom * (1.0 + (double)sdl_event.wheel.y / 10.0);
                viewport_item->SetSize(Vec2i(
                    viewport_item->texture_instance.GetTexture()->GetOriginalSize() * (float)new_zoom));

                auto relative_mouse =
                    Vec2f(sdl_event.wheel.mouse_x - (float)pos.x, sdl_event.wheel.mouse_y - (float)pos.y);
                offset.x = relative_mouse.x - (float)((relative_mouse.x - offset.x) * (new_zoom / zoom));
                offset.y = relative_mouse.y - (float)((relative_mouse.y - offset.y) * (new_zoom / zoom));
                zoom = new_zoom;

                viewport_item->SetRelative(Vec2i(offset));
                this->Refresh();
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if ((sdl_event.button.button == SDL_BUTTON_MIDDLE) &&
                !event_summary.rapid_context.event_captured &&
                PointCollides((int)sdl_event.button.x, (int)sdl_event.button.y)) {
                event_summary.rapid_context.event_captured = true;
                dragging_from = Vec2f(sdl_event.button.x, sdl_event.button.y);
                dragging = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (sdl_event.button.button == SDL_BUTTON_MIDDLE)
                dragging = false;
            break;
        }
    }
}

void Viewport::Render() {
    auto drawing = Assets::Get()->GetDrawing();

    auto slice = size.x / 25;
    auto delta =
        (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - started).count() / 50)
            % slice;
    drawing->SetColor(255, 255, 255, 255);
    for (int i = 0; i < 26; i++) {
        auto top = Vec2f(pos) + Vec2f((float)(i * slice + delta), 0.0f);
        auto bottom = Vec2f(pos) + Vec2f((float)((i * slice + delta) * 2), (float)size.y);
        drawing->DrawLine(top, bottom);
    }

    RenderChildren();
}
}
