
#include "Packet.hpp"

#include <cstdlib>
#include <cstring>
#include <stdexcept>

Packet::Packet()
{
    SetDataSize(HEADER_SIZE);
    ClearHeader();
}

Packet::Packet(uint16_t _type, size_t _bodySize, const void* _body)
{
    SetDataSize(HEADER_SIZE + _bodySize);
    SetHeader(_type, _bodySize);
    memcpy(Body(), _body, _bodySize);
}

Packet::Packet(const Packet& _other)
{
    SetDataSize(_other.Size());
    memcpy(Data(), _other.Data(), _other.Size());
}

Packet::Packet(Packet&& _other)
{
    m_size = _other.m_size;
    m_data = _other.m_data;
}

Packet::~Packet()
{
    free(m_data);
}

Packet::byte* Packet::Data()
{
    return m_data;
}

Packet::byte* Packet::Header()
{
    return Data();
}

Packet::byte* Packet::Body()
{
    if (m_size > HEADER_SIZE)
        return &m_data[HEADER_SIZE];

    return nullptr;
}

const Packet::byte* Packet::Data() const
{
    return const_cast<Packet*>(this)->Data();
}

const Packet::byte* Packet::Header() const
{
    return Data();
}

const Packet::byte* Packet::Body() const
{
    return const_cast<Packet*>(this)->Body();
}

uint16_t Packet::Type() const
{
    return *reinterpret_cast<const uint16_t*>(&m_data[0]);
}

uint64_t Packet::BodySize() const
{
    return *reinterpret_cast<const uint64_t*>(&m_data[2]);
}

uint64_t Packet::Size() const
{
    return m_size;
}

void Packet::AllocateBody()
{
    uint64_t bodySize = BodySize();

    FreeBody();

    if (bodySize > 0)
        SetDataSize(HEADER_SIZE + bodySize);
}

void Packet::FreeBody()
{
    SetDataSize(HEADER_SIZE);
}

void Packet::ClearHeader()
{
    memset(m_data, 0, HEADER_SIZE);
}

void Packet::SetHeader(uint16_t _type, uint64_t _bodySize)
{
    *reinterpret_cast<uint16_t*>(&m_data[0]) = _type;
    *reinterpret_cast<uint64_t*>(&m_data[2]) = _bodySize;
}

void Packet::SetDataSize(size_t _size)
{
    if (m_size == _size)
        return;

    m_size = _size;

    if (m_data == nullptr)
        m_data = reinterpret_cast<byte*>(malloc(m_size));
    else
        m_data = reinterpret_cast<byte*>(realloc(m_data, m_size));

    if (m_data == nullptr)
        throw std::overflow_error("can't allocate data !");
}