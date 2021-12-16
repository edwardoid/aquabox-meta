#ifndef IP_ADDRESS_HPP
#define IP_ADDRESS_HPP

#include "string.hpp"
#include <esp_netif.h>

class IPAddress
{
public:
    IPAddress(uint8_t a = 0, uint8_t b = 0, uint8_t c = 0, uint8_t d = 0);
    uint8_t operator[](uint8_t component) const;
    String<16> str() const;
protected:
    friend class WiFiImpl;
    IPAddress(ip_addr_t ip);
private:
    ip_addr_t m_ip;
};


#endif // IP_ADDRESS_HPP