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
class TextureData {
private:
    friend class AssetsClass;
    const std::string m_Key;
    SDL_Texture* m_SDLTexture;
    float m_Width, m_Height;
    float m_WidthHalf, m_HeightHalf;
    std::string m_LoadExtension;

    bool m_AutomaticDeletion;

public:
    explicit TextureData(SDL_Texture* sdl_texture = nullptr, std::string key = "NaN", std::string load_extension = "NaN");
    ~TextureData();

    // Getting
    [[nodiscard]] SDL_Texture* SDLTexture() const { return m_SDLTexture; }
    [[nodiscard]] float GetWidth() const { return m_Width; }
    [[nodiscard]] float GetHeight() const { return m_Height; }
    [[nodiscard]] Vec2f GetSize() const { return { m_Width, m_Height }; }
    [[nodiscard]] Vec2f GetHalfSize() const { return { m_WidthHalf, m_HeightHalf }; }
    [[nodiscard]] const std::string& Key() const { return m_Key; }

    // Options
    TextureData* FlagForAutomaticDeletion();
    TextureData* SetSDLTexture(SDL_Texture* sdl_texture);
    TextureData* SetBlendMode(SDL_BlendMode blend_mode);
    TextureData* SetColorMod(Uint8 r, Uint8 g, Uint8 b);
    TextureData* SetColorMod(SDL_Color color);
    TextureData* SetColorModFloat(float r, float g, float b);
    TextureData* SetColorModFloat(SDL_FColor color);
    TextureData* SetAlphaMod(int alpha);
    TextureData* SetScaleMode(SDL_ScaleMode scale_mode);
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
    std::string m_LoadExtension;
    TTF_Font* m_TTFFont;
    float m_Size;

public:
    explicit Font(TTF_Font* ttf_font = nullptr, std::string key = "NaN", std::string load_extension = "NaN");
    ~Font();

    // Getting
    [[nodiscard]] TTF_Font* TTFFont() const { return m_TTFFont; }

    // Manipulating
    Font* CacheSize(float size) {
        this->m_Size = size;
        return this;
    }

};

class Drawing;
class LinkTexture;
class PregenerateTexture;
class LinkSound;
class LinkMusic;
class PreloadFont;
class LinkFont;
class AssetsClass { // This class should be completely extern
    Drawing* m_Drawing;
    bool m_SoundsEnabled;

    using Resources = std::vector<std::tuple<std::string, std::string, std::string>>;
    Resources m_TextureResources;
    Resources m_SoundResources;
    Resources m_MusicResources;
    Resources m_FontResources;

    std::unordered_map<std::string, TextureData*> m_Textures;
    std::unordered_map<std::string, Sound*> m_Sounds;
    std::unordered_map<std::string, Music*> m_Music;
    std::unordered_map<std::string, Font*> m_Fonts;
    TextureData* m_InvalidTextureDefault;

    static std::vector<PreloadFont*> m_PreloadFonts;
    static std::vector<LinkTexture*> m_LinkTextures;
    static std::vector<LinkSound*> m_LinkSounds;
    static std::vector<LinkMusic*> m_LinkMusic;
    static std::vector<LinkFont*> m_LinkFonts;
    static std::vector<PregenerateTexture*> m_PregenerateTextures;

    // Iterators
    Resources::iterator m_TextureResourcesIterator;
    Resources::iterator m_SoundResourcesIterator;
    Resources::iterator m_MusicResourcesIterator;
    std::vector<PreloadFont*>::iterator m_PreloadFontIterator;
    static std::vector<LinkTexture*>::iterator m_LinkTexturesIterator;
    static std::vector<LinkSound*>::iterator m_LinkSoundsIterator;
    static std::vector<LinkMusic*>::iterator m_LinkMusicIterator;
    static std::vector<LinkFont*>::iterator m_LinkFontsIterator;
    static std::vector<PregenerateTexture*>::iterator m_PregenerateTexturesIterator;

    static size_t sTotalWork, sWorkDone;

    static std::vector<TextureData*> m_AutomaticDeletionTextures;
    static bool sPreloadStage;
    static bool sLoadingStage;

    bool LoadingTextures();
    bool LoadingSounds();
    bool LoadingMusic();
    bool LoadingFonts(); // All at once
    bool GeneratingTextures();
    bool LinkingTextures(); // All at once
    bool LinkingSounds(); // All at once
    bool LinkingMusic(); // All at once
    bool LinkingFonts(); // All at once

public:
    AssetsClass(Drawing* drawing, bool sounds_enabled);
    ~AssetsClass();

    // Getting
    [[nodiscard]] Drawing* GetDrawing() const { return m_Drawing; }
    [[nodiscard]] TextureData* GetInvalidTexture() const { return m_InvalidTextureDefault; };
    [[nodiscard]] TextureData* GetTexture(const std::string& texture_key);
    [[nodiscard]] Sound* GetSound(const std::string& sound_key);
    [[nodiscard]] Music* GetMusic(const std::string& music_key);
    [[nodiscard]] Font* GetFont(const std::string& font_key);
    [[nodiscard]] bool SoundsEnabled() const { return m_SoundsEnabled; }
    [[nodiscard]] static bool IsLoading() { return sLoadingStage; }
    [[nodiscard]] static size_t GetTotalWork() { return sTotalWork; }
    [[nodiscard]] static size_t GetWorkDone() { return sWorkDone; }

    // Generating
    TextureData* TextureFromSurface(SDL_Surface* sdl_surface);
    SDL_Surface* CreateSDLSurface(int width, int height, SDL_PixelFormat format);
    TextureData* CreateTexture(SDL_PixelFormat format, SDL_TextureAccess access, int w, int h);
    TextureData* CopyTexture(SDL_Texture* copy_texture, SDL_TextureAccess access);
    TextureData* RenderTextBlended(TTF_Font* font, const std::string& text, SDL_Color color);
    TextureData* RenderTextBlended(TTF_Font* font, const char* text, SDL_Color color);
    TextureData* RenderTextSolid(TTF_Font* font, const std::string& text, SDL_Color color);
    TextureData* RenderTextSolid(TTF_Font* font, const char* text, SDL_Color color);
    bool SaveTextureToDisk(TextureData* texture, const std::string& filename);

    // Manipulating
    static void LinkPreloadedTexture(LinkTexture* register_texture);
    static void LinkPregeneratedTexture(PregenerateTexture* pregenerate_texture);
    static void LinkPreloadedSound(LinkSound* register_sound);
    static void LinkPreloadedMusic(LinkMusic* register_music);
    static void PreloadFont_(PreloadFont* preload_font);
    static void LinkPreloadedFont(LinkFont* register_font);
    static void SetMusicVolume(int volume);
    static void PauseMusic();
    static void AutomaticallyDeleteTexture(TextureData* texture);
    void ThreadedLoading();

};

typedef Singleton<AssetsClass> Assets;

class LinkTexture {
private:
    friend class AssetsClass;
    std::string m_Key;
    TextureData* m_Texture;

    using TextureCallback = std::function<void(TextureData*)>;
    TextureCallback m_LoadCallback;

public:
    explicit LinkTexture(std::string texture_key);
    explicit LinkTexture(std::string texture_key, TextureCallback load_callback);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] TextureData* GetTexture() const { return m_Texture; } // todo: error handling
    [[nodiscard]] SDL_Texture* GetSDLTexture() const { // todo: error handling
        return m_Texture->SDLTexture();
    }

};

class PregenerateTexture {
private:
    friend class AssetsClass;
    std::string m_Key;
    TextureData* m_Texture;

    using TextureCallback = std::function<TextureData*(AssetsClass*)>;
    TextureCallback m_GenerateCallback;

public:
    explicit PregenerateTexture(std::string texture_key, TextureCallback generate_callback);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] TextureData* GetTexture() const { return m_Texture; } // todo: error handling
    [[nodiscard]] SDL_Texture* GetSDLTexture() const {
        return m_Texture->SDLTexture(); // todo: error handling
    }

};

class LinkSound {
private:
    friend class AssetsClass;
    std::string m_Key;
    Sound* m_Sound;

public:
    explicit LinkSound(std::string sound_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Sound* GetSound() const;

};

class LinkMusic {
private:
    friend class AssetsClass;
    std::string m_Key;
    Music* m_Music;

public:
    explicit LinkMusic(std::string music_key);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] Music* GetMusic() const;

};

class PreloadFont {
private:
    friend class AssetsClass;
    std::string m_Key;
    std::string m_FontID;
    float m_Size;
    Font* m_Font;

public:
    explicit PreloadFont(std::string font_key, std::string font_id, float ptsize);

    // Getting
    [[nodiscard]] const std::string& Key() const { return m_Key; }
    [[nodiscard]] const std::string& FontID() const { return m_FontID; }
    [[nodiscard]] float Size() const { return m_Size; }
    [[nodiscard]] Font* GetFont() const;
    [[nodiscard]] TTF_Font* GetTTFFont() const;

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