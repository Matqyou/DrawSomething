//
// Created by Mat on 12/3/2024.
//

#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "../../../shared/core/Singleton.h"
#include "../../../shared/core/Vec2.h"

struct TextureInfo {
    Uint32 format;
    int access, w, h;
};

class AssetsClass;
class Texture {
private:
    friend class AssetsClass;
    const std::string m_Key;
    SDL_Texture* m_SDLTexture;
    TextureInfo m_Information;
    std::string m_LoadExtension;

    bool m_AutomaticDeletion;

public:
    explicit Texture(SDL_Texture* sdl_texture = nullptr, std::string key = "NaN", std::string load_extension = "NaN");
    ~Texture();

    // Getting
    [[nodiscard]] SDL_Texture* SDLTexture() const { return m_SDLTexture; }
    [[nodiscard]] Uint32 GetFormat() const { return m_Information.format; }
    [[nodiscard]] int GetAccess() const { return m_Information.access; }
    [[nodiscard]] int GetWidth() const { return m_Information.w; }
    [[nodiscard]] int GetHeight() const { return m_Information.h; }
    [[nodiscard]] Vec2i GetSize() const { return { m_Information.w, m_Information.h }; }
    [[nodiscard]] const std::string& Key() const { return m_Key; }

    // Options
    Texture* FlagForAutomaticDeletion();

    // Manipulating
    void SetSDLTexture(SDL_Texture* sdl_texture);
    void SetBlendMode(SDL_BlendMode blend_mode);
    void SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    void SetAlphaMod(int alpha);
};

class Sound {
private:
    friend class AssetsClass;
    const std::string m_Key;
    Mix_Chunk* m_MixChunk;
    std::string m_LoadExtension;

public:
    explicit Sound(std::string key = "NaN", Mix_Chunk* mix_chunk = nullptr, std::string load_extension = "NaN");
    ~Sound();

    // Getting
    [[nodiscard]] Mix_Chunk* MixChunk() const { return m_MixChunk; }

    // Manipulating
    void SetVolume(int volume);
    void PlaySound();
};

class Music {
private:
    friend class AssetsClass;
    const std::string m_Key;
    Mix_Music* m_MixMusic;
    std::string m_LoadExtension;

public:
    explicit Music(std::string key = "NaN", Mix_Music* mix_music = nullptr, std::string load_extension = "NaN");
    ~Music();

    // Getting
    [[nodiscard]] Mix_Music* MixMusic() const { return m_MixMusic; }

    // Manipulating
    void PlayMusic(int loops);

};

class Font {
private:
    friend class AssetsClass;
    const std::string m_Key;
    TTF_Font* m_TTFFont;
    std::string m_LoadExtension;

public:
    explicit Font(TTF_Font* ttf_font = nullptr, std::string key = "NaN", std::string load_extension = "NaN");
    ~Font();

    // Getting
    [[nodiscard]] TTF_Font* TTFFont() const { return m_TTFFont; }

};

class LoadedTexture;
class LoadedSound;
class LoadedMusic;
class LoadedFont;
class AssetsClass {
    SDL_Renderer* m_Renderer;
    bool m_SoundsEnabled;

    std::unordered_map<std::string, Texture*> m_Textures;
    std::unordered_map<std::string, Sound*> m_Sounds;
    std::unordered_map<std::string, Music*> m_Music;
    std::unordered_map<std::string, Font*> m_Fonts;
    Texture* m_InvalidTexture;

    static std::vector<Texture*> m_AutomaticDeletionTextures;

    static std::vector<LoadedTexture*> m_RegisterTextures;
    static std::vector<LoadedSound*> m_RegisterSounds;
    static std::vector<LoadedMusic*> m_RegisterMusic;
    static std::vector<LoadedFont*> m_RegisterFonts;

    void LoadTextures(SDL_Renderer* renderer);
    void LoadSounds();
    void LoadMusic();
    void LoadFonts();

public:
    AssetsClass(SDL_Renderer* renderer, bool sounds_enabled);
    ~AssetsClass();

    // Getting
    Texture* GetTexture(const std::string& texture_key);
    Sound* GetSound(const std::string& sound_key);
    Music* GetMusic(const std::string& music_key);
    bool SoundsEnabled() const { return m_SoundsEnabled; }

    // Generating
    Texture* TextureFromSurface(SDL_Surface* sdl_surface);
    Texture* CreateTexture(Uint32 format, int access, int w, int h);

    // Manipulating
    static void RequireTexture(LoadedTexture* register_texture);
    static void RequireSound(LoadedSound* register_sound);
    static void RequireMusic(LoadedMusic* register_music);
    static void RequireFont(LoadedFont* register_font);
    static void SetMusicVolume(int volume);
    static void PauseMusic();
    static void AutomaticallyDeleteTexture(Texture* texture);

};

typedef Singleton<AssetsClass> Assets;

class LoadedTexture {
private:
    friend class AssetsClass;
    std::string m_Key;
    Texture* m_Texture;

public:
    explicit LoadedTexture(std::string texture_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Texture* GetTexture() const { return m_Texture; }

};

class LoadedSound {
private:
    friend class AssetsClass;
    std::string m_Key;
    Sound* m_Sound;

public:
    explicit LoadedSound(std::string sound_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Sound* GetSound() const;

};

class LoadedMusic {
private:
    friend class AssetsClass;
    std::string m_Key;
    Music* m_Music;

public:
    explicit LoadedMusic(std::string music_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Music* GetMusic() const;

};

class LoadedFont {
private:
    friend class AssetsClass;
    std::string m_Key;
    int m_Size;
    Font* m_Font;

public:
    explicit LoadedFont(std::string font_key, int ptsize);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] int Size() const { return m_Size; }
    [[nodiscard]] Font* GetFont() const;

};