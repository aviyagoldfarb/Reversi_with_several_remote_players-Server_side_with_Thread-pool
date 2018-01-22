//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "JoinCommand.h"
#include <cstring>

JoinCommand::JoinCommand() {
    pthread_mutex_init(&count_mutex, NULL);
}

void JoinCommand::execute(vector<string> args, Server server,  vector<Game>* gamesPtr, int clientSocket) {
    int blackClientSocketCopy;
    int whiteClientSocketCopy;
    pthread_mutex_lock(&count_mutex);
    for (int i = 0; i < gamesPtr->size(); i++) {
        if ((!args[0].compare((*gamesPtr)[i].name)) && ((*gamesPtr)[i].whiteClientSocket == 0)) {
            (*gamesPtr)[i].whiteClientSocket = clientSocket;
            blackClientSocketCopy = (*gamesPtr)[i].blackClientSocket;
            whiteClientSocketCopy = (*gamesPtr)[i].whiteClientSocket;
            //delete the game from the gamesPtr vector
            pthread_mutex_unlock(&count_mutex);
            //joined the game, write 1 to the client
            server.writeIntToClient(clientSocket, 1);
            //play the game
            server.twoClientsCommunication(blackClientSocketCopy, whiteClientSocketCopy);
            //delete the game from the list of games
            pthread_mutex_lock(&count_mutex);
            gamesPtr->erase(gamesPtr->begin() + i);
            pthread_mutex_unlock(&count_mutex);
            return;
        }
    }
    pthread_mutex_unlock(&count_mutex);
    //cant join this game, write -1 to the client
    server.writeIntToClient(clientSocket, -1);
}