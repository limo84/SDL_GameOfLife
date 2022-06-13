#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

typedef struct Sprite {
  SDL_Texture *texture;
  SDL_Rect src;
  SDL_Rect dst;
} Sprite;

void Sprite_Draw();

#endif