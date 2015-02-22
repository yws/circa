// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

namespace circa {

#define blob_data circa_blob_data
#define blob_size circa_blob_size
#define set_blob_from_backing_value circa_set_blob_from_backing_value

char* set_blob_alloc_raw(Value* value, const char* data, u32 numBytes);

void blob_setup_type(Type* type);
void blob_install_functions(NativePatch* patch);

} // namespace circa