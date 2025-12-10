#include "anim.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>

extern SDL_Renderer * renderer;

void animation_line_init(animation_line_t * self, float x0, float y0, float x1, float y1, int states, uint8_t r, uint8_t g, uint8_t b) {
    self->x0 = x0;
    self->y0 = y0;
    self->x1 = x1;
    self->y1 = y1;
    self->x = x0;
    self->y = y0;
    self->current_state = 0;
    self->states = states;
    self->r = r;
    self->g = g;
    self->b = b;
}

void animation_line_advance(animation_line_t *self) {
    if(self->current_state == self->states) return;
    float x = self->x1 - self->x0;
    float y = self->y1 - self->y0;
    float dt = self->states / 2;
    float dx = x / dt;
    float dy = y / dt;
    self->current_state++;
    int offset = 0;
    if(self->current_state < self->states / 2) {
        self->x = self->x + dx;
        self->y = self->y + dy;
    } else {
        self->x = self->x - dx;
        self->y = self->y - dy;
    }

}

void animation_line_draw(animation_line_t *self) {
    if(animation_line_is_done(self)) return;
    SDL_SetRenderDrawColor(renderer, self->r, self->g, self->b, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, self->x0, self->y0, self->x, self->y);
}

int animation_line_is_done(animation_line_t *self) {
    return self->current_state == self->states;
}

void animation_line_reset(animation_line_t * self) {
    self->current_state = 0;
    self->x = self->x0;
    self->y = self->y0;
}
