//
// Created by Matq on 31/01/2025.
//

#include "ConfirmationScreen.h"
#include "ui/CommonUI.h"

static LinkTexture sTextureConfirmation("main_menu.confirmation");
static LinkTexture sTextureDelete("main_menu.delete");
static LinkTexture sTextureCancel("main_menu.cancel");

ConfirmationScreen::ConfirmationScreen()
	: FullscreenMenu()
{
	auto title = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "Are you sure?", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, 10))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Title");

	question = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
					 "?",
					 { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, 10))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Question");

	delete_button = (Button *)(new Button())
		->SetSize(Vec2i(221, 54)/2)
		->SetRelative(Vec2i(0, -10))
		->SetTexture(sTextureDelete.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetColor(0, 255, 0, 255)
		->SetName("Yes");
	delete_button->SetCallback([this]()  {  this->SetEnabled(false); });

	auto cancel_button = (Button *)(new Button())
		->SetSize(Vec2i(221, 54)/2)
		->SetRelative(Vec2i(0, -10))
		->SetTexture(sTextureCancel.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetColor(255, 0, 0, 255)
		->SetName("Cancel");
	cancel_button->SetCallback([this]() 	{ 	this->SetEnabled(false); });

	auto buttons_frame = (new Frame())
		->SetRelative(Vec2i(0, -15))
		->SetFlexInvolved(false, false)
		->SetAlign(Align::CENTER, Align::BOTTOM)
		->SetFlex(Flex::WIDTH, 50)
		->SetAdaptive(true, true)
		->SetName("ButtonsFrame")
		->AddChildren({ cancel_button, delete_button });

	auto confirmation_frame = (Frame *)(new Frame())
		->SetSize(Vec2i(367, 191))
		->SetTexture(sTextureConfirmation.GetTexture())
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
	this->question->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							   prompt_question,
							   { 255, 255, 255, 255 });
	this->parent->Refresh();
}

void ConfirmationScreen::SetAgreeCallback(Callback callback)
{
	delete_button->SetCallback(std::move(callback));
}