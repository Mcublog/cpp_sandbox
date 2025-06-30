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

#include "libs/smalloc/smalloc.h"
#include "libs/smalloc/smalloc_i.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME cmain
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------
static uint8_t poll[MIN_POOL_SZ] = {0};

class debug_resource : public std::pmr::memory_resource
{
  public:
    explicit debug_resource(std::string name, void *buffer, size_t size)
        : _name{std::move(name)}
    {
        int err = sm_set_default_pool(buffer, size, 0, &oom_handle);
    }

    void *do_allocate(size_t bytes, size_t alignment) override
    {
        LOG_INFO("%s: do_allocate: %d", _name.c_str(), bytes);
        void* ret = sm_malloc(bytes);
        print_stat();
        return ret;
    }
    void do_deallocate(void *ptr, size_t bytes, size_t alignment) override
    {
        LOG_INFO("%s: do_deallocate(): %d", _name.c_str(), bytes);
        sm_free(ptr);
        print_stat();
    }

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override
    {
        return this == &other;
    }

  private:
    std::string _name;
    std::pmr::memory_resource *_upstream;

    static size_t oom_handle(struct smalloc_pool *pool, size_t size)
    {
        LOG_ERROR("oom_handle: not enough mem: %d", size);
        return (size_t)-1;
    }

    static void print_stat()
    {
        size_t total, user, free = 0;
        int nr_blocks = 0;
        sm_malloc_stats(&total, &user, &free, &nr_blocks);
        LOG_INFO("total: %ld uses: %ld free: %ld nr_blocks: %d", total,
                 user, free, nr_blocks);
    }
};

// void *operator new(size_t s) throw()
// {
//     LOG_ERROR("call new");
//     return nullptr;
// }

int main(void)
{
    LOG_INFO("alocato test");

    char buffer[MIN_POOL_SZ] = {}; // a small buffer on the stack
    std::fill_n(std::begin(buffer), std::size(buffer) - 1, '_');

    debug_resource dbg{"default", std::data(buffer), std::size(buffer)};
    std::pmr::vector<std::string> strings{&dbg};

    strings.emplace_back("Hello Short String 0");
    strings.emplace_back("Hello Short String 1");

    LOG_INFO("-----------Test---------");
    for (const auto &i: strings)
        LOG_INFO("%s", i.c_str());
    LOG_INFO("------------------------");

    // strings.emplace_back("Hello Short String 2"); // oom_handle

    return 0;
}
