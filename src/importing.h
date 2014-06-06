// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

// importing.cpp : Functions to help import C functions into Circa.

#pragma once

#include "common_headers.h"

#include "function.h"

namespace circa {

// Create a Circa function, using the given C evaluation function, and
// a header in Circa-syntax.
//
// Example function header: "function do-something(int, string) -> int"
#if 0
Term* import_function(Block* block, EvaluateFunc func, const char* header);

// Install an evaluate function into an existing function object.
void install_function(Term* function, EvaluateFunc evaluate);
Term* install_function(Block* block, const char* name, EvaluateFunc evaluate);
#endif

Term* import_type(Block* block, Type* type);

} // namespace circa
