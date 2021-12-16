#include "ip_address.hpp"
#include <tcpip_adapter.h>

IPAddress::IPAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    m_ip = IPADDR4_INIT_BYTES(a, b, c, d);
}

IPAddress::IPAddress(ip_addr_t ip)
    : m_ip(ip)
{}

uint8_t IPAddress::operator[] (uint8_t i) const
{
    return ip4_addr_get_byte(const_cast<ip_addr_t*>(&m_ip), i);
}

String<16> IPAddress::str() const
{
    String<16> s;
    s += ip4addr_ntoa(&m_ip);
    return s;
}