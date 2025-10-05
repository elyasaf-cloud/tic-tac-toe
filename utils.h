#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>


extern const char MARKINGS[2];

enum {NUM_PLAYERS = 2, SEQUENCES_LENGTH = 3, POSSIBLE_SEQUENCES = 8 ,BOARD_SIZE = 9, NAME_LENGHT = 32};
//I didn't use const, so that it would be possible to create arrays with these variables

typedef enum {PLAYER_X, PLAYER_O} player_kind;

typedef enum {EMPTY, X, O, WIN_X, WIN_Y} filling;

typedef enum {YES = 'y', NO = 'n', QUIT = 'q'} answer;

typedef struct Player
{
    char name[NAME_LENGHT];
    player_kind kind;
    uint16_t victories;
} player;

//UI functions

void opening_screen();

void print_data_message(const char *message, bool is_succesed);

void show_details(player *players[NUM_PLAYERS], const uint8_t who_first);

void get_name(player *new_player);

void show_board(const filling board[BOARD_SIZE], const bool is_end);

const int get_slot(const player *current, const filling board[BOARD_SIZE]);

void show_leader(const player *player_x, const player *player_y);

answer ask_user(const char *question);

void end_games(player *players[NUM_PLAYERS]);

void windows_end();

//logic functions

bool is_allocate_success(player *players[NUM_PLAYERS]);

void create_player(player *new_player, const player_kind kind);

bool is_slot(const long loc);

void update_board(const player *current, filling board[BOARD_SIZE], const int selecyed_slot);

bool check_victory(filling board[BOARD_SIZE]);

bool is_board_full(const filling board[BOARD_SIZE]);

void update_victories(player *winner);

const player *determines_winner(player *players[NUM_PLAYERS]);

void free_player(player **to_free);

//file functions

bool is_file_exists();

bool get_players_data_from_file(player *players[NUM_PLAYERS]);

bool set_players_data_to_file(player *players[NUM_PLAYERS]);
