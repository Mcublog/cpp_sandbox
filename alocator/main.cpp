/**
 * @file main.cpp
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2025-06-30
 *
 * @copyright Mcublog Copyright (c) 2025
 *
 */
#include <memory_resource>
#include <string>

#include "alocator/static_allocator.hpp"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmain
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------
// void *operator new(size_t s) throw()
// {
//     LOG_ERROR("call new");
//     return nullptr;
// }

int main(void)
{
    LOG_INFO("alocator test");

    custom_resource *salloc = static_allocator_get();
    std::pmr::vector<std::string> strings{salloc};

    strings.emplace_back("Hello Short String 0");
    strings.emplace_back("Hello Short String 1");

    LOG_INFO("-----------Test---------");
    for (const auto &i: strings)
        LOG_INFO("%s", i.c_str());
    LOG_INFO("------------------------");

    // strings.emplace_back("Hello Short String 2"); // oom_handle

    return 0;
}
