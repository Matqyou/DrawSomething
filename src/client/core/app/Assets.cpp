//
// Created by Mat on 12/3/2024.
//

#include "Assets.h"
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <algorithm>
#include <cstring>
#include <utility>
#include "Drawing.h"

using namespace Strings;

template<> AssetsClass *Singleton<AssetsClass>::instance_ = nullptr;
template<> const char *Singleton<AssetsClass>::singleton_name_ = "Assets";
std::vector<Texture *> AssetsClass::m_AutomaticDeletionTextures = { };
std::vector<LinkTexture *> AssetsClass::m_LinkTextures = { };
std::vector<PregenerateTexture *> AssetsClass::m_PregenerateTextures = { };
std::vector<LinkSound *> AssetsClass::m_LinkSounds = { };
std::vector<LinkMusic *> AssetsClass::m_LinkMusic = { };
std::vector<PreloadFont *> AssetsClass::m_PreloadFonts = { };
std::vector<LinkFont *> AssetsClass::m_LinkFonts = { };

// Iterators
std::vector<LinkTexture *>::iterator AssetsClass::m_LinkTexturesIterator = std::vector<LinkTexture *>::iterator{ };
std::vector<LinkSound *>::iterator AssetsClass::m_LinkSoundsIterator = std::vector<LinkSound *>::iterator{ };
std::vector<LinkMusic *>::iterator AssetsClass::m_LinkMusicIterator = std::vector<LinkMusic *>::iterator{ };
std::vector<LinkFont *>::iterator AssetsClass::m_LinkFontsIterator = std::vector<LinkFont *>::iterator{ };
std::vector<PregenerateTexture *>::iterator
	AssetsClass::m_PregenerateTexturesIterator = std::vector<PregenerateTexture *>::iterator{ };

bool AssetsClass::sPreloadStage = true;
bool AssetsClass::sLoadingStage = true;
size_t AssetsClass::sTotalWork = 0;
size_t AssetsClass::sWorkDone = 0;

DiskTexture::DiskTexture(Texture *texture, const std::string& load_extension)
{
	m_Texture = texture;
	m_LoadExtension = load_extension;
	texture->FlagForAutomaticDeletion();
}

DiskTexture::~DiskTexture()
{

}

Sound::Sound(std::string key, Mix_Chunk *mix_chunk, std::string load_extension)
	: m_Key(std::move(key)),
	  m_LoadExtension(std::move(load_extension))
{
	m_MixChunk = mix_chunk;
}

Sound::~Sound()
{
	if (m_MixChunk)
		Mix_FreeChunk(m_MixChunk);
}

void Sound::SetVolume(int volume)
{
	Mix_VolumeChunk(m_MixChunk, volume);
}

void Sound::PlaySound()
{
	if (m_MixChunk == nullptr || !Assets::Get()->SoundsEnabled())
		return;

	Mix_PlayChannel(-1, m_MixChunk, 0);
}

Music::Music(std::string key, Mix_Music *mix_music, std::string load_extension)
	: m_Key(std::move(key)),
	  m_LoadExtension(std::move(load_extension))
{
	m_MixMusic = mix_music;
}

Music::~Music()
{
	if (m_MixMusic)
		Mix_FreeMusic(m_MixMusic);
}

void Music::PlayMusic(int loops)
{
	if (m_MixMusic == nullptr || !Assets::Get()->SoundsEnabled())
		return;

	Mix_PlayMusic(m_MixMusic, loops);
}

Font::Font(TTF_Font *ttf_font, std::string key, std::string load_extension)
	: m_Key(std::move(key)),
	  m_LoadExtension(std::move(load_extension))
{
	m_TTFFont = ttf_font;
	m_Size = -1.0f;
}

Font::~Font()
{
	if (m_TTFFont)
		TTF_CloseFont(m_TTFFont);
}

std::vector<std::tuple<std::string, std::string, std::string>> GetResourceKeys(const char *directory,
																			   const std::unordered_set<
																				   std::string>& supported_extensions)
{
	std::vector<std::tuple<std::string, std::string, std::string>> results;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(directory))
	{
		if (!entry.is_regular_file())
			continue;

		const std::filesystem::path& file_path = entry.path();

		// Extension in lowercase
		const std::string extension = file_path.extension().string();
		std::string extension_lower(extension);
		std::transform(extension_lower.begin(), extension_lower.end(), extension_lower.begin(), ::tolower);

		// Process the path string for logging or other use
		std::string identificator = file_path.string();
		identificator = ErasePrefix(identificator, directory);
		identificator = EraseSuffix(identificator, extension);

		// Convert to lowercase and replace slashes to periods
		std::transform(identificator.begin(), identificator.end(), identificator.begin(), ::tolower);
		std::replace(identificator.begin(), identificator.end(), '/', '.');
		std::replace(identificator.begin(), identificator.end(), '\\', '.');

		// Check if the extension is supported
		if (supported_extensions.find(extension) == supported_extensions.end())
		{
			std::wcout << FStringColorsW(L"[Assets] &8Unsupported file format '%s' for '%s'\n",
										 extension.c_str(),
										 identificator.c_str());
			continue;
		}

		// Add the key and path to the results
		results.emplace_back(identificator, file_path.string(), extension);
	}

	return results;
}

bool AssetsClass::LoadingTextures()
{
	if (m_TextureResourcesIterator == m_TextureResources.end())
		return false;

	sWorkDone++;

	auto entry = *m_TextureResourcesIterator;
	++m_TextureResourcesIterator;
	std::string& texture_key = std::get<0>(entry);
	std::string& file_path = std::get<1>(entry);
	std::string& extension = std::get<2>(entry);

	// Using `DiskTextures` so we can display the extensions of the sprite files
	auto disk_it = m_DiskTextures.find(texture_key);
	if (disk_it != m_DiskTextures.end())
	{
		std::wcout << FStringColorsW(L"[Assets] &cDuplicate texture '%s' for existing '%s'(%s)\n",
									 extension.c_str(),
									 texture_key.c_str(),
									 disk_it->second->GetExtension().c_str());
		return true;
	}

	// Load the surface
	SDL_Surface *TempSurface = IMG_Load(file_path.c_str());
	if (TempSurface == nullptr)
	{
		std::wcout << FStringColorsW(L"[Assets] &cFailed to load texture '%s'\n", file_path.c_str());
		std::wcout << FStringColorsW(L"[Assets] &cReason (%s)\n", SDL_GetError());
		return true;
	}

	// Convert to texture
	SDL_Texture *NewSDLTexture = SDL_CreateTextureFromSurface(m_Drawing->Renderer(), TempSurface);
	SDL_DestroySurface(TempSurface);

	// Check if texture has a visual hitbox file
	auto vis_it = std::find_if(m_VisHitboxResources.begin(), m_VisHitboxResources.end(),
							   [&texture_key](const std::tuple<std::string, std::string, std::string>& item)
							   {
								   return std::get<0>(item) == texture_key; // Compare first string
							   });

	Texture *new_texture;
	if (vis_it != m_VisHitboxResources.end())
	{
		HitboxFile visual_hitbox;
		if (visual_hitbox.OpenFile(R"(.\assets\images\)", texture_key))
		{
			new_texture = new VisualTexture(NewSDLTexture, Rect4f(visual_hitbox.Hitbox()));
		}
		else
		{
			std::wcout
				<< FStringColorsW(L"[Assets] &cFailed to load visual hitbox file '%s'\n", std::get<1>(*vis_it).c_str());
			return true;
		}
	}
	else
	{ new_texture = new Texture(NewSDLTexture); }

	// Add it to all the textures
	m_Textures[texture_key] = new_texture;
	m_DiskTextures[texture_key] = new DiskTexture(new_texture, extension);
	std::wcout << FStringColorsW(L"[Assets] &9Loaded texture '%s'\n", texture_key.c_str());

	return true;
}

bool AssetsClass::LoadingSounds()
{
	if (m_SoundResourcesIterator == m_SoundResources.end())
		return false;

	sWorkDone++;

	auto entry = *m_SoundResourcesIterator;
	++m_SoundResourcesIterator;
	std::string& key = std::get<0>(entry);
	std::string& file_path = std::get<1>(entry);
	std::string& extension = std::get<2>(entry);

	auto it = m_Sounds.find(key);
	if (it != m_Sounds.end())
	{
		std::wcout << FStringColorsW(L"[Assets] &cDuplicate sound '%s' for existing '%s'(%s)\n",
									 extension.c_str(),
									 key.c_str(),
									 it->second->m_LoadExtension.c_str());
		return true;
	}

	// Load the sound
	Mix_Chunk *NewMixChunk = Mix_LoadWAV(file_path.c_str());
	if (!NewMixChunk)
	{
		std::wcout << FStringColorsW(L"[Assets] &cFailed to load sound '%s'\n", file_path.c_str());
		std::wcout << FStringColorsW(L"[Assets] &cReason (%s)\n", SDL_GetError());
		return true;
	}

	// Add it to all the textures
	auto new_sound = new Sound(key, NewMixChunk, extension);
	m_Sounds[key] = new_sound;

	return true;
}

bool AssetsClass::LoadingMusic()
{
	if (m_MusicResourcesIterator == m_MusicResources.end())
		return false;

	sWorkDone++;

	auto entry = *m_MusicResourcesIterator;
	++m_MusicResourcesIterator;
	std::string& key = std::get<0>(entry);
	std::string& file_path = std::get<1>(entry);
	std::string& extension = std::get<2>(entry);

	auto it = m_Music.find(key);
	if (it != m_Music.end())
	{
		std::wcout << FStringColorsW(L"[Assets] &cDuplicate music '%s' for existing '%s'(%s)\n",
									 extension.c_str(),
									 key.c_str(),
									 it->second->m_LoadExtension.c_str());
		return true;
	}

	// Load the sound
	Mix_Music *NewMixMusic = Mix_LoadMUS(file_path.c_str());
	if (!NewMixMusic)
	{
		std::wcout << FStringColorsW(L"[Assets] &cFailed to load music '%s'\n", file_path.c_str());
		std::wcout << FStringColorsW(L"[Assets] &cReason (%s)\n", SDL_GetError());
		return true;
	}

	// Add it to all the textures
	auto new_music = new Music(key, NewMixMusic, extension);
	m_Music[key] = new_music;

	return true;
}

bool AssetsClass::LoadingFonts()
{
	if (m_PreloadFontIterator == m_PreloadFonts.end())
		return false;

	sWorkDone++;

	auto required_font = *m_PreloadFontIterator;
	++m_PreloadFontIterator;
	const std::string& font_key = required_font->Key();
	const std::string& font_id = required_font->FontID();
	float font_size = required_font->Size();

	auto it = m_Fonts.find(font_key);
	if (it != m_Fonts.end())
	{
		auto [other_font_id, other_font] = *it;
		std::wcout << FStringColorsW(L"[Assets] &cDuplicate font '%s'&r(%s:%.1fpt)\n",
									 font_key.c_str(),
									 font_id.c_str(),
									 font_size);
		std::wcout << FStringColorsW(L"[Assets] &cExisting font '%s'&r(%s:%.1fpt)\n",
									 other_font->m_Key.c_str(),
									 other_font_id.c_str(),
									 other_font->m_Size);
		return true;
	}

	for (auto entry : m_FontResources)
	{
		std::string& id = std::get<0>(entry);
		std::string& file_path = std::get<1>(entry);
		std::string& extension = std::get<2>(entry);

		if (font_id == id)
		{
			auto ttf_font = TTF_OpenFont(file_path.c_str(), font_size);
			if (!ttf_font)
				throw std::runtime_error(FString("Failed to load font '%s' because (%s)",
												 file_path.c_str(),
												 SDL_GetError()));

			auto new_font = (new Font(ttf_font, font_key, extension))
				->CacheSize(font_size);
			required_font->m_Font = new_font;
			m_Fonts[font_key] = new_font;
			std::wcout << Strings::FStringColorsW(L"[Assets] &9Loaded font '%s' &7(%s:%.1fpt)\n",
												  font_key.c_str(),
												  font_id.c_str(),
												  font_size);
		}
	}

	return true;
}

bool AssetsClass::GeneratingTextures()
{
	if (m_PregenerateTexturesIterator == m_PregenerateTextures.end())
		return false;

	sWorkDone++;

	auto generate_texture = *m_PregenerateTexturesIterator;
	++m_PregenerateTexturesIterator;
	const std::string& texture_key = generate_texture->Key();

	// Find if a texture with the same key exists
	auto it = m_Textures.find(texture_key);
	if (it != m_Textures.end())
	{
		std::wcout << FStringColorsW(L"[Assets] &cDuplicate pre-generated texture '%s'\n", texture_key.c_str());
		return true;
	}

	// If no callback was set, no generation can take place
	if (!generate_texture->m_GenerateCallback)
	{
		std::wcout
			<< FStringColorsW(L"[Assets] &cCould not pre-generate '%s', invalid callback\n", texture_key.c_str());
		return true;
	}

	// If generator returned nullptr, there must've been an error
	auto new_texture = generate_texture->m_GenerateCallback(this);
	if (new_texture == nullptr)
	{
		std::wcout << FStringColorsW(L"[Assets] &cCould not pre-generate '%s', invalid texture\n", texture_key.c_str());
		return true;
	}

	new_texture->FlagForAutomaticDeletion();
	generate_texture->m_Texture = new_texture;
	m_Textures[texture_key] = new_texture;
	std::wcout << FStringColorsW(L"[Assets] &eGenerated texture '%s'\n", texture_key.c_str());

	return true;
}

bool AssetsClass::LinkingTextures()
{
	if (m_LinkTextures.empty())
		return false; // todo: bad

	sWorkDone += m_LinkTextures.size();

	std::wcout << FStringColorsW(L"[Assets] &5Loaded %i textures\n", m_Textures.size());
	m_InvalidTextureDefault = nullptr;
	m_InvalidTextureDefault = GetTexture("invalid");

	for (auto required_texture : m_LinkTextures)
	{
		const std::string& texture_key = required_texture->Key();
		auto link_texture = GetTexture(texture_key);
		required_texture->m_Texture = link_texture;

		if (link_texture == nullptr)
		{
			std::wcout
				<< FStringColorsW(L"[Assets] &cCould not link '%s' (texture not found)\n", texture_key.c_str());
			continue;
		}

		required_texture->m_LoadCallback(link_texture);
	}
	std::wcout << FStringColorsW(L"[Assets] &5Linked %zu textures\n", m_LinkTextures.size());
	m_LinkTextures.clear();

	return true;
}

bool AssetsClass::LinkingSounds()
{
	if (m_LinkSounds.empty())
		return false; // todo: bad

	sWorkDone += m_LinkSounds.size();

	std::wcout << FStringColorsW(L"[Assets] &5Loaded %i sounds\n", m_Sounds.size());

	// Link
	for (auto required_sound : m_LinkSounds)
	{
		const std::string& sound_key = required_sound->Key();

		required_sound->m_Sound = GetSound(sound_key);
	}
	std::wcout << FStringColorsW(L"[Assets] &5Linked %zu sounds\n", m_LinkSounds.size());
	m_LinkSounds.clear();

	return true;
}

bool AssetsClass::LinkingMusic()
{
	if (m_LinkMusic.empty())
		return false; // todo: bad

	sWorkDone += m_LinkMusic.size();

	std::wcout << FStringColorsW(L"[Assets] &5Loaded %i music\n", m_Music.size());

	// Link
	for (auto required_music : m_LinkMusic)
	{
		const std::string& music_key = required_music->Key();

		required_music->m_Music = GetMusic(music_key);
	}
	std::wcout << FStringColorsW(L"[Assets] &5Linked %zu music\n", m_LinkMusic.size());
	m_LinkMusic.clear();

	return true;
}

bool AssetsClass::LinkingFonts()
{
	if (m_LinkFonts.empty())
		return false; // todo: very bad

	sWorkDone += m_LinkFonts.size();

	for (auto required_font : m_LinkFonts)
	{
		const std::string& font_key = required_font->Key();

		required_font->m_Font = GetFont(font_key);
	}

	auto loaded_fonts_message = FStringW(L"[Assets] &5Loaded %zu fonts", m_PreloadFonts.size());
	if (!m_LinkFonts.empty()) loaded_fonts_message += FStringW(L" &d(%zu linked)", m_LinkFonts.size());
	std::wcout << FStringColorsW(L"%ls\n", loaded_fonts_message.c_str());

	m_LinkFonts.clear();
	return true;
}

AssetsClass::AssetsClass(Drawing *drawing, bool sounds_enabled)
	: m_InvalidTextureDefault(nullptr)
{
	m_Drawing = drawing;
	m_SoundsEnabled = sounds_enabled;

	// Todo: Loading assets in realtime (adding/removing files?)

	std::wcout << Strings::FStringColorsW(L"[Assets] &aLooking for assets..\n");
	std::unordered_set<std::string> texture_extensions = {
		".png", ".jpg", ".jpeg", ".bmp", ".gif", ".tif", ".tiff", ".webp"
	};
	std::unordered_set<std::string> sound_extensions = { // ".wav" doesn't work for some reason
		".aiff", ".voc", ".mp3", ".ogg", ".flac", ".mod",
		".s3m", ".it", ".xm", ".mid", ".midi", ".opus"
	};
	std::unordered_set<std::string> font_extensions = {
		".ttf", ".otf", ".fon", ".fnt",
		".bdf", ".pcf", ".sfnt", ".ttc",
	};
	m_VisHitboxResources = GetResourceKeys(R"(.\assets\images\)", { ".vis" });
	m_TextureResources = GetResourceKeys(R"(.\assets\images\)", texture_extensions);
	m_SoundResources = GetResourceKeys(R"(.\assets\sounds\)", sound_extensions);
	m_MusicResources = GetResourceKeys(R"(.\assets\music\)", sound_extensions);
	m_FontResources = GetResourceKeys(R"(.\assets\fonts\)", font_extensions);

	AssetsClass::sPreloadStage = false;
	m_TextureResourcesIterator = m_TextureResources.begin();
	m_SoundResourcesIterator = m_SoundResources.begin();
	m_MusicResourcesIterator = m_MusicResources.begin();
	m_PreloadFontIterator = m_PreloadFonts.begin();
	m_PregenerateTexturesIterator = m_PregenerateTextures.begin();
	m_LinkTexturesIterator = m_LinkTextures.begin();
	m_LinkSoundsIterator = m_LinkSounds.begin();
	m_LinkMusicIterator = m_LinkMusic.begin();
	m_LinkFontsIterator = m_LinkFonts.begin();

	std::wcout << Strings::FStringColorsW(L"[Assets] &2Found %zu textures in assets\n", m_TextureResources.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &2Found %zu sounds in assets\n", m_SoundResources.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &2Found %zu music in assets\n", m_MusicResources.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &2Found %zu fonts in assets\n", m_FontResources.size());
	std::wcout
		<< Strings::FStringColorsW(L"[Assets] &dFound %zu different font instances\n", m_PreloadFonts.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &eFound %zu texture links\n", m_LinkTextures.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &eFound %zu sound links\n", m_LinkSounds.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &eFound %zu music links\n", m_LinkMusic.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &eFound %zu font links\n", m_LinkFonts.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &dFound %zu pre-generated texture instances\n",
										  m_PregenerateTextures.size());
	std::wcout << Strings::FStringColorsW(L"[Assets] &aReady to load assets..\n");

	sTotalWork =
		m_TextureResources.size() + m_SoundResources.size() + m_MusicResources.size() + m_PreloadFonts.size()
			+ m_LinkTextures.size() + m_LinkSounds.size() + m_LinkMusic.size() + m_LinkFonts.size()
			+ m_PregenerateTextures.size();
}

AssetsClass::~AssetsClass()
{
	size_t preloaded_textures = m_Textures.size(); // Also deleted here
	size_t destroyed_textures = 0;
	size_t destroyed_sounds = 0;
	size_t destroyed_music = 0;
	size_t destroyed_fonts = 0;
	for (auto texture : m_AutomaticDeletionTextures)
	{
		delete texture;
		destroyed_textures++;
	}
	for (const auto& entry : m_Sounds)
	{
		delete entry.second;
		destroyed_sounds++;
	}
	for (const auto& entry : m_Music)
	{
		delete entry.second;
		destroyed_music++;
	}
	for (const auto& entry : m_Fonts)
	{
		delete entry.second;
		destroyed_fonts++;
	}

	auto other_destroyed_textures = destroyed_textures - preloaded_textures;
	auto unloaded_textures_message = FStringW(L"[Assets] &5Destroyed %zu textures", preloaded_textures);
	if (other_destroyed_textures > 0)
		unloaded_textures_message += FStringW(L" &d(+%zu dynamic)", other_destroyed_textures);
	std::wcout << FStringColorsW(L"%ls\n", unloaded_textures_message.c_str());
	std::wcout << FStringColorsW(L"[Assets] &5Destroyed %zu sounds\n", destroyed_sounds);
	std::wcout << FStringColorsW(L"[Assets] &5Destroyed %zu music\n", destroyed_music);
	std::wcout << FStringColorsW(L"[Assets] &5Destroyed %zu fonts\n", destroyed_fonts);
}

Texture *AssetsClass::GetTexture(const std::string& texture_key)
{
	auto it = m_Textures.find(texture_key);
	if (it != m_Textures.end())
		return it->second;

	if (m_InvalidTextureDefault != nullptr)
		return m_InvalidTextureDefault;

	throw std::runtime_error(Strings::FString(
		"Assets.h GetTexture() texture '%s' was not found (no invalid texture found either)\n",
		texture_key.c_str()));
}

const std::unordered_map<std::string, Texture *>& AssetsClass::GetAllTextures()
{
	return m_Textures;
}

Sound *AssetsClass::GetSound(const std::string& sound_key)
{
	auto it = m_Sounds.find(sound_key);
	if (it != m_Sounds.end())
		return it->second;

	return nullptr;
}

Music *AssetsClass::GetMusic(const std::string& music_key)
{
	auto it = m_Music.find(music_key);
	if (it != m_Music.end())
		return it->second;

	return nullptr;
}

Font *AssetsClass::GetFont(const std::string& font_key)
{
	auto it = m_Fonts.find(font_key);
	if (it != m_Fonts.end())
		return it->second;

	return nullptr;
}

Texture *AssetsClass::TextureFromSurface(SDL_Surface *sdl_surface)
{
	SDL_Texture *NewSDLTexture = SDL_CreateTextureFromSurface(m_Drawing->Renderer(), sdl_surface);

	return new Texture(NewSDLTexture);
}

SDL_Surface *AssetsClass::CreateSDLSurface(int width, int height, SDL_PixelFormat format)
{
	SDL_Surface *sdl_surface = SDL_CreateSurface(width, height, format);
	return sdl_surface;
}

Texture *AssetsClass::CreateTexture(SDL_PixelFormat format, SDL_TextureAccess access, int w, int h)
{
	SDL_Texture *NewSDLTexture = SDL_CreateTexture(m_Drawing->Renderer(), format, access, w, h);

	return new Texture(NewSDLTexture);
}

SDL_Texture *AssetsClass::CopySDLTexture(SDL_Texture *copy_texture, SDL_TextureAccess access)
{
	auto original_render_target = SDL_GetRenderTarget(m_Drawing->Renderer());

	SDL_Texture *NewSDLTexture = SDL_CreateTexture(m_Drawing->Renderer(),
												   copy_texture->format,
												   access,
												   copy_texture->w,
												   copy_texture->h);
//    SDL_SetRenderTarget(m_Drawing->Renderer(), NewSDLTexture);
	m_Drawing->SetRenderTargetSDL(NewSDLTexture);
	m_Drawing->RenderTextureFullscreen(copy_texture, nullptr);
	m_Drawing->SetRenderTargetSDL(original_render_target);
	return NewSDLTexture;
}

//VisualTexture* AssetsClass::RenderTextBlendedVisual(TTF_Font* font, const std::string& text, SDL_Color color) {
//    SDL_Surface* sdl_surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
//    SDL_Texture* NewSDLTexture = SDL_CreateTextureFromSurface(m_Drawing->Renderer(), sdl_surface);
//    SDL_DestroySurface(sdl_surface);
//
//    Vec2f text_size;
//    SDL_GetTextureSize(NewSDLTexture, &text_size.x, &text_size.y);
//
//    return new VisualTexture(NewSDLTexture, Rect4f(0.0f, 0.0f, text_size.x, text_size.y));
//}

Texture *AssetsClass::RenderTextBlended(TTF_Font *font, const std::string& text, SDL_Color color)
{
	SDL_Surface *sdl_surface = TTF_RenderText_Blended(font, text.c_str(), text.size(), color);
	Texture *text_render = TextureFromSurface(sdl_surface);
	SDL_DestroySurface(sdl_surface);
	return text_render;
}

Texture *AssetsClass::RenderTextBlended(TTF_Font *font, const char *text, SDL_Color color)
{
	SDL_Surface *sdl_surface = TTF_RenderText_Blended(font, text, strlen(text), color);
	Texture *text_render = TextureFromSurface(sdl_surface);
	SDL_DestroySurface(sdl_surface);
	return text_render;
}

Texture *AssetsClass::RenderTextBlendedOutline(TTF_Font *font, const char *text, int thickness,
											   SDL_Color text_color, SDL_Color outline_color)
{
	auto original_render_target = SDL_GetRenderTarget(m_Drawing->Renderer());

	TTF_SetFontOutline(font, thickness);
	SDL_Surface *outlineSurface = TTF_RenderText_Blended(font, text, strlen(text),
														 { outline_color.r, outline_color.g, outline_color.b, 255 });

	TTF_SetFontOutline(font, 0);

	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, strlen(text), text_color);

	SDL_Texture *outlineTexture = SDL_CreateTextureFromSurface(m_Drawing->Renderer(), outlineSurface);
	SDL_SetTextureAlphaMod(outlineTexture, outline_color.a);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(m_Drawing->Renderer(), textSurface);

	auto result = SDL_CreateTexture(m_Drawing->Renderer(),
									SDL_PIXELFORMAT_RGBA8888,
									SDL_TEXTUREACCESS_TARGET,
									outlineTexture->w, outlineTexture->h);
	m_Drawing->SetRenderTargetSDL(result);
	m_Drawing->RenderTextureFullscreen(outlineTexture);
	SDL_FRect text_destination = {
		(float)thickness, (float)thickness,
		(float)textTexture->w, (float)textTexture->h
	};
	m_Drawing->RenderTexture(textTexture, nullptr, text_destination);
	m_Drawing->SetRenderTargetSDL(original_render_target);

	SDL_DestroyTexture(outlineTexture);
	SDL_DestroyTexture(textTexture);

	return new VisualTexture(result, Rect4f(text_destination.x, text_destination.y, text_destination.w, text_destination.h));
}

Texture *AssetsClass::RenderTextSolid(TTF_Font *font, const std::string& text, SDL_Color color)
{
	SDL_Surface *sdl_surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), color);
	Texture *text_render = TextureFromSurface(sdl_surface);
	SDL_DestroySurface(sdl_surface);
	return text_render;
}

Texture *AssetsClass::RenderTextSolid(TTF_Font *font, const char *text, SDL_Color color)
{
	SDL_Surface *sdl_surface = TTF_RenderText_Solid(font, text, strlen(text), color);
	Texture *text_render = TextureFromSurface(sdl_surface);
	SDL_DestroySurface(sdl_surface);
	return text_render;
}

Texture *AssetsClass::RenderTextureOnTextureCentered(Texture *outer, Texture *inner, bool copy)
{
	auto original_render_target = SDL_GetRenderTarget(m_Drawing->Renderer());

	auto result_texture = copy ? outer->CopyTexture(SDL_TEXTUREACCESS_TARGET) : outer;
	auto outer_size = outer->GetOriginalSize();
	auto inner_size = inner->GetOriginalSize();
	auto position = Rectangles::CenterRelative(inner_size, outer_size);
	m_Drawing->SetRenderTarget(result_texture);
	m_Drawing->RenderTexture(inner->SDLTexture(), nullptr, position);
	m_Drawing->SetRenderTargetSDL(original_render_target);
	return result_texture;
}

bool AssetsClass::SaveTextureToDisk(Texture *texture, const std::string& filename)
{
	auto original_render_target = SDL_GetRenderTarget(m_Drawing->Renderer());

	m_Drawing->SetRenderTarget(texture);
	SDL_Surface *sdl_surface = SDL_RenderReadPixels(m_Drawing->Renderer(), nullptr);
	m_Drawing->SetRenderTargetSDL(original_render_target);

	bool save_result = IMG_SavePNG(sdl_surface, filename.c_str());
	SDL_DestroySurface(sdl_surface);
	if (!save_result)
	{
		std::wcout << Strings::FStringColorsW(L"[Assets] &cFailed to export texture to disk\n");
		std::wcout << Strings::FStringColorsW(L"[Assets] &cReason: %s\n", SDL_GetError());
		return false;
	}

	std::wcout << Strings::FStringColorsW(L"[Assets] &aExported texture to disk as %s\n", filename.c_str());
	return true;
}

void AssetsClass::LinkPreloadedTexture(LinkTexture *link_texture)
{
	if (!sPreloadStage)
		throw std::runtime_error(Strings::FString("You cannot link texture '%s' at this stage",
												  link_texture->m_Key.c_str()));

	m_LinkTextures.push_back(link_texture);
}

void AssetsClass::LinkPregeneratedTexture(PregenerateTexture *pregenerate_texture)
{
	if (!sPreloadStage)
		throw std::runtime_error(Strings::FString("You cannot pre-generate texture '%s' at this stage",
												  pregenerate_texture->m_Key.c_str()));

	m_PregenerateTextures.push_back(pregenerate_texture);
}

void AssetsClass::LinkPreloadedSound(LinkSound *link_sound)
{
	if (!sPreloadStage)
		throw std::runtime_error(Strings::FString("You cannot link sound '%s' at this stage",
												  link_sound->m_Key.c_str()));

	m_LinkSounds.push_back(link_sound);
}

void AssetsClass::LinkPreloadedMusic(LinkMusic *link_music)
{
	if (!sPreloadStage)
		throw std::runtime_error(Strings::FString("You cannot link music '%s' at this stage",
												  link_music->m_Key.c_str()));

	m_LinkMusic.push_back(link_music);
}

void AssetsClass::PreloadFont_(PreloadFont *preload_font)
{
	if (!sPreloadStage)
		throw std::runtime_error(Strings::FString("You cannot pre-load font '%s' at this stage",
												  preload_font->m_Key.c_str()));

	m_PreloadFonts.push_back(preload_font);
}

void AssetsClass::LinkPreloadedFont(LinkFont *link_font)
{
	if (!sPreloadStage)
		throw std::runtime_error(Strings::FString("You cannot link font '%s' at this stage",
												  link_font->m_Key.c_str()));

	m_LinkFonts.push_back(link_font);
}

void AssetsClass::SetMusicVolume(int volume)
{
	Mix_VolumeMusic(volume);
}

void AssetsClass::PauseMusic()
{
	Mix_PauseMusic();
}

void AssetsClass::AutomaticallyDeleteTexture(Texture *texture)
{
	m_AutomaticDeletionTextures.push_back(texture);
}

void AssetsClass::ThreadedLoading()
{
	while (sLoadingStage)
	{
		if (LoadingTextures()) break;
		if (LoadingSounds()) break;
		if (LoadingMusic()) break;
		if (LoadingFonts()) break;
		if (LinkingTextures()) break;
		if (LinkingSounds()) break;
		if (LinkingMusic()) break;
		if (LinkingFonts()) break;
		if (GeneratingTextures()) break;

		sLoadingStage = false;
		std::wcout << Strings::FStringColorsW(L"[Assets] &aWork finished %zu/%zu\n", GetWorkDone(), GetTotalWork());
	}
}

LinkTexture::LinkTexture(std::string
						 texture_key)
	: m_Key(std::move(texture_key))
{
	m_Texture = nullptr;
	m_LoadCallback = [](Texture *)
	{ };

	AssetsClass::LinkPreloadedTexture(this);
}

LinkTexture::LinkTexture(std::string
						 texture_key, TextureCallback
						 load_callback)
	: m_Key(std::move(texture_key))
{
	m_Texture = nullptr;
	m_LoadCallback = std::move(load_callback);

	AssetsClass::LinkPreloadedTexture(this);
}

PregenerateTexture::PregenerateTexture(std::string
									   texture_key, TextureCallback
									   generate_callback)
	: m_Key(std::move(texture_key)),
	  m_GenerateCallback(std::move(generate_callback))
{
	m_Texture = nullptr;

	AssetsClass::LinkPregeneratedTexture(this);
}

LinkSound::LinkSound(std::string
					 sound_key)
	: m_Key(std::move(sound_key))
{
	m_Sound = nullptr;

	AssetsClass::LinkPreloadedSound(this);
}

Sound *LinkSound::GetSound() const
{
	if (m_Sound == nullptr)
		throw std::runtime_error(FString("[Sound] GetSound '%s' was nullptr", m_Key.c_str()));

	return m_Sound;
}

LinkMusic::LinkMusic(std::string
					 music_key)
	: m_Key(std::move(music_key))
{
	m_Music = nullptr;

	AssetsClass::LinkPreloadedMusic(this);
}

Music *LinkMusic::GetMusic() const
{
	if (m_Music == nullptr)
		throw std::runtime_error(FString("[Music] GetMusic '%s' was nullptr", m_Key.c_str()));

	return m_Music;
}

PreloadFont::PreloadFont(std::string
						 font_key, std::string
						 font_id, float
						 ptsize)
	: m_Key(std::move(font_key)),
	  m_FontID(std::move(font_id))
{
	m_Font = nullptr;
	m_Size = ptsize;

	AssetsClass::PreloadFont_(this);
}

Font *PreloadFont::GetFont() const
{
	if (m_Font == nullptr)
		throw std::runtime_error(FString("[Font] GetFont '%s' was nullptr", m_Key.c_str()));

	return m_Font;
}

TTF_Font *PreloadFont::GetTTFFont() const
{
	if (m_Font == nullptr)
		throw std::runtime_error(FString("[Font] GetTTFFont - m_Font '%s' was nullptr", m_Key.c_str()));

	TTF_Font *ttf_font = m_Font->TTFFont();
	if (ttf_font == nullptr)
		throw std::runtime_error(FString("[Font] GetTTFFont - TTF_Font '%s' was nullptr", m_Key.c_str()));

	return ttf_font;
}

LinkFont::LinkFont(std::string
				   font_key)
	: m_Key(std::move(font_key)),
	  m_FontID("NaN")
{
	m_Font = nullptr;
	m_Size = -1;

	AssetsClass::LinkPreloadedFont(this);
}

Font *LinkFont::GetFont() const
{
	if (m_Font == nullptr)
		throw std::runtime_error(FString("[Font] GetFont '%s' was nullptr", m_Key.c_str()));

	return m_Font;
}
