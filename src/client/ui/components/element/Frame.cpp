//
// Created by Matq on 16/12/2024.
//

#include "Frame.h"

Frame::Frame()
    : Element(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
    name = L"Frame";
}

Frame::Frame(const Vec2i& pos, const Vec2i& size, ElementDraw draw)
    : Element(pos, size, draw){
    name = L"Frame";
}

Frame::Frame(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, SDL_Texture* sdl_texture)
    : Element(pos, size, visual, offset, sdl_texture) {
    name = L"Frame";
}

Frame::Frame(const Vec2i& pos, const Vec2i& size, SDL_Texture* sdl_texture)
    : Element(pos, size, size, Vec2i(0, 0), sdl_texture){
    name = L"Frame";
}

Frame::Frame(const Vec2i& pos, const Vec2i& size, const VisualTexture& visual_texture)
    : Element(pos, size, size, Vec2i(0, 0), nullptr){
    name = L"Frame";

    SetDraw(DRAW_VISUAL_TEXTURE);
    SetVisualTexture(visual_texture);
}