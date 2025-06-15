//
// Created by Matq on 26/01/2025.
//

#include "MainHeader.h"
#include "ui/structures/window_texture/WindowTexture.h"
#include "ui/components/element/TextElement.h"
#include "ui/components/element/Button.h"
#include "ui/CommonUI.h"
#include "game/GameData.h"
#include "ui/menus/main/MainMenu.h"

Texture *sGenerateCurrencyFrame(AssetsClass *assets)
{
	auto drawing = assets->GetDrawing();

	Sheet sheet;
	WindowTexture wt(&sheet, 12, 12, 12, 12);
	sheet.Generate(assets, assets->GetTexture("window.sheet5"));
	Texture *generated = wt.Generate(assets, Vec2f(66, 28));

	Texture *currency_frame = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 66, 28);
	drawing->SetRenderTarget(currency_frame);
	generated->SetColorMod(254, 144, 72);
	drawing->RenderTextureFullscreen(generated->SDLTexture(), nullptr);

	return currency_frame;
}

//static LinkTexture sTextureHeader("header", Main::sCallbackHeader);
static LinkTexture sTextureTitle("title");
static LinkTexture sTextureCoin("icons.coin");
static LinkTexture sTextureBomb("icons.bomb");
static LinkTexture sTextureSettings("main_menu.header.settings");
static LinkTexture sTextureSettingsPressed("main_menu.header.settings_pressed");
static LinkTexture sTextureShop("main_menu.header.shop");
static LinkTexture sTextureAdmin("main_menu.header.admin");
static LinkTexture sTextureGames("main_menu.header.games");
static LinkTexture sTextureProfile("main_menu.header.profile");
static LinkTexture sTexturePlus("main_menu.header.plus");
static LinkTexture sTexturePlusPressed("main_menu.header.plus_pressed");
static PregenerateTexture sTextureHeader("main_menu_header", [](AssetsClass *assets) -> Texture *
{
	auto header_texture = assets->GetTexture("header");
	return header_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET)
		->SetColorMod(0, 211, 112);
});
static PregenerateTexture sTextureCurrencyFrame("main_menu.header.currency_frame", sGenerateCurrencyFrame);

MainHeader::MainHeader(MainMenu *parent)
	: Frame()
{
	this->name = L"Header";
	this->SetSize(Vec2i(0, 58));
	this->SetTexture(sTextureHeader.GetTexture()); // Rect4f(0, 0, 100, 361)
	this->SetDraw(DRAW_TEXTURE);
	this->SetFullyOccupy(true, false);

	auto currency_background = (new Frame())
		->SetRelative(Vec2i(3, 0))
		->SetSize(Vec2i(66, 28))
		->SetTexture(sTextureCurrencyFrame.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFlexInvolved(false, false)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("CurrencyBackground");
	num_coins = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "NaN", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(2, 0))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("NumberOfCoins");
	auto coin = (new Frame())
		->SetSize(Vec2i(32, 32))
		->SetTexture(sTextureCoin.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("Coin");
//	more_coins_button = (Button *)(new Button(sTexturePlus.GetTexture(), sTexturePlusPressed.GetTexture()))
//		->SetRelative(Vec2i(2, 0))
//		->SetSize(Vec2i(15, 15))
//		->SetAlign(Align::DONT, Align::BOTTOM)
//		->SetName("Plus");
	auto currency_frame = (new Frame())
		->SetRelative(Vec2i(10, 0))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetFlex(Flex::WIDTH, 2)
		->SetAdaptive(true, true)
		->SetName("CurrencyFrame")
		->AddChildren({ currency_background, coin, num_coins }); // more_coins_button


	auto bombs_background = (new Frame())
		->SetRelative(Vec2i(3, 0))
		->SetSize(Vec2i(66, 28))
		->SetTexture(sTextureCurrencyFrame.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFlexInvolved(false, false)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("BombsBackground");
	num_bombs = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "NaN", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(-7, 0))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("NumberOfBombs");
	auto bomb = (new Frame())
		->SetRelative(Vec2i(0, -2))
		->SetSize(Vec2i(40, 40))
		->SetTexture(sTextureBomb.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("Bomb");
	auto bombs_frame = (new Frame())
		->SetRelative(Vec2i(10 + 80, 0))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetFlex(Flex::WIDTH, 2)
		->SetAdaptive(true, true)
		->SetName("CurrencyFrame")
		->AddChildren({ bombs_background, bomb, num_bombs }); // more_coins_button


	auto logo = (new Frame())
		->SetSize(Vec2i(374 * 0.6, 82 * 0.6))
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(sTextureTitle.GetTexture())
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("Logo");

	shop_button = (Button *)(new Button(sTextureShop.GetTexture(), sTextureShop.GetTexture()))
		->SetSize(Vec2i(40, 40))
		->SetName("Shop");
	shop_button->SetCallback([parent]()
							 {
								 auto shop_menu = parent->GetShopScreen();
								 parent->GetAdminScreen()->SetEnabled(false);
								 parent->GetGamesScreen()->SetEnabled(false);
								 parent->GetSettingsScreen()->SetEnabled(false);
								 parent->GetProfileScreen()->SetEnabled(false);
								 if (shop_menu)
								 {
									 shop_menu->SetEnabled(!shop_menu->enabled);
									 shop_menu->parent->Refresh();

									 if (shop_menu->enabled)
									 {
										 auto packet = (new Packet("/get_shop", "GET"));
										 packet->SetErroredCallback([packet]()
																	{ delete packet; });
										 packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																	 {
																		 std::string code = server_response.GetCode();
																		 std::string message = server_response.GetMsg();

																		 if (code == "get_shop_success")
																		 {
																			 Centralized.main_menu->GetShopScreen()
																				 ->ParseFromJson(server_response.response_json);
																			 Centralized.main_menu->RefreshMenu();

																			 dbg_msg("&d[SERVER Response]&r Received shop items: %s\n", message.c_str());
																		 }
																		 delete packet;
																	 });
										 packet->Send();
									 }
								 }
							 });

	admin_button = (Button *)(new Button(sTextureAdmin.GetTexture(), sTextureAdmin.GetTexture()))
		->SetSize(Vec2i(40, 40))
		->SetName("Admin");
	admin_button->SetCallback([parent]()
							  {
								  auto admin_menu = parent->GetAdminScreen();
								  parent->GetGamesScreen()->SetEnabled(false);
								  parent->GetShopScreen()->SetEnabled(false);
								  parent->GetSettingsScreen()->SetEnabled(false);
								  parent->GetProfileScreen()->SetEnabled(false);
								  if (admin_menu)
								  {
									  admin_menu->SetEnabled(!admin_menu->enabled);
									  admin_menu->parent->Refresh();

									  if (admin_menu->enabled)
									  {
										  auto packet = (new Packet("/all_users", "GET"));
										  packet->SetErroredCallback([packet]()
																	 {
																		 delete packet;
																	 });
										  packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																	  {
																		  std::string code = server_response.GetCode();
																		  std::string message = server_response.GetMsg();

																		  if (code == "all_users")
																		  {
																			  if (server_response.response_json.contains("users") && server_response.response_json["users"].is_array())
																			  {
																				  Centralized.ClearUserList();
																				  for (const json& user_data : server_response.response_json["users"])
																					  if (user_data.is_object())
																					  {
																						  Account *new_user = &(new Account())->ParseFromJson(user_data);
																						  Centralized.AddToUserList(new_user);
																					  }
																			  }

																			  Centralized.main_menu->GetAdminScreen()->RefreshData();
																			  Centralized.main_menu->RefreshMenu();

																			  dbg_msg("&d[SERVER Response]&r Received users list: %s\n", message.c_str());
																		  }
																		  delete packet;
																	  });
										  packet->Send();
									  }

								  }
							  });

	games_button = (Button *)(new Button(sTextureGames.GetTexture(), sTextureGames.GetTexture()))
		->SetSize(Vec2i(40, 40))
		->SetName("Games");
	games_button->SetCallback([parent]()
							  {
								  auto games_menu = parent->GetGamesScreen();
								  parent->GetShopScreen()->SetEnabled(false);
								  parent->GetAdminScreen()->SetEnabled(false);
								  parent->GetSettingsScreen()->SetEnabled(false);
								  parent->GetProfileScreen()->SetEnabled(false);
								  if (games_menu)
								  {
									  games_menu->SetEnabled(!games_menu->enabled);
									  games_menu->parent->Refresh();

									  if (games_menu->enabled)
									  {
										  auto packet = (new Packet("/all_game_stats", "GET"));
										  packet->SetErroredCallback([packet]()
																	 { delete packet; });
										  packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																	  {
																		  Centralized.main_menu->GetGamesScreen()
																			  ->ParseFromJson(server_response.response_json);
																		  Centralized.main_menu->RefreshMenu();

																		  dbg_msg("&d[SERVER Response]&r Received games statistics\n");
																		  delete packet;
																	  });
										  packet->Send();
									  }
								  }
							  }
	);

	profile_button = (Button *)(new Button(sTextureProfile.GetTexture(), sTextureProfile.GetTexture()))
		->SetSize(Vec2i(40, 40))
		->SetName("Profile");
	profile_button->SetCallback([parent]()
								{
									auto profile_menu = parent->GetProfileScreen();
									parent->GetAdminScreen()->SetEnabled(false);
									parent->GetGamesScreen()->SetEnabled(false);
									parent->GetShopScreen()->SetEnabled(false);
									parent->GetSettingsScreen()->SetEnabled(false);
									if (profile_menu)
									{
										profile_menu->SetEnabled(!profile_menu->enabled);
										profile_menu->parent->Refresh();

										if (profile_menu->enabled)
										{
											profile_menu->RefreshData();
//										 auto packet = (new Packet("/get_shop", "GET"));
//										 packet->SetErroredCallback([packet]()
//																	{ delete packet; });
//										 packet->SetResponseCallback([packet](const NetworkResponse& server_response)
//																	 {
//																		 std::string code = server_response.GetCode();
//																		 std::string message = server_response.GetMsg();
//
//																		 if (code == "get_shop_success")
//																		 {
//																			 Centralized.main_menu->GetShopScreen()
//																				 ->ParseFromJson(server_response.response_json);
//																			 Centralized.main_menu->RefreshMenu();
//
//																			 dbg_msg("&d[SERVER Response]&r Received shop items: %s\n", message.c_str());
//																		 }
//																		 delete packet;
//																	 });
//										 packet->Send();
										}
									}
								});

	auto settings_button = (Button *)(new Button(sTextureSettings.GetTexture(),
												 sTextureSettingsPressed.GetTexture()))
		->SetSize(Vec2i(40, 40))
		->SetName("Settings");
	settings_button->SetCallback([parent]()
								 {
									 auto settings = parent->GetSettingsScreen();
									 parent->GetAdminScreen()->SetEnabled(false);
									 parent->GetGamesScreen()->SetEnabled(false);
									 parent->GetShopScreen()->SetEnabled(false);
									 parent->GetProfileScreen()->SetEnabled(false);

									 settings->SetEnabled(!settings->enabled);
									 if (settings->enabled)
										 settings->parent->Refresh();
								 });

	auto buttons_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(-10, 0))
		->SetAdaptive(true, true)
		->SetAlign(Align::RIGHT, Align::CENTER)
		->SetFlex(Flex::WIDTH, 5)
		->SetName("ButtonsFrame")
		->AddChildren({ profile_button, admin_button, games_button, shop_button }); // settings_button

	this->AddChildren({ currency_frame, bombs_frame, logo, buttons_frame });
}

MainHeader::~MainHeader()
{

}

void MainHeader::RefreshData()
{
	auto account = Centralized.GetAccount();
	num_coins->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
								 std::to_string(account.coins).c_str(), 1,
								 { 255, 255, 255, 255 },
								 { 0, 0, 0, 255 });
	num_bombs->UpdateTextOutline(CommonUI::sFontSmaller.GetTTFFont(),
								 std::to_string(account.bombs).c_str(), 1,
								 { 255, 255, 255, 255 },
								 { 0, 0, 0, 255 });
	admin_button->SetEnabled(account.is_administrator);
	games_button->SetEnabled(account.is_administrator);
}
