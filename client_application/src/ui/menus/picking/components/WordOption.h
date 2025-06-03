//
// Created by Matq on 25/04/2025.
//

#pragma once

#include "ui/components/element/Button.h"
#include "ui/components/element/TextElement.h"

namespace Picking
{
enum Difficulty
{
	EASY,
	MEDIUM,
	HARD,
	NUM_DIFFICULTIES
};

class WordOption : public Button
{
private:
	Difficulty difficulty;
	std::string word;

	TextElement* content;

	Texture *background_texture;

public:
	explicit WordOption(Difficulty word_difficulty);
	~WordOption() override;

	// Sense
	[[nodiscard]] const std::string& GetWord() const { return word; }
	[[nodiscard]] Difficulty GetDifficulty() const { return difficulty; }

	// Manipulating
	void SetWord(const std::string& new_word);

};
}
