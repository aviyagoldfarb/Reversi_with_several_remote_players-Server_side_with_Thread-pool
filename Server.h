//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#ifndef EX4_SERVER_SERVER_H
#define EX4_SERVER_SERVER_H

#include "Point.h"
#include <vector>

struct CommandOrder {
    string command;
    vector<string> args;
};

class Server {
public:
    Server(int port);
    void start();
    void writeIntToClient(int clientSocket, int buffer);
    void writeToClient(int clientSocket, char* buffer);
    void twoClientsCommunication(int blackClientSocket, int whiteClientSocket);
    void handleClients(int blackClientSocket, int whiteClientSocket);
    Point readCell(int client);
    int getServerSocket();
    int getClientSocket();
    void setClientSocket(int newClientSocket);
    //vector<pthread_t> threads;
    void stop();

private:
    int port;
    //the socket's file descriptor
    int serverSocket;
    //the client's file descriptor
    int clientSocket;
    pthread_t serverThreadId;

    pthread_mutex_t count_mutex;

    //vector that will hold the id of each thread
    //vector<pthread_t> threads;
};

#endif //EX4_SERVER_SERVER_H
