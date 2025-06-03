//
// Created by Matq on 28/04/2025.
//

#include "client/ui/menus/main/admin/AdminScreen.h"
#include "client/game/GameData.h"
#include "client/ui/menus/main/admin/UserRecord.h"

static LinkTexture sTextureAdminTitle("main_menu.admin.all_users");
static LinkTexture sTextureAdminCard("main_menu.admin.card");

AdminScreen::AdminScreen()
	: ScreenMenu()
{
	Frame *title = (Frame *)(new Frame())
		->SetSize(Vec2i(sTextureAdminTitle.GetTexture()->GetOriginalSize()/2))
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

	auto card = (new Frame())
		->SetSize(Vec2i(994, 631-131/2))
		->SetRelative(Vec2i(0, 0))
		->SetTexture(sTextureAdminCard.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT)
		->SetName("Card")
		->AddChildren({ title, users_frame });

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
