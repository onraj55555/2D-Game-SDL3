#ifndef ANIM_H_
#define ANIM_H_

#include <stdint.h>

typedef struct animation_line_t animation_line_t;

struct animation_line_t {
    float x0, y0, x1, y1, x, y;
    uint8_t r, g, b;
    int current_state;
    int states;
};

void animation_line_init(animation_line_t * self, float x0, float y0, float x1, float y1, int states, uint8_t r, uint8_t g, uint8_t b);
void animation_line_advance(animation_line_t * self);
void animation_line_draw(animation_line_t * self);
int animation_line_is_done(animation_line_t * self);
void animation_line_reset(animation_line_t * self);

typedef struct generic_animation_t generic_animation_t;

struct generic_animation_t {
    uint64_t current_step; // current state of the animation
    uint64_t steps; // amount of steps the animation takes
    uint64_t duration; // time (ms) that the animation takes
    uint64_t time_step; // time (ms) between updates
    uint64_t last_update_time; // last time animation actually updates, used to determine if it should be updated or not
    void * generic_data;
    void (*draw)(generic_animation_t * self);
    void (*advance)(generic_animation_t * self);
};

void generic_animation_init(generic_animation_t * self, uint64_t steps, uint64_t duration, void * generic_data, void (*draw)(generic_animation_t * self), void (*advance)(generic_animation_t * self));
void generic_animation_start(generic_animation_t * self);
int generic_animation_is_done(generic_animation_t * self);
void generic_animation_advance(generic_animation_t * self);
void generic_animation_draw(generic_animation_t * self);
void generic_animation_draw_and_advance(generic_animation_t * self);

#endif
