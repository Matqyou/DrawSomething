//
// Created by Matq on 28/04/2025.
//

#include "AdminScreen.h"
#include "game/GameData.h"
#include "UserRecord.h"
#include "ui/RenderPresets.h"
#include "ui/components/element/Button.h"

static LinkTexture sTextureAdminTitle("main_menu.admin.all_users");
static LinkTexture sTextureAdminCard("main_menu.admin.card");
static LinkTexture sTextureAdminCardOutline("main_menu.admin.card_outline");
static LinkTexture sTextureClose("main_menu.settings.x");

AdminScreen::AdminScreen()
	: ScreenMenu()
{
	Frame *title = (Frame *)(new Frame())
		->SetSize(Vec2i(sTextureAdminTitle.GetTexture()->GetOriginalSize() / 2))
		->SetRelative(Vec2i(0, 13))
		->SetTexture(sTextureAdminTitle.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Title");

	users_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(25, 25))
		->SetAdaptive(true, true)
		->SetFlex(Flex::HEIGHT, 3)
		->SetName("UsersFrame");

	auto close_button = (Button *)(new Button(sTextureClose.GetTexture(),
											  sTextureClose.GetTexture()))
		->SetRelative(Vec2i(-15, 15))
		->SetSize(Vec2i(32, 32))
		->SetColor(150, 0, 0, 255)
		->SetAlign(Align::RIGHT, Align::TOP)
		->SetFlexInvolved(false, false)
		->SetName("CloseButton");
	close_button->SetCallback([this]()
							  {
								  this->SetEnabled(false);
							  });

	auto card_texture = RenderPresets::ColorButton(sTextureAdminCard.GetTexture(),
												   { 239, 121, 47, 255 },
												   sTextureAdminCardOutline.GetTexture());
	auto card = (new Frame())
		->SetSize(Vec2i(card_texture->GetOriginalSize() / 2))
		->SetRelative(Vec2i(0, 0))
		->SetTexture(card_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT)
		->SetName("Card")
		->AddChildren({ title, users_frame, close_button });

	this->SetName("AdminMenu");
	this->AddChildren({ card });
}

AdminScreen::~AdminScreen()
{

}

void AdminScreen::RefreshData()
{
	for (auto user : users_frame->children)
		delete user;
	users_frame->children.clear();

	for (auto user : Centralized.GetAdminUserList())
		users_frame->AddChildren({ (new UserRecord())->UpdateInfo(user) });
}
