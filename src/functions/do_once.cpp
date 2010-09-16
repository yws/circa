// Copyright (c) 2007-2010 Paul Hodge. All rights reserved.

#include "circa.h"

namespace circa {
namespace do_once_function {

    CA_FUNCTION(empty_evaluate)
    {
    }

    void write_bytecode(bytecode::WriteContext* context, Term* term)
    {
        ca_assert(term->function->name == "do_once");
        bytecode::JumpIfOperation* jumpToEnd = (bytecode::JumpIfOperation*) context->writePos;
        bytecode::write_jump_if(context, term->input(0)->stackIndex, 0);

        // fixme: support state inside do_once

        bytecode::write_bytecode_for_branch(context, term->nestedContents, -1);

        if (jumpToEnd)
            jumpToEnd->offset = context->getOffset();
    }

    void formatSource(StyledSource* source, Term* term)
    {
        append_phrase(source, "do once", term, phrase_type::KEYWORD);
        append_phrase(source, term->stringPropOptional("syntax:postHeadingWs", "\n"),
                term, token::WHITESPACE);
        format_branch_source(source, term->nestedContents, NULL);
        append_phrase(source, term->stringPropOptional("syntax:preEndWs", ""),
                term, token::WHITESPACE);
                
        append_phrase(source, "end", term, phrase_type::KEYWORD);
    }

    void setup(Branch& kernel)
    {
        DO_ONCE_FUNC = import_function(kernel, empty_evaluate, "do_once(state bool)");
        function_t::get_attrs(DO_ONCE_FUNC).writeBytecode = write_bytecode;
        function_t::get_attrs(DO_ONCE_FUNC).formatSource = formatSource;
    }
}
}