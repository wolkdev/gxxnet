
#include "Socket.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void tcpserver()
{
    char buffer[500];
    char message[] = "hello world";
    size_t size;

    Socket socket = Socket::Create(Socket::PROTOCOL::TCP);

    if (socket.Bind(8080) && socket.Listen())
    {
        std::cout << "tcpserver Is Listening !" << std::endl;

        Socket client = socket.Accept();

        client.Send(message, sizeof(message), &size);
        client.Receive(buffer, sizeof(buffer), &size);

        std::cout << buffer << std::endl;
    }
    else
    {
        std::cout << "tcpserver Initialization Failed ! = " << socket.GetLastError() << std::endl;
    }
}

void tcpclient(std::string& _ip)
{
    char buffer[500];
    char message[] = "hello my friend !";
    size_t size;

    Socket socket = Socket::Create(Socket::PROTOCOL::TCP);

    if (socket.Connect(_ip, 8080))
    {
        std::cout << "tcpclient Connected !" << std::endl;

        socket.Receive(buffer, sizeof(buffer), &size);
        std::cout << buffer << std::endl;
        socket.Send(message, sizeof(message), &size);
    }
    else
    {
        std::cout << "tcpclient Connection Failed ! = " << socket.GetLastError() << std::endl;
    }
}

void udpsender(std::string& _ip)
{
    char buffer[500];
    char message[] = "hello world";
    size_t size;

    Socket socket = Socket::Create(Socket::PROTOCOL::UDP);

    if (socket.Bind(_ip, 8080))
    {
        std::cout << "udpsender Binded !" << std::endl;

        socket.Send(message, sizeof(message), &size);
        socket.Receive(buffer, sizeof(buffer), &size);

        std::cout << buffer << std::endl;
    }
    else
    {
        std::cout << "udpsender Bind Failed ! = " << socket.GetLastError() << std::endl;
    }
}

void udpreceiver(std::string& _ip)
{
    char buffer[500];
    char message[] = "hello my friend !";
    size_t size;

    Socket socket = Socket::Create(Socket::PROTOCOL::UDP);

    if (socket.Bind(8080))
    {
        std::cout << "udpreceiver Binded !" << std::endl;

        socket.Receive(buffer, sizeof(buffer), &size);
    }
    else
    {
        std::cout << "udpreceiver Bind Failed ! = " << socket.GetLastError() << std::endl;
    }
}

int main(int argc, char const* argv[])
{
    if (argc >= 2)
    {
        std::string arg = std::string(argv[1]);

        if (argc >= 3)
        {
            std::string ip = std::string(argv[2]);
            
            if (arg == "tcpclient")
            {
                tcpclient(ip);
            }
            else if (arg == "udpreceiver")
            {
                udpreceiver(ip);
            }
            else if (arg == "udpsender")
            {
                udpsender(ip);
            }
        }

        if (arg == "tcpserver")
        {
            tcpserver();
        }
    }

    return 0;
}