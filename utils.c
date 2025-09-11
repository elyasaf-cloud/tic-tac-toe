#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char markings[2] = {'X', 'O'};
const char fills[3] = {'e', 'X', 'O'};

static bool check_sequence(const filling slot1, const filling slot2, const filling slot3)
{
    if (slot1 != EMPTY && slot1 == slot2 && slot2 == slot3)
    {
        return true;
    }
    return false;
}

static const player *show_leader(const player *player_x, const player *player_y)
{
    //shows the state of the wins, and returns a pointer to the leader
    printf("\n\nVictories table:\n\n%s(%c): %d victories\n%s(%c): %d victories\n",
         player_x->name, markings[player_x->kind], player_x->victories,
         player_y->name, markings[player_y->kind], player_y->victories);
    if (player_x->victories > player_y->victories)
        return player_x;
    if (player_y->victories > player_x->victories)
        return player_y;
    return NULL; //draw
}

player *create_player(const player_kind kind)
{
    player *new_player = malloc(sizeof(*new_player));
    if (!new_player)
    {
        printf("Malloc failed!\n");
        exit(1);
    }
    printf("\nEnter your name, player %c: ", markings[kind]);
    if (fgets(new_player->name, sizeof(new_player->name), stdin) != NULL 
    && new_player->name[0] != '\n'
    && new_player->name[0] != '\0') //checks if the input is correct
    {
        size_t len = strlen(new_player->name);
        if (new_player->name[len - 1] == '\n')
        {
            new_player->name[len - 1] = '\0';
        }
        else //the input was too long
        {
            clean_buffer();
        }
    }
    else //enters a default name
    {
        snprintf(new_player->name, sizeof(new_player->name), "player %c", markings[kind]);
    }
    new_player->kind = kind;
    new_player->victories = 0;
    return new_player;
}

void free_player(player *players[2])
{
    for (int i = 0; i < 2; i++)
    {
        if (players[i])
        {
            free(players[i]);
        }
    }
}

void make_turn(const player *current, filling board[9])
{
    //step 1 - show current board, with numbers in the empty slots, from 1 to 9
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            printf("\n");
        }
        printf("%c   ", board[i] == EMPTY ? (i + 1) + '0' : fills[board[i]]);
        //the starting number is 1, therefore the slot number is i + 1
        //the adding of '0' turns int into char
    }
    //step 2 - The user selects a slot
    int loc;
    while (true) //waiting to valid input
    {
        printf("\n\n%s, in which slot number would you like to place your %c? "
            ,current->name, markings[current->kind]);
        if (scanf("%d", &loc) != 1 || loc < 1 || loc > 9)
        {
            printf("\nInvalid input");
            clean_buffer();
            continue;
        }
        if (board[loc - 1] != EMPTY) //checks if the slot has alreay been initialized
        //the starting number is 1, therefore the index is loc - 1
        {
            printf("\nOccupied slot");
            clean_buffer();
            continue;
        }
        clean_buffer();
        break;
    }
    //step 3 - updating the board
    board[loc - 1] = current->kind + 1; //X for PLAYER_X, O for PLAYER_O
}

bool check_victory(const filling board[9])
{
    //checks the balanced sequences - 0 1 2, 3 4 5, and 6 7 8
    for (int i = 0; i < 9; i += 3)
    {
        if (check_sequence(board[0 + i], board[1 + i], board[2 + i]))
        {
            return true;
        }
    }
    //checks the vertical sequences - 0 3 6, 1 4 7, and 2 5 8
    for (int i = 0; i < 3; i++)
    {
        if (check_sequence(board[0 + i], board[3 + i], board[6 + i]))
        {
            return true;
        }
    }
    //checks the diagonal sequences - 0 4 8 and 2 4 6
    if (check_sequence(board[0], board[4], board[8]) || check_sequence(board[2], board[4], board[6]))
    {
        return true;
    }
    return false;
}

bool is_board_full(const filling board[9])
{
    for (int i = 0; i < 9; i++)
    {
        if (board[i] == EMPTY)
        {
            return false;
        }
    }
    return true;
}

void victory(const bool is_draw, player *winner, const player *loser, const filling board[9])
{
    if (is_draw)
    {
        printf("\nDraw!\n");
    }
    else
    {
        winner->victories++;
        printf("\n%s defeated %s!\n", winner->name, loser->name);
    }
    //shows the final image, whith 'e' in the empty slots
    for (int i = 0; i < 9; i++)
    {
        if (i % 3 == 0)
        {
            printf("\n");
        }
        printf("%c   ", fills[board[i]]);
    }
    if (winner->kind == PLAYER_X)
        show_leader(winner, loser);
    else
        show_leader(loser, winner);
}

void end(player *players[2])
{
    const player *final_winner = show_leader(players[0], players[1]); //returns NULL if there is a draw
    if (!final_winner)
    {
        printf("\n\n     Draw!\n\n\n");
    }
    else
    {
        printf("\n\nThe final winner is %s!\n\n\n", final_winner->name);
    }
}

void clean_buffer()
{
    char c;
    while (c = getchar() != '\n' && c != EOF);    
}

void clean_screen()
{
    #ifdef _WIN32 //for windows 
        system("cls");
    #else
        system("clear");
    #endif
}