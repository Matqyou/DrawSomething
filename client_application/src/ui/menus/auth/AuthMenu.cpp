//
// Created by Matq on 16/01/2025.
//

#include "AuthMenu.h"
#include "core/Application.h"
#include "ui/components/element/TextBox.h"
#include "ui/structures/window_texture/WindowTexture.h"
#include "ui/components/element/Button.h"
#include "ui/CommonUI.h"
#include "ui/components/element/TextElement.h"
#include "ui/menus/auth/components/ScrollingBackground.h"
#include "network/NetworkClient.h"
#include "game/GameData.h"
#include "ui/menus/main/MainMenu.h"

namespace Auth
{

Texture *sGenerateAuthField(AssetsClass *assets, const Vec2f& size, bool light)
{
	auto drawing = assets->GetDrawing();

	Sheet sheet;
	WindowTexture wt(&sheet, 11, 11, 11, 11);
	sheet.Generate(assets, assets->GetTexture(light ? "window.light" : "window.dark"));
	Texture *generated = wt.Generate(assets, size);

	Texture *auth_field = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
												SDL_TEXTUREACCESS_TARGET,
												(int)size.x, (int)size.y);
	drawing->SetRenderTarget(auth_field);
	drawing->SetColor(64, 104, 153, 255);
	drawing->Clear();

	generated->SetBlendMode(SDL_BLENDMODE_MUL);
	generated->SetAlphaMod(128);
	drawing->RenderTextureFullscreen(generated->SDLTexture(), nullptr);

	return auth_field;
}

static PregenerateTexture sTextureHeader("auth_menu_header", [](AssetsClass *assets) -> Texture *
{
	auto header_texture = assets->GetTexture("header");
	return header_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET)
		->SetColorMod(94, 152, 224);
});
static LinkTexture sTextureCard("auth.card");
static LinkTexture sTextureDoodles("doodles");
static LinkTexture sTextureTitle("title");

static PregenerateTexture sTextureAuthField
	("auth.field", [](AssetsClass *a) -> Texture *
	{ return sGenerateAuthField(a, Vec2f(205, 35), false); });
static PregenerateTexture sTextureAuthButton
	("auth.button", [](AssetsClass *a) -> Texture *
	{ return sGenerateAuthField(a, Vec2f(100, 40), true); });

}

AuthMenu::AuthMenu()
	: FullscreenMenu()
{
	name = L"AuthMenu";

	loading_screen = new LoadingScreen();

	// Header
	header = (Frame *)(new Frame())
		->SetSize(Vec2i(0, 58))
		->SetTexture(Auth::sTextureHeader.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFullyOccupy(true, false)
		->SetName("Header");

	// Title
	title = (Frame *)(new Frame())
		->SetSize(Vec2i(374, 82))
		->SetTexture(Auth::sTextureTitle.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Title");

	// Login
	subtitle = (TextElement *)(new TextElement())
		->SetRelative(Vec2i(0, -5))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Subtitle");

	username_field = (TextBox *)(new TextBox())
		->SetCallback([](std::string& text)
					  {
						  std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
					  })
		->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
		->SetPlaceholder("Username")
		->SetTextColor({ 150, 255, 0, 255 })
		->SetPlaceholderColor({ 150, 255, 0, 255 })
		->SetMaxLength(16)
		->SetFont(CommonUI::sFontRegular.GetFont()->TTFFont())
		->SetSize(Vec2i(205, 35))
		->SetTexture(Auth::sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Field");

	password_field = (TextBox *)(new TextBox())
		->SetCallback([](std::string& text)
					  {
						  std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
					  })
		->SetFilter([](const std::string& input_text) -> std::string
					{
						return std::string(input_text.length(), '*');
					})
		->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
		->SetPlaceholder("Password")
		->SetTextColor({ 255, 100, 0, 255 })
		->SetPlaceholderColor({ 255, 100, 0, 255 })
		->SetMaxLength(16)
		->SetFont(CommonUI::sFontRegular.GetFont()->TTFFont())
		->SetSize(Vec2i(205, 35))
		->SetTexture(Auth::sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Field");

	auto login_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontRegular.GetTTFFont(), "Submit", 1,
							{ 255, 255, 255, 255 }, { 0, 0, 0, 64 })
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("ButtonLabel");

	auto login_button = (Button *)(new Button(Auth::sTextureAuthButton.GetTexture(), nullptr))
		->SetSize(Vec2i(100, 30))
		->SetName("Submit")
		->AddChildren({ login_label });
	login_button->SetCallback([this, login_button]()
							  {
								  auth_frame->SetFocus(login_button);
								  AttemptLogin();
							  });

	auto goto_register_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontRegular.GetTTFFont(), "Register", 1,
							{ 255, 255, 255, 255 }, { 0, 0, 0, 64 })
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("ButtonLabel");
	auto goto_register_button = (Button *)(new Button(Auth::sTextureAuthButton.GetTexture(), nullptr))
		->SetSize(Vec2i(100, 30))
		->SetName("Register")
		->AddChildren({ goto_register_label });
	goto_register_button->SetCallback([this]()
									  { this->OpenRegisterScreen(); });

	login_buttons_frame = (Frame *)(new Frame())
		->SetFlex(Flex::WIDTH, 5)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetAdaptive(true, true)
		->SetName("ButtonsFrame")
		->AddChildren({ login_button, goto_register_button });

	// Register
	nickname_field = (TextBox *)(new TextBox())
		->SetCallback([](std::string& text)
					  {
						  std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
					  })
		->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
		->SetPlaceholder("Nickname")
		->SetTextColor({ 150, 255, 0, 255 })
		->SetPlaceholderColor({ 150, 255, 0, 255 })
		->SetMaxLength(24)
		->SetFont(CommonUI::sFontRegular.GetFont()->TTFFont())
		->SetSize(Vec2i(205, 35))
		->SetTexture(Auth::sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Field");

	email_field = (TextBox *)(new TextBox())
		->SetCallback([](std::string& text)
					  {
						  std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
					  })
		->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
		->SetPlaceholder("email@domain.com")
		->SetTextColor({ 150, 255, 0, 255 })
		->SetPlaceholderColor({ 150, 255, 0, 255 })
		->SetMaxLength(64)
		->SetFont(CommonUI::sFontRegular.GetFont()->TTFFont())
		->SetSize(Vec2i(205, 35))
		->SetTexture(Auth::sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Field");

	confirmation_field = (TextBox *)(new TextBox())
		->SetCallback([](std::string& text)
					  {
						  std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
					  })
		->SetFilter([](const std::string& input_text) -> std::string
					{
						return std::string(input_text.length(), '*');
					})
		->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
		->SetPlaceholder("Confirm Password")
		->SetTextColor({ 255, 100, 0, 255 })
		->SetPlaceholderColor({ 255, 100, 0, 255 })
		->SetMaxLength(16)
		->SetFont(CommonUI::sFontRegular.GetFont()->TTFFont())
		->SetSize(Vec2i(205, 35))
		->SetTexture(Auth::sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Field");

	auto register_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontRegular.GetTTFFont(), "Submit", 1,
							{ 255, 255, 255, 255 }, { 0, 0, 0, 64 })
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("ButtonLabel");

	auto register_button = (Button *)(new Button(Auth::sTextureAuthButton.GetTexture(), nullptr))
		->SetSize(Vec2i(100, 30))
		->SetName("Register")
		->AddChildren({ register_label });
	register_button->SetCallback([this, register_button]()
								 {
									 auth_frame->SetFocus(register_button);
									 AttemptRegister();
								 });

	auto goto_login_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontRegular.GetTTFFont(), "Back", 1,
							{ 255, 255, 255, 255 }, { 0, 0, 0, 64 })
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("ButtonLabel");
	auto goto_login_button = (Button *)(new Button(Auth::sTextureAuthButton.GetTexture(), nullptr))
		->SetSize(Vec2i(100, 30))
		->SetName("Login")
		->AddChildren({ goto_login_label });
	goto_login_button->SetCallback([this]()
								   { this->OpenLoginScreen(); });

	register_buttons_frame = (Frame *)(new Frame())
		->SetFlex(Flex::WIDTH, 5)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetAdaptive(true, true)
		->SetName("ButtonsFrame")
		->AddChildren({ register_button, goto_login_button });

	// Background
	auto background = (new Auth::ScrollingBackground())
		->SetFullyOccupy(true, true)
		->SetFlexInvolved(false, false)
		->SetTexture(Auth::sTextureDoodles.GetTexture()->SetAlphaMod(75))
		->SetName("Background");

	// Main auth frame
	response_message = "";
	response = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontRegular.GetFont()->TTFFont(),
					 "No Error",
					 { 200, 0, 0, 255 })
		->SetFlexInvolved(false, false)
		->SetDraw(DONT_DRAW)
		->SetRelative(Vec2i(0, -5))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Error");

	auth_frame = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT, 5)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetName("AuthFrame");

	auto auth_card = (new Frame())
		->SetSize(Vec2i(427, 309))
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(Auth::sTextureCard.GetTexture())
		->SetName("Card")
		->AddChildren({ auth_frame });

	// Content
	auto content = (new Frame())
		->SetFullyOccupy(true, false)
		->SetOccupy(false, true)
		->AddChildren({ background, auth_card })
		->SetName("Content");

	this->SetColor(94, 152, 224, 255);
	this->SetFlex(Flex::HEIGHT);
	this->SetName("AuthMenu");
	this->AddChildren({ header, content, loading_screen });
	this->RefreshMenu();

	OpenLoginScreen();
}

AuthMenu::~AuthMenu()
{
	auth_frame->SetChildren({ });
}

void AuthMenu::OpenLoginScreen()
{
	this->SetColor(94, 152, 224, 255);
	header->GetVisualTexture().GetTexture()->SetColorMod(100, 190, 255);
	subtitle->UpdateText(CommonUI::sFontRegular.GetFont()->TTFFont(),
						 "Enter your login and START DRAWING!",
						 { 255, 255, 255, 255 });

	auth_frame->SetChildren({ title, subtitle,
							  username_field, password_field,
							  response,
							  login_buttons_frame });
	this->RefreshMenu();
}

void AuthMenu::OpenRegisterScreen()
{
//	this->SetColor(183, 0, 211, 255);
//	header->GetVisualTexture().GetTexture()->SetColorMod(183, 0, 211);
	subtitle->UpdateText(CommonUI::sFontRegular.GetFont()->TTFFont(),
						 "Create an account to be found by others!",
						 { 255, 255, 255, 255 });

	auth_frame->SetChildren({ subtitle,
							  username_field, nickname_field, email_field,
							  password_field, confirmation_field,
							  response,
							  register_buttons_frame });
	this->RefreshMenu();
}

void AuthMenu::SetResponse(const std::string& message, bool success_color)
{
	response_message = message;
	if (message.empty())
	{
		response->SetFlexInvolved(false, false)
			->SetDraw(DONT_DRAW);
	}
	else
	{
		response->UpdateTextOutline(CommonUI::sFontRegular.GetFont()->TTFFont(),
									message.c_str(), 1,
									success_color ? SDL_Color(0, 255, 0, 255) : SDL_Color(255, 0, 0, 255),
									success_color ? SDL_Color(0, 50, 0, 255) : SDL_Color(50, 0, 0, 255))
			->SetFlexInvolved(true, true)
			->SetDraw(DRAW_TEXTURE);
	}
}

void AuthMenu::SetLoginDetails(const std::string& username,
							   const std::string& password)
{
	username_field->SetText(username);
	password_field->SetText(password);
}

void AuthMenu::SetRegisterDetails(const std::string& username,
								  const std::string& nickname,
								  const std::string& email,
								  const std::string& password,
								  const std::string& confirmation)
{
	username_field->SetText(username);
	nickname_field->SetText(nickname);
	email_field->SetText(email);
	password_field->SetText(password);
	confirmation_field->SetText(confirmation);
}

void AuthMenu::AttemptLogin()
{
	loading_screen->StartShowing("Getting you in.. right now");

	json login_data;
	login_data["username"] = username_field->GetText().GetText();
	login_data["password"] = password_field->GetText().GetText();
	auto packet = (new Packet("/login", "POST", &login_data));
	packet->SetErroredCallback([this, packet]()
							   {
								   loading_screen->Hide();
								   SetResponse(packet->ErrorReason(), false);
								   RefreshMenu();
								   delete packet;
							   });
	packet->SetResponseCallback([packet](const NetworkResponse& server_response)
								{
									std::string code = server_response.GetCode();
									std::string message = server_response.GetMsg();

									if (code == "login_success")
									{
										Centralized.auth_menu->GetLoadingScreen()->Hide();
										Centralized.auth_menu->SetResponse(message, true);
										Centralized.auth_menu->RefreshMenu();

										Centralized.current_menu = (FullscreenMenu *)Centralized.main_menu;

										std::string session_key = server_response.response_json.value("session_key", "no_key_provided");
										std::string expires_at = server_response.response_json.value("expires_at", "no_date_provided");

										NetworkClient::SetSessionKey(session_key);
										if (server_response.response_json.contains("user") && server_response.response_json["user"].is_object())
										{
											json user_data = server_response.response_json["user"];
											auto account = Centralized.GetAccount().ParseFromJson(user_data);
											Centralized.main_menu->Header()->RefreshData();
										}

										if (server_response.response_json.contains("games") && server_response.response_json["games"].is_array())
										{
											Centralized.ClearGames();
											for (const json& game_data : server_response.response_json["games"])
												if (game_data.is_object())
												{
													auto add_game = (new GameInfo())->ParseFromJson(game_data, false);
													Centralized.AddGame(add_game);
												}

											Centralized.SortGames();
										}

										Centralized.main_menu->Profile()->RefreshData();
										Centralized.main_menu->Games()->RefreshData();
										Centralized.main_menu->RefreshMenu();

										dbg_msg("&d[SERVER Response]&r Logged in: %s &8(until %s, %s)\n",
												message.c_str(), expires_at.c_str(), session_key.c_str());
									}
									else if (code == "login_fail")
									{
										Centralized.auth_menu->GetLoadingScreen()->Hide();
										Centralized.auth_menu->SetResponse(message, false);
										Centralized.auth_menu->RefreshMenu();

										dbg_msg("&d[SERVER Response]&r Invalid login: %s\n", message.c_str());
									}

									delete packet;
								});
	packet->Send();
}

void AuthMenu::AttemptRegister()
{
	loading_screen->StartShowing("One moment.. Creating your account :)");

	json register_data;
	register_data["username"] = username_field->GetText().GetText();
	register_data["password"] = password_field->GetText().GetText();
	register_data["confirmation"] = confirmation_field->GetText().GetText();
	register_data["display_name"] = nickname_field->GetText().GetText();
	register_data["email"] = email_field->GetText().GetText();
	auto packet = (new Packet("/register", "POST", &register_data));
	packet->SetErroredCallback([this, packet]()
							   {
								   loading_screen->Hide();
								   SetResponse(packet->ErrorReason(), false);
								   RefreshMenu();
								   delete packet;
							   });
	packet->SetResponseCallback([packet](const NetworkResponse& server_response)
								{
									std::string code = server_response.GetCode();
									std::string message = server_response.GetMsg();

									if (code == "register_fail")
									{
										Centralized.auth_menu->GetLoadingScreen()->Hide();
										Centralized.auth_menu->SetResponse(message, false);
										Centralized.auth_menu->RefreshMenu();

										dbg_msg("&d[SERVER Response]&r Registration failed: %s\n", message.c_str());
									}
									else if (code == "register_success")
									{
										Centralized.auth_menu->GetLoadingScreen()->Hide();
										Centralized.auth_menu->SetResponse(message, true);
										Centralized.auth_menu->OpenLoginScreen();

										dbg_msg("&d[SERVER Response]&r Registration successful: %s\n", message.c_str());
									}

									delete packet;
								});
	packet->Send();
}