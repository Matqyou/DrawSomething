//
// Created by Matq on 22/01/2025.
//

#include "IntermissionMenu.h"
#include "core/Application.h"
#include "ui/components/element/Button.h"
#include "ui/CommonUI.h"
#include "game/GameData.h"

namespace Intermission
{
static Texture *sGenerateContinueButton_(AssetsClass *assets, Texture *button_base)
{
	if (!button_base->UsesHitbox()) throw std::runtime_error("sGenerateContinueButton_() button_base does not use visual hitbox");

	auto drawing = assets->GetDrawing();

	auto continue_text = assets->RenderTextBlended(CommonUI::sFontGiant1.GetFont()->TTFFont(),
												   "continue",
												   { 255, 255, 255, 255 });
	const auto& button_size = button_base->GetOriginalSize();
	auto continue_button = new VisualTexture(SDL_CreateTexture(drawing->Renderer(),
															   SDL_PIXELFORMAT_RGBA8888,
															   SDL_TEXTUREACCESS_TARGET,
															   (int)button_size.x,
															   (int)button_size.y),
											 ((VisualTexture *)(button_base))->GetHitbox());
	drawing->SetRenderTarget(continue_button);
	drawing->RenderTextureFullscreen(button_base->SDLTexture(), nullptr);
	auto text_rect = Rectangles::CenterRelative(continue_text->GetOriginalSize(), Vec2f(386, 112));
	drawing->RenderTexture(continue_text->SDLTexture(), nullptr, text_rect);
	delete continue_text;

	return continue_button;
}
static Texture *sGenerateContinueButton(AssetsClass *assets)
{
	return sGenerateContinueButton_(assets, assets->GetTexture("button2"));
}
static Texture *sGenerateContinueButtonPressed(AssetsClass *assets)
{
	return sGenerateContinueButton_(assets, assets->GetTexture("button_pressed"));
}

static LinkTexture sTextureHeader("intermission.header.background");
static LinkTexture sTextureTurn("intermission.turn");
static LinkTexture sTextureCoin("intermission.coin");
static LinkTexture sTextureButton("button2");
static LinkTexture sTextureButtonPressed("button_pressed");
static PregenerateTexture sTextureContinueButton("intermission.continue_button", sGenerateContinueButton);
static PregenerateTexture
	sTextureContinueButtonPressed("intermission.continue_button_pressed", sGenerateContinueButtonPressed);

}

IntermissionMenu::IntermissionMenu()
	: FullscreenMenu()
{
	name = L"IntermissionMenu";
	end_animation_callback = { };
	countdown_started = false;
	auto assets = Assets::Get();

	// New record text
	auto new_record = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontBiggest.GetFont()->TTFFont(),
					 "NEW RECORD!",
					 { 0, 0, 0, 255 })
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("NewRecordText");


	// New record
	auto bar = (new Frame())
		->SetRelative(Vec2i(0, 100))
		->SetSize(Vec2i(0, 68))
		->SetTexture(Intermission::sTextureHeader.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFullyOccupy(true, false)
		->SetFlexInvolved(false, false)
		->SetName("NewRecordBar")
		->AddChildren({ new_record });

	// Top Frame
	auto top_frame = (new Frame())
		->SetSize(Vec2i(0, 175))
		->SetFullyOccupy(true, false)
		->SetName("TopFrame")
		->AddChildren({ bar });

	auto turn_text = (new TextElement())
		->UpdateText(CommonUI::sFontGiant1.GetFont()->TTFFont(),
					 "turn",
					 { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, 10))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("TurnText");

	text_turn_number = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontGiant2.GetFont()->TTFFont(),
					 "NaN", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, -10))
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("TurnNumber");

	auto turn_text_frame = (new Frame())
		->SetFlex(Flex::HEIGHT)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetName("TurnTextFrame")
		->AddChildren({ turn_text, text_turn_number });

	auto turn = (new Frame())
		->SetSize(Vec2i(295 * 0.9, 295 * 0.9))
		->SetTexture(Intermission::sTextureTurn.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetName("Turn")
		->AddChildren({ turn_text_frame });

	auto continue_button = (Button *)(new Button(Intermission::sTextureContinueButton.GetTexture(),
												 Intermission::sTextureContinueButtonPressed.GetTexture()))
		->SetSize(Vec2i(193, 56))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("ContinueButton");
	continue_button->SetCallback([this]()
								 {
									 this->menu_a->SetEnabled(false);
									 this->menu_b->SetEnabled(true);
									 this->BeginAnimation();
									 this->RefreshMenu();
								 });

	auto turn_frame = (new Frame())
		->SetFlex(Flex::HEIGHT, 85)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetName("TurnFrame")
		->AddChildren({ turn, continue_button });

	auto bottom_frame = (new Frame())
		->SetFullyOccupy(true, false)
		->SetOccupy(false, true)
		->SetName("BottomFrame")
		->AddChildren({ turn_frame });

	menu_a = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT)
		->SetFullyOccupy(true, true)
		->SetName("MenuA")
		->AddChildren({ top_frame, bottom_frame })
		->SetEnabled(true);

	// B: Mini title text
	auto mini_title_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontMiniProfile.GetFont()->TTFFont(),
					 "The drawing was guessed correctly!",
					 { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("TitleText");

	// B: Title text
	auto title_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontBiggest.GetFont()->TTFFont(),
					 "VERI AMAZINK",
					 { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("TitleText");

	// B: Title frame
	auto title = (new Frame())
		->SetSize(Vec2i(150, 68))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->SetName("TitleFrame")
		->AddChildren({ mini_title_text, title_text });

	mini_profile1 = new Intermission::MiniProfile();
	mini_profile2 = new Intermission::MiniProfile();

	auto mini_profiles = (Frame *)(new Frame())
		->SetFlex(Flex::WIDTH, 75)
		->SetAdaptive(true, true)
		->SetName("MiniProfiles")
		->AddChildren({ mini_profile1, mini_profile2 });

	coins_earned_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "COINS EARNED:", { 200, 240, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("CoinsEarnedText");

	auto coin = (Frame *)(new Frame())
		->SetSize(Vec2i(36, 36))
		->SetTexture(Intermission::sTextureCoin.GetTexture())
		->SetDraw(DRAW_TEXTURE);
	auto coin_text = (new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 "+1", { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT);
	auto coin_frame = (new Frame())->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->AddChildren({ coin, coin_text });

	auto coin2 = (Frame *)(new Frame())
		->SetSize(Vec2i(36, 36))
		->SetTexture(Intermission::sTextureCoin.GetTexture())
		->SetDraw(DRAW_TEXTURE);
	auto coin_text2 = (new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 "+1", { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT);
	auto coin_frame2 = (new Frame())->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->AddChildren({ coin2, coin_text2 });

	auto coin3 = (Frame *)(new Frame())
		->SetSize(Vec2i(36, 36))
		->SetTexture(Intermission::sTextureCoin.GetTexture())
		->SetDraw(DRAW_TEXTURE);
	auto coin_text3 = (new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 "+1", { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::DONT);
	auto coin_frame3 = (new Frame())->SetFlex(Flex::HEIGHT)
		->SetAdaptive(true, true)
		->AddChildren({ coin3, coin_text3 });

	coins_frame = (Frame *)(new Frame())
		->SetAlign(Align::CENTER, Align::DONT)
		->SetFlex(Flex::WIDTH, 6)
		->SetAdaptive(true, true)
		->SetName("CoinsFrame")
		->AddChildren({ coin_frame, coin_frame2, coin_frame3 });

	auto coins_earned_frame = (Frame *)(new Frame())
		->SetAlign(Align::CENTER, Align::DONT)
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT)
		->AddChildren({ coins_earned_text, coins_frame });

	auto stats_elements = (Frame *)(new Frame())
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT, 6)
		->SetAdaptive(true, true)
		->SetName("StatsElements")
		->AddChildren({ mini_profiles, coins_earned_frame });

	scribbles = (Intermission::Scribbles *)(new Intermission::Scribbles(Vec2i(400, 400), 35))
		->SetAlpha(100)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Scribbles")
		->AddChildren({ stats_elements });

	auto stats_frame = (new Frame())
		->SetFlex(Flex::HEIGHT, 30)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetAdaptive(true, true)
		->SetName("StatsFrame")
		->AddChildren({ title, scribbles });

	menu_b = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT)
		->SetFullyOccupy(true, true)
		->SetName("MenuB")
		->AddChildren({ stats_frame })
		->SetEnabled(false);

	this->SetTurnNumber(-1);

	this->SetFlex(Flex::HEIGHT);
	this->AddChildren({ menu_a, menu_b });
	this->SetName("IntermissionMenu");
	this->SetColor(253, 164, 62, 255);
	this->RefreshMenu();
}

IntermissionMenu::~IntermissionMenu()
{

}

void IntermissionMenu::SetTurnNumber(int turn_number)
{
	this->text_turn_number->UpdateText(CommonUI::sFontGiant2.GetFont()->TTFFont(),
									   Strings::FString("%d", turn_number).c_str(),
									   { 255, 255, 255, 255 });
}

void IntermissionMenu::ShowIntermission()
{
	menu_a->SetEnabled(true);
	menu_b->SetEnabled(false);
}

void IntermissionMenu::BeginAnimation()
{
	scribbles->GenerateZigZag(3, 15, 25, 30, 50, { 0.2, 0, 0, 1.0 });
	mini_profile1->SetEnabled(false);
	mini_profile2->SetEnabled(false);
	coins_earned_text->SetEnabled(false);
	coins_frame->SetEnabled(false);
}

void IntermissionMenu::SetProfile1(const std::string& username, Texture *profile_picture)
{
	mini_profile1->SetText(username);
	if (profile_picture)
		mini_profile1->SetProfilePicture(profile_picture);
}

void IntermissionMenu::SetProfile2(const std::string& username, Texture *profile_picture)
{
	mini_profile2->SetText(username);
	if (profile_picture)
		mini_profile2->SetProfilePicture(profile_picture);
}

void IntermissionMenu::RefreshData()
{
	auto game = Centralized.GetCurrentGame();

	// A
	this->SetTurnNumber(game->game_turn);

	// B
	this->SetProfile1(game->players[0].username, nullptr);
	this->SetProfile2(game->players[1].username, nullptr);
}

void IntermissionMenu::SetEndAnimationCallback(Callback end_callback)
{
	end_animation_callback = std::move(end_callback);
}

void IntermissionMenu::Tick(double elapsed_seconds)
{
	TickChildren(elapsed_seconds);

	if (menu_b->enabled && !scribbles->IsPlaying() && !countdown_started)
	{
		mini_profile1->SetEnabled(true);
		mini_profile2->SetEnabled(true);
		coins_earned_text->SetEnabled(true);
		coins_frame->SetEnabled(true);
		RefreshMenu();

		countdown_started = true;
		end_animation_countdown = std::chrono::steady_clock::now();
	}

	if (countdown_started && end_animation_callback &&
		std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - end_animation_countdown).count() > 3)
		end_animation_callback();
}