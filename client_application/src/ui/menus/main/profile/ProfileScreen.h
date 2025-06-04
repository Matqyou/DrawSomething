//
// Created by Matq on 23/05/2025.
//

#pragma once

#include "ui/menus/ScreenMenu.h"
#include "ui/components/element/TextBox.h"
#include "ui/components/element/TextElement.h"

class ProfileScreen : public ScreenMenu
{
private:
	Frame *profile_picture;
	TextBox *nickname_field;

	TextBox *username_field;
	TextBox *email_field;
	TextBox *password_field;
	TextBox *confirmation_field;

	TextElement *response;

	Texture *profile_picture_texture;

public:
	ProfileScreen();
	~ProfileScreen() override;

	// Manipulating
	void RefreshData();
	void SetProfilePicture(Texture *texture_data);
	void SetResponse(const std::string& message, bool success_color);

};
