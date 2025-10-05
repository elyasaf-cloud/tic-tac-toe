#include "utils.h"


static void finish(player *players[NUM_PLAYERS])
{
    //clear the screen, announcing the final winner
    end_games(players);
    //save the data (if the user want to), free the memory, and ends the running
    answer ans = ask_user("Want to save your game data for next time");
    if (ans == YES)
    {
        if (set_players_data_to_file(players))
        {
            print_data_message("saved", true);
        }
        else
        {
            print_data_message("saving", false);
        }
    }
    else
    {
        printf("\n");
    }
    #ifdef _WIN32 //for windows
        windows_end();
    #endif
    free_player(&players[0]);
    free_player(&players[1]);
}

int main()
{
    opening_screen();
    //creates two players
    player *players[NUM_PLAYERS];
    if (!is_allocate_success(players))
    {
        printf("Failed to allocate memory for players\n");
        exit(1);
    }

    //if the user wants it
    //gets the data from previous games
    bool need_creating = true;
    if (is_file_exists())
    {
        answer ans = ask_user("Want to continue the previous games"); //return 'y', 'n' or 'q'
        if (ans == QUIT)
        {
            free_player(&players[0]);
            free_player(&players[1]);
            return 0;
        }
        if (ans == YES)
        {
            if (get_players_data_from_file(players))
            {
                need_creating = false;
            }
            else
            {
                print_data_message("reading", false);
            }
        }
    }
    
    if (need_creating)
    {
        //creates new players
        create_player(players[0], PLAYER_X);
        get_name(players[0]); //gets a name from the player
        if (strcmp(players[0]->name, "q") == 0 || strcmp(players[0]->name, "Q") == 0)
        {
            free_player(&players[0]);
            free_player(&players[1]);
            return 0;
        }
        create_player(players[1], PLAYER_O);
        get_name(players[1]);
        if (strcmp(players[1]->name, "q") == 0 || strcmp(players[1]->name, "Q") == 0)
        {
            free_player(&players[0]);
            free_player(&players[1]);
            return 0;
        }
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
                finish(players);
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
        answer ans = ask_user("Want to start another game"); //return 'y', 'n' or 'q'
        if (ans == YES)
        {
            who_first ^= 1; //each game a different player starts
            continue;
        }
        else
        {
            finish(players);
            return 0;
        }
    }
}