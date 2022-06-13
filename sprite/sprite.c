#include "sprite.h"

void Sprite_Draw(SDL_Renderer *renderer, Sprite sprite) {
  SDL_RenderCopy(renderer, sprite.texture, &sprite.src, &sprite.dst);
}