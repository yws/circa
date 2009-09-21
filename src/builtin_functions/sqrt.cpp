// Copyright (c) 2007-2009 Paul Hodge. All rights reserved.

#include "circa.h"

namespace circa {
namespace sqrt_function {

    void evaluate(Term* caller)
    {
        as_float(caller) = std::sqrt(caller->input(0)->toFloat());
    }

    void setup(Branch& kernel)
    {
        import_function(kernel, evaluate, "sqrt(number) : number;"
                "'Square root' end");
    }
}
} // namespace circa
