#include "anim.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
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
    if(self->current_state < self->states / 2) {
        self->x = self->x + dx;
        self->y = self->y + dy;
    } else {
        self->x = self->x - dx;
        self->y = self->y - dy;
    }
    self->current_state++;
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

void generic_animation_init(
    generic_animation_t *self,
    uint64_t steps,
    uint64_t duration,
    void *generic_data,
    void (*draw)(generic_animation_t *),
    void (*advance)(generic_animation_t *)
) {
    self->steps = steps;
    self->current_step = steps;
    self->duration = duration;
    self->time_step = self->duration / self->steps;
    self->last_update_time = 0;

    self->generic_data = generic_data;
    self->draw = draw;
    self->advance = advance;
}

void generic_animation_start(generic_animation_t *self) {
    self->current_step = 0;
    self->last_update_time = SDL_GetTicks();
}

int generic_animation_is_done(generic_animation_t *self) {
    return self->current_step == self->steps;
}

void generic_animation_advance(generic_animation_t *self) {
    if(generic_animation_is_done(self)) return;

    float current_time = SDL_GetTicks();

    if(current_time - self->last_update_time < self->time_step) return;

    self->current_step++;
    self->advance(self);
    self->last_update_time = current_time;
}

void generic_animation_draw(generic_animation_t *self) {
    self->draw(self);
}

void generic_animation_draw_and_advance(generic_animation_t *self) {
    generic_animation_draw(self);
    generic_animation_advance(self);
}
