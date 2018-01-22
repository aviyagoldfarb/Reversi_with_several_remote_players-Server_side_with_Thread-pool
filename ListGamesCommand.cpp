//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#define MAX_GAME_LIST_BUFFER 250

#include "ListGamesCommand.h"
#include <cstring>

ListGamesCommand ::ListGamesCommand() {
    pthread_mutex_init(&count_mutex, NULL);
}

void ListGamesCommand::execute(vector<string> args, Server server,  vector<Game>* gamesPtr, int clientSocket) {
    char gamesListBuffer[MAX_GAME_LIST_BUFFER];
    memset(gamesListBuffer, '\0', MAX_GAME_LIST_BUFFER);
    pthread_mutex_lock(&count_mutex);
    for (int i = 0; i < gamesPtr->size(); i++) {
        if (((*gamesPtr)[i].blackClientSocket != 0) && ((*gamesPtr)[i].whiteClientSocket == 0)) {
            if (i == 0){
                strcpy(gamesListBuffer, (*gamesPtr)[i].name.c_str());
            }
            else{
                strcat(gamesListBuffer, (*gamesPtr)[i].name.c_str());
            }
        }
        strcat(gamesListBuffer, " ");
    }
    pthread_mutex_unlock(&count_mutex);
    server.writeToClient(clientSocket, gamesListBuffer);
}