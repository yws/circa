// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#pragma once

namespace circa {

int blob_size(caValue* blob);

void blob_resize(caValue* blob, int size);
void blob_append_char(caValue* blob, char c);
void blob_append_int(caValue* blob, unsigned int val);

char blob_read_char(char* data, int* pos);
unsigned int blob_read_int(char* data, int* pos);

void blob_to_hex_string(caValue* blob, caValue* str);

bool is_blob(caValue* val);
char* as_blob(caValue* val);
void set_blob(caValue* val, int length);
void blob_setup_type(Type* type);

} // namespace circa