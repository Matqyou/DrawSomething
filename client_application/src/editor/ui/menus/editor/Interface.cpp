//
// Created by Matq on 02/02/2025.
//

#include "Interface.h"
#include "ui/components/element/TextElement.h"
#include "ui/components/element/Scrollable.h"
#include "ui/components/element/Button.h"
#include "editor/ui/menus/editor/components/texture_button/TextureButton.h"
#include "ui/CommonUI.h"

namespace Editor {
Interface::Interface()
    : FullscreenMenu() {
    this->SetColor(0, 0, 0, 255);
    this->SetFlex(Flex::WIDTH);
    this->SetName("EditorInterface");

    auto assets = Assets::Get();

    this->viewport = new Viewport();

    auto textures_list = (new Scrollable())
        ->SetSize(Vec2i(400, 0))
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(false, true)
        ->SetName("TexturesList");

    std::vector<Element*> new_children;
    size_t index = 0;
    for (const auto& texture_record : assets->GetAllTextures()) {
        auto texture_key_str = texture_record.first;
        auto record_state = (TextureStateIndicator*)(new TextureStateIndicator())
            ->SetRelative(Vec2i(-3, 0))
            ->SetSize(Vec2i(20, 20))
            ->SetAlign(Align::RIGHT, Align::CENTER);
        auto texture_key = (TextureButton*)(new TextureButton(texture_key_str.c_str(), record_state))
            ->SetName(texture_key_str.c_str());

        auto tone = index % 2 == 0 ? 60 : 70;
        auto focus_tone = index % 2 == 0 ? 80 : 90;
        auto record_line = (Button*)(new Button())
            ->SetDraw(DRAW_RECT)
            ->SetColor(tone, tone, tone, 255)
            ->SetFocusColor(focus_tone, tone, tone, 255)
            ->SetAdaptive(false, true)
            ->SetFullyOccupy(true, false)
            ->SetName("RecordLine")
            ->AddChildren({ texture_key, record_state });
        record_line->SetCallback([this, textures_list, record_line, texture_key]() {
            textures_list->SetFocus(record_line);
            this->viewport->OpenByTextureButton(texture_key);
            this->Refresh();
        });
        new_children.push_back(record_line);
        index++;
    }
    textures_list->AddChildren(new_children);

    this->AddChildren({ textures_list, viewport });
    this->RefreshMenu();
}

Interface::~Interface() {

}
}
