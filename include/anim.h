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

#endif
