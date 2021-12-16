#ifndef STRING_HPP
#define STRING_HPP

#include <stdint.h>
#include "buffer.hpp"

template <uint32_t MAX_SIZE = 16>
class String: public Buffer<MAX_SIZE + 1, char>
{
public:
    String()
    {
        this->at(this->size()) = '\0';
    }

    String(const char* src)
    {
        *this = src;
    }

    String(const String& src)
    {
        *this = src;
    }
    const String& operator = (const char* src)
    {
        this->clear();
        *this += src;
        return *this;
    }
    const String& operator = (const String& src)
    {
        this->clear();
        *this += src;
        return *this;
    }
    const String& operator += (const String& src)
    {
        this->add(src.cdata(), src.size());
        this->at(this->size()) = '\0';
        return *this;
    }
    const String& operator += (const char* src)
    {
        this->add(src, strlen(src));
        this->at(this->size()) = '\0';
        return *this;
    }
};

#endif // STRING_HPP