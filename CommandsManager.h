//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#ifndef EX5_SERVER_COMMANDSMANAGER_H
#define EX5_SERVER_COMMANDSMANAGER_H

#include <map>
#include "Command.h"
#include "Server.h"

class CommandsManager {
public:
    static CommandsManager* getInstance(Server* server);
    static CommandsManager* deleteInstance();
    void executeCommand(string command, vector<string> args, int socket = 0);
    //void stopClients();
private:
    // A singleton
    CommandsManager(Server* server);
    CommandsManager(const CommandsManager &);
    ~CommandsManager();
    static CommandsManager* instance;
    static pthread_mutex_t lock;
    map<string, Command *> commandsMap;
    Server* server;
    vector<Game> games;
};

#endif //EX5_SERVER_COMMANDSMANAGER_H