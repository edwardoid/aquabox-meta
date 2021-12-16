#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <math.h>
#include <stdint.h>
#include <memory.h>


#include "log.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))

template< typename T>
class Array
{
public:
    Array(uint16_t capacity = 0)
    : m_size(0)
    , m_capacity(capacity)
    , m_data(nullptr)
    {}
    
    ~Array()
    {
        if (m_data != nullptr)
        delete []m_data;
    }

    void add(const T& obj)
    {
        if (m_size == m_capacity)
        {
            shrink(m_capacity + 1); // Trade to memory. Can't allocate too much memory
        }

        m_data[m_size++] = obj;
    }

    void fast_pop()
    {
        if (m_size) --m_size;
    }

    void shrink(uint16_t size)
    {
        if (size == m_size) return;

        if (size > 0)
        {
            T* np = new T[size];

            m_size = min(m_size, m_capacity);
            for(uint16_t i = 0; i < m_size; ++i)
            {
                np[i] = m_data[i];
            }
            delete[] m_data;
            m_capacity = size;
            m_data = np;
        }
        else
        {
            if (m_data != nullptr)
                delete[] m_data;
            m_data = nullptr;
        }
    }
    inline T& at(uint16_t i) { return m_data[i]; };
    inline void clear() { m_size = 0; }
    inline T& operator[](uint16_t i) { return &m_data[i]; }
    inline const T& operator[](uint16_t i) const { return m_data[i]; }
    inline uint16_t size() const { return m_size; }
    inline uint16_t capacity() const { return m_capacity; }
    inline T* data() { return m_data; }
    inline const T* cdata() const { return m_data; }
    inline T* end() { return m_data + m_size; }
protected:
    uint8_t m_size;
    uint8_t m_capacity;
    T* m_data;
};

#endif // BUFFER_HPP