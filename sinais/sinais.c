#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_textengine.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>

struct Window {
  char *title;
  int width;
  int height;
};

struct Window g_window = {"Sinais do Outro Lado", 800, 400};

typedef struct {
  uint8_t r, g, b, a;
} Color;

struct Text {
  char *text, *font_path;
  float pos_x, pos_y, pts;
  Color color;
};

struct Text g_text = {"Sinais do Outro Lado",
                      "/home/tavin/.local/share/fonts/Sinais Estrangeiro.ttf",
                      10,
                      10,
                      36,
                      {63, 167, 63, 255}};

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("SDL initialization failed: %s", SDL_GetError());
  }

  if (!TTF_Init()) {
    SDL_Log("TTF initialization failed: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  if (!SDL_CreateWindowAndRenderer(g_window.title, g_window.width,
                                   g_window.height, 0, &window, &renderer)) {
    SDL_Log("Window/Renderer creation failed: %s", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  TTF_TextEngine *engine = TTF_CreateRendererTextEngine(renderer);
  if (!engine) {
    SDL_Log("TTF_CreateRendererTextEngine failed: %s", SDL_GetError());
  }

  TTF_Font *font = TTF_OpenFont(g_text.font_path, g_text.pts);
  if (!font) {
    SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
  }

  TTF_Text *text = TTF_CreateText(engine, font, g_text.text, 0);
  TTF_SetTextColor(text, g_text.color.r, g_text.color.g, g_text.color.b,
                   g_text.color.a);

  bool running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_DrawRendererText(text, g_text.pos_x, g_text.pos_y);

    SDL_RenderPresent(renderer);
  }

  TTF_DestroyText(text);
  TTF_CloseFont(font);
  TTF_DestroyRendererTextEngine(engine);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
