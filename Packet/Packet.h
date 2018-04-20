#ifndef SOCKET_PROGRAMMING_PACKET_H
#define SOCKET_PROGRAMMING_PACKET_H

class Packet {
public:
    enum PacketType {DATA, END};

    PacketType type;
    void* data;
    unsigned int dataSize;
    unsigned int count;
};


#endif //SOCKET_PROGRAMMING_PACKET_H
