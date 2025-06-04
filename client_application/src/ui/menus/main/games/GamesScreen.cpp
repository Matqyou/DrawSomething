//
// Created by Matq on 04/06/2025.
//

#include "GamesScreen.h"
#include "ui/components/element/TextElement.h"
#include "ui/CommonUI.h"
#include "ui/components/element/Button.h"
#include "ui/RenderPresets.h"

static LinkTexture sTextureStatistics("main_menu.all_games.statistics");
static LinkTexture sTextureCard("main_menu.all_games.card");
static LinkTexture sTextureCardOutline("main_menu.all_games.card_outline");
static LinkTexture sTextureClose("main_menu.settings.x");

GamesScreen::GamesScreen()
	: ScreenMenu()
{
	auto title = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 10))
		->SetSize(Vec2i(sTextureStatistics.GetTexture()->GetOriginalSize() / 3))
		->SetTexture(sTextureStatistics.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Statistics");

	auto economy = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontMiniProfile2.GetTTFFont(), "Economy Statistics", 2,
							{ 0, 100, 0, 255 },
							{ 255, 255, 255, 255 });
	average_coins = (TextElement *)(new TextElement());
	total_coins_earned = (TextElement *)(new TextElement());
	total_coins_circulation = (TextElement *)(new TextElement());
	total_coins_spent = (TextElement *)(new TextElement());
	auto economy_frame = (Frame *)(new Frame())
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT)
		->SetName("Economy")
		->AddChildren({ economy, average_coins, total_coins_circulation, total_coins_earned, total_coins_spent });

	auto game_stats = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontMiniProfile2.GetTTFFont(), "Game Statistics", 2,
							{ 100, 0, 100, 255 },
							{ 255, 255, 255, 255 });
	total_games = (TextElement *)(new TextElement());
	active_games = (TextElement *)(new TextElement());
	average_turns = (TextElement *)(new TextElement());
	games_24h = (TextElement *)(new TextElement());
	games_7d = (TextElement *)(new TextElement());
	highest_turn_reached = (TextElement *)(new TextElement());
	highest_turn_game = (TextElement *)(new TextElement());
	auto game_stats_frame = (Frame *)(new Frame())
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT)
		->SetName("GameStats")
		->AddChildren({ game_stats, total_games, active_games, average_turns, games_24h, games_7d, highest_turn_reached,
						highest_turn_game });

	auto user_stats = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontMiniProfile2.GetTTFFont(), "User Statistics", 2,
							{ 0, 100, 150, 255 },
							{ 255, 255, 255, 255 });
	total_users = (TextElement *)(new TextElement());
	active_users_24h = (TextElement *)(new TextElement());
	active_users_7d = (TextElement *)(new TextElement());
	new_users_24h = (TextElement *)(new TextElement());
	new_users_7d = (TextElement *)(new TextElement());
	auto users_frame = (Frame *)(new Frame())
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT)
		->SetName("GameStats")
		->AddChildren({ user_stats, total_users, active_users_24h, active_users_7d, new_users_24h, new_users_7d });

	Frame *content = (Frame *)(new Frame())
		->SetRelative(Vec2i(20, 0))
		->SetAdaptive(true, true)
		->SetFlex(Flex::WIDTH, 50)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("Content")
		->AddChildren({ economy_frame, game_stats_frame, users_frame });

	auto close_button = (Button *)(new Button(sTextureClose.GetTexture(),
											  sTextureClose.GetTexture()))
		->SetRelative(Vec2i(-10, 10))
		->SetSize(Vec2i(32, 32))
		->SetColor(150, 0, 0, 255)
		->SetAlign(Align::RIGHT, Align::TOP)
		->SetFlexInvolved(false, false)
		->SetName("CloseButton");
	close_button->SetCallback([this]()
							  {
								  this->SetEnabled(false);
							  });

	auto card_texture = RenderPresets::ColorButton(sTextureCard.GetTexture(),
												   { 50, 150, 100, 255 },
												   sTextureCardOutline.GetTexture());
	Frame *card = (Frame *)(new Frame())
		->SetSize(Vec2i(card_texture->GetOriginalSize() / 2))
		->SetTexture(card_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT, 5)
		->SetName("Card")
		->AddChildren({ title, content, close_button });

	this->SetEnabled(false);
	this->SetColor(0, 0, 0, 200);
	this->SetDraw(DRAW_RECT);
	this->SetFullyOccupy(true, true);
	this->SetName("GamesMenu");
	this->AddChildren({ card });
}

GamesScreen::~GamesScreen()
{

}

void GamesScreen::ParseFromJson(const json& json_data)
{
	auto avg_coins = json_data["economy_stats"].value("avg_coins_per_user", 0);
	auto total_earned = json_data["economy_stats"].value("total_coins_earned", 0);
	auto total_circulation = json_data["economy_stats"].value("total_coins_in_circulation", 0);
	auto total_spent = json_data["economy_stats"].value("total_coins_spent", 0);

	average_coins->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									 Strings::FString("Average Coins Per User: %d", avg_coins).c_str(), 2,
									 { 0, 0, 0, 255 },
									 { 255, 255, 255, 255 });
	total_coins_earned->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
										  Strings::FString("Total Coins Earned: %d", total_earned).c_str(), 2,
										  { 0, 150, 0, 255 },
										  { 255, 255, 255, 255 });
	total_coins_circulation->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
											   Strings::FString("Total Coins In Circulation: %d", total_circulation).c_str(), 2,
											   { 0, 0, 0, 255 },
											   { 255, 255, 255, 255 });
	total_coins_spent->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
										 Strings::FString("Total Coins Spent: %d", total_spent).c_str(), 2,
										 { 150, 0, 0, 255 },
										 { 255, 255, 255, 255 });

	auto num_games = json_data["game_stats"].value("total_games", 0);
	auto num_active_games = json_data["game_stats"].value("active_games", 0);
	auto avg_turns = json_data["game_stats"].value("avg_turns_per_game", 0);
	auto created_24h = json_data["game_stats"].value("games_created_24h", 0);
	auto created_7d = json_data["game_stats"].value("games_created_7d", 0);
	auto highest_turn = json_data["game_stats"].value("max_turns_ever", 0);
	auto highest_active_turn = json_data["game_stats"].value("max_turns_active", 0);

	total_games->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									Strings::FString("Total Games: %d", num_games).c_str(), 2,
									{ 0, 0, 0, 255 },
									{ 255, 255, 255, 255 });
	active_games->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									Strings::FString("Active Games: %d", num_active_games).c_str(), 2,
									{ 0, 150, 0, 255 },
									{ 255, 255, 255, 255 });
	average_turns->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									 Strings::FString("Average Turns Per Game: %d", avg_turns).c_str(), 2,
									 { 0, 0, 0, 255 },
									 { 255, 255, 255, 255 });
	games_24h->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
								 Strings::FString("Games Created Within a Day: %d", created_24h).c_str(), 2,
								 { 0, 0, 0, 255 },
								 { 255, 255, 255, 255 });
	games_7d->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
								Strings::FString("Games Created Within a Week: %d", created_7d).c_str(), 2,
								{ 0, 0, 0, 255 },
								{ 255, 255, 255, 255 });
	highest_turn_reached->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
											Strings::FString("Highest Turn Reached: %d", highest_turn).c_str(), 2,
											{ 0, 0, 0, 255 },
											{ 255, 255, 255, 255 });
	highest_turn_game->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
										 Strings::FString("Highest Active Turn: %d", highest_active_turn).c_str(), 2,
										 { 0, 0, 0, 255 },
										 { 255, 255, 255, 255 });

	auto num_users = json_data["user_stats"].value("total_users", 0);
	auto num_active_24h = json_data["user_stats"].value("active_users_24h", 0);
	auto num_active_7d = json_data["user_stats"].value("active_users_7d", 0);
	auto num_created_24h = json_data["user_stats"].value("new_users_24h", 0);
	auto num_created_7d = json_data["user_stats"].value("new_users_7d", 0);

	total_users->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
								   Strings::FString("Total Users: %d", num_users).c_str(), 2,
								   { 0, 0, 0, 255 },
								   { 255, 255, 255, 255 });
	active_users_24h->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
										Strings::FString("Active Users in the Past Day: %d", num_active_24h).c_str(), 2,
										{ 0, 0, 0, 255 },
										{ 255, 255, 255, 255 });
	active_users_7d->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									   Strings::FString("Active Users in the Past Week: %d", num_active_7d).c_str(), 2,
									   { 0, 0, 0, 255 },
									   { 255, 255, 255, 255 });
	new_users_24h->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									 Strings::FString("New Users in the Past Day: %d", num_created_24h).c_str(), 2,
									 { 0, 0, 0, 255 },
									 { 255, 255, 255, 255 });
	new_users_7d->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
									Strings::FString("New Users in the Past Week: %d", num_created_7d).c_str(), 2,
									{ 0, 0, 0, 255 },
									{ 255, 255, 255, 255 });
}
