//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "CommandsManager.h"
#include "StartCommand.h"
#include "JoinCommand.h"
#include "ListGamesCommand.h"

CommandsManager* CommandsManager::instance = 0;
pthread_mutex_t CommandsManager::lock;

CommandsManager* CommandsManager::getInstance(Server* server) {
    if (instance == 0) {
        pthread_mutex_lock(&lock);
        if (instance == 0) {
            instance = new CommandsManager(server);
        }
        pthread_mutex_unlock(&lock);
    }
    return instance;
}

CommandsManager* CommandsManager::deleteInstance() {
    if (instance != 0) {
        pthread_mutex_lock(&lock);
        if (instance != 0) {
            delete instance;
        }
        pthread_mutex_unlock(&lock);
    }
}

CommandsManager::CommandsManager(Server* server) : server(server) {
    commandsMap["start"] = new StartCommand();
    commandsMap["list_games"] = new ListGamesCommand();
    commandsMap["join"] = new JoinCommand();
}

void CommandsManager::executeCommand(string command, vector<string> args, int clientSocket) {
    Command *commandObj = commandsMap[command];
    commandObj->execute(args, *server, &games, clientSocket);
}
/*
void CommandsManager::stopClients() {
    for (int i = 0; i < games.size(); i++) {
        server->writeIntToClient(games[i].blackClientSocket, -2);
        if (games[i].whiteClientSocket != 0) {
            server->writeIntToClient(games[i].whiteClientSocket, -2);
        }
    }
}
*/
CommandsManager::~CommandsManager() {
    map<string, Command *>::iterator it;
    for (it = commandsMap.begin(); it != commandsMap.end(); it++) {
        delete it->second;
    }
}