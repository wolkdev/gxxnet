
#include "Socket.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int x_pos_;

void server()
{
    char buffer[500];
    char message[] = "hello world";
    size_t size;

    Socket socket = Socket::Create(Socket::PROTOCOL::TCP);

    if (socket.Bind(8080) && socket.Listen())
    {
        std::cout << "Server Is Listening !" << std::endl;

        Socket client = socket.Accept();

        client.Send(message, sizeof(message), &size);
        client.Receive(buffer, sizeof(buffer), &size);

        std::cout << buffer << std::endl;
    }
    else
    {
        std::cout << "Server Initialization Failed !" << std::endl;
    }
}

void client(std::string _ip)
{
    char buffer[500];
    char message[] = "hello my friend !";
    size_t size;

    Socket socket = Socket::Create(Socket::PROTOCOL::TCP);

    if (socket.Connect(_ip, 8080))
    {
        std::cout << "Client Connected !" << std::endl;

        socket.Receive(buffer, sizeof(buffer), &size);
        std::cout << buffer << std::endl;
        socket.Send(message, sizeof(message), &size);
    }
    else
    {
        std::cout << "Client Connection Failed !" << std::endl;
    }
}

int main(int argc, char const* argv[])
{
    if (argc >= 2)
    {
        std::string arg = std::string(argv[1]);

        if (argc >= 3 && arg == "client")
        {
            std::string ip = std::string(argv[2]);
            client(ip);
        }
        else if (arg == "server")
        {
            server();
        }
    }

    return 0;
}