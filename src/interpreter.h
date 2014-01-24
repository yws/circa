// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#pragma once

#include "common_headers.h"
#include "stack.h"
#include "loops.h"
#include "object.h"
#include "tagged_value.h"
#include "term_list.h"

namespace circa {

// -- Stack --

// (Re)initialize the stack to have just one frame, using 'main' as its block. Existing data
// is erased.
void stack_init(Stack* stack, Block* main);

// Pop the topmost frame from the stack.
void stack_pop(Stack* stack);

// Reset a Stack to its default value.
void stack_reset(Stack* stack);

// Pop all but the topmost frame, set the PC to the first term, and delete all temporary
// values. If there is a state register, feed the output back into its input.
void stack_restart(Stack* stack);

caValue* stack_get_state(Stack* stack);

caValue* stack_find_nonlocal(Frame* frame, Term* term);

// Returns whether evaluation has been stopped due to an error.
bool stack_errored(Stack* stack);

void stack_to_string(Stack* stack, caValue* out, bool withBytecode);
void stack_trace_to_string(Stack* stack, caValue* out);

void stack_extract_state(Stack* stack, caValue* output);

// -- Frame --

Term* frame_caller(Frame* frame);
Term* frame_term(Frame* frame, int index);
caValue* frame_state(Frame* frame);
Block* frame_block(Frame* frame);

void frame_extract_state(Frame* frame, caValue* output);

void stack_run(Stack* stack);
void vm_run(Stack* stack);

// Deprecated
void evaluate_block(Stack* stack, Block* block);

// Copy all of the outputs from the topmost frame. This is an alternative to finish_frame
// - you call it when the block is finished evaluating. But instead of passing outputs
// to the parent frame (like finish_frame does), this copies them to your list.
void fetch_stack_outputs(Stack* stack, caValue* outputs);

// Functions used by eval functions.
caValue* get_input(Stack* stack, int index);
void consume_input(Stack* stack, Term* term, caValue* dest);
void consume_input(Stack* stack, int index, caValue* dest);
int num_inputs(Stack* stack);
void consume_inputs_to_list(Stack* stack, List* list);
caValue* get_output(Stack* stack, int index);
caValue* get_caller_output(Stack* stack, int index);

Term* current_term(Stack* stack);
Block* current_block(Stack* stack);

// Get a register on the topmost frame.
caValue* get_top_register(Stack* stack, Term* term);

caValue* stack_env_insert(Stack* stack, caValue* name);
caValue* stack_env_get(Stack* stack, caValue* name);

// Create an output value for the current term, using the declared type's
// initialize function.
void create_output(Stack* stack);

// Signal that a runtime error has occurred.
void raise_error(Stack* stack);
void raise_error_msg(Stack* stack, const char* msg);
void raise_error_too_many_inputs(Stack* stack);
void raise_error_not_enough_inputs(Stack* stack);
void raise_error_input_type_mismatch(Stack* stack);

void stack_extract_current_path(Stack* stack, caValue* path);

// Kernel setup.
void interpreter_install_functions(NativePatch* patch);

bool is_stack(caValue* value);
Stack* as_stack(caValue* value);
void set_stack(caValue* value, Stack* stack);

} // namespace circa
