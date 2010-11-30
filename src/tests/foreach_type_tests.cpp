// Copyright (c) 2007-2010 Paul Hodge. All rights reserved.

#include <circa.h>

namespace circa {
namespace foreach_type_tests {

bool run_test_for_type(Term* type, List& exampleValues)
{
    Branch branch;

    TaggedValue x;
    copy(exampleValues[0], &x);

    TaggedValue cpy;
    copy(&x, &cpy);
    test_assert(equals(&x, &cpy));

    TaggedValue y;
    copy(exampleValues[1], &y);
    test_assert(!equals(&x,&y));

    reset(&x);
    reset(&y);
    test_assert(equals(&x,&y));

    #if 0

    Term* x = create_value(branch, type, "x");
    test_assert(branch.eval("x == x"));

    TaggedValue* cpy = branch.eval("cpy = copy(x)");
    test_assert(branch);

    Term* y = create_value(branch, type, "y");
    test_assert(branch.eval("x != y"));

    TaggedValue* cnd1 = branch.eval("cond(true, x, y)");
    TaggedValue* cnd2 = branch.eval("cond(false, x, y)");
    test_assert(branch);
    test_assert(equals(cnd1, x));
    test_assert(equals(cnd2, y));

    branch.eval("boxed = [x y]");
    TaggedValue* unbox1 = branch.eval("boxed[0]");
    TaggedValue* unbox2 = branch.eval("boxed[1]");
    test_assert(branch);
    test_assert(equals(unbox1, x));
    test_assert(equals(unbox2, y));
    branch.clear();

    // Cast an integer to this type. This might cause an error but it shouldn't
    // crash.
    Term* cast = branch.compile("cast(1)");
    change_type(cast, type);
    evaluate_branch(branch);
    branch.clear();

    #endif
    return true;
}

void check_int()
{
    List intExamples;
    set_int(intExamples.append(), 5);
    set_int(intExamples.append(), 3);
    run_test_for_type(INT_TYPE, intExamples);
}

void check_float()
{
    List floatExamples;
    set_float(floatExamples.append(), 1.2);
    set_float(floatExamples.append(), -0.001);
    run_test_for_type(FLOAT_TYPE, floatExamples);
}

void check_string()
{
    List stringExamples;
    set_string(stringExamples.append(), "hello");
    set_string(stringExamples.append(), "goodbye");
    run_test_for_type(STRING_TYPE, stringExamples);
}

void check_ref()
{
    List refExamples;
    Term* refTarget1 = alloc_term();
    Term* refTarget2 = alloc_term();
    set_ref(refExamples.append(), refTarget1);
    set_ref(refExamples.append(), refTarget2);
    test_assert(run_test_for_type(REF_TYPE, refExamples));
}

void register_tests()
{
    REGISTER_TEST_CASE(foreach_type_tests::check_int);
    REGISTER_TEST_CASE(foreach_type_tests::check_float);
    REGISTER_TEST_CASE(foreach_type_tests::check_string);
    REGISTER_TEST_CASE(foreach_type_tests::check_ref);
}

}
} // namespace circa
