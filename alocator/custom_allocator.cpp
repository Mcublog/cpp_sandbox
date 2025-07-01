/**
 * @file custom_allocator.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-07-01
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <cstddef>
#include <memory_resource>
#include <string>

#include "custom_allocator.hpp"

#include "libs/smalloc/smalloc.h"
#include "libs/smalloc/smalloc_i.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME custaloc
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------
custom_resource::custom_resource(const char *name, void *buffer, size_t size)
{
    this->name = name;
    int err = sm_set_default_pool(buffer, size, 0, &custom_resource::oom_handle);
}

void *custom_resource::do_allocate(size_t bytes, size_t alignment)
{
    LOG_INFO("%s: do_allocate: %d", name, bytes);
    void *ret = sm_malloc(bytes);
    print_stat();
    return ret;
}
void custom_resource::do_deallocate(void *ptr, size_t bytes, size_t alignment)
{
    LOG_INFO("%s: do_deallocate(): %d", name, bytes);
    sm_free(ptr);
    print_stat();
}

size_t custom_resource::oom_handle(struct smalloc_pool *pool, size_t size)
{
    LOG_ERROR("oom_handle: not enough mem: %d", size);
    return (size_t)-1;
}

void custom_resource::print_stat()
{
    size_t total, user, free = 0;
    int nr_blocks = 0;
    sm_malloc_stats(&total, &user, &free, &nr_blocks);
    LOG_INFO("total: %ld uses: %ld free: %ld nr_blocks: %d", total, user, free,
             nr_blocks);
}