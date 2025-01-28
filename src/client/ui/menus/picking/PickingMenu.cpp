//
// Created by Matq on 26/01/2025.
//

#include "PickingMenu.h"
#include "components/Header.h"

PickingMenu::PickingMenu()
    : FullscreenMenu() {

    auto header = new Picking::Header();

    SetColor(94, 152, 224, 255);
    SetName("PickingMenu");
    AddChildren({ header });
    RefreshMenu();
}

PickingMenu::~PickingMenu() {

}
