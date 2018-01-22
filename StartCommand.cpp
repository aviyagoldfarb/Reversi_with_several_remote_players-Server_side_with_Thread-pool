//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "StartCommand.h"
#include <cstring>

StartCommand :: StartCommand() {
    pthread_mutex_init(&count_mutex, NULL);
}

void StartCommand ::execute(vector<string> args, Server server, vector<Game> *gamesPtr, int clientSocket) {
    pthread_mutex_lock(&count_mutex);
    for (int i = 0; i < gamesPtr->size(); i++) {
        if (!args[0].compare((*gamesPtr)[i].name)) {
            pthread_mutex_unlock(&count_mutex);
            //A game with this name is already exist, write -1 to the client
            server.writeIntToClient(clientSocket, -1);
            return;
        }
    }
    Game game;
    game.name = args[0];
    game.blackClientSocket = clientSocket;
    game.whiteClientSocket = 0;

    gamesPtr->push_back(game);
    pthread_mutex_unlock(&count_mutex);

    //A new game was created, write 1 to the client
    server.writeIntToClient(clientSocket, 1);
    return;
}