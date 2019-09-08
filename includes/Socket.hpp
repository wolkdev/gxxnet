#ifndef SOCKET__HPP
#define SOCKET__HPP

#include <string>

class Socket
{
    public:
    enum PROTOCOL
    {
        TCP,
        UDP
    };

    private:
    int nativeSocket = 0;

    private:
    Socket() { }

    public:
    static Socket Create(PROTOCOL _protocol);

    void Close();

    bool IsValid();

    bool Connect(const std::string& _address, unsigned short _port);
    bool Bind(const std::string& _address, unsigned short _port);

    bool Bind(unsigned short _port);
    bool Listen();

    Socket Accept();
    bool Send(const char* _data, size_t _size, size_t* _count = nullptr);
    bool Receive(char* _buffer, size_t _bufferSize, size_t* _count = nullptr);
};

#endif // !SOCKET__HPP