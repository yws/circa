// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#include "common_headers.h"

#include "blob.h"
#include "kernel.h"
#include "tagged_value.h"
#include "type.h"

namespace circa {

const int BLOB_SLICE = 1;
const int BLOB_RAW = 2;

struct BlobAbstract {
    int concreteType;
    int refcount;
    u32 numBytes;
};

struct BlobRaw {
    int concreteType;
    int refcount;
    u32 numBytes;
    char data[0];
};

struct BlobSlice {
    int concreteType;
    int refcount;
    u32 numBytes;
    Value backingValue;
    char* data;
};

BlobSlice* blob_alloc_slice()
{
    BlobSlice* slice = (BlobSlice*) malloc(sizeof(*slice));
    slice->concreteType = BLOB_SLICE;
    slice->refcount = 1;
    initialize_null(&slice->backingValue);
    slice->data = NULL;
    slice->numBytes = 0;
    return slice;
}

BlobRaw* blob_alloc_raw(char* data, u32 numBytes)
{
    BlobRaw* blob = (BlobRaw*) malloc(sizeof(*blob) + numBytes);
    blob->concreteType = BLOB_RAW;
    blob->refcount = 1;
    blob->numBytes = numBytes;
    if (data == NULL)
        memset(blob->data, 0, numBytes);
    else
        memcpy(blob->data, data, numBytes);
    return blob;
}

char* set_blob_alloc_raw(Value* value, char* data, u32 numBytes)
{
    BlobRaw* raw = blob_alloc_raw(data, numBytes);
    make_no_initialize(TYPES.blob, value);
    value->value_data.ptr = raw;
    return raw->data;
}

void decref(BlobAbstract* blob)
{
    if (blob == NULL)
        return;

    ca_assert(blob->refcount > 0);

    blob->refcount--;
    if (blob->refcount == 0) {
        switch (blob->concreteType) {
        case BLOB_SLICE:
            BlobSlice* slice = (BlobSlice*) blob;
            set_null(&slice->backingValue);
            break;
        }
        free(blob);
    }
}

CIRCA_EXPORT void circa_blob_data(Value* blobVal, char** dataOut, u32* sizeOut)
{
    ca_assert(is_blob(blobVal));

    if (blobVal->value_data.ptr == NULL) {
        *dataOut = NULL;
        *sizeOut = 0;
        return;
    }

    BlobAbstract* blob = (BlobAbstract*) blobVal->value_data.ptr;

    switch (blob->concreteType) {
    case BLOB_SLICE: {
        BlobSlice* slice = (BlobSlice*) blob;
        *dataOut = slice->data;
        *sizeOut = slice->numBytes;
        break;
    }
    case BLOB_RAW: {
        BlobRaw* raw = (BlobRaw*) blob;
        *dataOut = raw->data;
        *sizeOut = raw->numBytes;
        break;
    }
    }
}

void blob_copy(Value* source, Value* dest)
{
    make_no_initialize(TYPES.blob, dest);

    if (source->value_data.ptr != NULL) {
        dest->value_data.ptr = source->value_data.ptr;
        ((BlobAbstract*) source->value_data.ptr)->refcount++;
    }
}

void blob_release(Value* value)
{
    if (value->value_data.ptr == NULL)
        return;
    decref((BlobAbstract*) value->value_data.ptr);
}

int blob_hash(Value* value)
{
    char* data;
    u32 size;
    blob_data(value, &data, &size);

    // Dumb and simple hash function
    int result = 0;
    int byte = 0;
    for (u32 i=0; i < size; i++) {
        result = result ^ (data[i] << (8 * byte));
        byte = (byte + 1) % 4;
    }
    return result;
}

char* blob_touch(Value* blobVal)
{
    BlobAbstract* blob = (BlobAbstract*) blobVal->value_data.ptr;

    if (blob == NULL)
        return NULL;

    if (blob->concreteType == BLOB_RAW && ((BlobRaw*) blob)->refcount == 1)
        return ((BlobRaw*) blob)->data;

    char* data;
    u32 numBytes;
    blob_data(blobVal, &data, &numBytes);
    return set_blob_alloc_raw(blobVal, data, numBytes);
}

u32 blob_size(Value* blobVal)
{
    BlobAbstract* blob = (BlobAbstract*) blobVal->value_data.ptr;
    if (blob == NULL)
        return 0;
    return blob->numBytes;
}

CIRCA_EXPORT void circa_set_blob_from_backing_value(Value* blob, Value* backingValue, char* data, u32 numBytes)
{
    BlobSlice* slice = blob_alloc_slice();
    copy(backingValue, &slice->backingValue);
    slice->data = data;
    slice->numBytes = numBytes;

    make_no_initialize(TYPES.blob, blob);
    blob->value_data.ptr = slice;
}

void blob_setup_type(Type* type)
{
    set_string(&type->name, "Blob");
    type->copy = blob_copy;
    type->release = blob_release;
    type->hashFunc = blob_hash;
}

void make_blob(Stack* stack)
{
    u32 size = circa_int_input(stack, 0);
    set_blob_alloc_raw(circa_output(stack, 0), NULL, size);
}

void Blob__size(Stack* stack)
{
    set_int(circa_output(stack, 0), blob_size(circa_input(stack, 0)));
}

void Blob__slice(Stack* stack)
{
    Value* existingBlob = circa_input(stack, 0);
    int offset = circa_int_input(stack, 1);
    int size = circa_int_input(stack, 2);

    char* existingData;
    u32 existingNumBytes;
    blob_data(existingBlob, &existingData, &existingNumBytes);
    
    if ((offset + size) >= existingNumBytes)
        return circa_output_error(stack, "Offset+size is out of bounds");

    circa_set_blob_from_backing_value(circa_output(stack, 0), existingBlob, existingData+offset, size);
}

#define BLOB_SET(type) \
    Value* blob = circa_input(stack, 0); \
    int offset = circa_int_input(stack, 1); \
    \
    if ((offset + sizeof(type)) > blob_size(blob)) \
        return circa_output_error(stack, "Offset is out of bounds"); \
    \
    char* data = blob_touch(blob); \
    *(type*)(data + offset) = circa_int_input(stack, 2); \
    move(blob, circa_output(stack, 0));

void Blob__set_u8(Stack* stack)
{
    BLOB_SET(u8);
}

void Blob__set_u16(Stack* stack)
{
    BLOB_SET(u16);
}

void Blob__set_u32(Stack* stack)
{
    BLOB_SET(u32);
}

void Blob__set_i8(Stack* stack)
{
    BLOB_SET(i8);
}

void Blob__set_i16(Stack* stack)
{
    BLOB_SET(i16);
}

void Blob__set_i32(Stack* stack)
{
    BLOB_SET(i32);
}

#define BLOB_GET(type) \
    Value* blob = circa_input(stack, 0); \
    int offset = circa_int_input(stack, 1); \
    \
    char* data; \
    u32 numBytes; \
    blob_data(blob, &data, &numBytes); \
    \
    if ((offset + sizeof(type)) > numBytes) \
        return circa_output_error(stack, "Offset is out of bounds"); \
    \
    type result = *((type*) data + offset); \
    set_int(circa_output(stack, 0), result);

void Blob__u8(Stack* stack)
{
    BLOB_GET(u8);
}

void Blob__u16(Stack* stack)
{
    BLOB_GET(u16);
}

void Blob__u32(Stack* stack)
{
    BLOB_GET(u32);
}

void Blob__i8(Stack* stack)
{
    BLOB_GET(i8);
}

void Blob__i16(Stack* stack)
{
    BLOB_GET(i16);
}

void Blob__i32(Stack* stack)
{
    BLOB_GET(i32);
}

void blob_install_functions(NativePatch* patch)
{
    circa_patch_function(patch, "make_blob", make_blob);
    circa_patch_function(patch, "Blob.size", Blob__size);
    circa_patch_function(patch, "Blob.slice", Blob__slice);
    circa_patch_function(patch, "Blob.set_u8", Blob__set_u8);
    circa_patch_function(patch, "Blob.set_u16", Blob__set_u16);
    circa_patch_function(patch, "Blob.set_u32", Blob__set_u32);
    circa_patch_function(patch, "Blob.set_i8", Blob__set_i8);
    circa_patch_function(patch, "Blob.set_i16", Blob__set_i16);
    circa_patch_function(patch, "Blob.set_i32", Blob__set_i32);
    circa_patch_function(patch, "Blob.i8", Blob__i8);
    circa_patch_function(patch, "Blob.i16", Blob__i16);
    circa_patch_function(patch, "Blob.i32", Blob__i32);
    circa_patch_function(patch, "Blob.u8", Blob__u8);
    circa_patch_function(patch, "Blob.u16", Blob__u16);
    circa_patch_function(patch, "Blob.u32", Blob__u32);
}

} // namespace circa
