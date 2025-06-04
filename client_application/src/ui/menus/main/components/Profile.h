//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "ui/components/element/TextElement.h"

namespace Main
{
class Profile : public Frame
{
private:
	Frame *profile_picture;
	TextElement *profile_nickname;
	TextElement *profile_username;
	TextElement *profile_email;
	TextElement *is_administrator;

	TextElement *games_played;
	TextElement *rounds_played;
	TextElement *highest_turn;
	TextElement *coins_earned;
	TextElement *coins_spent;

	Texture *profile_picture_texture;
	Texture *top_base_composition;

public:
	Profile();
	~Profile() override;

	// Manipulating
	void RefreshData();
	void SetProfilePicture(Texture *texture_data);

};
}
