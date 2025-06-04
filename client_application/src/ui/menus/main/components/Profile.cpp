//
// Created by Matq on 26/01/2025.
//

#include "Profile.h"
#include "ui/components/element/TextElement.h"
#include "ui/CommonUI.h"
#include "game/GameData.h"
#include "ui/components/element/Button.h"
#include "ui/RenderPresets.h"
#include "ui/menus/main/MainMenu.h"
#include "ui/menus/auth/AuthMenu.h"

namespace Main
{
static LinkTexture sTextureBase("main_menu.profile.base");
static LinkTexture sTextureBottomBase("main_menu.profile.bottom");
static LinkTexture sTexturePictureFrame("main_menu.profile.picture_frame");
static LinkTexture sTextureDefaultProfilePicture("icons.icon_white");
static LinkTexture sTextureProfilePictureFrame("main_menu.games.picture_frame");
static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");

static LinkTexture sTextureTopBase("main_menu.profile.top_base");
static LinkTexture sTextureStyles[4] =
	{
		LinkTexture("main_menu.profile.lines"),
		LinkTexture("main_menu.profile.squiggles"),
		LinkTexture("main_menu.profile.more_lines"),
		LinkTexture("main_menu.profile.scribbles"),
	};

static LinkTexture sTextureEdit("main_menu.profile.edit_button");

void Profile::SetProfilePicture(Texture *texture_data)
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

Profile::Profile()
	: Frame()
{
	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	profile_picture_texture = nullptr;

	auto random_style = sTextureStyles[rand() % 4];
	top_base_composition = sTextureTopBase.GetTexture()->CopyTexture(SDL_TEXTUREACCESS_TARGET);
	top_base_composition->SetColorMod(0, 140, 100);
	drawing->SetRenderTarget(top_base_composition);
	random_style.GetTexture()->SetAlphaMod(43);
	drawing->RenderTextureFullscreen(random_style.GetSDLTexture(), nullptr);
	drawing->SetRenderTarget(nullptr);

	profile_picture = (Frame *)(new Frame())
		->SetSize(Vec2i(141, 141))
		->SetTexture(profile_picture_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetName("ProfilePicture");

	profile_nickname = (TextElement *)(new TextElement())->SetAlign(Align::CENTER, Align::DONT);
	profile_username = (TextElement *)(new TextElement())->SetAlign(Align::CENTER, Align::DONT);
	profile_email = (TextElement *)(new TextElement())->SetAlign(Align::CENTER, Align::DONT);
	is_administrator = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 "Administrator",
					 { 200, 50, 200, 255 })
		->SetEnabled(false)
		->SetAlign(Align::CENTER, Align::DONT);

	auto profile_info_frame = (new Frame())
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT)
		->SetName("ProfileInfoFrame")
		->AddChildren({ profile_nickname, profile_username, profile_email, is_administrator }); // edit_button

	auto profile_info = (new Frame())
		->SetSize(Vec2i(202, 141))
		->SetTexture(top_base_composition)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("ProfileInfo")
		->AddChildren({ profile_info_frame });

	auto profile_frame = (new Frame())
		->SetRelative(Vec2i(27, 68 - 39))
		->SetFlex(Flex::WIDTH, 3)
		->SetAdaptive(false, true)
		->SetName("ProfileFrame")
		->AddChildren({ profile_picture, profile_info });

	auto stats_title = (new TextElement())
		->UpdateTextOutline(CommonUI::sFontMediumBigger.GetTTFFont(), "Game Statistics", 2,
							{ 255, 255, 255, 255 }, { 0, 0, 0, 255 })
		->SetRelative(Vec2i(18, 12))
		->SetName("StatsTitle");

	games_played = (TextElement *)(new TextElement())->SetName("GamesPlayed");
	rounds_played = (TextElement *)(new TextElement())->SetName("RoundsPlayed");
	highest_turn = (TextElement *)(new TextElement())->SetName("HighestTurn");
	coins_earned = (TextElement *)(new TextElement())->SetName("CoinsEarned");
	coins_spent = (TextElement *)(new TextElement())->SetName("CoinsSpent");

	auto gamestats_frame = (new Frame())
		->SetSize(Vec2i(300, 148))
		->SetRelative(Vec2i(20 + 5, 32 + 19 + 8))
		->SetFlex(Flex::HEIGHT)
		->SetName("GameStatsFrame")
		->AddChildren({ games_played, rounds_played, highest_turn, coins_earned, coins_spent });

	auto logout_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Logout", 2,
														{ 255, 255, 255, 255 },
														{ 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto logout_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
															{ 255, 0, 0 },
															sTextureButtonOutline.GetTexture(), logout_text)
		->FlagForAutomaticDeletion();
	auto logout_button = (Button *)(new Button(logout_button_texture,
											   logout_button_texture))
		->SetRelative(Vec2i(-30, -30))
		->SetSize(Vec2i(logout_button_texture->GetOriginalSize() / 2))
		->SetFlexInvolved(false, false)
		->SetAlign(Align::RIGHT, Align::BOTTOM)
		->SetName("Logout");
	logout_button->SetCallback([]()
							   {
								   Centralized.main_menu->GetLoadingScreen()->StartShowing("Logging out..");

								   auto packet = (new Packet("/logout", "POST"));
								   packet->SetErroredCallback([packet]()
															  { delete packet; });
								   packet->SetResponseCallback([packet](const NetworkResponse& server_response)
															   {
																   Centralized.main_menu->GetLoadingScreen()->Hide();

																   Centralized.GetAccount().Logout();
																   Centralized.current_menu = (FullscreenMenu *)Centralized.auth_menu;
																   Centralized.auth_menu->SetResponse("Logged out successfully", true);
																   Centralized.auth_menu->RefreshMenu();

																   delete packet;
															   });
								   packet->Send();
							   });

	auto stats_frame = (new Frame())
		->SetSize(Vec2i(352, 237))
		->SetRelative(Vec2i(26, 207))
		->SetTexture(sTextureBottomBase.GetTexture()->SetColorMod(0, 149, 100))
		->SetDraw(DRAW_TEXTURE)
		->SetName("StatsFrame")
		->AddChildren({ stats_title, gamestats_frame, logout_button });

	SetProfilePicture(nullptr);

	this->SetRelative(Vec2i(-25, 100 - 58 - 17));
	this->SetSize(Vec2i(501 * 0.8, 586 * 0.8));
	this->SetTexture(sTextureBase.GetTexture());
	this->SetDraw(DRAW_TEXTURE);
	this->SetAlign(Align::RIGHT, Align::DONT);
	this->SetName("Profile");
	this->AddChildren({ profile_frame, stats_frame });
}

Profile::~Profile()
{
	delete profile_picture_texture;
	delete top_base_composition;
}

void Profile::RefreshData()
{
	auto account = Centralized.GetAccount();
	profile_nickname->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
								 Strings::FString("%s", account.nickname.c_str()).c_str(),
								 { 75, 175, 255, 255 });
	profile_username->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
								 Strings::FString("%s", account.username.c_str()).c_str(),
								 { 50, 175, 175, 255 });
	profile_email->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							  Strings::FString("%s", account.email.c_str()).c_str(),
							  { 230, 230, 230, 255 });
	is_administrator->SetEnabled(account.is_administrator);

	games_played->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							 Strings::FString("Games played: %d", account.games_played).c_str(),
							 { 230, 230, 230, 255 });
	rounds_played->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							  Strings::FString("Rounds played: %d", account.rounds_played).c_str(),
							  { 230, 230, 230, 255 });
	highest_turn->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							 Strings::FString("Highest turn: %d", account.highest_turn).c_str(),
							 { 230, 230, 230, 255 });
	coins_earned->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							 Strings::FString("Coins earned: %d", account.coins_earned).c_str(),
							 { 200, 255, 200, 255 });
	coins_spent->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
							Strings::FString("Coins spent: %d", account.coins_spent).c_str(),
							{ 255, 200, 200, 255 });

	SetProfilePicture(nullptr); // waiting
}
}
