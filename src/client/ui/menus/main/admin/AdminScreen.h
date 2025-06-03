//
// Created by Matq on 28/04/2025.
//

#pragma once

#include "client/ui/menus/ScreenMenu.h"

class AdminScreen : public ScreenMenu
{
private:
	Frame* users_frame;

public:
	AdminScreen();
	~AdminScreen() override;

	// Manipulating
	void RefreshData();

};
