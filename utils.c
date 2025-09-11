#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char markings[2] = {'X', 'O'};
//const char fills[3] = {'e', 'X', 'O'};

static bool check_sequence_0(const uint32_t board, const uint32_t sequence)
{
    return (board & sequence) == 0;
}

static bool check_sequence_1(const uint32_t board, const uint32_t sequence)
{
    return (board & sequence) == sequence;
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

/*void make_turn(const player *current, uint32_t *board)
{
    //step 1, show current board, with numbers in the empty slots
    uint32_t is_marked = 1u << 9;
    for (int i = 0; i < 9; i++, is_marked <<= 1)
    {
        if (i % 3 == 0)
            printf("\n");
        if(*board & is_marked) //checks if the slot has been initialized
        {
            printf("%c   ", markings[(*board & (is_marked >> 9)) != 0]);
        }
        else
        {
            printf("%d   ", i + 1); //start with 1
        }
    }
    //step 2, The user selects a slot
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
        if (*board & (1u << (loc + 8))) //checks if the slot has alreay been initialized
        //8 and not 9, because the the starting number is 1
        {
            printf("\nOccupied slot");
            clean_buffer();
            continue;
        }
        
        clean_buffer();
        break;
    }
    //step 3, updating the board
    *board |= current->kind << loc - 1;
    *board |= 1u << (loc + 8); //indicates that the slot has been initialized
}*/

void make_turn(const player *current, filling *boardf[9])
{
    //step 1, show current board, with numbers in the empty slots
    uint32_t is_marked = 1u << 9;
    for (int i = 0; i < 9; i++, is_marked <<= 1)
    {
        if (i % 3 == 0)
            printf("\n");
        printf("%c   ", boardf[i] == EMPTY ? i : markings[(int)boardf[i]]);
        if(*board & is_marked) //checks if the slot has been initialized
        {
            printf("%c   ", markings[(*board & (is_marked >> 9)) != 0]);
        }
        else
        {
            printf("%d   ", i + 1); //start with 1
        }
    }
    //step 2, The user selects a slot
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
        if (*board & (1u << (loc + 8))) //checks if the slot has alreay been initialized
        //8 and not 9, because the the starting number is 1
        {
            printf("\nOccupied slot");
            clean_buffer();
            continue;
        }
        
        clean_buffer();
        break;
    }
    //step 3, updating the board
    *board |= current->kind << loc - 1;
    *board |= 1u << (loc + 8); //indicates that the slot has been initialized
}

bool check_victory(const player *current, const uint32_t board)
{
    //defining the desired sequence, XXX or OOO
    static const bool (*check_win[2])(uint32_t, uint32_t) = {check_sequence_0, check_sequence_1};
    //testing straight sequences - balanced and perpendicular (each of them has 3 options)
    //the test only occurs after verifying that the slots have been initialized
    static const uint32_t straight_sequences[2] = {0b111, 0b1001001};
    static const int multiplier[2] = {3, 1}; //turns 111 into 111000 and so on
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (check_sequence_1(board, straight_sequences[i] << ((multiplier[i] * j) + 9)))
            {
                if (check_win[current->kind](board, straight_sequences[i] << (multiplier[i] * j)))
                {
                    return true;
                }
            }
        }
    }
    //checking the diagonal sequences (2 options)
    static const uint32_t diagonal_sequences[2] = {0b100010001, 0b001010100};
    for (int i = 0; i < 2; i++)
    {
        if (check_sequence_1(board, diagonal_sequences[i] << 9))
        {
                if (check_win[current->kind](board, diagonal_sequences[i]))
                {
                    return true;
                }
        }
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

void victory(const bool is_draw, player *winner, const player *loser, const uint32_t final_board)
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
    uint32_t is_marked = 1 << 9;
    for (int i = 0; i < 9; i++, is_marked <<= 1)
    {
        if (i % 3 == 0)
            printf("\n");
        if(final_board & is_marked)
        {
            printf("%c   ", markings[(final_board & (is_marked >> 9)) != 0]);
        }
        else
        {
            printf("e   ");
        }
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