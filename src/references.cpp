// Copyright 2008 Paul Hodge

#include "circa.h"

namespace circa {

void remove_referencer(Term* term, Ref* ref)
{
    assert_good_pointer(term);

    std::vector<Ref*>::iterator it;
    for (it = term->refs.begin(); it != term->refs.end();) {
        if (*it == ref) {
            it = term->refs.erase(it);
        } else {
            ++it;
        }
    }

    if (term->refs.size() == 0)
        delete_term(term);
}

void delete_term(Term* term)
{
    assert_good_pointer(term);

    if (term->state != NULL)
        delete_term(term->state);
    term->state = NULL;

    dealloc_value(term);

    // Clear references
    std::vector<Ref*>::iterator it;
    for (it = term->refs.begin(); it != term->refs.end(); ++it) {
        (*it)->_target = NULL;
    }

    term->refs.clear();
    term->inputs.clear();
    term->type = NULL;
    term->function = NULL;

#if DEBUG_CHECK_FOR_BAD_POINTERS
    DEBUG_GOOD_POINTER_SET.erase(term);
#endif

#if !DEBUG_NEVER_DELETE_TERMS
    delete term;
#endif
}

} // namespace circa
