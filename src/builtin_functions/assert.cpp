// Copyright (c) 2007-2009 Paul Hodge. All rights reserved.

#include <circa.h>

namespace circa {
namespace assert_function {

    void evaluate(Term* caller)
    {
        if (!as_bool(caller->input(0)))
            error_occurred(caller, "Assert failed");
    }

    void setup(Branch& kernel)
    {
        import_function(kernel, evaluate, "assert(bool condition);"
            "'Raises a runtime error if condition is false' end");
    }
}
}
