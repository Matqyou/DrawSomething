//
// Created by Matq on 31/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../components/element/TextElement.h"
#include "../../components/element/Button.h"

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
