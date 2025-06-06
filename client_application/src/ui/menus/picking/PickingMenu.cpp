//
// Created by Matq on 26/01/2025.
//

#include "PickingMenu.h"
#include "ui/menus/picking/components/Header.h"
#include "ui/CommonUI.h"
#include "ui/menus/auth/components/ScrollingBackground.h"
#include "ui/menus/picking/components/WordOption.h"
#include "ui/RenderPresets.h"
#include "game/GameData.h"

static LinkTexture sTextureCard("picking.card");
static LinkTexture sTextureCardOutline("picking.card_outline");
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

	auto card_texture = RenderPresets::ColorButton(sTextureCard.GetTexture(),
												   { 239, 121, 47, 255 },
												   sTextureCardOutline.GetTexture())
		->FlagForAutomaticDeletion();
	auto words_card = (new Frame())
		->SetSize(Vec2i(427, 309))
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(card_texture)
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
													  { 255, 255, 255, 255 },
													  { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto back_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														  { 200, 200, 200 },
														  sTextureButtonOutline.GetTexture(), back_text)
		->FlagForAutomaticDeletion();
	auto back_button = (Button *)(new Button(back_button_texture,
											 back_button_texture))
		->SetDraw(DRAW_TEXTURE)
		->SetSize(Vec2i(back_button_texture->GetOriginalSize() / 2))
		->SetRelative(Vec2i(10, 10))
		->SetFlexInvolved(false, false)
		->SetAlign(Align::LEFT, Align::TOP)
		->SetName("Back");
	back_button->SetCallback([]()
							 {
								 Centralized.current_menu = (FullscreenMenu *)Centralized.main_menu;
								 Centralized.current_menu->RefreshMenu();
							 });

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