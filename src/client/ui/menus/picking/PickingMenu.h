//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "components/WordOption.h"

class PickingMenu : public FullscreenMenu
{
private:
	using PickCallback = std::function<void(Picking::WordOption* word_option)>;
//	PickCallback callback;

	Picking::WordOption *easy_word;
	Picking::WordOption *medium_word;
	Picking::WordOption *hard_word;

public:
	PickingMenu();
	~PickingMenu() override;

	// Options
	PickingMenu *SetPickCallback(const PickCallback& new_callback)
	{
		this->easy_word->SetCallback([this, new_callback]() { new_callback(easy_word); });
		this->medium_word->SetCallback([this, new_callback]() { new_callback(medium_word); });
		this->hard_word->SetCallback([this, new_callback]() { new_callback(hard_word); });
		return this;
	}

	// Manipulating
	void SetWords(const std::string& new_easy_word,
				  const std::string& new_medium_word,
				  const std::string& new_hard_word);

};
