#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "player.h"
#include "anim.h"

int width = 640;
int height = 480;

SDL_Window * window;
SDL_Renderer * renderer;

player_t player = { 0 };

float player_size = 10;
float player_speed_modifier = 0.1;

uint64_t last_ticks = 0;

animation_line_t animation;

uint64_t animation_ticks = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    SDL_SetAppMetadata("2D game", "1.0", "com.game.2d");

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Could not initialise SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(!SDL_CreateWindowAndRenderer("2D game", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Could not create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    player_init(&player, width / 2., height / 2., player_size, player_speed_modifier);

    animation_line_init(&animation, 0, 0, 50, 50, 20, 0, 255, 0);

    last_ticks = SDL_GetTicks();
    animation_ticks = last_ticks;

    return SDL_APP_CONTINUE;
}

void handle_key_down(SDL_Keycode key_code) {
    switch(key_code) {
        case SDLK_UP: player_set_speed(&player, 0., -1.); break;
        case SDLK_DOWN: player_set_speed(&player, 0., 1.); break;
        case SDLK_LEFT: player_set_speed(&player, -1., 0.); break;
        case SDLK_RIGHT: player_set_speed(&player, 1., 0.); break;
    }
}

void handle_key_up(SDL_Keycode key_code) {
    switch(key_code) {
        case SDLK_UP: if(player.vy < 0.) player_set_speed(&player, 0., 0.); break;
        case SDLK_DOWN: if(player.vy > 0.) player_set_speed(&player, 0., 0.); break;
        case SDLK_LEFT: if(player.vx < 0.) player_set_speed(&player, 0., 0.); break;
        case SDLK_RIGHT: if(player.vx > 0.) player_set_speed(&player, 0., 0.); break;
    }
}

void mouse_down() {
    float x = 0.;
    float y = 0.;

    SDL_MouseButtonFlags mouse_button_flags = SDL_GetMouseState(&x, &y);

    if(mouse_button_flags & SDL_BUTTON_LEFT) {
        printf("LMB pressed!\n");
        printf("x=%f, y=%f\n", x, y);
        animation.x0 = player.x;
        animation.y0 = player.y;
        animation.x1 = x;
        animation.y1 = y;
        animation_line_reset(&animation);
    }
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if(event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
    if(event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_down();
    } else if(event->type == SDL_EVENT_KEY_DOWN) {
        handle_key_down(event->key.key);
    } else if(event->type == SDL_EVENT_KEY_UP) {
        handle_key_up(event->key.key);
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    uint64_t current_ticks = SDL_GetTicks();
    uint64_t delta_time = current_ticks - last_ticks;
    player_move(&player, delta_time);

    if(current_ticks - 50 >= animation_ticks) {
        if(!animation_line_is_done(&animation)) {
            animation_line_advance(&animation);
        }

        animation_ticks = current_ticks;
    }

    player_draw(&player);
    animation_line_draw(&animation);

    SDL_RenderPresent(renderer);

    last_ticks = current_ticks;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
