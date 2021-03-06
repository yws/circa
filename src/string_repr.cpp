// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#include "common_headers.h"

#include "circa/circa.h"

#include "list.h"
#include "names.h"
#include "parser.h"
#include "string_type.h"
#include "tagged_value.h"
#include "token.h"
#include "type.h"

namespace circa {

struct ParseContext
{
    TokenStream* tokens;
    Value* out;
};

static void drop_whitespace(TokenStream* tokens)
{
    while (tokens->nextIs(tok_Whitespace))
        tokens->consume();
}

static void parse_value(TokenStream* tokens, Value* out)
{
    set_null(out);

    // ignore leading whitespace
    drop_whitespace(tokens);

    if (tokens->finished()) {
        set_string(out, "unexpected end of string");

    } else if (tokens->nextIs(tok_Integer)) {
        Value next;
        tokens->getNextStr(&next);
        set_int(out, atoi(as_cstring(&next)));
        tokens->consume(tok_Integer);
    } else if (tokens->nextIs(tok_Float)) {
        Value next;
        tokens->getNextStr(&next);
        set_float(out, (float) atof(as_cstring(&next)));
        tokens->consume(tok_Float);
    } else if (tokens->nextIs(tok_String)) {
        tokens->consumeStr(out, tok_String);
        string_unquote_and_unescape(out);
    } else if (tokens->nextIs(tok_LSquare)) {
        tokens->consume(tok_LSquare);
        drop_whitespace(tokens);

        set_list(out, 0);

        while (!tokens->nextIs(tok_RSquare) && !tokens->finished()) {
            Value* element = list_append(out);
            parse_value(tokens, element);

            if (tokens->nextIs(tok_Comma))
                tokens->consume(tok_Comma);
            drop_whitespace(tokens);
        }

        if (!tokens->finished())
            tokens->consume(tok_RSquare);
    } else if (tokens->nextIs(tok_True)) {
        set_bool(out, true);
        tokens->consume();
    } else if (tokens->nextIs(tok_False)) {
        set_bool(out, false);
        tokens->consume();
    } else if (tokens->nextIs(tok_Minus)) {
        tokens->consume(tok_Minus);

        parse_value(tokens, out);

        if (is_int(out)) {
            set_int(out, -1 * as_int(out));
        } else if (is_float(out)) {
            set_float(out, -1 * as_float(out));
        } else {
            set_string(out, "error, minus sign must preceed number");
        }

    } else {
        set_string(out, "unrecognized token: ");
        Value next;
        tokens->getNextStr(&next);
        string_append(out, &next);
        tokens->consume();
    }

    // ignore trailing whitespace
    drop_whitespace(tokens);
}

void parse_string_repr(const char* str, int len, Value* out)
{
    Value strVal;
    set_string(&strVal, str, len);
    TokenStream tokens(&strVal);
    parse_value(&tokens, out);
}

void parse_string_repr(Value* str, Value* out)
{
    ca_assert(str != out);

    TokenStream tokens(str);
    parse_value(&tokens, out);
}

void write_string_repr(Value* value, Value* out)
{
    ca_assert(value != out);
    set_null(out);

    // For certain types, just use to_string
    if (is_int(value) || is_float(value) || is_bool(value)) {
        to_string(value, out);
    } else if (is_list(value)) {
        set_string(out, "");
        string_append(out, "[");

        for (int i=0; i < list_length(value); i++) {
            if (i > 0)
                string_append(out, ", ");

            Value elementStr;
            write_string_repr(list_get(value, i), &elementStr);
            string_append(out, &elementStr);
        }

        string_append(out, "]");
    } else if (is_string(value)) {
        copy(value, out);
        string_quote_and_escape(out);
    } else {
        set_string(out, "error: no string repr for type ");
        string_append(out, as_cstring(&value->value_type->name));
    }
}

CIRCA_EXPORT void circa_to_string(Value* value, Value* out)
{
    write_string_repr(value, out);
}

CIRCA_EXPORT void circa_parse_string(const char* str, Value* valueOut)
{
    parse_string_repr(str, (int) strlen(str), valueOut);
}

CIRCA_EXPORT void circa_parse_string_len(const char* str, int len, Value* valueOut)
{
    parse_string_repr(str, len, valueOut);
}

} // namespace circa
