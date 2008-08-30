
#include "common_headers.h"

#include "builtins.h"
#include "errors.h"
#include "function.h"
#include "operations.h"
#include "term.h"
#include "type.h"

namespace circa {

Type::Type()
  : name("undefined"),
    parentType(NULL),
    alloc(NULL),
    dealloc(NULL),
    duplicate(NULL),
    equals(NULL),
    remapPointers(NULL),
    toString(NULL)
{
}

bool is_instance(Term* term, Term* type)
{
    Term* actualType = term->type;

    while (actualType != NULL) {

        if (actualType == type)
            return true;

        actualType = as_type(actualType)->parentType;
    }

    return false;
}

void assert_instance(Term* term, Term* type)
{
    if (!is_instance(term, type))
        throw errors::TypeError(term, type);
}

bool is_type(Term* term)
{
    return term->type == TYPE_TYPE;
}

Type* as_type(Term* term)
{
    if (!is_type(term))
        throw errors::TypeError(term, TYPE_TYPE);

    return (Type*) term->value;
}

void Type_alloc(Term* caller)
{
    caller->value = new Type();
}

std::string Type_toString(Term* caller)
{
    return std::string("<Type " + as_type(caller)->name + ">");
}

void set_member_function(Term* type, std::string name, Term* function)
{
    Type* typeData = as_type(type);
    as_function(function);

    typeData->memberFunctions.bind(function, name);
}

Term* get_member_function(Term* type, std::string name)
{
    return as_type(type)->memberFunctions[name];
}

} // namespace circa
