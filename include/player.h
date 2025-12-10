#ifndef PLAYER_H_
#define PLAYER_H_

#include <stdint.h>

typedef struct player_t player_t;

struct player_t {
    float x, y;
    float vx, vy;
    float size;
    float speed_modifier;
};

void player_init(player_t * self, float x, float y, float size, float speed_modifier);
void player_draw(player_t * self);
void player_move(player_t * self, uint64_t delta_time);
void player_set_speed(player_t * self, float vx, float vy);
void player_set_x_speed(player_t * self, float vx);
void player_set_y_speed(player_t * self, float vy);

#endif
