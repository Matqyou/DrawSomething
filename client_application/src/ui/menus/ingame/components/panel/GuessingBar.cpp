//
// Created by Matq on 21/01/2025.
//

#include "GuessingBar.h"
#include "ui/CommonUI.h"
#include "Letter.h"
#include "network/NetworkClient.h"
#include "game/GameData.h"
#include "ui/RenderPresets.h"
#include <ranges>

namespace Ingame
{
Texture *sGenerateCompleteText(AssetsClass *assets)
{
	auto drawing = assets->GetDrawing();

	const Vec2i text_size = Vec2i(275, 50);
	auto text_render = assets->RenderTextBlended(CommonUI::sFontBiggest.GetTTFFont(),
												 "correct!",
												 { 255, 255, 255, 255 });
	auto text_rect = Rectangles::CenterRelative(Vec2i(text_render->GetOriginalSize()), text_size);
	auto complete_text = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
											   SDL_TEXTUREACCESS_TARGET,
											   text_size.x, text_size.y)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(complete_text);
	drawing->SetColor(50, 200, 50, 170);
	drawing->Clear();
	drawing->RenderTexture(text_render->SDLTexture(), nullptr, text_rect);
	return complete_text;
}
Texture *sGenerateGuessingBar(AssetsClass *assets, SDL_Color sdl_color)
{
	auto drawing = assets->GetDrawing();

	auto original_guessing_bar = assets->GetTexture("game.ingame_panel.guessing_bar");
	Vec2f guessing_bar_size = original_guessing_bar->GetOriginalSize();
	auto guessing_bar = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
											  SDL_TEXTUREACCESS_TARGET,
											  (int)guessing_bar_size.x, (int)guessing_bar_size.y)
		->SetScaleMode(SDL_SCALEMODE_NEAREST)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(guessing_bar);
	original_guessing_bar->SetColorMod(sdl_color);
	original_guessing_bar->SetAlphaMod(170);
	drawing->RenderTextureFullscreen(original_guessing_bar->SDLTexture(), nullptr);
	return guessing_bar;
}
Texture *sGenerateGuessingBarBlue(AssetsClass *assets) { return sGenerateGuessingBar(assets, { 100, 190, 255 }); }
Texture *sGenerateGuessingBarGreen(AssetsClass *assets) { return sGenerateGuessingBar(assets, { 75, 200, 75 }); }

PregenerateTexture sTextureGuessingBarBlue("game.ingame_panel.guesing_bar.blue", sGenerateGuessingBarBlue);
PregenerateTexture sTextureGuessingBarGreen("game.ingame_panel.guesing_bar.green", sGenerateGuessingBarGreen);
PregenerateTexture sTextureCompleteText("game.ingame_panel.complete_text", sGenerateCompleteText);

static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");

GuessingBar::GuessingBar()
	: Frame()
{
	this->name = L"Guessing";
	this->SetSize(Vec2i(0, 90));
	this->SetTexture(sTextureGuessingBarBlue.GetTexture());
	this->SetDraw(DRAW_TEXTURE);
	this->SetFullyOccupy(true, false);
	this->SetFlexInvolved(true, false);
	this->SetAlign(Align::DONT, Align::ABOVE_TOP);

	this->guess_word = "not set";

	this->complete_text = (Frame *)(new Frame())
		->SetSize(Vec2i(275, 50))
		->SetTexture(sTextureCompleteText.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFlexInvolved(false, false)
		->SetAlign(Align::CENTER, Align::ABOVE_TOP)
		->SetEnabled(false)
		->SetName("CompleteText");

	this->guessing_frame = (Frame *)(new Frame())
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetFlex(Flex::WIDTH, 8);

	auto assets = Assets::Get();
	auto give_up_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Give Up", 2,
														 { 255, 255, 255, 255 },
														 { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto give_up_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
															 { 200, 200, 200 },
															 sTextureButtonOutline.GetTexture(), give_up_text)
		->FlagForAutomaticDeletion();
	auto give_up_button = (Button *)(new Button(give_up_button_texture,
												give_up_button_texture))
		->SetRelative(Vec2i(-15, -15))
		->SetSize(Vec2i(give_up_button_texture->GetOriginalSize() / 2))
		->SetFlexInvolved(false, false)
		->SetAlign(Align::RIGHT, Align::ABOVE_TOP)
		->SetName("GiveUp");
	give_up_button->SetCallback([]()
								{
									json give_up_data;
									give_up_data["game_id"] = Centralized.GetCurrentGame()->game_id;
									auto packet = new Packet("/give_up_guessing", "POST", &give_up_data);
									packet->SetErroredCallback([packet]()
															   { delete packet; });
									packet->SetResponseCallback([packet](const NetworkResponse& server_Response)
																{
																	Centralized.current_menu = (FullscreenMenu *)Centralized.main_menu;
																	Centralized.current_menu->RefreshMenu();
																	delete packet;
																});
									packet->Send();
								});

	this->AddChildren({ complete_text, guessing_frame, give_up_button });
}

GuessingBar::~GuessingBar()
{

}

bool GuessingBar::IsAnsweredCorrectly()
{
	return GetTypedWord() == guess_word;
}

std::string GuessingBar::GetTypedWord()
{
	std::string user_answer;
	for (auto slot : letter_slots)
	{
		auto occupy_letter = slot->GetOccupyLetter();
		if (occupy_letter != nullptr)
			user_answer += occupy_letter->GetLetter();
	}

	return user_answer;
}

void GuessingBar::GenerateForWord(const std::string& word)
{
	this->complete_text->SetEnabled(false);
	SetTexture(sTextureGuessingBarBlue.GetTexture());
	UpdateTexturePlacement();

	for (auto slot : letter_slots)
		delete slot;
	letter_slots.clear();

	guess_word = word;
	auto num_slots = word.length();
	for (int i = 0; i < num_slots; i++)
	{
		auto new_slot = new LetterSlot();
		new_slot->UpdateRender(LetterSlot::sColorBlue);
		new_slot->SetCallback([new_slot]()
							  { new_slot->RemoveLetter(); });
		letter_slots.push_back(new_slot);
	}

	std::vector<Element *> letter_slots_as_elements(letter_slots.begin(), letter_slots.end());
	guessing_frame->SetChildren(letter_slots_as_elements);
}

bool GuessingBar::AddLetter(Letter *new_letter)
{
	size_t used_slots = 0;
	bool find_available = true;
	for (auto slot : letter_slots)
	{
		if (slot->GetOccupyLetter() == nullptr)
		{
			if (find_available)
			{
				find_available = false;
				new_letter->SetOccupySlot(slot);
				slot->OccupyLetter(new_letter);
				slot->SetClickable(true);
			}
		}
		else
		{ used_slots++; }
	}

	if (used_slots == letter_slots.size() - 1 &&
		IsAnsweredCorrectly())
	{

		this->complete_text->SetEnabled(true);
		this->parent->Refresh();
		this->SetTexture(sTextureGuessingBarGreen.GetTexture());
		this->UpdateTexturePlacement();

		for (auto letter_slot : letter_slots)
		{
			Letter *OccupyLetter = letter_slot->GetOccupyLetter();
			if (OccupyLetter != nullptr)
				OccupyLetter->SetAnsweredCorrectly();
		}

		RunCallback();
	}

	return !find_available;
}

void GuessingBar::RemoveLastLetter()
{
	for (auto letter_slot : std::ranges::reverse_view(letter_slots))
	{
		if (letter_slot->GetOccupyLetter() != nullptr)
		{
			letter_slot->RemoveLetter();
			break;
		}
	}
}

void GuessingBar::RunCallback() const
{
	if (callback != nullptr)
		callback();
}

void GuessingBar::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary)
{
	HandleEventChildren(sdl_event, event_summary);

	switch (sdl_event.type)
	{
		case SDL_EVENT_KEY_DOWN:
		{
			if (sdl_event.key.scancode == SDL_SCANCODE_BACKSPACE)
				RemoveLastLetter();

			break;
		}
	}
}
}
