#include "player.h"
#include <SDL3/SDL.h>
#include "m.h"

extern SDL_Renderer * renderer;

void player_init(player_t *self, float x, float y, float size, float speed_modifier) {
    self->x = x;
    self->y = y;
    self->vx = 0.;
    self->vy = 0.;
    self->size = size;
    self->speed_modifier = speed_modifier;
}

void player_draw(player_t *self) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_FRect p = {
        .x = self->x - self->size / 2.,
        .y = self->y - self->size / 2.,
        .w = self->size,
        .h = self->size
    };
    SDL_RenderFillRect(renderer, &p);
}

void player_move(player_t *self, uint64_t delta_time) {
    self->x += self->vx * delta_time * self->speed_modifier;
    self->y += self->vy * delta_time * self->speed_modifier;
}

void player_set_speed(player_t *self, float vx, float vy) {
    self->vx = vx;
    self->vy = vy;
}

void player_set_x_speed(player_t *self, float vx) {
    self->vx = vx;
    if(self->vx * self->vy != 0.) {
        self->vx *= one_over_sqrt_two;
        self->vy *= one_over_sqrt_two;
        return;
    }
    if(self->vx == 0.) {
        if(self->vy > 0.) self->vy = 1.;
        else self->vy = -1.;
        return;
    }
}

void player_set_y_speed(player_t *self, float vy) {
    self->vy = vy;
    if(self->vx * self->vy != 0.) {
        self->vx *= one_over_sqrt_two;
        self->vy *= one_over_sqrt_two;
        return;
    }
    if(self->vy == 0.) {
        if(self->vx > 0.) self->vx = 1.;
        else self->vx = -1.;
        return;
    }
}
