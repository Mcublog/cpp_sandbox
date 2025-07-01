#pragma once

#include <memory_resource>
#include <string>
#include <cstddef>

class custom_resource : public std::pmr::memory_resource
{
  public:
    explicit custom_resource(const char *name, void *buffer, size_t size);

    void *do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void *ptr, size_t bytes, size_t alignment) override;

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override
    {
        return this == &other;
    }

  private:
    std::pmr::memory_resource *_upstream;
    const char *name;

    static size_t oom_handle(struct smalloc_pool *pool, size_t size);
    static void print_stat();
};