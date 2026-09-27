#pragma once

#include <string>
#include <vector>

struct InterfaceInfo {
    std::string name; // e.g. "wlan0", "eth0"
    std::string ip;   // e.g. "192.168.1.45"
};

class NetworkUtils {
public:
    // Disables instantiation since this is a static utility class
    NetworkUtils() = delete;

    // Returns the primary outbound IPv4 address (via the route resolution trick)
    static std::string getPrimaryIP();

    // Returns all active, non-loopback network interfaces and their IPs
    static std::vector<InterfaceInfo> getAllInterfaceIPs();
};