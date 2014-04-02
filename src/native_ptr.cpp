// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#include "common_headers.h"

#include "kernel.h"
#include "tagged_value.h"
#include "type.h"
#include "native_ptr.h"
#include "types/common.h"

namespace circa {

struct NativePtr
{
    int refCount;
    void* ptr;
    caNativePtrRelease release;
};

bool is_native_ptr(caValue* value)
{
    return value->value_type == TYPES.native_ptr;
}

void* as_native_ptr(caValue* value)
{
    ca_assert(is_native_ptr(value));
    NativePtr* data = (NativePtr*) value->value_data.ptr;
    return data->ptr;
}

void set_native_ptr(caValue* value, void* ptr, caNativePtrRelease release)
{
    make_no_initialize(TYPES.native_ptr, value);
    NativePtr* data = new NativePtr();
    data->refCount = 1;
    data->ptr = ptr;
    data->release = release;
    value->value_data.ptr = data;
}

void native_ptr_initialize(Type*, caValue* value)
{
    NativePtr* data = new NativePtr();
    data->refCount = 1;
    data->ptr = NULL;
    data->release = NULL;
    value->value_data.ptr = data;
}

void native_ptr_copy(Type*, caValue* source, caValue* dest)
{
    make_no_initialize(source->value_type, dest);

    NativePtr* data = (NativePtr*) source->value_data.ptr;
    data->refCount++;
    dest->value_data.ptr = data;
}

void native_ptr_release(caValue* value)
{
    NativePtr* data = (NativePtr*) value->value_data.ptr;
    data->refCount--;
    if (data->refCount <= 0) {
        if (data->release != NULL)
            data->release(data->ptr);
        delete data;
        value->value_data.ptr = NULL;
    }
}

void native_ptr_setup_type(Type* type)
{
    set_string(&type->name, "native_ptr");
    type->initialize = native_ptr_initialize;
    type->copy = native_ptr_copy;
    type->release = native_ptr_release;
    type->storageType = sym_StorageTypeOpaquePointer;
    type->hashFunc = common_type_callbacks::shallow_hash_func;
}

CIRCA_EXPORT void* circa_native_ptr(caValue* val)
{
    return as_native_ptr(val);
}

CIRCA_EXPORT void circa_set_native_ptr(caValue* val, void* ptr, caNativePtrRelease release)
{
    set_native_ptr(val, ptr, release);
}

} // namespace circa