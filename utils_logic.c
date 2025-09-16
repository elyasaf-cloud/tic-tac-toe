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

player *create_player(const player_kind kind)
{
    //allocates memory for a new player
    //enters parameters into its fields
    //returns NULL if faild
    player *new_player = NULL;
    static const uint8_t MAX_RETRIES = 3;
    for (uint8_t retry_count = 0; retry_count < MAX_RETRIES; retry_count++)
    {
        new_player = malloc(sizeof(*new_player));
        if (new_player)
        {
            break;
        }
    }
    if (!new_player)
    {
        return NULL;
    }
    new_player->kind = kind;
    new_player->victories = 0;
    return new_player;
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
    board[selected_slot] = current->kind + 1; //X for PLAYER_X, O for PLAYER_O
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