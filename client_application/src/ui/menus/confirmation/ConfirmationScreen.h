//
// Created by Matq on 31/01/2025.
//

#pragma once

#include "ui/menus/FullscreenMenu.h"
#include "ui/components/element/TextElement.h"
#include "ui/components/element/Button.h"

class ConfirmationScreen : public FullscreenMenu
{
private:
	TextElement *question;
	Button *delete_button;
	using Callback = std::function<void()>;

public:
	ConfirmationScreen();
	~ConfirmationScreen() override;

	// Manipulating
	void Prompt(const char *prompt_question);
	void SetAgreeCallback(Callback callback);

};
