#ifndef PACKET__HPP
#define PACKET__HPP

#include <cstdint>
#include <cstddef>

class Packet
{
    public:
    typedef char byte;

    public:
    enum
    {
        HEADER_SIZE = 2 + 8 // 2 byte for type + 8 byte for bodySize
    };

    private:
    size_t m_size = 0;
    byte* m_data = nullptr;

    public:
    Packet();
    Packet(uint16_t _type, size_t _bodySize = 0, const void* _body = nullptr);
    Packet(const Packet& _other);
    Packet(Packet&& _other);
    ~Packet();

    byte* Data();
    byte* Header();
    byte* Body();

    const byte* Data() const;
    const byte* Header() const;
    const byte* Body() const;

    uint16_t Type() const;
    uint64_t BodySize() const;

    size_t Size() const;

    void AllocateBody();
    void FreeBody();

    void ClearHeader();

    private:
    void SetHeader(uint16_t _type, uint64_t _bodySize);
    void SetDataSize(size_t _size);
};

#endif // !PACKET__HPP