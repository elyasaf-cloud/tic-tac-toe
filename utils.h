#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {PLAYER_X, PLAYER_O} player_kind;

typedef enum {EMPTY, X, O} fill;

typedef struct Player
{
    char name[15];
    player_kind kind;
    uint16_t victories;
} player;

player *create_player(const player_kind kind);

void free_player(player *players[2]);

void make_turn(const player *current, uint32_t *board);

bool check_victory(const player *current, const uint32_t board);

void victory(const bool is_draw, player *winner, const player *loser, const uint32_t final_board);

void end(player *players[2]);

void clean_buffer();

void clean_screen();


