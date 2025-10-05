#include "utils.h"
#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define ACCESS _access
    #define FILE_NAME "data.txt"
#else
    #include <unistd.h>
    #define ACCESS access
    //hide the file
    #define FILE_NAME ".data.txt"
#endif


bool is_file_exists()
{   
    return (ACCESS(FILE_NAME, F_OK) == 0);
}

bool get_players_data_from_file(player *players[NUM_PLAYERS])
{
    //enter the data from the file to the players
    //return false if faild
    FILE *get_data = fopen(FILE_NAME, "r");
    if (!get_data)
    {
        return false;
    }
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (!players[i])
        {
            fclose(get_data);
            return false;
        }
        char line[128];
        if (!fgets(line, sizeof(line), get_data))
        {
            fclose(get_data);
            return false;
        }       
        line[strcspn(line, "\r\n")] = '\0';
        int temp_kind, temp_victories;
        int parsed = sscanf(line, "%31[^|]|%d|%u",
            players[i]->name,
            &temp_kind,
            &temp_victories);
        if (parsed != 3)
        {
            fclose(get_data);
            return false;
        }
        players[i]->kind = temp_kind;
        players[i]->victories = temp_victories;
        
    }
    fclose(get_data);
    return true;
}

bool set_players_data_to_file(player *players[NUM_PLAYERS])
{
    //save the data of the players to a temporary file
    //return false if faild
    //if succeeded, savee the temp file as the normal file
    const char *temp_name = FILE_NAME "_tmp";
    FILE *set_data = fopen(temp_name, "w");
    if (!set_data)
    {
        return false;
    }
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (!players[i] ||
            fprintf(set_data, "%s|%d|%u\n",
            players[i]->name,
            (int)players[i]->kind,
            players[i]->victories) < 0)
        {
            fclose(set_data);
            remove(temp_name);
            return false;
        }
    }
    if (fclose(set_data) != 0)
    {
        remove(temp_name);
        return false;
    }
    if (rename(temp_name, FILE_NAME) != 0)
    {
        remove(temp_name);
        return false;
    }
    //hide file on windows
    #ifdef _WIN32
        SetFileAttributesA(FILE_NAME, FILE_ATTRIBUTE_HIDDEN);
    #endif
    return true;
}