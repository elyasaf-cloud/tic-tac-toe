#include "utils.h"


const char MARKINGS[2] = {'X', 'O'};
static const char TITLE[] = "tic-tac-toe | press q at any step to quit";
static const char END_TITLE[] = "tic-tac-toe | game over";
static const char FILLS[5] = {'e', 'X', 'O', 'X', 'O'};

static void announces_final_result(const player *final_winner)
{
    //get NULL pointer when there is draw
    if (!final_winner)
    {
        printf("\n\n     Draw!\n\n\n");
    }
    else
    {
        printf("\n\nThe final winner is %s!\n\n\n", final_winner->name);
    }
}

static void clean_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void clean_screen()
{
    printf("\033[2J");
    printf("\033[H");// Move cursor to home position
}

void opening_screen()
{
    clean_screen();
    printf("%s\n", TITLE);
}

void print_data_message(const char *message, bool is_succesed)
{
    if (is_succesed)
    {
        printf("\nData %s successfully\n\n", message);
    }
    else
    {
        printf("\nData %s error\n\n", message);
    }
}

void show_details(player *players[NUM_PLAYERS], const uint8_t who_first)
{
    clean_screen();
    printf("%s\n\n", TITLE);
    printf("%s is X, with %d vicyories\n\n", players[0]->name, players[0]->victories);
    printf("%s is O, with %d vicyories\n\n", players[1]->name, players[1]->victories);
    printf("Let's get started!\n\n");
    printf("This time, %s starts\n", players[who_first]->name);
}

void get_name(player *new_player)
{
    char name[NAME_LENGHT];
    printf("\nEnter your name, player %c: ", MARKINGS[new_player->kind]);
    if (fgets(name, NAME_LENGHT, stdin) != NULL 
    && name[0] != '\n'
    && name[0] != '\0')
    //checks if the input is valid and not empty
    {
        size_t len = strlen(name);
        if (name[len - 1] == '\n')
        {
            name[len - 1] = '\0';
        }
        else //the input was too long
        {
            clean_buffer();
        }
        strcpy(new_player->name, name);
    }
    //else
    //the player will remain with the default name
}

void show_board(const filling board[BOARD_SIZE], const bool is_end)
{
    //shows the state of the board, with numbers in the empty slots, from 1 to 9
    //if this is the end of the game, the empty slots will be marked with 'e'
    //and it will mark the winning sequence with '!', too
    if (is_end)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            if (i % 3 == 0)
            {
                printf("\n");
            }
            printf("%c%c   ",board[i] > NUM_PLAYERS ? '!' : ' ', FILLS[board[i]]);
            //when board[i] > NUM_PLAYERS(2), is mean that its value is WIN_X or WIN_Y
        }
    }
    else
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            if (i % 3 == 0)
            {
                printf("\n");
            }
            printf("%c   ", board[i] == EMPTY ? (i + 1) + '0' : FILLS[board[i]]);
            //the starting number is 1, therefore the slot number is i + 1
            //the adding of '0' turns int into char
        }
    }
}

const int get_slot(const player *current, const filling board[BOARD_SIZE])
{
    //asks the player to choose a slot
    //emits customized error messages until valid input is received
    char input[8];
    char *endptr;
    long loc;
    while (true)
    {
        printf("\n\n%s, in which slot number would you like to place your %c? "
            ,current->name, MARKINGS[current->kind]);
        //reads input as string first
        if (!fgets(input, sizeof(input), stdin))
        {
            printf("\nError reading input");
            continue;
        }
        //checks if the input was too long
        if (!strchr(input, '\n'))
        {
            printf("\nInput too long");
            clean_buffer();
            continue;
        }
        //trurn -1 if the player press 'q'
        if (strcmp(input, "q\n") == 0 || strcmp(input, "Q\n") == 0)
        {
            return -1;
        }
        //converts to number with error checking
        loc = strtol(input, &endptr, 10);
        if (endptr == input || *endptr != '\n')
        {
            printf("\nPlease enter a valid number");
            continue;
        }
        //checks range
        if (!is_slot(loc))
        {
            printf("\nPlease enter a number between 1 and %d", BOARD_SIZE);
            continue;
        }
        //checks if slot is available
        if (board[loc - 1] != EMPTY)
        {
            printf("\nThat slot is already taken");
            continue;
        }
        break;
    }
    return (int)(loc - 1); //converts to 0-based
}

void show_leader(const player *player_x, const player *player_o)
{
    //shows the status of victories, and returns a pointer to the leader
    printf("\n\nVictories table:\n\n%s(%c): %d victories\n%s(%c): %d victories\n",
         player_x->name, MARKINGS[player_x->kind], player_x->victories,
         player_o->name, MARKINGS[player_o->kind], player_o->victories);
}

answer ask_user(const char *question)
{
    char input[8];
    while (true) //waiting to valid input ('y','n' or 'q')
    {
        printf("\n%s? (answer y/n): ", question);
        if (!fgets(input, sizeof(input), stdin))
        {
            printf("\nError reading input\n");
            continue;
        }
        if (input[0] == '\n' || input[0] == '\0')
        {
            printf("\nplease enter your answer\n");
            continue;
        }
        input[0] = tolower(input[0]);
        if (strcmp(input, "y\n") != 0
        && strcmp(input, "n\n") != 0
        && strcmp(input, "q\n") != 0)
        {
            if (!strchr(input, '\n')) //input was too long
            {
                clean_buffer();
            }
            printf("\nPlease enter y or n\n");
            continue;
        }
        break;
    }
    return input[0];
}

void end_games(player *players[NUM_PLAYERS])
{
    //shows the status of victories
    //announces the final winner (or draw)
    clean_screen();
    printf("%s\n", END_TITLE);
    show_leader(players[0], players[1]);
    const player *winner = determines_winner(players);
    announces_final_result(winner);
}

void windows_end()
{
    printf("Press any key to exit");
    getchar();
}