#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {PLAYER_X, PLAYER_O} player_kind;

typedef enum {EMPTY, X, O} filling;

typedef struct Player
{
    char name[15];
    player_kind kind;
    uint16_t victories;
} player;

player *create_player(const player_kind kind);

void free_player(player *players[2]);

void make_turn(const player *current, filling board[9]);

bool check_victory(const filling board[9]);

bool is_board_full(const filling board[9]);

void victory(const bool is_draw, player *winner, const player *loser, const filling board[9]);

void end(player *players[2]);

void clean_buffer();

void clean_screen();


