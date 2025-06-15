//
// Created by Matq on 15/06/2025.
//

#include "ExportMenu.h"
#include "game/GameData.h"
#include "ui/CommonUI.h"
#include "ui/RenderPresets.h"
#include "ui/menus/main/MainMenu.h"
#include "core/Application.h"

#include <windows.h>
#include <commdlg.h>

static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");
static LinkTexture sTextureCard("export.card");
static LinkTexture sTextureCardOutline("export.card_outline");

static PregenerateTexture sTextureHeader("export_menu_header", [](AssetsClass *assets) -> Texture *
{
	auto header_texture = assets->GetTexture("header");
	return header_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET)
		->SetColorMod(253, 164, 62);
});

// Function to get file extension from path
const char *get_file_extension(const char *filename)
{
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}

// Function to open Windows Save File dialog
int get_save_file_path(char *filepath, size_t max_path_len, HWND parent_window)
{
	OPENFILENAMEA ofn;
	char szFile[260] = { 0 };

	// Initialize OPENFILENAME structure
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent_window;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "PNG Files\0*.png\0JPEG Files\0*.jpg;*.jpeg\0All Files\0*.*\0";
	ofn.nFilterIndex = 1; // Default to PNG
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = "Save Texture As...";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT; //| OFN_EXTENSIONDONTADD;
	ofn.lpstrDefExt = "png"; // Default extension

	// Display the Save dialog
	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		// Check if user added an extension
		const char *ext = get_file_extension(szFile);
		if (strlen(ext) == 0)
		{
			// No extension provided, add default based on filter selection
			if (ofn.nFilterIndex == 1)
			{
				strcat(szFile, ".png");
			}
			else if (ofn.nFilterIndex == 2)
			{
				strcat(szFile, ".jpg");
			}
		}

		strncpy(filepath, szFile, max_path_len - 1);
		filepath[max_path_len - 1] = '\0';
		return 1; // Success
	}

	return 0; // User cancelled or error
}

// Helper function to get HWND from SDL window (if you have an SDL window)
HWND get_hwnd_from_sdl_window(SDL_Window *sdl_window)
{
	if (!sdl_window) return NULL;

	SDL_PropertiesID props = SDL_GetWindowProperties(sdl_window);
	return (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
}

// Function to save SDL_Texture to disk with Windows file dialog
int save_texture_with_dialog(Texture *texture)
{
	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();
	char filepath[512];

	// Open Windows Save File dialog
	auto parent_window = get_hwnd_from_sdl_window(Application::Get()->GetWindow());
	if (!get_save_file_path(filepath, sizeof(filepath), parent_window))
	{
		printf("Save operation cancelled by user\n");
		return 0; // User cancelled, not an error
	}

	// Validate file extension
	const char *ext = get_file_extension(filepath);
	if (_stricmp(ext, "png") != 0 && _stricmp(ext, "jpg") != 0 && _stricmp(ext, "jpeg") != 0)
	{
		fprintf(stderr, "Error: Unsupported file extension. Use .png, .jpg, or .jpeg\n");
		return 0;
	}

	// Get texture properties
	auto size = Vec2i(texture->GetOriginalSize());

	// Set texture as render target and read pixels
	auto original_target = SDL_GetRenderTarget(drawing->Renderer());
	drawing->SetRenderTarget(texture);

	// Read pixels from texture
	auto sdl_surface = SDL_RenderReadPixels(drawing->Renderer(), nullptr);
	if (!sdl_surface)
		return 0;

	// Restore original render target
	drawing->SetRenderTargetSDL(original_target);

	// Save the surface based on file extension
	int result = -1;
	if (_stricmp(ext, "png") == 0)
	{
		result = IMG_SavePNG(sdl_surface, filepath);
	}
	else if (_stricmp(ext, "jpg") == 0 || _stricmp(ext, "jpeg") == 0)
	{
		result = IMG_SaveJPG(sdl_surface, filepath, 90); // 90% quality
	}

	SDL_DestroySurface(sdl_surface);

	printf("Texture saved successfully to: %s\n", filepath);
	return 1; // Success
}

ExportMenu::ExportMenu()
	: FullscreenMenu()
{
	name = L"ExportMenu";

	auto header = (Frame *)(new Frame())
		->SetSize(Vec2i(0, 58))
		->SetTexture(sTextureHeader.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetFullyOccupy(true, false)
		->SetName("Header");

	auto title = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Would you like to save the image?", 3,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetRelative(Vec2i(0, 3))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Title");
	drawing_preview = (Frame *)(new Frame())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Preview");

	auto assets = Assets::Get();
	auto continue_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Skip", 2,
														  { 255, 255, 255, 255 },
														  { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto continue_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
															  { 200, 200, 200, 255 },
															  sTextureButtonOutline.GetTexture(), continue_text)
		->FlagForAutomaticDeletion();
	auto continue_button = (Button *)(new Button(continue_button_texture,
												 continue_button_texture))
		->SetSize(Vec2i(continue_button_texture->GetOriginalSize() / 2))
		->SetName("Continue");
	continue_button->SetCallback([]()
								 {
									 Centralized.main_menu->SwitchToThisMenu();
								 });

	auto save_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Save", 2,
													  { 255, 255, 255, 255 },
													  { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto save_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														  { 100, 255, 100, 255 },
														  sTextureButtonOutline.GetTexture(), save_text)
		->FlagForAutomaticDeletion();
	auto save_button = (Button *)(new Button(save_button_texture,
											 save_button_texture))
		->SetSize(Vec2i(save_button_texture->GetOriginalSize() / 2))
		->SetName("Done");
	save_button->SetCallback([]()
							 {
								 if (save_texture_with_dialog(Centralized.GetDrawnImage()))
									 Centralized.main_menu->SwitchToThisMenu();
							 });

	auto buttons_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(-25, 0))
		->SetFlex(Flex::WIDTH, 10)
		->SetAdaptive(true, true)
		->SetAlign(Align::RIGHT, Align::DONT)
		->AddChildren({ save_button, continue_button });

	auto card_texture = RenderPresets::ColorButton(sTextureCard.GetTexture(),
												   { 239, 121, 47, 255 },
												   sTextureCardOutline.GetTexture())
		->FlagForAutomaticDeletion();
	auto card = (Frame *)(new Frame())
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(card_texture)
		->SetSize(Vec2i(card_texture->GetOriginalSize() / 2))
		->SetFlex(Flex::HEIGHT, 10)
		->SetAlign(Align::CENTER, Align::CENTER)
		->AddChildren({ title, drawing_preview, buttons_frame });

	auto content = (Frame *)(new Frame())
		->SetFullyOccupy(true, false)
		->SetOccupy(false, true)
		->SetName("Content")
		->AddChildren({ card });

	this->SetFlex(Flex::HEIGHT);
	this->AddChildren({ header, content });
	this->SetName("IntermissionMenu");
	this->SetColor(253, 164, 62, 255);
	this->RefreshMenu();
}

ExportMenu::~ExportMenu()
{

}

void ExportMenu::SwitchToThisMenu()
{
	drawing_preview->SetTexture(Centralized.GetDrawnImage())
		->ResizeToTexture();
	drawing_preview->SetSize(Vec2i(drawing_preview->GetSize() * 0.9));

	Centralized.current_menu = (FullscreenMenu *)this;
	RefreshMenu();
}