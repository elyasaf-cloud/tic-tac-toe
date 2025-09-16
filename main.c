#include "utils.h"


int main()
{
    opening_screen();
    //creates two players
    player *players[NUM_PLAYERS];
    players[0] = create_player(PLAYER_X);
    if (!players[0])
    {
        printf("Failed to allocate memory for player\n");
        exit(1);
    }
    get_name(players[0]); //gets a name from the player
    if (strcmp(players[0]->name, "q") == 0 || strcmp(players[0]->name, "Q") == 0)
    {
        return 0;
    }
    players[1] = create_player(PLAYER_O);
    if (!players[1])
    {
        printf("Failed to allocate memory for player\n");
        free_player(&players[0]);
        exit(1);
    }
    get_name(players[1]);
    if (strcmp(players[1]->name, "q") == 0 || strcmp(players[1]->name, "Q") == 0)
    {
        free_player(&players[0]);
        free_player(&players[1]);
        return 0;
    }

    uint8_t who_first = 0; //in the first game, X open
    while (true) //runs games
    {
        show_details(players, who_first);
        filling board[BOARD_SIZE] = {EMPTY};
        uint8_t whose_turn = who_first;
        while (true) //runs turns
        {
            show_board(board, false);
            int loc = get_slot(players[whose_turn], board);
            if (loc == -1) //the user press 'q'
            {
                //clear the screen, announcing the final winner, free the memory, and ends the running
                end_games(players);
                free_player(&players[0]);
                free_player(&players[1]);
                return 0;
            }
            update_board(players[whose_turn], board, loc);
            if(check_victory(board))
            //checks if a victory has been achieved, and marks the winning streak
            {
                //announcing the winner, updating the status of the victories
                printf("\n%s defeated %s!\n",players[whose_turn]->name, players[whose_turn ^ 1]->name);
                show_board(board, true);
                update_victories(players[whose_turn]);
                show_leader(players[0], players[1]);
                break;
            }
            //checks if the board is full
            if (is_board_full(board))
            {
                //announcing a draw
                printf("\nDraw!\n");
                show_board(board, true);
                show_leader(players[0], players[1]);
                break;
            }
            whose_turn ^= 1;
        }
        
        //continues to a new game / ends the program
        const char ans = ask_to_continue(); //return 'y', 'n' or 'q'
        if (ans == 'y')
        {
            who_first ^= 1; //each game a different player starts
            continue;
        }
        else
        {
            //clear the screen, announcing the final winner, free the memory, and ends the running
            end_games(players);
            free_player(&players[0]);
            free_player(&players[1]);
            return 0;
        }
    }
}