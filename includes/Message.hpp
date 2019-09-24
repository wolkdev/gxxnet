#ifndef MESSAGE__HPP
#define MESSAGE__HPP

#include "Packet.hpp"

class Message
{
    public:
    enum TYPE
    {
        EMPTY,
        CONNECTION,
        CONNECTION_FAILED,
        DISCONNECTION,
        ACCEPT,
        DATA,
    };

    public:
    TYPE type = EMPTY;
    int senderID = 0;
    Packet* packet = nullptr;

    Message() { }
    Message(TYPE _type) : type(_type) { }
};

#endif // !MESSAGE__HPP