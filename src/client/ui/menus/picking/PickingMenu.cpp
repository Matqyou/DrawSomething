//
// Created by Matq on 26/01/2025.
//

#include "PickingMenu.h"
#include "components/Header.h"
#include "../../CommonUI.h"
#include "../auth/components/ScrollingBackground.h"
#include "components/WordOption.h"
#include "client/ui/RenderPresets.h"

static LinkTexture sTextureCard("picking.card");
static LinkTexture sTextureDoodles("doodles"); //

static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");

PickingMenu::PickingMenu()
	: FullscreenMenu()
{
	auto header = new Picking::Header();

	auto title = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontMiniProfile2.GetTTFFont(),
							"Pick the word you will be drawing!", 2,
							{ 0, 0, 0, 255 },
							{ 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT);

	easy_word = (Picking::WordOption *)(new Picking::WordOption(Picking::Difficulty::EASY));
	medium_word = (Picking::WordOption *)(new Picking::WordOption(Picking::Difficulty::MEDIUM));
	hard_word = (Picking::WordOption *)(new Picking::WordOption(Picking::Difficulty::HARD));

	auto words_frame = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT, 10)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetName("WordsFrame")
		->AddChildren({ title, easy_word, medium_word, hard_word });

	auto words_card = (new Frame())
		->SetSize(Vec2i(427, 309))
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(sTextureCard.GetTexture()->SetColorMod(239, 121, 47))
		->SetName("Card")
		->AddChildren({ words_frame });

	// Content
	auto background = (new Auth::ScrollingBackground())
		->SetFullyOccupy(true, true)
		->SetFlexInvolved(false, false)
		->SetTexture(sTextureDoodles.GetTexture()->SetAlphaMod(75))
		->SetName("Background");

	auto assets = Assets::Get();
	auto back_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Back", 2,
													  { 200, 200, 200, 255 },
													  { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto back_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														  { 255, 255, 255 },
														  sTextureButtonOutline.GetTexture(), back_text)
		->FlagForAutomaticDeletion();
	auto back_button_texture_pressed = RenderPresets::ColorButton(sTextureButton.GetTexture(),
																  { 255, 255, 255 },
																  sTextureButtonOutline.GetTexture(), back_text, true)
		->FlagForAutomaticDeletion();
	auto back_button = (Button *)(new Button())
		->SetPressedTexture(back_button_texture_pressed)
		->SetTexture(back_button_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetSize(Vec2i(back_button_texture->GetOriginalSize() / 2))
		->SetRelative(Vec2i(-10, 10))
		->SetFlexInvolved(false, false)
		->SetAlign(Align::RIGHT, Align::TOP)
		->SetName("Back");

	auto content = (new Frame())
		->SetFullyOccupy(true, false)
		->SetOccupy(false, true)
		->AddChildren({ background, words_card, back_button })
		->SetName("Content");

	this->SetColor(165, 83, 33, 255);
	this->SetFlex(Flex::HEIGHT);
	this->SetName("PickingMenu");
	this->AddChildren({ header, content });
	this->RefreshMenu();
}

PickingMenu::~PickingMenu()
{

}

void PickingMenu::SetWords(const std::string& new_easy_word,
						   const std::string& new_medium_word,
						   const std::string& new_hard_word)
{
	easy_word->SetWord(new_easy_word);
	medium_word->SetWord(new_medium_word);
	hard_word->SetWord(new_hard_word);
}