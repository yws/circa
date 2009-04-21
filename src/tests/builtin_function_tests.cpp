// Copyright 2008 Paul Hodge

#include "common_headers.h"

#include <circa.h>

namespace circa {
namespace builtin_function_tests {

void test_math()
{
    Branch branch;

    Term* two = float_value(&branch, 2);
    Term* three = float_value(&branch, 3);
    Term* negative_one = float_value(&branch, -1);

    test_assert(as_float(apply_and_eval(&branch, ADD_FUNC, RefList(two,three))) == 5);
    test_assert(as_float(apply_and_eval(&branch, ADD_FUNC, RefList(two,negative_one))) == 1);

    apply_and_eval(&branch, MULT_FUNC, RefList(two,three));
    test_assert(as_float(apply_and_eval(&branch, MULT_FUNC, RefList(two,three))) == 6);
    test_assert(as_float(apply_and_eval(&branch, MULT_FUNC, RefList(negative_one,three))) == -3);
}

void test_int()
{
    Branch branch;

    test_assert(as_type(INT_TYPE).equals != NULL);
    test_assert(as_type(INT_TYPE).toString != NULL);

    Term* four = int_value(&branch, 4);
    Term* another_four = int_value(&branch, 4);
    Term* five = int_value(&branch, 5);

    test_assert(equals(four, another_four));
    test_assert(!equals(four, five));

    test_assert(four->toString() == "4");
}

void test_float()
{
    Branch branch;

    test_assert(as_type(FLOAT_TYPE).equals != NULL);
    test_assert(as_type(FLOAT_TYPE).toString != NULL);

    Term* point_one = float_value(&branch, .1);
    Term* point_one_again = float_value(&branch, .1);
    Term* point_two = float_value(&branch, 0.2);

    test_assert(equals(point_one, point_one_again));
    test_assert(equals(point_two, point_two));

    test_equals(point_one->toString(), "0.1");
}

void test_string()
{
    Branch branch;

    test_equals(as_string(branch.eval("concat(\"hello \", \"world\")")), "hello world");
}

void test_concat()
{
    Branch branch;

    test_assert(eval_as<std::string>("concat('a ', 'b', ' c')") == "a b c");
}

void test_bool()
{
    Branch branch;

    test_assert(as_string(branch.eval("if_expr(true, 'a', 'b')")) == "a");
    test_assert(as_string(branch.eval("if_expr(false, 'a', 'b')")) == "b");
}

void test_builtin_equals()
{
    test_assert(eval_as<bool>("equals(1,1)"));
    test_assert(!eval_as<bool>("equals(1,2)"));
    test_assert(eval_as<bool>("equals('hello','hello')"));
    test_assert(!eval_as<bool>("equals('hello','goodbye')"));

    Branch branch;
    Term* term = branch.eval("equals(5.0, add)");
    test_assert(term->hasError());
}

void test_map()
{
    Branch branch;

    branch.eval("ages = map(string, int)");
    branch.eval("ages('Henry') := 11");
    branch.eval("ages('Absalom') := 205");

    test_assert(branch.eval("ages('Henry')")->asInt() == 11);
    test_assert(branch.eval("ages('Absalom')")->asInt() == 205);
}

void test_list()
{
    Branch branch;
    Term* l = branch.eval("l = list(1,2,\"foo\")");

    test_assert(is_branch(l));
    test_assert(as_branch(l)[0]->asInt() == 1);
    test_assert(as_branch(l)[1]->asInt() == 2);
    test_assert(as_branch(l)[2]->asString() == "foo");
}

void test_range()
{
    Branch branch;
    Term* t = branch.eval("range(5)");

    test_assert(as_branch(t).numTerms() == 5);
    for (int i=0; i < 5; i++)
        test_assert(as_int(as_branch(t)[i]) == i);

    // there was once a bug where the result list would grow on every call
    evaluate_term(t);
    evaluate_term(t);
    evaluate_term(t);
    evaluate_term(t);
    test_assert(as_branch(t).numTerms() == 5);
}

void register_tests()
{
    REGISTER_TEST_CASE(builtin_function_tests::test_int);
    REGISTER_TEST_CASE(builtin_function_tests::test_float);
    REGISTER_TEST_CASE(builtin_function_tests::test_math);
    REGISTER_TEST_CASE(builtin_function_tests::test_string);
    REGISTER_TEST_CASE(builtin_function_tests::test_concat);
    REGISTER_TEST_CASE(builtin_function_tests::test_bool);
    REGISTER_TEST_CASE(builtin_function_tests::test_builtin_equals);
    REGISTER_TEST_CASE(builtin_function_tests::test_map);
    REGISTER_TEST_CASE(builtin_function_tests::test_list);
    REGISTER_TEST_CASE(builtin_function_tests::test_range);
}

} // namespace builtin_function_tests

} // namespace circa
