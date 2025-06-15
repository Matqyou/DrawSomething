//
// Created by Matq on 13/06/2025.
//

#pragma once

#include "FullscreenMenu.h"

class HeaderFullscreenMenu : public FullscreenMenu
{
private:
	virtual void ReassignChildren() = 0;

public:
	HeaderFullscreenMenu();
	~HeaderFullscreenMenu();

	// Manipulating
	void SwitchToThisMenu() override;

};
