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
    typedef void (*InitializeFunc)(Term* caller);
    typedef void (*EvaluateFunc)(Term* caller);

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
    InitializeFunc initialize;
    EvaluateFunc evaluate;

    // External functions
    Term* feedbackAccumulationFunction;
    Term* feedbackPropogateFunction;
    Term* generateCppFunction;
    Term* printCircaSourceFunction;

    Function();

    InputProperties& getInputProperties(unsigned int index);
    void setInputMeta(int index, bool value);
    void setInputModified(int index, bool value);

    // Hosted functions
    static void duplicate(Term* source, Term* dest);
    static void remapPointers(Term* term, ReferenceMap const& map);
    static void visitPointers(Term* term, PointerVisitor& visitor);
    static void subroutine_create(Term* caller);
    static void call_subroutine__initialize(Term* caller);
    static void call_subroutine(Term* caller);
    static void name_input(Term* caller);
    static void get_input_name(Term* caller);
    static void subroutine_apply(Term* caller);
};

bool is_function(Term* term);
Function& as_function(Term*);

void initialize_functions(Branch* kernel);

} // namespace circa

#endif
