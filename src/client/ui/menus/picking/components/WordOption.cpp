//
// Created by Matq on 25/04/2025.
//

#include "WordOption.h"
#include "../../../components/element/TextElement.h"
#include "../../../components/element/Frame.h"
#include "../../../CommonUI.h"

namespace Picking
{
static LinkTexture sTextureWord("picking.word");
static LinkTexture sTextureCoin("icons.coin");
static const char *sWordDifficulties[NUM_DIFFICULTIES] = {
	"Easy",
	"Medium",
	"Hard"
};
static const SDL_Color sDifficultyColors[NUM_DIFFICULTIES] = {
	SDL_Color(75, 220, 75, 255),
	SDL_Color(220, 220, 75, 255),
	SDL_Color(220, 75, 75, 255),
};

WordOption::WordOption(Difficulty word_difficulty)
{
	this->difficulty = word_difficulty;
	this->word = "-";
	this->SetName(sWordDifficulties[(int)word_difficulty]);

	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	auto label = sWordDifficulties[(int)word_difficulty];
	auto color = sDifficultyColors[(int)word_difficulty];
	auto difficulty_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(), label, 2,
							color, { 0, 0, 0, 128 })
		->SetRelative(Vec2i(10, 0))
		->SetAlign(Align::DONT, Align::CENTER);
	content = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmall.GetTTFFont(), "-", { 0, 0, 0, 255 })
		->SetAlign(Align::CENTER, Align::CENTER);
	auto coins = (Frame *)(new Frame())
		->SetRelative(Vec2i(-10, 0))
		->SetFlex(Flex::WIDTH, 2)
		->SetAdaptive(true, true)
		->SetAlign(Align::RIGHT, Align::CENTER);

	for (int i = 0; i < (int)word_difficulty + 1; i++)
		coins->AddChildren({
							   (new Frame())
								   ->SetSize(Vec2i(26, 26))
								   ->SetTexture(sTextureCoin.GetTexture())
								   ->SetDraw(DRAW_TEXTURE)
						   });

	auto background_color = word_difficulty == EASY ? SDL_Color(222, 129, 49) :
							word_difficulty == MEDIUM ? SDL_Color(255, 126, 48) :
							SDL_Color(255, 112, 48);
	{
		auto word_texture =  sTextureWord.GetTexture();

		background_texture = word_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET);
		drawing->SetRenderTarget(background_texture);
		word_texture->SetColorMod(background_color);
		drawing->RenderTextureFullscreen(word_texture->SDLTexture());
	}
	drawing->SetRenderTarget(nullptr);

	this->SetSize(Vec2i(356, 57));
	this->SetTexture(background_texture);
	this->SetAlign(Align::CENTER, Align::DONT);
	this->SetDraw(DRAW_TEXTURE);
	this->AddChildren({ difficulty_label, content, coins });
}

WordOption::~WordOption()
{
	delete background_texture;
}

void WordOption::SetWord(const std::string& new_word)
{
	this->word = new_word;
	this->content->UpdateTextOutline(CommonUI::sFontMediumBigger.GetTTFFont(),
									 new_word.c_str(), 2,
									 { 0, 0, 0, 255 },
									 { 255, 255, 255, 255 });
}
}
