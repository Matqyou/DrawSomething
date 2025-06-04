//
// Created by Matq on 23/05/2025.
//

#include "ProfileScreen.h"
#include "ui/CommonUI.h"
#include "ui/structures/window_texture/Sheet.h"
#include "ui/structures/window_texture/WindowTexture.h"
#include "game/GameData.h"
#include "ui/components/element/Button.h"
#include "ui/RenderPresets.h"
#include "ui/components/element/TextElement.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/menus/auth/AuthMenu.h"
#include "windows_system/WindowsSystem.h"

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
	drawing->SetColor(0, 150, 175, 255);
	drawing->Clear();

	generated->SetBlendMode(SDL_BLENDMODE_MUL);
	generated->SetAlphaMod(128);
	drawing->RenderTextureFullscreen(generated->SDLTexture(), nullptr);

	return auth_field;
}

static PregenerateTexture sTextureAuthField
	("main_menu.profile_menu.field", [](AssetsClass *a) -> Texture *
	{ return sGenerateAuthField(a, Vec2f(250, 35), false); });
static PregenerateTexture sTextureAuthButton
	("main_menu.profile_menu.button", [](AssetsClass *a) -> Texture *
	{ return sGenerateAuthField(a, Vec2f(250, 40), true); });

static LinkTexture sTextureCard("main_menu.profile_menu.card");
static LinkTexture sTextureCardOutline("main_menu.profile_menu.card_outline");
static LinkTexture sTextureTitle("main_menu.profile_menu.profile_information");
static LinkTexture sTextureDefaultProfilePicture("icons.icon_white");
static LinkTexture sTextureProfilePictureFrame("main_menu.games.picture_frame");
static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");
static LinkTexture sTextureClose("main_menu.settings.x");
static LinkTexture sTextureUpload("main_menu.header.upload");

ProfileScreen::ProfileScreen()
	: ScreenMenu()
{
	profile_picture_texture = nullptr;

	auto profile_information = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 10))
		->SetSize(Vec2i(sTextureTitle.GetTexture()->GetOriginalSize() / 3))
		->SetTexture(sTextureTitle.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("ProfileInformation");

	Button *upload_button = (Button *)(new Button())
		->SetRelative(Vec2i(-5, -5))
		->SetSize(Vec2i(40, 40))
		->SetTexture(sTextureUpload.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::RIGHT, Align::BOTTOM)
		->SetName("Upload");
	upload_button->SetCallback([this]()
							   {
								   auto png_path = WindowsSystem::OpenPngFileDialog();
								   if (png_path.empty())
									   return;

								   auto sdl_surface = IMG_Load(png_path.c_str());
								   if (!sdl_surface)
								   {
									   dbg_msg("&cFailed to load selected image: %s\n", png_path.c_str());
									   dbg_msg("&cReason: %s\n", SDL_GetError());
									   return;
								   }

								   auto texture = Assets::Get()->TextureFromSurface(sdl_surface)
									   ->FlagForAutomaticDeletion();
								   SDL_DestroySurface(sdl_surface);

								   SetProfilePicture(texture);
								   Centralized.main_menu->Profile()->SetProfilePicture(texture);

								   Packet *packet = new Packet("/upload_profile_picture", "POST");
								   packet->Send(false, png_path);
							   });

	profile_picture = (Frame *)(new Frame())
		->SetSize(Vec2i(230, 230))
		->SetTexture(nullptr)
		->SetDraw(DRAW_TEXTURE)
		->SetName("ProfilePicture")
		->AddChildren({ upload_button });

	TextElement *nickname_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), "New Display Name", 2,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetName("Label");
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
		->SetSize(Vec2i(250, 35))
		->SetTexture(sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Field");

	TextElement *username_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), "New Username", 2,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetName("Label");
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
		->SetSize(Vec2i(250, 35))
		->SetTexture(sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::RIGHT, Align::DONT)
		->SetName("Field");

	Frame *nickname_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(20, 0))
		->SetFlex(Flex::HEIGHT, 1)
		->SetAdaptive(true, true)
		->SetName("Confirmation")
		->AddChildren({ nickname_label, nickname_field });
	Frame *username_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(20, 0))
		->SetFlex(Flex::HEIGHT, 1)
		->SetAdaptive(true, true)
		->SetName("Username")
		->AddChildren({ username_label, username_field });

	Frame *left = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT, 5)
		->SetAdaptive(true, true)
		->SetName("Left")
		->AddChildren({ profile_picture, nickname_frame, username_frame });

	TextElement *email_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), "New Email", 2,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetName("Label");
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
		->SetSize(Vec2i(250, 35))
		->SetTexture(sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::RIGHT, Align::DONT)
		->SetName("Field");

	TextElement *password_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), "New Password", 2,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetName("Label");
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
		->SetSize(Vec2i(250, 35))
		->SetTexture(sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::RIGHT, Align::DONT)
		->SetName("Field");

	TextElement *confirmation_label = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), "Password Confirmation", 2,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetName("Label");
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
		->SetSize(Vec2i(250, 35))
		->SetTexture(sTextureAuthField.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::RIGHT, Align::DONT)
		->SetName("Field");

	Frame *email_frame = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT, 1)
		->SetAdaptive(true, true)
		->SetName("Email")
		->AddChildren({ email_label, email_field });
	Frame *password_frame = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT, 1)
		->SetAdaptive(true, true)
		->SetName("Password")
		->AddChildren({ password_label, password_field });
	Frame *confirmation_frame = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT, 1)
		->SetAdaptive(true, true)
		->SetName("Confirmation")
		->AddChildren({ confirmation_label, confirmation_field });

	Frame *right = (Frame *)(new Frame())
		->SetRelative(Vec2i(20, 0))
		->SetSize(Vec2i(425, 0))
		->SetFlex(Flex::HEIGHT, 5)
		->SetAdaptive(false, true)
		->SetName("Right")
		->AddChildren({ email_frame, password_frame, confirmation_frame });

	Frame *content = (Frame *)(new Frame())
		->SetRelative(Vec2i(20, 10))
		->SetOccupy(false, true)
		->SetFullyOccupy(true, false)
		->SetFlex(Flex::WIDTH, 5)
		->SetName("Content")
		->AddChildren({ left, right });

	auto assets = Assets::Get();
	auto delete_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Delete", 2,
														{ 255, 255, 255, 255 },
														{ 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto delete_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
															{ 255, 0, 0 },
															sTextureButtonOutline.GetTexture(), delete_text)
		->FlagForAutomaticDeletion();
	auto save_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Save", 2,
													  { 255, 255, 255, 255 },
													  { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto save_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														  { 100, 255, 100 },
														  sTextureButtonOutline.GetTexture(), save_text)
		->FlagForAutomaticDeletion();

	Button *delete_button = (Button *)(new Button())
		->SetRelative(Vec2i(-15 - 100, -15))
		->SetSize(Vec2i(delete_button_texture->GetOriginalSize() / 2))
		->SetTexture(delete_button_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetFlexInvolved(false, false)
		->SetAlign(Align::BOTTOM, Align::RIGHT)
		->SetName("Delete");
	delete_button->SetCallback([]()
							   {
								   ConfirmationScreen *confirmation_screen = Centralized.main_menu->GetConfirmationScreen();
								   confirmation_screen->SetAgreeCallback([confirmation_screen]()
																		 {
																			 auto packet = new Packet("/delete_profile", "POST");
																			 packet->Send();
																			 packet->SetErroredCallback([packet]()
																										{ delete packet; });
																			 packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																										 {
																											 std::string message = server_response.GetMsg();

																											 Centralized.auth_menu->SetResponse(message, true);
																											 Centralized.current_menu = (FullscreenMenu *)Centralized.auth_menu;
																											 Centralized.current_menu->RefreshMenu();
																											 delete packet;
																										 });

																			 confirmation_screen->SetEnabled(false);
																		 });

								   confirmation_screen->Prompt("You want to delete your account?");
							   });

	Button *save_button = (Button *)(new Button())
		->SetRelative(Vec2i(-15, -15))
		->SetSize(Vec2i(save_button_texture->GetOriginalSize() / 2))
		->SetTexture(save_button_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetFlexInvolved(false, false)
		->SetAlign(Align::BOTTOM, Align::RIGHT)
		->SetName("Save");
	save_button->SetCallback([this]()
							 {
//								 loading_screen->StartShowing("One moment.. Creating your account :)");
								 SetResponse("", true);

								 json profile_data;
								 profile_data["username"] = username_field->GetText().GetText();
								 profile_data["password"] = password_field->GetText().GetText();
								 profile_data["confirmation"] = confirmation_field->GetText().GetText();
								 profile_data["display_name"] = nickname_field->GetText().GetText();
								 profile_data["email"] = email_field->GetText().GetText();
								 auto packet = new Packet("/edit_profile", "POST", &profile_data);
								 packet->SetErroredCallback([this, packet]()
															{
																SetResponse(packet->ErrorReason(), false);
																Refresh();
																delete packet;
															});
								 packet->SetResponseCallback([this, packet](const NetworkResponse& server_response)
															 {
																 std::string message = server_response.GetMsg();

																 SetResponse(message, server_response.success_evaluation);
																 if (server_response.success_evaluation && server_response.response_json.contains("user") && server_response.response_json["user"].is_object())
																 {
																	 json user_data = server_response.response_json["user"];
																	 auto account = Centralized.GetAccount().ParseFromJson(user_data);

																	 Centralized.main_menu->Header()->RefreshData();
																	 Centralized.main_menu->Profile()->RefreshData();
//																	 Centralized.main_menu->Games()->RefreshData();
																	 RefreshData();
																 }

																 Centralized.main_menu->RefreshMenu();
																 delete packet;
															 });
								 packet->Send();
							 });

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

	response = (TextElement *)(new TextElement())
		->SetRelative(Vec2i(-15, -15 - 45))
		->SetAlign(Align::BOTTOM, Align::RIGHT)
		->SetFlexInvolved(false, false)
		->SetDraw(DONT_DRAW)
		->SetName("Response");

	auto card_texture = RenderPresets::ColorButton(sTextureCard.GetTexture(),
												   { 0, 255, 255, 255 },
												   sTextureCardOutline.GetTexture());
	Frame *card = (Frame *)(new Frame())
		->SetSize(Vec2i(card_texture->GetOriginalSize() / 2))
		->SetTexture(card_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT, 5)
		->SetName("Card")
		->AddChildren({ profile_information, content, delete_button, save_button, close_button, response });

	SetProfilePicture(nullptr);

	this->SetEnabled(false);
	this->SetColor(0, 0, 0, 200);
	this->SetDraw(DRAW_RECT);
	this->SetFullyOccupy(true, true);
	this->SetFlex(Flex::HEIGHT);
	this->SetName("ProfileMenu");
	this->AddChildren({ card });
}

ProfileScreen::~ProfileScreen()
{
	delete profile_picture_texture;
}

void ProfileScreen::RefreshData()
{
	auto account = Centralized.GetAccount();
	nickname_field->SetText(account.nickname);

	username_field->SetText(account.username);
	email_field->SetText(account.email);
	password_field->SetText("");
	confirmation_field->SetText("");

	SetProfilePicture(account.profile_picture);
}

void ProfileScreen::SetProfilePicture(Texture *texture_data)
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
}

void ProfileScreen::SetResponse(const std::string& message, bool success_color)
{
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
