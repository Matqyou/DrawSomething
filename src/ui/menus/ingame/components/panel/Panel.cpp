//
// Created by Matq on 12/01/2025.
//

#include "Panel.h"
#include "core/Application.h"
#include "ui/components/element/Button.h"
#include "network/NetworkClient.h"
#include "game/GameData.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/menus/intermission/IntermissionMenu.h"

namespace Ingame
{
LinkTexture Panel::game_palette_background("game.ingame_panel.letter_palette.background");
LinkTexture Panel::game_palette_bomba("game.ingame_panel.letter_palette.bomba");

void Panel::ResetBomba()
{
	bomba_button->SetClickable(true);
}

Panel::Panel()
	: Frame()
{
	// Guessing bar
	this->guessing_bar = new GuessingBar();
	this->guessing_bar->SetCallback([this]()
									{
										json guess_data;
										guess_data["game_id"] = Centralized.GetCurrentGame()->game_id;
										guess_data["guess"] = guessing_bar->GetTypedWord();
										auto packet = (new Packet("/guess_word", "POST", &guess_data));
										packet->SetErroredCallback([packet]()
																   {
																	   delete packet;
																   });
										packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																	{
																		std::string code = server_response.GetCode();
																		std::string message = server_response.GetMsg();

																		if (code == "guess_success")
																		{
																			if (server_response.response_json.contains("user") && server_response.response_json["user"].is_object())
																			{
																				json user_data = server_response.response_json["user"];
																				auto account = Centralized.GetAccount().ParseFromJson(user_data);
																				Centralized.main_menu->Header()->RefreshData();
																			}

																			if (server_response.response_json.contains("game") && server_response.response_json["game"].is_object())
																			{
																				json game_data = server_response.response_json["game"];
																				int game_id = game_data.value("game_id", -1);
																				Centralized.GetGame(game_id)->ParseFromJson(game_data, true);
																				Centralized.SortGames();
																			}

																			Centralized.current_menu = (FullscreenMenu *)Centralized.intermission_menu;
																			Centralized.intermission_menu->RefreshData();
																			Centralized.intermission_menu->ShowIntermission();
																			Centralized.intermission_menu->RefreshMenu();

																			dbg_msg("&d[SERVER Response]&r Guessed correctly: %s\n", message.c_str());
																		}
																		else if (code == "guess_fail")
																		{
																			dbg_msg("&d[SERVER Response]&r Guessing fail: %s\n", message.c_str());
																		}
																		delete packet;
																	});
										packet->Send();
									});

	// Rows frame
	this->letters = new Letters(guessing_bar);

	// Rows side
	auto rows = (new Frame())
		->SetOccupy(true, false)
		->SetFullyOccupy(false, true)
		->SetName("Rows")
		->AddChildren({ letters });

	// Bomba
	this->bomba_button = (Button *)(new Button(game_palette_bomba.GetTexture(), nullptr))
		->SetSize(Vec2i(98, 147))
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("Bomba");
	this->bomba_button->SetCallback([this]()
									{
										letters->BlowUp();
										bomba_button->SetClickable(false);
									});

	// Bomba side
	auto bomba_frame = (new Frame())
		->SetSize(Vec2i(114, 0))
		->SetFullyOccupy(false, true)
		->SetFlex(Flex::WIDTH)
		->SetName("Bomba")
		->AddChildren({ bomba_button });

	// Letters bar
	auto letters_bar = (new Frame())
		->SetSize(Vec2i(0, 165))
		->SetTexture(game_palette_background.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFullyOccupy(true, false)
		->SetFlex(Flex::WIDTH)
		->SetName("Bottom")
		->AddChildren({ rows, bomba_frame });

	this->SetFullyOccupy(true, false);
	this->SetAdaptive(false, true);
	this->SetFlex(Flex::HEIGHT);
	this->SetName("Panel");
	this->AddChildren({ guessing_bar, letters_bar });
}

bool Panel::RandomizeWord(const std::string& word)
{
	if (letters->RandomizeWord(word))
	{ // todo: make dynamic num of letters but stay at 12
		guessing_bar->GenerateForWord(word);
		ResetBomba();
		return true;
	}

	return false;
}
}
