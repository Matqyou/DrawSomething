//
// Created by Matq on 28/04/2025.
//

#include "client/ui/menus/main/admin/UserRecord.h"
#include "client/ui/CommonUI.h"
#include "client/ui/components/element/Button.h"

static LinkTexture sTextureRecord("main_menu.admin.record");
static LinkTexture sTextureDelete("x");

UserRecord::UserRecord()
{
	user_id = (TextElement *)(new TextElement())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("UserID");
	username = (TextElement *)(new TextElement())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("UserID");
	nickname = (TextElement *)(new TextElement())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("UserID");
	email = (TextElement *)(new TextElement())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("UserID");
	created = (TextElement *)(new TextElement())
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("UserID");

	auto delete_record = (Button*)(new Button(sTextureDelete.GetTexture(), nullptr))
		->SetSize(Vec2i(26, 26))
		->SetRelative(Vec2i(-4, 0))
		->SetAlign(Align::RIGHT, Align::CENTER);

	this->SetSize(Vec2i(953, 35));
	this->SetTexture(sTextureRecord.GetTexture());
	this->SetDraw(DRAW_TEXTURE);
	this->SetFlex(Flex::WIDTH, 30);
	this->SetName("UserRecord");
	this->AddChildren({ user_id, username, nickname, email, created, delete_record });
}

UserRecord::~UserRecord()
{

}

UserRecord *UserRecord::UpdateInfo(Account *user_account)
{
	user_id->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
					  Strings::FString("ID - #%d", user_account->user_id).c_str(),
					  { 230, 230, 230, 255 });
	username->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
						Strings::FString("user: %s", user_account->username.c_str()).c_str(),
						{ 230, 230, 230, 255 });
	nickname->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
						Strings::FString("nick: %s", user_account->nickname.c_str()).c_str(),
						{ 230, 230, 230, 255 });
	email->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
						 Strings::FString("email: %s", user_account->email.c_str()).c_str(),
						 { 230, 230, 230, 255 });
	created->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
						 Strings::FString("created: %s", user_account->created.c_str()).c_str(),
						 { 230, 230, 230, 255 });
	return this;
}