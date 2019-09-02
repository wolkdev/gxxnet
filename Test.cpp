
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

    Socket socket(Socket::PROTOCOL::TCP);
    socket.Bind(8080);
    socket.Listen();

    Socket client;
    socket.Accep(client);

    client.Send(message, sizeof(message), size);
    client.Receive(buffer, sizeof(buffer), size);

    std::cout << buffer << std::endl;
}

void client(std::string _ip)
{
    char buffer[500];
    char message[] = "hello my friend !";
    size_t size;

    Socket socket(Socket::PROTOCOL::TCP);
    socket.Connect(_ip, 8080);

    socket.Receive(buffer, sizeof(buffer), size);

    std::cout << buffer << std::endl;

    socket.Send(message, sizeof(message), size);
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

    system("pause");

    return 0;
}