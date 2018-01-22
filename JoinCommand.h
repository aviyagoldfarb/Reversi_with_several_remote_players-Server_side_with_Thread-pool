//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#ifndef EX5_SERVER_JOINCOMMAND_H
#define EX5_SERVER_JOINCOMMAND_H

#include "Command.h"
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <map>

class JoinCommand : public Command {
    public:
    JoinCommand();
    virtual void execute(vector<string> args, Server server, vector<Game>* games, int clientSocket);
    ~JoinCommand(){
        pthread_mutex_destroy(&count_mutex);
    }
};

#endif //EX5_SERVER_JOINCOMMAND_H
