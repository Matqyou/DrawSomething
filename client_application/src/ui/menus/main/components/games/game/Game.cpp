//
// Created by Matq on 26/01/2025.
//

#include "Game.h"
#include "ui/structures/window_texture/WindowTexture.h"
#include "ui/menus/main/components/games/CurrentGames.h"
#include "Statistics.h"
#include "game/GameData.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/menus/picking/PickingMenu.h"
#include "ui/menus/ingame/IngameMenu.h"
#include "ui/RenderPresets.h"

namespace Main
{
static Texture *sGenerateButton(AssetsClass *assets,
								TTF_Font *font,
								const char *text,
								SDL_Color text_color,
								const Vec2f& button_size,
								SDL_Color button_color,
								float left_scaling,
								float right_scaling,
								float top_scaling,
								float bottom_scaling)
{
	auto drawing = assets->GetDrawing();

	assets->GetTexture("window.sheet4")
		->SetScaleMode(SDL_SCALEMODE_NEAREST);

	Sheet sheet;
	WindowTexture window_texture(&sheet, left_scaling, right_scaling, top_scaling, bottom_scaling);
	sheet.Generate(assets, assets->GetTexture("window.sheet4"));
	window_texture.Generate(assets, button_size);
	Texture *generated = window_texture.GetGenerated();
	generated->SetColorMod(button_color);

	auto button_size_ = window_texture.GetGenerated()->GetOriginalSize();
	auto button_texture = assets->RenderTextBlended(font,
													text, text_color);
	auto play_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
											 SDL_TEXTUREACCESS_TARGET,
											 (int)button_size_.x,
											 (int)button_size_.y)
		->SetScaleMode(SDL_SCALEMODE_NEAREST)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(play_button);
	drawing->RenderTextureFullscreen(generated->SDLTexture(), nullptr);
	auto text_rect = Rectangles::CenterRelative(button_texture->GetOriginalSize(), button_size_);
	drawing->RenderTexture(button_texture->SDLTexture(), nullptr, text_rect);
	delete button_texture;

	return play_button;
}
static Texture *sGeneratePlayButton(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "Play",
						   { 255, 255, 255, 255 },
						   Vec2f(100, 45),
						   { 0, 200, 0, 255 },
						   15,
						   15,
						   15,
						   15);
}
static Texture *sGeneratePlayButtonPressed(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "Play",
						   { 255, 255, 255, 255 },
						   Vec2f(100, 45),
						   { 0, 180, 0, 255 },
						   15,
						   15,
						   15,
						   15);
}
static Texture *sGenerateDeleteButton(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "Delete",
						   { 255, 255, 255, 255 },
						   Vec2f(100, 45),
						   { 220, 0, 0, 255 },
						   15,
						   15,
						   15,
						   15);
}
static Texture *sGenerateDeleteButtonPressed(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "Delete",
						   { 255, 255, 255, 255 },
						   Vec2f(100, 45),
						   { 180, 0, 0, 255 },
						   15,
						   15,
						   15,
						   15);
}
static Texture *sGenerateUndefinedButton(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "NaN",
						   { 255, 255, 255, 255 },
						   Vec2f(100, 45),
						   { 220, 0, 220, 255 },
						   15,
						   15,
						   15,
						   15);
}
static Texture *sGenerateUndefinedButtonPressed(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "NaN",
						   { 255, 255, 255, 255 },
						   Vec2f(100, 45),
						   { 180, 0, 180, 255 },
						   15,
						   15,
						   15,
						   15);
}
static Texture *sGenerateTurnFrame(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontDefault.GetTTFFont(),
						   "",
						   { 0, 0, 0, 0 },
						   Vec2f(90, 90),
						   { 104, 195, 235, 255 },
						   10,
						   10,
						   10,
						   10);
}
static Texture *sGenerateStatsButton(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontSmallerBold.GetTTFFont(),
						   "Stats",
						   { 69, 19, 0, 255 },
						   Vec2f(60, 20),
						   { 255, 144, 71, 255 },
						   7,
						   7,
						   7,
						   7);
}
static Texture *sGenerateStatsButtonPressed(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontSmallerBold.GetTTFFont(),
						   "Stats",
						   { 69, 19, 0, 255 },
						   Vec2f(60, 20),
						   { 188, 104, 52, 255 },
						   7,
						   7,
						   7,
						   7);
}
static Texture *sGenerateCloseButton(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontSmallerBold.GetTTFFont(),
						   "Close",
						   { 69, 19, 0, 255 },
						   Vec2f(60, 20),
						   { 255, 144, 71, 255 },
						   7,
						   7,
						   7,
						   7);
}
static Texture *sGenerateCloseButtonPressed(AssetsClass *assets)
{
	return sGenerateButton(assets,
						   CommonUI::sFontSmallerBold.GetTTFFont(),
						   "Close",
						   { 69, 19, 0, 255 },
						   Vec2f(60, 20),
						   { 188, 104, 52, 255 },
						   7,
						   7,
						   7,
						   7);
}

static LinkTexture sTextureGamesContent("main_menu.games.game_new");
static LinkTexture sTextureGamesContentOutline("main_menu.games.game_new_outline");
static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");
static LinkTexture sTextureTurn("main_menu.games.turn");
static LinkTexture sTextureTurnOutline("main_menu.games.turn_outline");
static LinkTexture sTextureOnline("main_menu.ball");
static LinkTexture sTextureProfilePictureFrame("main_menu.games.picture_frame");

static PregenerateTexture sTextureStatsButton("main_menu.stats_button", sGenerateStatsButton);
static PregenerateTexture sTextureStatsButtonPressed("main_menu.stats_button_pressed", sGenerateStatsButtonPressed);
static PregenerateTexture sTextureCloseButton("main_menu.close_button", sGenerateCloseButton);
static PregenerateTexture sTextureCloseButtonPressed("main_menu.close_button_pressed", sGenerateCloseButtonPressed);

LinkTexture Game::sTextureDefaultProfilePicture("icons.icon");

void Game::SetTurnNumber(int turn)
{
	turn_number->UpdateText(CommonUI::sFontBigger.GetTTFFont(),
							Strings::FString("%d", turn).c_str(),
							{ 255, 255, 255, 255 });
}
void Game::SetProfilePicture(Texture *texture_data)
{
	Texture *profile_picture_ = texture_data != nullptr ? texture_data : sTextureDefaultProfilePicture.GetTexture();
	{
		auto assets = Assets::Get();
		auto drawing = assets->GetDrawing();

		auto original_render_target = SDL_GetRenderTarget(drawing->Renderer());

		auto frame_texture = sTextureProfilePictureFrame.GetTexture()
			->SetColorMod(50, 50, 50);

		delete profile_picture_texture;
		profile_picture_texture = profile_picture_->CopyTexture(SDL_TEXTUREACCESS_TARGET);
		drawing->SetRenderTarget(profile_picture_texture);
		drawing->RenderTextureFullscreen(frame_texture->SDLTexture());
		drawing->SetRenderTargetSDL(original_render_target);
	}
	profile_picture->SetTexture(profile_picture_texture);
//    profile_picture->UpdateVisualTexture(); //
}
void Game::SetUsername(const char *username)
{
	picture_text->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(),
									username, 2,
									{ 0, 0, 0, 255 },
									{ 255, 255, 255, 255 });
}

void Game::SetState(GameInfo::GameState game_state)
{
	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	SDL_Color text_color = game_state == GameInfo::GameState::YOUR_MOVE ?
						   SDL_Color{ 230, 255, 230, 255 } : SDL_Color{ 150, 150, 150, 255 };
	const char *game_state_text = game_state == GameInfo::GameState::YOUR_MOVE ? "ready" : "waiting";
	const char *description_text = game_state == GameInfo::GameState::YOUR_MOVE ? "your move" : "waiting their move";
	state_text->UpdateText(CommonUI::sFontDefault.GetTTFFont(), game_state_text, text_color);
	state_description->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), description_text, text_color);

	Texture *texture = game_state == GameInfo::GameState::YOUR_MOVE ?
					   play_button_texture : play_grayed_button_texture;
	play_button->SetTexture(texture);
	play_button->SetPressedTexture(texture);
	play_button->SetClickable(game_state == GameInfo::GameState::YOUR_MOVE);
	play_button->SetEnabled(game_state == GameInfo::GameState::YOUR_MOVE);

	SDL_Color background_color = game_state == GameInfo::GameState::YOUR_MOVE ?
								 SDL_Color{ 255, 0, 157 } : SDL_Color{ 145, 85, 122 };
	{
		auto background_texture = sTextureGamesContent.GetTexture();

		background_texture->SetColorMod(255, 255, 255); // reset changes from other color mods
		game_background = background_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET);
		drawing->SetRenderTarget(game_background);
		background_texture->SetColorMod(background_color);
		drawing->RenderTextureFullscreen(background_texture->SDLTexture());
//		drawing->RenderTextureFullscreen(background_outline_texture->SDLTexture());

	}
	drawing->SetRenderTarget(nullptr);

	game_frame->SetTexture(game_background);
//	game_button->SetTexture(texture);
//	game_button->ChangePressedTexture(pressed_texture);
}

Game::Game(CurrentGames *games)
	: Frame()
{
	this->games_ = games;
	this->game_info_ = nullptr;

	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	{
		auto turn_box_texture = sTextureTurn.GetTexture();
		auto turn_box_outline_texture = sTextureTurnOutline.GetTexture()
			->SetAlphaMod(220);
		auto turn_box_size = turn_box_texture->GetOriginalSize();

		turn_texture = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
											 (int)turn_box_size.x, (int)turn_box_size.y);
		drawing->SetRenderTarget(turn_texture);
		turn_box_texture->SetColorMod(0, 170, 200);
		drawing->RenderTextureFullscreen(turn_box_texture->SDLTexture(), nullptr);
		drawing->RenderTextureFullscreen(turn_box_outline_texture->SDLTexture(), nullptr);
	}

	{
		auto play_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Play", 3,
														  { 255, 255, 255, 255 },
														  { 0, 0, 0, 255 });
		play_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														 { 0, 180, 0 },
														 sTextureButtonOutline.GetTexture(), play_text);
		delete play_text;
	}
	{
		auto play_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Play", 3,
														  { 255, 255, 255, 255 },
														  { 0, 0, 0, 255 });
		play_grayed_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
																{ 0, 180, 0 },
																sTextureButtonOutline.GetTexture(), play_text, true);
		delete play_text;
	}
	{
		auto delete_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Delete", 3,
															{ 255, 255, 255, 255 },
															{ 0, 0, 0, 255 });

		delete_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														   { 255, 0, 0 },
														   sTextureButtonOutline.GetTexture(), delete_text, false);
		delete delete_text;
	}

	profile_picture_texture = nullptr;

	this->statistics = new Statistics();

	auto turn_text = (new TextElement())
		->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "turn", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, 5))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("TurnTitle");

	this->turn_number = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "NaN", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, -5))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("TurnNumber");

	auto stats = (Button *)(new Button(sTextureStatsButton.GetTexture(),
									   sTextureStatsButtonPressed.GetTexture()))
		->SetRelative(Vec2i(0, -5))
		->SetSize(Vec2i(60, 20))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("StatsButton");
	stats->SetCallback([this, stats]()
					   {
						   this->statistics->SetEnabled(!this->statistics->enabled);
						   if (this->statistics->enabled)
						   {
							   stats->SetTexture(sTextureCloseButton.GetTexture());
							   stats->SetPressedTexture(sTextureCloseButtonPressed.GetTexture());
						   }
						   else
						   {
							   stats->SetTexture(sTextureStatsButton.GetTexture());
							   stats->SetPressedTexture(sTextureStatsButtonPressed.GetTexture());
						   }

						   this->parent->parent->parent->Refresh();
					   });

	auto turn_frame = (new Frame())
		->SetRelative(Vec2i(0, -3))
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->SetName("TurnFrame")
		->AddChildren({ turn_text, turn_number, stats });

	auto turn = (new Frame())
		->SetSize(Vec2i(70, 90))
		->SetTexture(turn_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("Turn")
		->AddChildren({ turn_frame });

	this->online = (Frame *)(new Frame())
		->SetSize(Vec2i(11, 11))
		->SetRelative(Vec2i(5, 5))
		->SetTexture(sTextureOnline.GetTexture())
		->SetFlexInvolved(false, false)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::RIGHT, Align::BOTTOM)
		->SetEnabled(false)
		->SetName("Online");

	this->profile_picture = (Frame *)(new Frame())
		->SetSize(Vec2i(66, 66))
		->SetTexture(assets->GetTexture("icons.icon_white"))
		->SetDraw(DRAW_TEXTURE)
		->SetName("ProfilePicture")
		->AddChildren({ online });

	this->picture_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "NaN", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(3, 0))
		->SetName("PictureName");

	auto picture_frame = (new Frame())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->SetName("PictureFrame")
		->AddChildren({ profile_picture, picture_text });

	this->state_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "state", { 150, 150, 150, 255 })
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("StateText");

	this->state_description = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "description", { 150, 150, 150, 255 })
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("StateDescription");

	auto state_frame = (new Frame())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetFullyOccupy(false, true)
		->SetOccupy(true, false)
		->SetFlex(Flex::HEIGHT)
		->SetAdaptive(false, true)
		->SetName("StateFrame")
		->AddChildren({ state_text, state_description });

	this->play_button = (Button *)(new Button(play_button_texture, play_button_texture))
		->SetSize(Vec2i(play_button_texture->GetOriginalSize() / 2))
		->SetName("PlayButton");
	this->play_button->SetCallback([this]()
								   {
									   LoadingScreen *loading_screen = games_->loading_screen_;
									   loading_screen->StartShowing("Please wait while we prepare the game :o");

									   json game_data;
									   game_data["game_id"] = game_info_->game_id;
									   auto packet = (new Packet("/start_game", "POST", &game_data));
									   packet->SetErroredCallback([packet]()
																  { delete packet; });
									   packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																   {
																	   std::string code = server_response.GetCode();
																	   std::string message = server_response.GetMsg();

																	   if (code == "game_start_success")
																	   {
																		   Centralized.main_menu->GetLoadingScreen()->Hide();
																		   if (server_response.response_json.contains("game") && server_response.response_json["game"].is_object())
																		   {
																			   json game_data = server_response.response_json["game"];

																			   int game_id = game_data.value("game_id", -1);
																			   std::string easy_word = game_data.value("choice_easy", "");
																			   std::string medium_word = game_data.value("choice_medium", "");
																			   std::string hard_word = game_data.value("choice_hard", "");

																			   auto updated_game = Centralized.GetGame(game_id)->ParseFromJson(game_data, true);
																			   Centralized.SetCurrentGame(updated_game);

																			   // Picking stage for drawing
																			   if (updated_game->turn_stage == GameInfo::TurnStage::PICKING)
																			   {
																				   Centralized.current_menu = (FullscreenMenu *)Centralized.picking_menu;
																				   Centralized.picking_menu->SetWords(easy_word, medium_word, hard_word);//
																				   Centralized.picking_menu->RefreshMenu();
																			   }
																			   else if (updated_game->turn_stage == GameInfo::TurnStage::GUESSING)
																			   { // Guessing stage
																				   std::string replay_data = server_response.response_json.value("guessing_recording", "");
																				   Centralized.ingame_menu->GetCanvas()->LoadReplayData(replay_data);

																				   Centralized.current_menu = (FullscreenMenu *)Centralized.ingame_menu;
																				   Centralized.ingame_menu->PrepareGuess();
																				   Centralized.ingame_menu->RefreshMenu();
																			   }
																			   else
																			   { // Drawing stage
																				   Centralized.current_menu = (FullscreenMenu *)Centralized.ingame_menu;
																				   Centralized.ingame_menu->PrepareDraw();
																				   Centralized.ingame_menu->RefreshMenu();
																			   }
																		   }

																		   if (server_response.response_json.contains("owned_colors") && server_response.response_json["owned_colors"].is_array())
																		   {
																			   Centralized.UpdateOwnedColors(server_response.response_json["owned_colors"]);
																			   Centralized.ingame_menu->GetColorSelector()->RefreshData();
																			   Centralized.ingame_menu->RefreshMenu();
																		   }

																		   dbg_msg("&d[SERVER Response]&r Game started: %s\n", message.c_str());
																	   }
																	   else if (code == "game_start_fail")
																	   {
																		   dbg_msg("&d[SERVER Response]&r Game start fail: %s\n", message.c_str());
																	   }

																	   delete packet;
																   });
									   packet->Send();
								   });

	this->delete_button = (Button *)(new Button(delete_button_texture, delete_button_texture))
		->SetSize(Vec2i(delete_button_texture->GetOriginalSize() / 2))
		->SetName("DeleteButton");

	this->delete_button->SetCallback([this]()
									 {
										 ConfirmationScreen *confirmation_screen = games_->confirmation_screen_;
										 confirmation_screen->SetAgreeCallback([this, confirmation_screen]()
																			   {
																				   json delete_data;
																				   delete_data["game_id"] = game_info_->game_id;
																				   auto packet = (new Packet("/delete_game", "POST", &delete_data));
																				   packet->SetErroredCallback([packet]()
																											  {
																												  delete packet;
																											  });
																				   packet->SetResponseCallback([packet](
																					   const NetworkResponse& server_response)
																											   {
																												   std::string code = server_response.GetCode();
																												   std::string message = server_response.GetMsg();

																												   if (code == "game_delete_success")
																												   {
																													   int game_id = server_response.response_json.value("game_id", -1);
																													   if (game_id != -1)
																													   {
																														   Centralized.RemoveGame(game_id);
																														   Centralized.SortGames();
																														   Centralized.main_menu->Games()->RefreshData();
																														   Centralized.main_menu->RefreshMenu();
																													   }

																													   dbg_msg("&d[SERVER Response]&r Game deleted: %s\n", message.c_str());
																												   }
																												   else if (code == "game_delete_fail")
																												   {
																													   dbg_msg("&d[SERVER Response]&r Game deletion fail: %s\n", message.c_str());
																												   }

																												   delete packet;
																											   });
																				   packet->Send();

																				   confirmation_screen->SetEnabled(false);
																			   });

										 confirmation_screen->Prompt("You want to delete this game?");
//            this->SetEnabled(false);
//            this->FlagToDestroy();
//
//            this->parent->parent->parent->Refresh();
									 });

	auto buttons_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(-15, 0))
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT, 5)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("ButtonsFrame")
		->AddChildren({ play_button, delete_button });

	auto info_frame = (new Frame())
		->SetRelative(Vec2i(5, 0))
		->SetSize(Vec2i(0, 102))
		->SetFlex(Flex::WIDTH, 5)
		->SetFullyOccupy(true, false)
		->SetName("InfoFrame")
		->AddChildren({ turn, picture_frame, state_frame, buttons_frame });\

	game_background = nullptr;

	game_frame = (Frame *)(new Frame())
		->SetSize(Vec2i(1120 * 0.5, 204 * 0.5))
		->SetTexture(sTextureGamesContent.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetName("GameFrame")
		->AddChildren({ info_frame });

	this->SetSize(Vec2i(1120 * 0.5, 204 * 0.5));
	this->SetFlex(Flex::HEIGHT);
	this->SetAdaptive(false, true); // for statistics
	this->SetName("GameContent");
	this->AddChildren({ game_frame, statistics });
}

Game::~Game()
{
	delete game_background;

	delete turn_texture;

	delete play_button_texture;
	delete play_grayed_button_texture;
	delete delete_button_texture;

	delete profile_picture_texture;
}

Game *Game::UpdateInfo(GameInfo *game_info)
{
	game_info_ = game_info;
	if (game_info != nullptr)
	{
		game_info->pfp_update_callback = [this]()
		{
			SetProfilePicture(game_info_->players[1].profile_picture);
		};

		SetTurnNumber(game_info->game_turn);
		SetProfilePicture(game_info->players[1].profile_picture);
		SetUsername(game_info->players[1].username.c_str());
		SetState(game_info->game_state);
		online->SetEnabled(game_info->players[1].online);

		statistics->UpdateInfo(game_info);
	}

	return this;
}
}
