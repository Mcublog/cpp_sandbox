/**
 * @file bitarray.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2024-10-06
 *
 * @copyright Mcublog Copyright (c) 2024
 *
 */
#include <algorithm>
#include <climits>
#include <cstring>

#include "bitarray.hpp"

//>>---------------------- Locals
#define BIT_CHAR(bit)         ((bit) / CHAR_BIT)
#define BIT_IN_CHAR(bit)      (1 << (CHAR_BIT - 1 - ((bit)  % CHAR_BIT)))
#define BITS_TO_CHARS(bits)   ((((bits) - 1) / CHAR_BIT) + 1)
//<<----------------------

void bitarray::set_all()
{
    uint32_t size = BITS_TO_CHARS(m_num_bits);

    /* set bits in all bytes to 1 */
    std::memset(m_array, UCHAR_MAX, size);

    /* zero any spare bits so increment and decrement are consistent */
    uint32_t bits = m_num_bits % CHAR_BIT;
    if (bits != 0)
    {
        uint8_t mask = UCHAR_MAX << (CHAR_BIT - bits);
        m_array[BIT_CHAR(m_num_bits - 1)] = mask;
    }
}

void bitarray::clear_all()
{
    uint32_t size = BITS_TO_CHARS(m_num_bits);

    /* set bits in all bytes to 0 */
    std::memset(m_array, 0, size);
}

void bitarray::set_bit(uint32_t bit)
{
    if (m_num_bits <= bit)
    {
        return;         /* bit out of range */
    }

    m_array[BIT_CHAR(bit)] |= BIT_IN_CHAR(bit);
}

void bitarray::clear_bit(uint32_t bit)
{
    if (m_num_bits <= bit)
    {
        return;         /* bit out of range */
    }

    /* create a mask to zero out desired bit */
    uint8_t mask = BIT_IN_CHAR(bit);
    mask = ~mask;

    m_array[BIT_CHAR(bit)] &= mask;
}

void bitarray::reverse()
{
    uint32_t size = BITS_TO_CHARS(m_num_bits);
    uint8_t *istart = m_array, *iend = m_array + size;
    std::reverse(istart, iend);
}