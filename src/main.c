#include<stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include "constants.h"

// Global Variables
bool is_game_running = false;
int last_frame_time = 0;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct game_object {
  float x;
  float y;
  float width;
  float height;
  float vel_x;
  float vel_y;
} ball, paddle;

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
  ball.vel_x = 300;
  ball.vel_y = 300;

  paddle.x = (WINDOW_WIDTH / 2) - (paddle.width / 2);
  paddle.y = WINDOW_HEIGHT - 40;
  paddle.width = 100;
  paddle.height = 20;
  paddle.vel_x = 0;
  paddle.vel_y = 0;


}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      is_game_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        is_game_running = false;
      if (event.key.keysym.sym == SDLK_LEFT)
        paddle.vel_x = -400;
      if (event.key.keysym.sym == SDLK_RIGHT)
        paddle.vel_x = +400;
      break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_LEFT)
        paddle.vel_x = 0;
      if (event.key.keysym.sym == SDLK_RIGHT)
        paddle.vel_x = 0;
      break;
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


  ball.x += ball.vel_x * delta_time;
  ball.y += ball.vel_y * delta_time;
  paddle.x += paddle.vel_x * delta_time;
  paddle.y += paddle.vel_y * delta_time;

  // If ball is lower than the left wall, force the position to be zero before
  // flipping the velocity
  if (ball.x <= 0) {
    ball.x = 0;
    ball.vel_x = -ball.vel_x;
  }

  // if ball is lower than left wall, force the position to be zero before
  // flipping the velocity
  if (ball.y < 0) {
    ball.y = 0;
    ball.vel_y = -ball.vel_y;
  }

  // if the ball x plus its width is greater than the window width, force the
  // position to be at the right of the screen
  if (ball.x + ball.width > WINDOW_WIDTH) {
    ball.x = WINDOW_WIDTH - ball.width;
    ball.vel_x = -ball.vel_x;
  }

  // if the ball y plus its height is greater than window height, force the
  // position to be at the bottom of the screen
  if (ball.y + ball.height > WINDOW_HEIGHT) {
    ball.y = WINDOW_HEIGHT - 40;
    ball.vel_y = -ball.vel_y;
  }

  // Check for ball collision with paddle
  if (ball.y + ball.height >= paddle.y &&
      ball.x + ball.width >= paddle.x &&
      ball.x <= paddle.x + paddle.width) {
    ball.vel_y = -ball.vel_y;
  }

  // Prevent the paddle from moving outside the boundaries of the window
  if (paddle.x <= 0) {
    paddle.x = 0;
  }

  if (paddle.x >= WINDOW_WIDTH - paddle.width) {
    paddle.x = WINDOW_WIDTH - paddle.width;
  }

  // Check if game over
  if (ball.y + ball.height >  WINDOW_HEIGHT) {
    ball.x = WINDOW_WIDTH / 2;
    ball.y = 0;
  }

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
    fprintf(stderr, "SDL_RenderFillRect failed for (ball_rect): %s \n", SDL_GetError());
  }

  SDL_Rect paddle_rect = {
      (int)paddle.x,
      (int)paddle.y,
      (int)paddle.width,
      (int)paddle.height
  };
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  if (SDL_RenderFillRect(renderer, &paddle_rect) == -1) {
    fprintf(stderr, "SDL_RenderFillRect failed for (paddle_rect): %s \n", SDL_GetError());
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