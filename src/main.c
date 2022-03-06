#include<stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include "constants.h"

bool is_game_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL \n");
    return false;
  }

  window = SDL_CreateWindow(
      NULL,
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      SDL_WINDOW_BORDERLESS
      );

  if (! window) {
    fprintf(stderr, "SDL_CreateWindow failed \n");
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (! renderer) {
    fprintf(stderr, "SDL_CreateRenderer failed \n");
    return false;
  }

  return true;
}

void destroy_window() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void setup() {

}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      is_game_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_game_running = false;
      }
    default: ;

  }

}

void update() {

}

void render() {
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);


  SDL_RenderPresent(renderer);
}

int main() {
  is_game_running = initialize_window();

  setup();

  while(is_game_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  return 0;
}