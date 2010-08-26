// Copyright (c) 2007-2010 Paul Hodge. All rights reserved.

#pragma once

#include "common_headers.h"
#include "references.h"
#include "tagged_value.h"

namespace circa {

struct EvalContext
{
    bool interruptSubroutine;

    TaggedValue subroutineOutput;

    // Error information:
    bool errorOccurred;
    Ref errorTerm;
    std::string errorMessage;

    // Top-level inlined state
    TaggedValue topLevelState;

    EvalContext() : interruptSubroutine(false), errorOccurred(false) {}

    void clearError() {
        errorOccurred = false;
        errorTerm = NULL;
        errorMessage = "";
    }

};

void evaluate_term(EvalContext* cxt, Term* caller, Term* function, RefList const& inputs, TaggedValue* output);
void evaluate_term(EvalContext* cxt, Term* term);
void evaluate_term(Term* term);
void evaluate_branch(EvalContext* context, Branch& branch);
void evaluate_bytecode(EvalContext* cxt, bytecode::BytecodeData* data, List* stack);
void evaluate_bytecode(Branch& branch);

// Shorthand to call evaluate_branch with a new EvalContext:
EvalContext evaluate_branch(Branch& branch);

// Perform 'apply' and then evaluate the result.
Term* apply_and_eval(Branch& branch, Term* function, RefList const& inputs);

// Find the named function in this branch, and then do the above 'apply_and_eval'.
Term* apply_and_eval(Branch& branch,
                    std::string const& functionName,
                    RefList const& inputs);

// Evaluates the given term, but doesn't evaluate any functions that have side effects.
// This function will also recursively evaluate inputs if needed, but it won't go outside
// of the current branch.
void evaluate_without_side_effects(Term* term);

void copy_stack_back_to_terms(Branch& branch, List* stack);
void capture_inputs(List* stack, bytecode::CallOperation* callOp, List* inputs);
TaggedValue* get_input(List* stack, bytecode::CallOperation* callOp, int index);
TaggedValue* get_output(List* stack, bytecode::CallOperation* callOp);
void evaluate_single_term(EvalContext* context, Term* caller, Term* function,
        List* inputs, TaggedValue* output);

} // namespace circa
