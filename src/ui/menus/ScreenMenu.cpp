//
// Created by Matq on 24/05/2025.
//

#include "ScreenMenu.h"

ScreenMenu::ScreenMenu()
	: Frame()
{
	this->SetEnabled(false);
	this->SetColor(0, 0, 0, 200);
	this->SetDraw(DRAW_RECT);
	this->SetFullyOccupy(true, true);
	this->SetName("UnnamedScreen");
}

ScreenMenu::~ScreenMenu()
{

}
