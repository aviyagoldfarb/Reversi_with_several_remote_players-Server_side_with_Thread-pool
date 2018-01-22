//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "Server.h"
#include "CommandsManager.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <cstdlib>

#define MAX_CONNECTED_CLIENTS 10
#define MAX_COMMAND_LENGTH 50

using namespace std;

static void* acceptClient(void*);
static void* getCommand(void*);

pthread_mutex_t lock_mutex;

Server::Server(int port) :port(port), serverSocket(0), serverThreadId(0) {
    cout << "Server" << endl;
    pthread_mutex_init(&count_mutex, NULL);
}

void Server::start() {
    //Create a socket point
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw "Error opening socket";
    }
    // Assign a local address to the socket
    struct sockaddr_in serverAddress;
    bzero((void *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        throw "Error on binding";
    }

    // Start listening to incoming connections
    listen(serverSocket, MAX_CONNECTED_CLIENTS);

    /*in order to handle each client on a separate thread,
    and to be able to exit from the server and close all sockets/threads*/
    int rc = pthread_create(&serverThreadId, NULL, &acceptClient, (void *)this);
    if (rc) {
        cout << "Error: unable to create thread, " << rc << endl;
        exit(-1);
    }
}

static void* acceptClient(void* server) {
    Server* hostServer = (Server*)server;
    int serverSocket = hostServer->getServerSocket();


    while(true){

        // Define the client socket's structures
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);

        cout << "Waiting for clients connections..." << endl;
        // Accept a new client connection
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientAddressLen);
        cout << "Client connected" << endl;
        if (clientSocket == -1)
            throw "Error on accept client";
        hostServer->setClientSocket(clientSocket);
        pthread_t threadId;
        //hostServer->threads.push_back(thread);
        //int numberOfThreads = (hostServer->threads).size()-1;
        int rc = pthread_create(&threadId/*(hostServer->threads[numberOfThreads])*/, NULL, &getCommand, (void *) hostServer);
        if (rc) {
            cout << "Error: unable to create thread, " << rc << endl;
            exit(-1);
        }
    }
}

static void* getCommand(void* server) {
    Server* hostServer = (Server*)server;

    pthread_mutex_lock(&lock_mutex);
    int clientSocket = hostServer->getClientSocket();
    pthread_mutex_unlock(&lock_mutex);

    bool gameNotPlayed = true;
    while(gameNotPlayed) {
        char buffer[MAX_COMMAND_LENGTH];
        memset(buffer, '\0', MAX_COMMAND_LENGTH);
        int n;
        CommandOrder co;
        CommandOrder *coPtr = &co;

        n = read(clientSocket, buffer, MAX_COMMAND_LENGTH);
        if (n == -1) {
            cout << "Error reading the command" << endl;
            return NULL;
        }
        if (n == 0) {
            cout << "Client disconnected" << endl;
            return NULL;
        }

        //break the buffer into command and arguments
        char *temp;
        int i = 0;
        temp = strtok(buffer, " ");
        while (temp != NULL) {
            if (i == 0) {
                string tempStr(temp);
                coPtr->command = tempStr;
            } else {
                string tempStr(temp);
                coPtr->args.push_back(temp);
            }
            temp = strtok(NULL, " ");
            i++;
        }

        if ((!coPtr->command.compare("join")) || (!coPtr->command.compare("start")) || (!coPtr->command.compare("list_games"))) {
            CommandsManager::getInstance(hostServer)->executeCommand(coPtr->command, coPtr->args, (int) clientSocket);
            if ((!coPtr->command.compare("join")) || (!coPtr->command.compare("start"))) {
                gameNotPlayed = false;
            }
        }
    }
    return NULL;
}

void Server::writeIntToClient(int clientSocket, int buffer) {
    int n;
    // Write the buffer's content to the client
    n = write(clientSocket, &buffer, sizeof(buffer));
    if (n == -1) {
        cout << "Error writing to clientSocket" << endl;
        return;
    }
    return;
}

void Server::writeToClient(int clientSocket, char* buffer) {
    int n;
    // Write the buffer's content to the client
    n = write(clientSocket, buffer, strlen(buffer));
    if (n == -1) {
        cout << "Error writing to clientSocket" << endl;
        return;
    }
    return;
}

void Server::twoClientsCommunication(int blackClientSocket, int whiteClientSocket) {
        int black = 1;
        int white = 2;
        int n;
        n = write(blackClientSocket, &black, sizeof(black));
        if (n == -1) {
            cout << "Error writing to blackClientSocket" << endl;
            return;
        }

        n = write(whiteClientSocket, &white, sizeof(white));
        if (n == -1) {
            cout << "Error writing to whiteClientSocket" << endl;
            return;
        }

        // the game
        handleClients(blackClientSocket, whiteClientSocket);

        // Close communication with the current clients
        close(blackClientSocket);
        close(whiteClientSocket);
}

// Handle requests from a specific client
void Server::handleClients(int blackClientSocket, int whiteClientSocket) {
    int x, y;

    while (true) {
        int n;

        //getting the chosen cell of the black player.
        Point blackPlayerChosenCell = this->readCell(blackClientSocket);

        if (blackPlayerChosenCell.getX() == -1 && blackPlayerChosenCell.getY() == -1) {
            return;
        }


        cout << "Got cell: ";
        blackPlayerChosenCell.pointToPrint();
        cout << endl;


        x = blackPlayerChosenCell.getX();
        y = blackPlayerChosenCell.getY();

        // write the result back to the white client
        n = write(whiteClientSocket, &x, sizeof(x));
        if (n == -1) {
            cout << "Error writing to whiteClientSocket" << endl;
            return;
        }
        n = write(whiteClientSocket, &y, sizeof(y));
        if (n == -1) {
            cout << "Error writing to whiteClientSocket" << endl;
            return;
        }

        //getting the chosen cell of the white player.
        Point whitePlayerChosenCell = this->readCell(whiteClientSocket);

        if (whitePlayerChosenCell.getX() == -1 && whitePlayerChosenCell.getY() == -1) {
            return;
        }


        cout << "Got cell: ";
        whitePlayerChosenCell.pointToPrint();
        cout << endl;


        x = whitePlayerChosenCell.getX();
        y = whitePlayerChosenCell.getY();

        // Write the result back to the black client
        n = write(blackClientSocket, &x, sizeof(x));
        if (n == -1) {
            cout << "Error writing to blackClientSocket" << endl;
            return;
        }
        n = write(blackClientSocket, &y, sizeof(y));
        if (n == -1) {
            cout << "Error writing to blackClientSocket" << endl;
            return;
        }

    }

}

Point Server::readCell(int client) {
    Point chosenCell(-1, -1);
    int x, y;
    int n;

    n = read(client, &x, sizeof(x));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return chosenCell;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return chosenCell;
    }

    n = read(client, &y, sizeof(y));
    if (n == -1) {
        cout << "Error reading the chosen cell" << endl;
        return chosenCell;
    }
    if (n == 0) {
        cout << "Client disconnected" << endl;
        return chosenCell;
    }

    chosenCell.setX(x);
    chosenCell.setY(y);

    return chosenCell;
}

int Server::getServerSocket(){
    return serverSocket;
}

int Server::getClientSocket(){
    return clientSocket;
}

void Server::setClientSocket(int newClientSocket){
    pthread_mutex_lock(&count_mutex);
    clientSocket = newClientSocket;
    pthread_mutex_unlock(&count_mutex);
}

void Server::stop() {
    //CommandsManager::getInstance(this)->stopClients();
    CommandsManager::getInstance(this)->deleteInstance();
    pthread_cancel(serverThreadId);
    close(serverSocket);
    pthread_mutex_destroy(&count_mutex);
    cout << "Server stopped" << endl;
}