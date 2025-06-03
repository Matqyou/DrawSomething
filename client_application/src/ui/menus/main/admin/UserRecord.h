//
// Created by Matq on 28/04/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "game/account/Account.h"
#include "ui/components/element/TextElement.h"

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
