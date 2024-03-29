#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <thread>
#include <pthread.h>
#include "Server.h"

#include "DefaultIO.h"
#include "SocketIO.h"
#include "CLI.h"

using namespace std;

// this is called from a new thread, so it's run in parallel
void handleClient(int client_sock)
{
    //creates socketIO class for specific client and CLI class to handle client terminal.
    SocketIO sio(client_sock);
    CLI cli(&sio, client_sock);
    cli.start();
}
//server constructor.
Server::Server(const int server_port)
{
    this->server_port = server_port;
}

/*
this function creates socket for server connection.
*/
int Server::createSock()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("error creating socket");
        exit(1);
    }
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(this->server_port);
    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        perror("error binding socket");
        exit(1);
    }
    if (listen(sock, 5) < 0)
    {
        perror("Error listening to socket");
        exit(1);
    }
    return sock;
}

/*
The acceptClient function get as input socket number and creat the client socket.
the function return the client socket.
*/

int Server::acceptClient(int sock)
{
    struct sockaddr_in client_sin;
    unsigned int addr_len = sizeof(client_sin);
    int client_sock = accept(sock, (struct sockaddr *)&client_sin, &addr_len);
    if (client_sock < 0)
    {
        perror("error accepting client");
        exit(1);
    }
    return client_sock;
}

int main(int argc, char *argv[])
{

    try
    {
        stoi(argv[1]);
    }
    catch (exception &)
    {
        perror("server port must be an int");
        exit(1);
    }

    int server_port = stoi(argv[1]);
    /*
    check server port is in valid range.
    */
    if (!(server_port >= 1024 && server_port <= 65535))
    {
        cout << "Error: invalid port_number" << endl;
        exit(1);
    }

    // initialize server object

    Server server = Server(server_port);
    //creates socket.
    int sock = server.createSock();

    //while true, accepts new clients and handles each client in a different thread.
    while (true)
    {
        int client_sock = server.acceptClient(sock);

        thread t(handleClient, client_sock);
        t.detach();
    }
}
