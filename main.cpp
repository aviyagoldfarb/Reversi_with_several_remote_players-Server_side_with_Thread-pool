//
// Udi Goldman 301683264 , Aviya Goldfarb 201509635
//

#include "Server.h"
#include <stdlib.h>
#include <fstream>

using namespace std;

int main() {
    int port;
    ifstream inFile;
    inFile.open("server_configuration_file.txt");

    if (inFile.is_open()) {
        inFile >> port;
        inFile.close();
    }
    else cout << "Unable to open file";

    Server server(port);

    try {
        server.start();
        cout << "Enter exit to stop server" << endl;
        string exitStr;
        cin >> exitStr;
        if (exitStr == "exit")
            server.stop();
    } catch (const char *msg) {
        cout << "Cannot start server. Reason: " << msg << endl;
        exit(-1);
    }
}