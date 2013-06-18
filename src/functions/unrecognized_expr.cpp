// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#include "circa/internal/for_hosted_funcs.h"

namespace circa {
namespace unrecognized_expr_function {

    void evaluate(caStack* stack)
    {
        Value msg;
        set_string(&msg, "");
        string_append(&msg, circa_caller_term(stack)->stringProp("message","Unrecognized expression").c_str());
        circa_output_error(stack, as_cstring(&msg));
    }

    void formatSource(caValue* source, Term* term)
    {
        append_phrase(source, term->stringProp("originalText",""), term, sym_None);
    }

    void setup(Block* kernel)
    {
        FUNCS.unrecognized_expression = import_function(kernel, evaluate,
                "unrecognized_expr(any ins :multiple)");
        as_function(FUNCS.unrecognized_expression)->formatSource = formatSource;
    }
}
}
