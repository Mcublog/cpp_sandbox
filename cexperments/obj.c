/**
 * @file obj.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2026-03-01
 *
 * @copyright Mcublog Copyright (c) 2026
 *
 */
#include <assert.h>
#include <string.h>

#include "obj.h"

typedef struct obj
{
    uint32_t id;
    uint32_t mark;
} obj;

const size_t obj_size(void)
{
    return sizeof(obj);
}

obj *obj_placement_new(void *mem, size_t s)
{
    assert(obj_size() == OBJ_SIZE);

    const size_t sz = obj_size();
    if (s < sz)
        return NULL;

    memset(mem, 0x00, sz);
    obj *o = (obj *)mem;
    o->mark = 0xDEADBEAF;
    return o;
}

uint32_t obj_id(const obj *self)
{
    return self->id;
}

uint32_t obj_mark(const obj *self)
{
    return self->mark;
}
