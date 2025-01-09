//
// Created by Matq on 16/12/2024.
//

#include "Frame.h"

Frame::Frame(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_FRAME, pos, size, DRAW_RECT) {

}

Frame::Frame(const Vec2i& pos, const Vec2i& size, ElementDraw draw)
    : Element(ELEMENT_FRAME, pos, size, draw){

}

Frame::Frame(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset)
    : Element(ELEMENT_FRAME, pos, size, visual, offset) {

}

Frame::Frame(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture)
    : Element(ELEMENT_FRAME, pos, size, visual, offset, texture) {

}

Frame::Frame(const Vec2i& pos, const Vec2i& size, Texture* texture)
    : Element(ELEMENT_FRAME, pos, size, size, Vec2i(0, 0), texture){

}