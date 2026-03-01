#pragma once

#include <stddef.h>
#include <stdint.h>


#define OBJ_SIZE (8U)

typedef struct obj obj;

#ifdef __cplusplus
extern "C"
{
#endif

    const size_t obj_size(void);
    obj *obj_placement_new(void *mem, size_t s);
    uint32_t obj_id(const obj *self);
    uint32_t obj_mark(const obj *self);

#ifdef __cplusplus
}
#endif