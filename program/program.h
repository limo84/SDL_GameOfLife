#ifndef PROGRAM_H
#define PROGRAM_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>

typedef struct Program {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  SDL_bool isRunning;
} Program;

int Program_Run();
void Program_Init();
void Program_MainLoop();
void Program_CleanUp();

void Program_ProcessEvents(SDL_Event *event, SDL_bool *isRunning, SDL_Rect *,
                           SDL_Rect *);
void Program_RenderGrid(SDL_Renderer *renderer);
void Program_ProcessNextStep();

#endif