// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#pragma once

#include "common_headers.h"

namespace circa {

Term* loop_find_iterator(Block* contents);
Term* loop_find_done_call(Block* block);
Term* loop_find_key(Block* block);
Term* loop_find_iterator_advance(Block* block);
const char* for_loop_get_iterator_name(Term* forTerm);

// Initialize the contents of a for-loop for a new term.
//   'iteratorType' is the type to use  for the iterator. If it's NULL then we'll infer a
//   type from the term's input.
void start_building_for_loop(Block* block, Term* listExpr, Value* indexName, Value* iteratorName, Type* iteratorType);
void finish_for_loop(Term* forTerm);
void finish_while_loop(Block* block);

Term* find_enclosing_for_loop(Term* term);
Block* find_enclosing_for_loop_contents(Term* term);
bool loop_produces_output_value(Term* forTerm);
bool enclosing_loop_produces_output_value(Term* term);

void list_names_that_must_be_looped(Block* contents, Value* names);

void loop_add_condition_check(Block* caseBlock, Term* condition);
Term* loop_find_condition_check(Block* block);
Term* loop_find_condition(Block* block);

} // namespace circa
