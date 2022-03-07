#include<stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include "constants.h"

bool is_game_running = false;
int last_frame_time = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct ball {
  float x;
  float y;
  float width;
  float height;
} ball;

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
  ball.x = 20;
  ball.y = 20;
  ball.width = 15;
  ball.height = 15;
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
  // sleep the execution until we reach the target frame time in milliseconds
  int wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  if (wait > 0 && wait <= FRAME_TARGET_TIME) {
    SDL_Delay(wait);
  }
  // The time factor which is converted into seconds to be used to update objects
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();


  ball.x += 70 * delta_time;
  ball.y += 50 * delta_time;
}

void render() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_Rect ball_rect = {
      (int)ball.x,
      (int)ball.y,
      (int)ball.width,
      (int)ball.height
  };

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  if (SDL_RenderFillRect(renderer, &ball_rect) == -1) {
    fprintf(stderr, "SDL_RenderFillRect failed: %s \n", SDL_GetError());
  }

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