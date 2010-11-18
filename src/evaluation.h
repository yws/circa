// Copyright (c) 2007-2010 Paul Hodge. All rights reserved.

#pragma once

#include "common_headers.h"
#include "for_loop.h"
#include "references.h"
#include "tagged_value.h"
#include "types/list.h"
#include "types/dict.h"

namespace circa {

struct EvalContext
{
    bool interruptSubroutine;

    TaggedValue subroutineOutput;

    // Error information:
    bool errorOccurred;
    Ref errorTerm;
    std::string errorMessage;

    // Tree of persistent state
    TaggedValue state;

    // Stack of temporary values
    List stack;

    // Names of state variables which need to be preserved at the end of this branch
    List openStateVariables;

    // Persistent state that should be used for the current branch
    TaggedValue currentScopeState;

    // State used for the current for loop
    ForLoopContext forLoopContext;

    EvalContext() : interruptSubroutine(false), errorOccurred(false), currentScopeState(NULL) {}

    void clearError() {
        errorOccurred = false;
        errorTerm = NULL;
        errorMessage = "";
    }
};

// Evaluate a single term against the given stack. After this function, the caller
// must reestablish any internal pointers to the contents of 'stack'.
void evaluate_single_term(EvalContext* context, Term* term);

void evaluate_branch_existing_frame(EvalContext* context, Branch& branch);

void wrap_up_open_state_vars(EvalContext* context, Branch& branch);

// Evaluate a branch with an existing EvalContext, stack, and branch. 'output' can be
// null, if it's not null then we'll copy the output register of this branch to it.
// When specifying 'output', don't use a value that is in the stack, because that
// seems to break things. Instead use a temporary.
void evaluate_branch_in_new_frame(EvalContext* context, Branch& branch, TaggedValue* output);

// Top-level call. Evalaute the branch and then preserve stack outputs back to terms.
void evaluate_branch(EvalContext* context, Branch& branch);
#ifdef BYTECODE
void evaluate_branch(Branch& branch);
#endif

// Shorthand to call evaluate_branch with a new EvalContext:
EvalContext evaluate_branch(Branch& branch);

// Perform 'apply' and then evaluate the result.
Term* apply_and_eval(Branch& branch, Term* function, RefList const& inputs);

// Find the named function in this branch, and then do the above 'apply_and_eval'.
Term* apply_and_eval(Branch& branch,
                    std::string const& functionName,
                    RefList const& inputs);

void copy_stack_back_to_terms(Branch& branch, List* stack);
void capture_inputs(List* stack, bytecode::CallOperation* callOp, List* inputs);
TaggedValue* get_input_relative(EvalContext* cxt, Term* term, int index, int relativeStack);
TaggedValue* get_input(EvalContext* cxt, Term* term, int index);
TaggedValue* get_output(EvalContext* cxt, Term* term);
TaggedValue* get_state_input(EvalContext* cxt, Term* term);
Dict* get_current_scope_state(EvalContext* cxt);
void fetch_state_container(Term* term, TaggedValue* container, TaggedValue* output);
void preserve_state_result(Term* term, TaggedValue* container, TaggedValue* result);

List* push_stack_frame(List* stack, int size);
void pop_stack_frame(List* stack);
List* get_stack_frame(List* stack, int relativeScope);

// Before evaluating the term, we'll check each input to see if it's not-null on
// the stack (or if the stack frame is missing). Any missing values will get
// copied from their respective terms.
void evaluate_with_lazy_stack(EvalContext* context, Term* term);

void evaluate_range_with_lazy_stack(EvalContext* context, Branch& branch, int start, int end);

} // namespace circa
