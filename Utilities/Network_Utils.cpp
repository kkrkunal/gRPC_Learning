#include "Network_Utils.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <net/if.h>
#endif

std::string NetworkUtils::getPrimaryIP() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return "127.0.0.1";
#endif

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
#ifdef _WIN32
        WSACleanup();
#endif
        return "127.0.0.1";
    }

    // Resolves outbound route via gateway without transmitting packets
    sockaddr_in target{};
    target.sin_family = AF_INET;
    target.sin_addr.s_addr = inet_addr("8.8.8.8");
    target.sin_port = htons(53);

    if (connect(sock, reinterpret_cast<struct sockaddr*>(&target), sizeof(target)) < 0) {
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return "127.0.0.1";
    }

    sockaddr_in local{};
#ifdef _WIN32
    int len = sizeof(local);
#else
    socklen_t len = sizeof(local);
#endif
    if (getsockname(sock, reinterpret_cast<struct sockaddr*>(&local), &len) < 0) {
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return "127.0.0.1";
    }

    char ip_buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &local.sin_addr, ip_buffer, sizeof(ip_buffer));

#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    return std::string(ip_buffer);
}

std::vector<InterfaceInfo> NetworkUtils::getAllInterfaceIPs() {
    std::vector<InterfaceInfo> result;

#ifndef _WIN32
    struct ifaddrs* ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1) {
        return result;
    }

    for (struct ifaddrs* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) continue;

        // Skip loopback interfaces or inactive interfaces
        if ((ifa->ifa_flags & IFF_LOOPBACK) || !(ifa->ifa_flags & IFF_UP)) {
            continue;
        }

        // Only parse IPv4 addresses
        if (ifa->ifa_addr->sa_family == AF_INET) {
            char host[INET_ADDRSTRLEN];
            auto* sa = reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr);
            inet_ntop(AF_INET, &(sa->sin_addr), host, sizeof(host));
            result.push_back({ifa->ifa_name, std::string(host)});
        }
    }

    freeifaddrs(ifaddr);
#endif

    return result;
}