#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>

#include "../sprite/sprite.h"
#include "program.h"

#define rows 26
#define cols 48

Program program;
Sprite button;

SDL_Point WindowSize = {1200, 650};
SDL_bool grid[rows][cols];
SDL_bool mouseReleased = 1;
SDL_bool mouseButtonDown = 0;
SDL_bool isPaused = 1;

int Program_Run()
{
  Program_Init();
  Program_MainLoop();
  return 0;
}

void Program_Init()
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      grid[i][j] = 0;
    }
  }
  //   SDL_Event event;
  program.isRunning = 1;

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("Video initialization failed\n");
    exit(0);
  }
  program.window =
      SDL_CreateWindow("Game Of Life", SDL_WINDOWPOS_CENTERED, 0, WindowSize.x,
                       WindowSize.y, SDL_WINDOW_SHOWN);
  if (!program.window)
  {
    printf("window failed\n");
    exit(0);
  }
  program.renderer =
      SDL_CreateRenderer(program.window, -1, SDL_RENDERER_ACCELERATED);
  if (!program.renderer)
  {
    printf("renderer failed\n");
    exit(0);
  }

  button.texture = IMG_LoadTexture(program.renderer, "GOL_StartButton.png");
  if (!button.texture)
  {
    printf("texture failed");
    exit(0);
  }
  button.src = (SDL_Rect){0, 0, 128, 64};
  button.dst = (SDL_Rect){WindowSize.x - 164, 32, 128, 64};
}

void Program_MainLoop()
{
  int delta = 501;
  int a = SDL_GetTicks();
  int b = SDL_GetTicks();
  while (program.isRunning)
  {
    Program_ProcessEvents(&program.event, &program.isRunning, &button.src,
                          &button.dst);
    if (delta >= 50)
    {
      // printf("delta: %i\n", delta);
      if (!isPaused)
      {
        Program_ProcessNextStep();
      }
      SDL_RenderClear(program.renderer);
      Program_RenderGrid(program.renderer);
      SDL_RenderCopy(program.renderer, button.texture, &button.src,
                     &button.dst);
      SDL_SetRenderDrawColor(program.renderer, 200, 200, 200, 255);
      SDL_RenderPresent(program.renderer);
      delta = 0;
      b = a;
    }
    a = SDL_GetTicks();
    delta = a - b;
  }
}

void Program_CleanUp() {}

void Program_ProcessEvents(SDL_Event *event, SDL_bool *isRunning,
                           SDL_Rect *button_src, SDL_Rect *button_dst)
{
  SDL_PollEvent(event);
  switch (event->type)
  {
  case SDL_QUIT:
    *isRunning = 0;
    break;
  default:
    break;
  }

  if (event->type == SDL_MOUSEBUTTONDOWN)
  {
    // ONE TIME BUTTON PRESS
    if (event->button.button == SDL_BUTTON_LEFT && mouseReleased == 1)
    {
      if (event->button.x > button_dst->x &&
          event->button.x < button_dst->x + button_dst->w &&
          event->button.y > button_dst->y &&
          event->button.y < button_dst->y + button_dst->h)
      {
        button_src->y = (button_src->y + 64) % 128;
        isPaused = !isPaused;
      }
      mouseReleased = 0;
      mouseButtonDown = 1;
    }
    // CONTINUOUS MOUSE EVENTS
    if (event->button.button == SDL_BUTTON_LEFT)
    {
      int row = event->button.y / 25;
      int col = event->button.x / 25;
      grid[row][col] = 1;
    }

    if (event->button.button == SDL_BUTTON_RIGHT)
    {
      int row = event->button.y / 25;
      int col = event->button.x / 25;
      grid[row][col] = 0;
    }
  }

  if (mouseButtonDown && event->type == SDL_MOUSEMOTION)
  {
    int row = event->button.y / 25;
    int col = event->button.x / 25;
    grid[row][col] = 1;
  }

  if (event->type == SDL_MOUSEBUTTONUP)
  {
    if (event->button.button == SDL_BUTTON_LEFT)
    {
      mouseReleased = 1;
      mouseButtonDown = 0;
    }
  }
  event = NULL;
}

void Program_RenderGrid(SDL_Renderer *renderer)
{
  // RENDER FORMS
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      if (grid[i][j] == 1)
      {
        SDL_Rect rect = {j * 25, i * 25, 25, 25};
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  // RENDER GRID
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  for (int i = 0; i < WindowSize.x; i += 25)
  {
    SDL_RenderDrawLine(renderer, i, 0, i, WindowSize.y);
  }
  for (int i = 0; i < WindowSize.y; i += 25)
  {
    SDL_RenderDrawLine(renderer, 0, i, WindowSize.x, i);
  }
}

void Program_ProcessNextStep()
{
  int aliveNeigbours = 0;
  SDL_bool other[rows][cols];
  // CLEAR OTHER
  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < 48; j++)
    {
      other[i][j] = 0;
    }
  }
  // FILL OTHER
  for (int i = 1; i < 25; i++)
  {
    for (int j = 1; j < 47; j++)
    {
      aliveNeigbours = 0;
      for (int k = -1; k <= 1; k++)
      {
        for (int l = -1; l <= 1; l++)
        {
          if (grid[i + k][j + l] == 1)
          {
            if (!(k == 0 && l == 0))
            {
              aliveNeigbours++;
            }
          }
        }
      }
      if (grid[i][j] == SDL_TRUE && aliveNeigbours < 2)
      {
        other[i][j] = 0;
      }
      else if (grid[i][j] == SDL_TRUE &&
               (aliveNeigbours == 2 || aliveNeigbours == 3))
      {
        other[i][j] = 1;
      }
      else if (grid[i][j] == SDL_TRUE && aliveNeigbours > 3)
      {
        other[i][j] = SDL_FALSE;
      }
      else if (grid[i][j] == SDL_FALSE && aliveNeigbours == 3)
      {
        other[i][j] = SDL_TRUE;
      }
    }
  }

  for (int i = 0; i < 26; i++)
  {
    for (int j = 0; j < 48; j++)
    {
      grid[i][j] = other[i][j];
    }
  }
}