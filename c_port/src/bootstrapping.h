#ifndef CIRCA__BOOTSTRAPPING__INCLUDED
#define CIRCA__BOOTSTRAPPING__INCLUDED

class Term;
class Branch;

#include "type.h"
#include "function.h"

Term* quick_eval_function(Branch* branch, std::string input);
Term* quick_exec_function(Branch* branch, std::string input);

// Create a new Type with the given properties. Also binds the name.
Term* quick_create_type(Branch* code, string name, Type::AllocFunc allocFunc,
        Function::ExecuteFunc toStringFunc, Type::CopyFunc copyFunc = NULL);

// Create a new Function with the given properties. Also binds the name.
Term* quick_create_function(Branch* code, string name, Function::ExecuteFunc executeFunc,
        TermList inputTypes, Term* outputType);

#endif
