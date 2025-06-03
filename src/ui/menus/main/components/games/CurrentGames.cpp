//
// Created by Matq on 26/01/2025.
//

#include "CurrentGames.h"
#include "ui/components/element/Button.h"
#include "ui/menus/main/components/games/game/Game.h"
#include "game/GameData.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/RenderPresets.h"

namespace Main
{
static LinkTexture sTextureCurrentGames("main_menu.games.current_games");
static LinkTexture sTextureGamesHeader("main_menu.games.title");
static LinkTexture sTextureGamesFooter("main_menu.games.footer_new");
static LinkTexture sTextureGamesFooterOverlay("main_menu.games.footer_new_overlay");

CurrentGames::CurrentGames(ConfirmationScreen *confirmation_screen, LoadingScreen *loading_screen)
	: Frame()
{
	this->confirmation_screen_ = confirmation_screen;
	this->loading_screen_ = loading_screen;

	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();
	{
		auto header_texture = sTextureGamesHeader.GetTexture()
			->SetColorMod(196, 0, 121);
		title_texture = header_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET);
		drawing->SetRenderTarget(title_texture);
		drawing->RenderTextureFullscreen(header_texture->SDLTexture());
	}
	{
		auto footer_text = assets->RenderTextBlendedOutline(CommonUI::sFontMediumBigger2x.GetTTFFont(),
															"+ New Game", 3,
															{ 255, 255, 255, 255 },
															{ 0, 0, 0, 255 });
		footer_enabled_texture = RenderPresets::ColorButton(sTextureGamesFooter.GetTexture(), { 0, 255, 0 },
															sTextureGamesFooterOverlay.GetTexture(),
															footer_text);
		footer_enabled_pressed_texture = RenderPresets::ColorButton(sTextureGamesFooter.GetTexture(), { 0, 255, 0 },
																	sTextureGamesFooterOverlay.GetTexture(),
																	footer_text, true);

		auto footer_text_maximum = assets->RenderTextBlendedOutline(CommonUI::sFontMediumBigger2x.GetTTFFont(),
																	"Max Games Reached: 5", 3,
																	{ 255, 255, 255, 255 },
																	{ 0, 0, 0, 255 });
		footer_disabled_texture = RenderPresets::ColorButton(sTextureGamesFooter.GetTexture(), { 150, 150, 150 },
															 sTextureGamesFooterOverlay.GetTexture(),
															 footer_text_maximum, true);
	}
	drawing->SetRenderTarget(nullptr);

	auto current_games_texture = sTextureCurrentGames.GetTexture();
	auto current_games = (Frame *)(new Frame())
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetTexture(current_games_texture)
		->SetSize(Vec2i(current_games_texture->GetOriginalSize() * 0.3))
		->SetDraw(DRAW_TEXTURE)
		->SetName("CurrentGames");

	// Games title
	auto games_title = (new Frame())
		->SetSize(Vec2i(1120 * 0.5, 120 * 0.5))
		->SetTexture(title_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetName("Title")
		->AddChildren({ current_games });

	this->games = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->SetName("Games");

	// Games create
	create_game = (Button *)(new Button(footer_enabled_texture,
											  footer_enabled_pressed_texture))
		->SetSize(Vec2i(footer_enabled_texture->GetOriginalSize() / 2))
		->SetName("Create");
	create_game->SetCallback([]()
							  {
								  auto packet = (new Packet("/create_game", "POST", nullptr));
								  packet->SetErroredCallback([packet]()
															 {
																 delete packet;
															 });
								  packet->SetResponseCallback([packet](const NetworkResponse& server_response)
															  {
																  std::string code = server_response.GetCode();
																  std::string message = server_response.GetMsg();

																  if (code == "game_create_success")
																  {
																	  if (server_response.response_json.contains("game") && server_response.response_json["game"].is_object())
																	  {
																		  json game_data = server_response.response_json["game"];
																		  Centralized.AddGame((new GameInfo())->ParseFromJson(game_data, false));
																		  Centralized.SortGames();
																		  Centralized.main_menu->Games()->RefreshData();
																		  Centralized.main_menu->RefreshMenu();
																	  }

																	  dbg_msg("&d[SERVER Response]&r Game created: %s\n", message.c_str());
																  }
																  else if (code == "game_create_fail")
																  {
																	  dbg_msg("&d[SERVER Response]&r Game creation fail: %s\n", message.c_str());
																  }

																  delete packet;
															  });
								  packet->Send();
							  });

	this->SetRelative(Vec2i(25, 100 - 58 - 17));
	this->SetFlex(Flex::HEIGHT);
	this->SetAdaptive(true, true);
	this->SetName("GamesTab");
	this->AddChildren({ games_title, games, create_game });
}

CurrentGames::~CurrentGames()
{
	delete title_texture;
	delete footer_enabled_texture;
	delete footer_enabled_pressed_texture;
	delete footer_disabled_texture;
}

void CurrentGames::RefreshData()
{
	for (auto game : games->children)
		delete game;
	games->children.clear();

	for (auto game : Centralized.GetOpenGames())
		games->AddChildren({ (new Game(this))->UpdateInfo(game) });

	bool max_reached = games->children.size() >= 5;
	create_game->SetTexture(max_reached ? footer_disabled_texture : footer_enabled_texture);
	create_game->SetClickable(!max_reached);
}
}
