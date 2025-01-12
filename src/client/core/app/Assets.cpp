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
#include "../../../shared/core/Strings.h"

using namespace Strings;

template<> AssetsClass* Singleton<AssetsClass>::instance_ = nullptr;
template<> const char* Singleton<AssetsClass>::singleton_name_ = "Assets";
std::vector<Texture*> AssetsClass::m_AutomaticDeletionTextures = { };
std::vector<PreloadTexture*> AssetsClass::m_LinkTextures = { };
std::vector<PreloadSound*> AssetsClass::m_LinkSounds = { };
std::vector<PreloadMusic*> AssetsClass::m_LinkMusic = { };
std::vector<PreloadFont*> AssetsClass::m_PreloadFonts = { };
std::vector<LinkFont*> AssetsClass::m_LinkFonts = { };

Texture::Texture(SDL_Texture* sdl_texture, std::string key, std::string load_extension)
    : m_Key(std::move(key)),
      m_LoadExtension(std::move(load_extension)) {
    m_SDLTexture = sdl_texture;

    SDL_GetTextureSize(m_SDLTexture, &m_Width, &m_Height);

    m_AutomaticDeletion = false;
}

Texture::~Texture() {
    if (m_SDLTexture)
        SDL_DestroyTexture(m_SDLTexture);
}

Texture* Texture::FlagForAutomaticDeletion() {
    if (!this->m_AutomaticDeletion) {
        this->m_AutomaticDeletion = true;
        AssetsClass::AutomaticallyDeleteTexture(this);
    }
    return this;
}

void Texture::SetSDLTexture(SDL_Texture* sdl_texture) {
    m_SDLTexture = sdl_texture;
    SDL_GetTextureSize(m_SDLTexture, &m_Width, &m_Height);
}

void Texture::SetBlendMode(SDL_BlendMode blend_mode) {
    SDL_SetTextureBlendMode(m_SDLTexture, blend_mode);
}

void Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(m_SDLTexture, r, g, b);
}

void Texture::SetAlphaMod(int alpha) {
    SDL_SetTextureAlphaMod(m_SDLTexture, alpha);
}

Sound::Sound(std::string key, Mix_Chunk* mix_chunk, std::string load_extension)
    : m_Key(std::move(key)),
      m_LoadExtension(std::move(load_extension)) {
    m_MixChunk = mix_chunk;
}

Sound::~Sound() {
    if (m_MixChunk)
        Mix_FreeChunk(m_MixChunk);
}

void Sound::SetVolume(int volume) {
    Mix_VolumeChunk(m_MixChunk, volume);
}

void Sound::PlaySound() {
    if (m_MixChunk == nullptr || !Assets::Get()->SoundsEnabled())
        return;

    Mix_PlayChannel(-1, m_MixChunk, 0);
}

Music::Music(std::string key, Mix_Music* mix_music, std::string load_extension)
    : m_Key(std::move(key)),
      m_LoadExtension(std::move(load_extension)) {
    m_MixMusic = mix_music;
}

Music::~Music() {
    if (m_MixMusic)
        Mix_FreeMusic(m_MixMusic);
}

void Music::PlayMusic(int loops) {
    if (m_MixMusic == nullptr || !Assets::Get()->SoundsEnabled())
        return;

    Mix_PlayMusic(m_MixMusic, loops);
}

Font::Font(TTF_Font* ttf_font, std::string key, std::string load_extension)
    : m_Key(std::move(key)),
      m_LoadExtension(std::move(load_extension)) {
    m_TTFFont = ttf_font;
}

Font::~Font() {
    if (m_TTFFont)
        TTF_CloseFont(m_TTFFont);
}

std::vector<std::tuple<std::string, std::string, std::string>> GetResourceKeys(const char* directory,
                                                                               const std::unordered_set<std::string>& supported_extensions) {
    std::vector<std::tuple<std::string, std::string, std::string>> results;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
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
        if (supported_extensions.find(extension) == supported_extensions.end()) {
            std::wcout << FStringColorsW(L"[Assets] &8Unsupported texture format '%s' for '%s'",
                                         extension.c_str(),
                                         identificator.c_str()) << std::endl;
            continue;
        }

        // Add the key and path to the results
        results.emplace_back(identificator, file_path.string(), extension);
    }

    return results;
}

void AssetsClass::LoadTextures(SDL_Renderer* renderer) {
    // Load
    std::unordered_set<std::string> texture_extensions = {
        ".png", ".jpg", ".jpeg", ".bmp", ".gif", ".tif", ".tiff", ".webp"
    };
    auto texture_keys = GetResourceKeys(R"(.\assets\images\)", texture_extensions);
    for (auto entry : texture_keys) {
        std::string& key = std::get<0>(entry);
        std::string& file_path = std::get<1>(entry);
        std::string& extension = std::get<2>(entry);

        auto it = m_Textures.find(key);
        if (it != m_Textures.end()) {
            std::wcout << FStringColorsW(L"[Assets] &8Duplicate texture '%s' for existing '%s'(%s)",
                                         extension.c_str(),
                                         key.c_str(),
                                         it->second->m_LoadExtension.c_str()) << std::endl;
            continue;
        }

        // Load the texture
        SDL_Surface* TempSurface = IMG_Load(file_path.c_str());
        if (!TempSurface) {
            std::wcout << FStringColorsW(L"[Assets] &cFailed to load texture '%s'", file_path.c_str()) << std::endl;
            std::wcout << FStringColorsW(L"[Assets] &cReason (%s)", SDL_GetError()) << std::endl;
            continue;
        }

        SDL_Texture* NewSDLTexture = SDL_CreateTextureFromSurface(renderer, TempSurface);
        SDL_DestroySurface(TempSurface);

        // Add it to all the textures
        auto new_texture = (new Texture(NewSDLTexture, key, extension))
            ->FlagForAutomaticDeletion();
        m_Textures[key] = new_texture;
    }
    std::wcout << FStringColorsW(L"[Assets] &5Loaded %i textures", m_Textures.size()) << std::endl;
    m_InvalidTextureDefault = nullptr;
    m_InvalidTextureDefault = GetTexture("invalid");

    // Link
    for (auto required_texture : m_LinkTextures) {
        const std::string& texture_key = required_texture->Key();

        required_texture->m_Texture = GetTexture(texture_key);
    }
    std::wcout << FStringColorsW(L"[Assets] &5Linked %zu textures", m_LinkTextures.size()) << std::endl;
    m_LinkTextures.clear();
}

void AssetsClass::LoadSounds() {
    // Load
    std::unordered_set<std::string> sound_extensions = {
        ".wav", ".aiff", ".voc", ".mp3", ".ogg", ".flac",
        ".mod", ".s3m", ".it", ".xm", ".mid", ".midi", ".opus"
    };
    auto sound_keys = GetResourceKeys(R"(.\assets\sounds\)", sound_extensions);
    for (auto entry : sound_keys) {
        std::string& key = std::get<0>(entry);
        std::string& file_path = std::get<1>(entry);
        std::string& extension = std::get<2>(entry);

        auto it = m_Sounds.find(key);
        if (it != m_Sounds.end()) {
            std::wcout << FStringColorsW(L"[Assets] &8Duplicate sound '%s' for existing '%s'(%s)",
                                         extension.c_str(),
                                         key.c_str(),
                                         it->second->m_LoadExtension.c_str()) << std::endl;
            continue;
        }

        // Load the sound
        Mix_Chunk* NewMixChunk = Mix_LoadWAV(file_path.c_str());
        if (!NewMixChunk) {
            std::wcout << FStringColorsW(L"[Assets] &cFailed to load sound '%s'", file_path.c_str()) << std::endl;
            std::wcout << FStringColorsW(L"[Assets] &cReason (%s)", SDL_GetError()) << std::endl;
            continue;
        }

        // Add it to all the textures
        auto new_sound = new Sound(key, NewMixChunk, extension);
        m_Sounds[key] = new_sound;
    }
    std::wcout << FStringColorsW(L"[Assets] &5Loaded %i sounds", m_Sounds.size()) << std::endl;

    // Link
    for (auto required_sound : m_LinkSounds) {
        const std::string& sound_key = required_sound->Key();

        required_sound->m_Sound = GetSound(sound_key);
    }
    std::wcout << FStringColorsW(L"[Assets] &5Linked %zu sounds", m_LinkSounds.size()) << std::endl;
    m_LinkSounds.clear();
}

void AssetsClass::LoadMusic() {
    // Load
    std::unordered_set<std::string> sound_extensions = {
        ".wav", ".aiff", ".voc", ".mp3", ".ogg", ".flac",
        ".mod", ".s3m", ".it", ".xm", ".mid", ".midi", ".opus"
    };
    auto music_keys = GetResourceKeys(R"(.\assets\music\)", sound_extensions);
    for (auto entry : music_keys) {
        std::string& key = std::get<0>(entry);
        std::string& file_path = std::get<1>(entry);
        std::string& extension = std::get<2>(entry);

        auto it = m_Music.find(key);
        if (it != m_Music.end()) {
            std::wcout << FStringColorsW(L"[Assets] &8Duplicate music '%s' for existing '%s'(%s)",
                                         extension.c_str(),
                                         key.c_str(),
                                         it->second->m_LoadExtension.c_str()) << std::endl;
            continue;
        }

        // Load the sound
        Mix_Music* NewMixMusic = Mix_LoadMUS(file_path.c_str());
        if (!NewMixMusic) {
            std::wcout << FStringColorsW(L"[Assets] &cFailed to load music '%s'", file_path.c_str()) << std::endl;
            std::wcout << FStringColorsW(L"[Assets] &cReason (%s)", SDL_GetError()) << std::endl;
            continue;
        }

        // Add it to all the textures
        auto new_music = new Music(key, NewMixMusic, extension);
        m_Music[key] = new_music;
    }
    std::wcout << FStringColorsW(L"[Assets] &5Loaded %i music", m_Music.size()) << std::endl;

    // Link
    for (auto required_music : m_LinkMusic) {
        const std::string& music_key = required_music->Key();

        required_music->m_Music = GetMusic(music_key);
    }
    std::wcout << FStringColorsW(L"[Assets] &5Linked %zu music", m_LinkMusic.size()) << std::endl;
    m_LinkMusic.clear();
}

void AssetsClass::LoadFonts() {
    std::unordered_set<std::string> font_extensions = {
        ".ttf", ".otf", ".fon", ".fnt",
        ".bdf", ".pcf", ".sfnt", ".ttc",
    };
    auto font_paths = GetResourceKeys(R"(.\assets\fonts\)", font_extensions);
    for (auto required_font : m_PreloadFonts) {
        const std::string& font_key = required_font->Key();
        const std::string& font_id = required_font->FontID();
        int font_size = required_font->Size();

        for (auto entry : font_paths) {
            std::string& id = std::get<0>(entry);
            std::string& file_path = std::get<1>(entry);
            std::string& extension = std::get<2>(entry);

            if (font_id == id) {
                auto ttf_font = TTF_OpenFont(file_path.c_str(), font_size);
                if (!ttf_font)
                    throw std::runtime_error(FString("Failed to load font '%s' because (%s)",
                                                     file_path.c_str(),
                                                     SDL_GetError()));

                auto new_font = new Font(ttf_font, font_key, extension);
                required_font->m_Font = new_font;
                m_Fonts[font_key] = new_font;

                break;
            }
        }
    }

    // Link
    for (auto required_font : m_LinkFonts) {
        const std::string& font_key = required_font->Key();

        required_font->m_Font = GetFont(font_key);
    }

    auto loaded_fonts_message = FStringW(L"[Assets] &5Loaded %zu fonts", m_PreloadFonts.size());
    if (!m_LinkFonts.empty())
        loaded_fonts_message += FStringW(L" &d(%zu linked)", m_LinkFonts.size());
    std::wcout << FStringColorsW(L"%ls", loaded_fonts_message.c_str()) << std::endl;

    m_PreloadFonts.clear();
    m_LinkFonts.clear();
}

AssetsClass::AssetsClass(SDL_Renderer* renderer, bool sounds_enabled)
    : m_InvalidTextureDefault(nullptr) {
    m_Renderer = renderer;
    m_SoundsEnabled = sounds_enabled;

    // Todo: Loading assets in realtime (adding/removing files?)
    LoadTextures(renderer);
    LoadSounds();
    LoadMusic();
    LoadFonts();
}

AssetsClass::~AssetsClass() {
    size_t preloaded_textures = m_Textures.size(); // Also deleted here
    size_t destroyed_textures = 0;
    size_t destroyed_sounds = 0;
    size_t destroyed_music = 0;
    size_t destroyed_fonts = 0;
    for (const auto& texture : m_AutomaticDeletionTextures) {
        delete texture;
        destroyed_textures++;
    }
    for (const auto& entry : m_Sounds) {
        delete entry.second;
        destroyed_sounds++;
    }
    for (const auto& entry : m_Music) {
        delete entry.second;
        destroyed_music++;
    }
    for (const auto& entry : m_Fonts) {
        delete entry.second;
        destroyed_fonts++;
    }

    auto other_destroyed_textures = destroyed_textures - preloaded_textures;
    auto unloaded_textures_message = FStringW(L"[Assets] &5Unloaded %zu textures", preloaded_textures);
    if (other_destroyed_textures > 0)
        unloaded_textures_message += FStringW(L" &d(+%zu dynamic)", other_destroyed_textures);
    std::wcout << FStringColorsW(L"%ls", unloaded_textures_message.c_str()) << std::endl;
    std::wcout << FStringColorsW(L"[Assets] &5Unloaded %zu sounds", destroyed_sounds) << std::endl;
    std::wcout << FStringColorsW(L"[Assets] &5Unloaded %zu music", destroyed_music) << std::endl;
    std::wcout << FStringColorsW(L"[Assets] &5Unloaded %zu fonts", destroyed_fonts) << std::endl;
}

Texture* AssetsClass::GetTexture(const std::string& texture_key) {
    auto it = m_Textures.find(texture_key);
    if (it != m_Textures.end())
        return it->second;

    std::wcout << FStringColorsW(L"[Assets] &8Texture '%s' not found", texture_key.c_str()) << std::endl;
    return m_InvalidTextureDefault;
}

Sound* AssetsClass::GetSound(const std::string& sound_key) {
    auto it = m_Sounds.find(sound_key);
    if (it != m_Sounds.end())
        return it->second;

    std::wcout << FStringColorsW(L"[Assets] &8Sound '%s' not found", sound_key.c_str()) << std::endl;
    return nullptr;
}

Music* AssetsClass::GetMusic(const std::string& music_key) {
    auto it = m_Music.find(music_key);
    if (it != m_Music.end())
        return it->second;

    std::wcout << FStringColorsW(L"[Assets] &8Music '%s' not found", music_key.c_str()) << std::endl;
    return nullptr;
}

Font* AssetsClass::GetFont(const std::string& font_key) {
    auto it = m_Fonts.find(font_key);
    if (it != m_Fonts.end())
        return it->second;

    std::wcout << FStringColorsW(L"[Assets] &8Font '%s' not found", font_key.c_str()) << std::endl;
    return nullptr;
}

Texture* AssetsClass::TextureFromSurface(SDL_Surface* sdl_surface) {
    SDL_Texture* NewSDLTexture = SDL_CreateTextureFromSurface(m_Renderer, sdl_surface);

    return new Texture(NewSDLTexture, "FromSurface", "NaN");
}

Texture* AssetsClass::CreateTexture(SDL_PixelFormat format, SDL_TextureAccess access, int w, int h) {
    SDL_Texture* NewSDLTexture = SDL_CreateTexture(m_Renderer, format, access, w, h);

    return new Texture(NewSDLTexture, "CreateTexture", "NaN");
}

void AssetsClass::LinkPreloadedTexture(PreloadTexture* link_texture) {
    m_LinkTextures.push_back(link_texture);
}

void AssetsClass::LinkPreloadedSound(PreloadSound* link_sound) {
    m_LinkSounds.push_back(link_sound);
}

void AssetsClass::LinkPreloadedMusic(PreloadMusic* link_music) {
    m_LinkMusic.push_back(link_music);
}

void AssetsClass::PreloadFont_(PreloadFont* preload_font) {
    m_PreloadFonts.push_back(preload_font);
}

void AssetsClass::LinkPreloadedFont(LinkFont* link_font) {
    m_LinkFonts.push_back(link_font);
}

void AssetsClass::SetMusicVolume(int volume) {
    Mix_VolumeMusic(volume);
}

void AssetsClass::PauseMusic() {
    Mix_PauseMusic();
}

void AssetsClass::AutomaticallyDeleteTexture(Texture* texture) {
    m_AutomaticDeletionTextures.push_back(texture);
}

PreloadTexture::PreloadTexture(std::string texture_key)
    : m_Key(std::move(texture_key)) {
    m_Texture = nullptr;

    AssetsClass::LinkPreloadedTexture(this);
}

PreloadSound::PreloadSound(std::string sound_key)
    : m_Key(std::move(sound_key)) {
    m_Sound = nullptr;

    AssetsClass::LinkPreloadedSound(this);
}

Sound* PreloadSound::GetSound() const {
    if (m_Sound == nullptr)
        throw std::runtime_error(FString("[Sound] GetSound '%s' was nullptr", m_Key.c_str()));

    return m_Sound;
}

PreloadMusic::PreloadMusic(std::string music_key)
    : m_Key(std::move(music_key)) {
    m_Music = nullptr;

    AssetsClass::LinkPreloadedMusic(this);
}

Music* PreloadMusic::GetMusic() const {
    if (m_Music == nullptr)
        throw std::runtime_error(FString("[Music] GetMusic '%s' was nullptr", m_Key.c_str()));

    return m_Music;
}

PreloadFont::PreloadFont(std::string font_key, std::string font_id, int ptsize)
    : m_Key(std::move(font_key)),
      m_FontID(std::move(font_id)) {
    m_Font = nullptr;
    m_Size = ptsize;

    AssetsClass::PreloadFont_(this);
}

Font* PreloadFont::GetFont() const {
    if (m_Font == nullptr)
        throw std::runtime_error(FString("[Font] GetFont '%s' was nullptr", m_Key.c_str()));

    return m_Font;
}

LinkFont::LinkFont(std::string font_key)
    : m_Key(std::move(font_key)),
      m_FontID("NaN") {
    m_Font = nullptr;
    m_Size = -1;

    AssetsClass::LinkPreloadedFont(this);
}

Font* LinkFont::GetFont() const {
    if (m_Font == nullptr)
        throw std::runtime_error(FString("[Font] GetFont '%s' was nullptr", m_Key.c_str()));

    return m_Font;
}