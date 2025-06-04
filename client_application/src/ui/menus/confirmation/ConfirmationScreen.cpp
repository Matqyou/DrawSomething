//
// Created by Matq on 31/01/2025.
//

#include "ConfirmationScreen.h"
#include "ui/CommonUI.h"
#include "ui/RenderPresets.h"

static LinkTexture sTextureConfirmation("main_menu.confirmation");
static LinkTexture sTextureConfirmationOutline("main_menu.confirmation_outline");
static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");

ConfirmationScreen::ConfirmationScreen()
	: ScreenMenu()
{
	auto title = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontBigger.GetTTFFont(), "Are you sure?", 3,
							{ 0, 0, 0, 255 },
							{ 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, 10))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Title");

	question = (TextElement *)(new TextElement())
		->SetRelative(Vec2i(0, 10))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Question");

	auto assets = Assets::Get();
	auto delete_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Delete", 2,
														{ 255, 255, 255, 255 },
														{ 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto delete_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
															{ 255, 0, 0 },
															sTextureButtonOutline.GetTexture(), delete_text)
		->FlagForAutomaticDeletion();
	auto back_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Back", 2,
														{ 255, 255, 255, 255 },
														{ 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto back_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
															{ 200, 200, 200 },
															sTextureButtonOutline.GetTexture(), back_text)
		->FlagForAutomaticDeletion();

	delete_button = (Button *)(new Button())
		->SetSize(Vec2i(delete_button_texture->GetOriginalSize() / 2))
		->SetRelative(Vec2i(0, -10))
		->SetTexture(delete_button_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetColor(0, 255, 0, 255)
		->SetName("Yes");
	delete_button->SetCallback([this]()
							   { this->SetEnabled(false); });

	auto cancel_button = (Button *)(new Button())
		->SetSize(Vec2i(back_button_texture->GetOriginalSize() / 2))
		->SetRelative(Vec2i(0, -10))
		->SetTexture(back_button_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetColor(255, 0, 0, 255)
		->SetName("Cancel");
	cancel_button->SetCallback([this]()
							   { this->SetEnabled(false); });

	auto buttons_frame = (new Frame())
		->SetRelative(Vec2i(0, -15))
		->SetFlexInvolved(false, false)
		->SetAlign(Align::CENTER, Align::BOTTOM)
		->SetFlex(Flex::WIDTH, 50)
		->SetAdaptive(true, true)
		->SetName("ButtonsFrame")
		->AddChildren({ cancel_button, delete_button });

	auto confirmation_texture = RenderPresets::ColorButton(sTextureConfirmation.GetTexture(),
														   { 255, 0, 100, 255 },
														   sTextureConfirmationOutline.GetTexture())
		->FlagForAutomaticDeletion();
	auto confirmation_frame = (Frame *)(new Frame())
		->SetSize(Vec2i(confirmation_texture->GetOriginalSize() / 2))
		->SetTexture(confirmation_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT, 10)
		->AddChildren({ title, question, buttons_frame });

	SetEnabled(false);
	SetColor(0, 0, 0, 150);
	SetFlexInvolved(false, false);
	SetFullyOccupy(true, true);
	SetName("ConfirmationScreen");
	AddChildren({ confirmation_frame });
}

ConfirmationScreen::~ConfirmationScreen()
{

}

void ConfirmationScreen::Prompt(const char *prompt_question)
{
	this->SetEnabled(true);
	this->question->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									  prompt_question, 2,
									  { 0, 0, 0, 255 },
									  { 255, 255, 255, 255 });
	this->parent->Refresh();
}

void ConfirmationScreen::SetAgreeCallback(Callback callback)
{
	delete_button->SetCallback(std::move(callback));
}