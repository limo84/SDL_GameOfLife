#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

#define rows 26
#define cols 48

SDL_Point WindowSize = {1200, 650};
SDL_bool grid[rows][cols];


void processEvents(SDL_Event *event, SDL_bool *isRunning);
void renderGrid(SDL_Renderer *renderer);
void processNextStep();

int main() {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      grid[i][j] = 0;
    }
  }
  grid[1][1] = 1;
  grid[1][2] = 1;
  grid[2][1] = 1;
  grid[2][2] = 1;

  grid[5][2] = 1;
  grid[5][3] = 1;
  grid[5][4] = 1;
  grid[5][5] = 1;

  grid[10][1] = 1;
  grid[10][2] = 1;
  grid[10][3] = 1;
  grid[10][4] = 1;
  grid[11][5] = 1;

  grid[10][3] = 1;
  grid[10][4] = 1;
  grid[10][5] = 1;
  grid[9][5] = 1;
  grid[8][4] = 1;
  SDL_Event event;
  SDL_bool isRunning = 1;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("Video initialization failed\n");
    exit(0);
  }
  SDL_Window *window =
      SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, 0, WindowSize.x,
                       WindowSize.y, SDL_WINDOW_SHOWN);
  if (!window) {
    printf("window failed\n");
    exit(0);
  }
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("renderer failed\n");
    exit(0);
  }

  int delta = 501;
  int a = SDL_GetTicks();
  int b = SDL_GetTicks();
  while (isRunning) {
    processEvents(&event, &isRunning);
    if (delta > 300) {
      printf("delta: %i\n", delta);
      processNextStep();
      SDL_RenderClear(renderer);
      renderGrid(renderer);
      SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
      SDL_RenderPresent(renderer);
      delta = 0;
      b = a;
    }
    a = SDL_GetTicks();
    delta = a - b;
  }
  return 0;
}

void processEvents(SDL_Event *event, SDL_bool *isRunning) {
  SDL_PollEvent(event);
  switch ((*event).type) {
  case SDL_QUIT:
    *isRunning = 0;
    break;
  default:
    break;
  }
}

void renderGrid(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (grid[i][j] == 1) {
        SDL_Rect rect = {i * 25, j * 25, 25, 25};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int i = 0; i < WindowSize.x; i += 25) {
    SDL_RenderDrawLine(renderer, i, 0, i, WindowSize.y);
  }
  for (int i = 0; i < WindowSize.y; i += 25) {
    SDL_RenderDrawLine(renderer, 0, i, WindowSize.x, i);
  }
}

void processNextStep() {
  int aliveNeigbours = 0;
  SDL_bool other[48][26];
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 48; j++) {
      other[i][j] = 0;
    }
  }
  for (int i = 1; i < 25; i++) {
    for (int j = 1; j < 47; j++) {
      aliveNeigbours = 0;
      for (int k = -1; k <= 1; k++) {
        for (int l = -1; l <= 1; l++) {
          if (grid[i + k][j + l] == 1) {
            if (!(k == 0 && l == 0)) {
              aliveNeigbours++;
            }
          }
        }
      }
      if (grid[i][j] == SDL_TRUE && aliveNeigbours < 2) {
        other[i][j] = 0;
      } else if (grid[i][j] == SDL_TRUE &&
                 (aliveNeigbours == 2 || aliveNeigbours == 3)) {
        other[i][j] = 1;
      } else if (grid[i][j] == SDL_TRUE && aliveNeigbours > 3) {
          other[i][j] = SDL_FALSE;
      } else if (grid[i][j] == SDL_FALSE && aliveNeigbours == 3) {
          other[i][j] = SDL_TRUE;
      }
    }
  }

  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 48; j++) {
      grid[i][j] = other[i][j];
    }
  }
}