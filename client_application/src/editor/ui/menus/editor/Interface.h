//
// Created by Matq on 02/02/2025.
//

#pragma once

#include "ui/menus/FullscreenMenu.h"
#include "editor/ui/menus/editor/components/viewport/Viewport.h"

namespace Editor {
class Interface : public FullscreenMenu {
private:
    Viewport* viewport;

public:
    Interface();
    ~Interface() override;

};
}
