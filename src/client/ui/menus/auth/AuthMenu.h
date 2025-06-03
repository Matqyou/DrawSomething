//
// Created by Matq on 16/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../structures/window_texture/WindowTexture.h"
#include "../../components/element/TextElement.h"
#include "../../components/element/TextBox.h"
#include "../loading/LoadingScreen.h"

class AuthMenu : public FullscreenMenu
{
private:
	enum
	{
		AUTH_NONE,
		AUTH_LOGIN,
		AUTH_REGISTER,
	};

	LoadingScreen *loading_screen;
	Frame *header;

	Frame *auth_frame;
	// Auth frame: shared
	Frame *title;
	TextElement *subtitle;
	TextElement *response;
	std::string response_message;

	TextBox *username_field;
	TextBox *password_field;
	// Auth frame: login
	Frame *login_buttons_frame;
	TextBox *nickname_field;
	TextBox *confirmation_field;
	TextBox *email_field;
	// Auth frame: register
	Frame *register_buttons_frame;

public:
	AuthMenu();
	~AuthMenu() override;

	// Sense
	[[nodiscard]] LoadingScreen* GetLoadingScreen() const { return loading_screen; }

	// Manipulating
	void OpenLoginScreen();
	void OpenRegisterScreen();
	void SetResponse(const std::string& response_message, bool success_color);

	void SetLoginDetails(const std::string& username,
						 const std::string& password);
	void SetRegisterDetails(const std::string& username,
							const std::string& nickname,
							const std::string& email,
							const std::string& password,
							const std::string& confirmation);

	void AttemptLogin();
	void AttemptRegister();

};
