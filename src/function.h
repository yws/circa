// Copyright 2008 Paul Hodge

#ifndef CIRCA_FUNCTION_INCLUDED
#define CIRCA_FUNCTION_INCLUDED

#include "common_headers.h"

#include "term.h"
#include "branch.h"

namespace circa {

#define INPUT_PLACEHOLDER_PREFIX "#input-"
#define OUTPUT_PLACEHOLDER_NAME "#output"

struct Function
{
    typedef void (*EvaluateFunc)(Term* caller);
    typedef PointerIterator* (*PointerIteratorFunc)(Term* caller);

    struct InputProperties {
        bool modified;
        bool meta;

        InputProperties() : modified(false), meta(false) {}
    };
    typedef std::vector<InputProperties> InputPropertiesList;

    ReferenceList inputTypes;
    InputPropertiesList inputProperties;
    Term* outputType;
    Term* stateType;

    bool pureFunction;
    bool hasSideEffects;
    bool variableArgs;

    std::string name;

    Branch subroutineBranch;

    // Code
    EvaluateFunc evaluate;
    PointerIteratorFunc startControlFlowIterator;

    // External functions
    Term* feedbackAccumulationFunction;
    Term* feedbackPropogateFunction;
    Term* generateCppFunction;
    Term* printCircaSourceFunction;

    Function();

    Term* inputType(int index);
    InputProperties& getInputProperties(unsigned int index);
    void setInputMeta(int index, bool value);
    void setInputModified(int index, bool value);

    // Hosted functions
    static void duplicate(Term* source, Term* dest);
    static void remapPointers(Term* term, ReferenceMap const& map);
    static void visitPointers(Term* term, PointerVisitor& visitor);
    static void subroutine_call_evaluate(Term* caller);
    static PointerIterator* subroutine_call_start_control_flow_iterator(Term* caller);
};

bool is_function(Term* term);
Function& as_function(Term*);

std::string get_placeholder_name_for_index(int index);

void set_subroutine_input_name(Function& func, int index, std::string const& name);

Branch& call_subroutine(Branch& branch, std::string const& functionName);
Branch& get_subroutine_branch(Term* term);
PointerIterator* start_function_pointer_iterator(Function* function);

// Call the start_control_flow_iterator function for this term's function,
// and return the new iterator. Returns NULL if there is no such function.
PointerIterator* start_control_flow_iterator(Term* term);

} // namespace circa

#endif
