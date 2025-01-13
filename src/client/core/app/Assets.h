//
// Created by Mat on 12/3/2024.
//

#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#include <vector>
#include <unordered_map>
#include <functional>
#include "SDL3_image/SDL_image.h"
#include "SDL3_mixer/SDL_mixer.h"
#include "SDL3_ttf/SDL_ttf.h"
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
    float m_Width, m_Height;
    float m_WidthHalf, m_HeightHalf;
    std::string m_LoadExtension;

    bool m_AutomaticDeletion;

public:
    explicit Texture(SDL_Texture* sdl_texture = nullptr, std::string key = "NaN", std::string load_extension = "NaN");
    ~Texture();

    // Getting
    [[nodiscard]] SDL_Texture* SDLTexture() const { return m_SDLTexture; }
    [[nodiscard]] float GetWidth() const { return m_Width; }
    [[nodiscard]] float GetHeight() const { return m_Height; }
    [[nodiscard]] Vec2f GetSize() const { return { m_Width, m_Height }; }
    [[nodiscard]] Vec2f GetHalfSize() const { return { m_WidthHalf, m_HeightHalf }; }
    [[nodiscard]] const std::string& Key() const { return m_Key; }

    // Options
    Texture* FlagForAutomaticDeletion();

    // Manipulating
    void SetSDLTexture(SDL_Texture* sdl_texture);
    void SetBlendMode(SDL_BlendMode blend_mode);
    void SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    void SetColorMod(SDL_Color color);
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

class Drawing;
class PreloadTexture;
class PregenerateTexture;
class PreloadSound;
class PreloadMusic;
class PreloadFont;
class LinkFont;
class AssetsClass {
    Drawing* m_Drawing;
    bool m_SoundsEnabled;

    std::unordered_map<std::string, Texture*> m_Textures;
    std::unordered_map<std::string, Sound*> m_Sounds;
    std::unordered_map<std::string, Music*> m_Music;
    std::unordered_map<std::string, Font*> m_Fonts;
    Texture* m_InvalidTextureDefault;

    static std::vector<Texture*> m_AutomaticDeletionTextures;

    static std::vector<PreloadTexture*> m_LinkTextures;
    static std::vector<PregenerateTexture*> m_PregenerateTextures;
    static std::vector<PreloadSound*> m_LinkSounds;
    static std::vector<PreloadMusic*> m_LinkMusic;
    static std::vector<PreloadFont*> m_PreloadFonts;
    static std::vector<LinkFont*> m_LinkFonts;

    void LoadTextures(SDL_Renderer* renderer);
    void LoadSounds();
    void LoadMusic();
    void LoadFonts();

public:
    AssetsClass(Drawing* drawing, bool sounds_enabled);
    ~AssetsClass();

    // Getting
    [[nodiscard]] Drawing* GetDrawing() const { return m_Drawing; }
    [[nodiscard]] Texture* GetTexture(const std::string& texture_key);
    [[nodiscard]] Sound* GetSound(const std::string& sound_key);
    [[nodiscard]] Music* GetMusic(const std::string& music_key);
    [[nodiscard]] Font* GetFont(const std::string& font_key);
    [[nodiscard]] bool SoundsEnabled() const { return m_SoundsEnabled; }

    // Generating
    Texture* TextureFromSurface(SDL_Surface* sdl_surface);
    Texture* CreateTexture(SDL_PixelFormat format, SDL_TextureAccess access, int w, int h);
    Texture* RenderTextBlended(TTF_Font* font, const std::string& text, SDL_Color color);
    Texture* RenderTextBlended(TTF_Font* font, const char* text, SDL_Color color);

    // Manipulating
    static void LinkPreloadedTexture(PreloadTexture* register_texture);
    static void LinkPregeneratedTexture(PregenerateTexture* pregenerate_texture);
    static void LinkPreloadedSound(PreloadSound* register_sound);
    static void LinkPreloadedMusic(PreloadMusic* register_music);
    static void PreloadFont_(PreloadFont* preload_font);
    static void LinkPreloadedFont(LinkFont* register_font);
    static void SetMusicVolume(int volume);
    static void PauseMusic();
    static void AutomaticallyDeleteTexture(Texture* texture);

};

typedef Singleton<AssetsClass> Assets;

class PreloadTexture {
private:
    friend class AssetsClass;
    std::string m_Key;
    Texture* m_Texture;

    using TextureCallback = std::function<void(Texture*)>;
    TextureCallback m_LoadCallback;

public:
    explicit PreloadTexture(std::string texture_key);
    explicit PreloadTexture(std::string texture_key, TextureCallback load_callback);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Texture* GetTexture() const { return m_Texture; }

};

class PregenerateTexture {
private:
    friend class AssetsClass;
    std::string m_Key;
    Texture* m_Texture;

    using TextureCallback = std::function<Texture*(AssetsClass*)>;
    TextureCallback m_GenerateCallback;

public:
    explicit PregenerateTexture(std::string texture_key, TextureCallback generate_callback);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Texture* GetTexture() const { return m_Texture; }

};

class PreloadSound {
private:
    friend class AssetsClass;
    std::string m_Key;
    Sound* m_Sound;

public:
    explicit PreloadSound(std::string sound_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Sound* GetSound() const;

};

class PreloadMusic {
private:
    friend class AssetsClass;
    std::string m_Key;
    Music* m_Music;

public:
    explicit PreloadMusic(std::string music_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Music* GetMusic() const;

};

class PreloadFont {
private:
    friend class AssetsClass;
    std::string m_Key;
    std::string m_FontID;
    int m_Size;
    Font* m_Font;

public:
    explicit PreloadFont(std::string font_key, std::string font_id, int ptsize);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] const std::string& FontID() const { return m_FontID; }
    [[nodiscard]] int Size() const { return m_Size; }
    [[nodiscard]] Font* GetFont() const;

};

class LinkFont {
private:
    friend class AssetsClass;
    std::string m_Key;
    std::string m_FontID;
    int m_Size;
    Font* m_Font;

public:
    explicit LinkFont(std::string font_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] const std::string& FontID() const { return m_FontID; }
    [[nodiscard]] int Size() const { return m_Size; }
    [[nodiscard]] Font* GetFont() const;

};