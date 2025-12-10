#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <math.h>
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

float animation_length = 10.;

uint64_t animation_ticks = 0;

typedef struct line_animation_t line_animation_t;

struct line_animation_t {
    float x0, y0, x1, y1;
    float nx, ny;
};

line_animation_t la = { 0 };

generic_animation_t ga_line = { 0 };

void line_animation_draw(generic_animation_t * self) {
    line_animation_t * line = (line_animation_t *)self->generic_data;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, line->x0, line->y0, line->x1, line->y1);
}

void line_animation_advance(generic_animation_t * self) {
    line_animation_t * line = (line_animation_t *)self->generic_data;
    if(self->current_step > self->steps / 2) {
        line->x1 = line->x0 + (self->steps - self->current_step) * line->nx * animation_length;
        line->y1 = line->y0 + (self->steps - self->current_step) * line->ny * animation_length;
    } else {
        line->x1 = line->x0 + self->current_step * line->nx * animation_length;
        line->y1 = line->y0 + self->current_step * line->ny * animation_length;
    }
}

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

    generic_animation_init(&ga_line, 20, 1000, &la, &line_animation_draw, &line_animation_advance);

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

        float dx = x - player.x;
        float dy = y - player.y;

        printf("dx=%f, dy=%f\n", dx, dy);

        float norm = sqrtf(dx * dx + dy * dy);

        // WARN: division by 0.0 is possible SO CHECK FOR THIS
        float nx = dx / norm;
        float ny = dy / norm;

        printf("nx=%f, ny=%f, len(n)=%f\n", nx, ny, sqrtf(nx * nx + ny * ny));

        float x1 = nx * animation_length;
        float y1 = ny * animation_length;

        printf("x1=%f, y1=%f\n", x1, y1);

        animation.x1 = nx * animation_length + player.x;
        animation.y1 = ny * animation_length + player.y;
        animation_line_reset(&animation);

        la.x0 = player.x;
        la.y0 = player.y;
        la.nx = nx;
        la.ny = ny;
        la.x1 = player.x;
        la.y1 = player.y;

        generic_animation_start(&ga_line);
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

    /*if(current_ticks - 50 >= animation_ticks) {
        if(!animation_line_is_done(&animation)) {
            animation_line_advance(&animation);
        }

        animation_ticks = current_ticks;
    }*/

    player_draw(&player);
    generic_animation_draw_and_advance(&ga_line);
    //animation_line_draw(&animation);

    SDL_RenderPresent(renderer);

    last_ticks = current_ticks;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
