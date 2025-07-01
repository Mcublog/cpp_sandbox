/**
 * @file static_allocator.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include "static_allocator.hpp"

#include "libs/smalloc/smalloc_i.h"
//>>---------------------- Locals
static uint8_t m_poll[MIN_POOL_SZ] = {0};
static custom_resource *m_ptr = nullptr;
//<<----------------------

custom_resource *static_allocator_get()
{
    if (!m_ptr)
    {
        static custom_resource m_resource{"static", m_poll, MIN_POOL_SZ};
        m_ptr = &m_resource;
    }
    return m_ptr;
}