
#ifndef SOCKET_PROGRAMMING_COMMON_H
#define SOCKET_PROGRAMMING_COMMON_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>

wchar_t* WSAGetLastErrorFormatted() {
    wchar_t *s = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR) &s, 0, nullptr);
    return s;
}

void printError(const char* msg) {
    auto formattedError = WSAGetLastErrorFormatted();
    fprintf(stderr, "%s: %S\n", msg, formattedError);
    delete[](formattedError);
}

#endif //SOCKET_PROGRAMMING_COMMON_H
