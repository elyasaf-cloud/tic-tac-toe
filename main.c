#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

//in this version, the board is represented by a binary number(uint32_t)
//in this number, the right 9 bits indicate whether a slot is marked with an X (0) or an O (1)
//subject to the following condition:
//For each bit, the bit 9 places to the left indicates whether it has already been initialized (1) or not (0)

const uint32_t EMPTY_BOARD = 0;
const uint32_t FULL_BOARD = 0b111111111 << 9;
const char TITLE[] = "tic-tac-toe";


int main()
{
    printf("%s\n", TITLE);
    player *players[2];
    players[0] = create_player(X);
    players[1] = create_player(O);
    uint8_t who_first = 0; //in the first game, X open
    printf("\nLet's get started!\n\nThis time, %s starts\n", players[who_first]->name);
    while (true) //runs games
    {
        uint32_t board = EMPTY_BOARD;
        uint8_t whose_turn = who_first;
        while (true) //runs turns
        {
            make_turn(players[whose_turn], &board);
            if(check_victory(players[whose_turn], board))
            {
                //announcing the winner, updating the status of the victories
                victory(false, players[whose_turn], players[whose_turn ^ 1], board);
                break;
            }
            //checks if the board is full
            if ((board & FULL_BOARD) == FULL_BOARD)
            {
                //announcing a draw
                victory(true, players[0], players[1], board);
                break;
            }
            whose_turn ^= 1;
        }
        //continues to a new game / ends the program
        while (true) //waiting to valid input
        {
            printf("\nWant to start another game? (answer y/n): ");
            char ans = getchar();
            clean_buffer();
            ans |= 32; //converts to lowercase
            if (ans == 'y')
            {
                break;
            }
            else if (ans == 'n')
            {
                //clear the screen, announcing the final winner, free the memory, and ends the running
                clean_screen();
                printf("%s\n", TITLE);
                end(players);
                free_player(players);
                #ifdef _WIN32 //for windows
                    printf("Press any key to exit");
                    getchar();
                #endif
                return 0;
            }
            else
            {
                printf("\nInvalid input\n");
            }
        }
        who_first ^= 1; //each game a different player starts
        clean_screen(); //clear the terminal  
        printf("%s\n\nThis time, %s starts\n", TITLE, players[who_first]->name);
    }

    return 0;
}