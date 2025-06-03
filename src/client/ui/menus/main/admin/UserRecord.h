//
// Created by Matq on 28/04/2025.
//

#pragma once

#include "client/ui/components/element/Frame.h"
#include "client/game/account/Account.h"
#include "client/ui/components/element/TextElement.h"

class UserRecord : public Frame
{
private:
	TextElement* user_id;
	TextElement* username;
	TextElement* nickname;
	TextElement* email;
	TextElement* created;

public:
	UserRecord();
	~UserRecord() override;

	// Options
	UserRecord* UpdateInfo(Account* user_account);

};
