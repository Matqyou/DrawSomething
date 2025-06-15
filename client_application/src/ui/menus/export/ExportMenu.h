//
// Created by Matq on 15/06/2025.
//

#pragma once

#include "ui/menus/FullscreenMenu.h"
class ExportMenu : public FullscreenMenu
{
private:
	Frame* drawing_preview;

public:
	ExportMenu();
	~ExportMenu();

	// Manipulating
	void SwitchToThisMenu() override;

};
