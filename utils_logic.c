#include "utils.h"



static bool check_sequence(const filling slot1, const filling slot2, const filling slot3)
{
    if (slot1 != EMPTY && slot1 == slot2 && slot2 == slot3)
    {
        return true;
    }
    return false;
}

static void mark_winning_sequence(filling board[BOARD_SIZE], uint8_t loc_1, uint8_t loc_2, uint8_t loc_3)
{
    //turns fillings into WIN_X / WIN_Y
    //the enum is
    //typedef enum {EMPTY, X, O, WIN_X, WIN_Y} filling;
    board[loc_1] += 2;
    board[loc_2] += 2;
    board[loc_3] += 2;
}

bool is_allocate_success(player *players[NUM_PLAYERS])
{
    //allocates memory for players
    static const uint8_t MAX_RETRIES = 3;
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        for (uint8_t retry_count = 0; retry_count < MAX_RETRIES; retry_count++)
        {
            players[i] = malloc(sizeof(player));
            if (players[i])
            {
                break;
            }
        }
        if (!players[i])
        {
            //free previously allocated 
            for (int j = 0; j < i; j++)
            {
                free(players[j]);
            }
            return false;
        }
    }
    return true;
}

void create_player(player *new_player, const player_kind kind)
{
    //enters parameters into player's fields
    if (!new_player)
    {
        return;
    }
    new_player->kind = kind;
    new_player->victories = 0;
    //enters a default name
    snprintf(new_player->name, NAME_LENGHT, "player %c", MARKINGS[new_player->kind]);
}

void free_player(player **to_free)
{
    if (to_free && *to_free)
    {
        free(*to_free);
        *to_free = NULL;
    }
}

bool is_slot(const long loc)
{
    return (loc >= 1 && loc <= BOARD_SIZE);
}

void update_board(const player *current, filling board[BOARD_SIZE], const int selected_slot)
{
    board[selected_slot] = (current->kind == PLAYER_X) ? X : O;
}

bool check_victory(filling board[BOARD_SIZE])
{
    //defines all possible sequences
    static const uint8_t sequences[POSSIBLE_SEQUENCES][SEQUENCES_LENGTH] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {0, 3, 6},
    {1, 4, 7},
    {2, 5, 8},
    {0, 4, 8},
    {2, 4, 6}
    } ;
    //checks all possible sequences
    for (int i = 0; i < POSSIBLE_SEQUENCES; i++)
    {
        if (check_sequence(board[sequences[i][0]], board[sequences[i][1]], board[sequences[i][2]]))
        {
            mark_winning_sequence(board, sequences[i][0], sequences[i][1], sequences[i][2]);
            return true;
        }
    }
   return false;
}

bool is_board_full(const filling board[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (board[i] == EMPTY)
        {
            return false;
        }
    }
    return true;
}

void update_victories(player *winner)
{
    winner->victories++;
}

const player *determines_winner(player *players[NUM_PLAYERS])
{
    if (players[0]->victories > players[1]->victories)
    {
        return players[0];
    }
    if (players[1]->victories > players[0]->victories)
    {
        return players[1];
    }
    return NULL; //draw
}