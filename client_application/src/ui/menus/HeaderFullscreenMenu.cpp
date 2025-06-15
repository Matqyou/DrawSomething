//
// Created by Matq on 13/06/2025.
//

#include "HeaderFullscreenMenu.h"
#include "game/GameData.h"

HeaderFullscreenMenu::HeaderFullscreenMenu()
	: FullscreenMenu()
{

}

HeaderFullscreenMenu::~HeaderFullscreenMenu()
{

}

void HeaderFullscreenMenu::SwitchToThisMenu()
{
	ReassignChildren();

	Centralized.current_menu = (FullscreenMenu*)this;
	RefreshMenu();
}