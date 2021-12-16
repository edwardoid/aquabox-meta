#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <math.h>
#include <stdint.h>
#include <memory.h>


#include "log.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

template < uint16_t MAX_SIZE, typename T>
class Buffer
{
public:
    Buffer()
    : m_size(0)
    {}

    void shrink(uint16_t sz) { m_size = min(capacity(), sz); };

    void add(const T* obj, uint16_t len)
    {
        if (m_size < MAX_SIZE) {
            uint16_t newSize = min(capacity(), m_size + len);
            memcpy(static_cast<void*>(&m_data[m_size]), static_cast<const void*>(obj), sizeof(obj) * (newSize - m_size));
            m_size = newSize;
        }
    }
    void add(const T& obj)
    {
        if (m_size < MAX_SIZE) {
            memcpy(static_cast<void*>(&m_data[m_size++]), static_cast<void*>(&obj), sizeof(obj));
        }
    }
    void takeLast() { m_size = m_size ? --m_size : 0; }
    void remove(int16_t count)
    {
        if (count < size) {
            size -= count;
            memcpy(static_cast<void*>(m_data), static_cast<void*>(&m_data[count]), sizeof(T) * size);
        } else {
            clear();
        }
    }
    inline T& at(uint16_t i) { return m_data[i]; };
    inline void clear() { m_size = 0; }
    inline T& operator[](uint16_t i) { return &m_data[i]; }
    inline const T& operator[](uint16_t i) const { return &m_data[i]; }
    inline uint16_t size() const { return m_size; }
    inline uint16_t capacity() const { return MAX_SIZE; }
    inline T* data() { return m_data; }
    inline const T* cdata() const { return m_data; }
    inline T* end() { return &m_data[m_size]; }
protected:
    uint8_t m_size;
    T m_data[MAX_SIZE];
};

#endif // BUFFER_HPP