//
// Created by Matq on 05/04/2025.
//

#include "ScrollingBackground.h"

namespace Auth {
ScrollingBackground::ScrollingBackground() {
    this->SetDraw(DRAW_TEXTURE);

    this->scrolling_offset = Vec2f(0.0f, 0.0f);
    this->scrolling_direction = Vec2f(1.0f, 0.5f);
    this->upscale_texture = 1.5f;

	start_offset = false;//
}

ScrollingBackground::~ScrollingBackground() {

}

void ScrollingBackground::Tick(double elapsed_seconds) {
    auto texture_size = texture_instance.GetTexture()->GetOriginalSize();

	if (!start_offset)
	{
		start_offset = true;
		scrolling_offset.x = texture_size.x * 0.1f;
		scrolling_offset.y = texture_size.y - (float)size.y / upscale_texture;
	}

    if (scrolling_offset.x < 0.0f) {
        scrolling_offset.x = 0.0f;
        scrolling_direction.x *= -1.0f;
    } else if (scrolling_offset.x + (float)size.x / upscale_texture > (float)texture_size.x) {
        scrolling_offset.x = (float)texture_size.x - (float)size.x / upscale_texture;
        scrolling_direction.x *= -1.0f;
    }
    if (scrolling_offset.y < 0.0f) {
        scrolling_offset.y = 0.0f;
        scrolling_direction.y *= -1.0f;
    } else if (scrolling_offset.y + (float)size.y / upscale_texture > (float)texture_size.y) {
        scrolling_offset.y = (float)texture_size.y - (float)size.y / upscale_texture;
        scrolling_direction.y *= -1.0f;
    }

    scrolling_offset += scrolling_direction * (float)elapsed_seconds * 5.0f;
}

void ScrollingBackground::Render() {
    auto drawing = Assets::Get()->GetDrawing();

    SDL_FRect scrolling_rect = {
        scrolling_offset.x,
        scrolling_offset.y,
        (float)size.x / upscale_texture,
        (float)size.y / upscale_texture,
    };
    drawing->RenderTexture(texture_instance.GetTexture()->SDLTexture(), &scrolling_rect, GetRect());
}
}
