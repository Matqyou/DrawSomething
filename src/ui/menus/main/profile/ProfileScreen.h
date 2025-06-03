//
// Created by Matq on 23/05/2025.
//

#pragma once

#include "ui/menus/ScreenMenu.h"

class ProfileScreen : public ScreenMenu
{
private:

public:
	ProfileScreen();
	~ProfileScreen() override;

	// Manipulating
	void RefreshData();

};
