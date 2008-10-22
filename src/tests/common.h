// Copyright 2008 Andrew Fischer

#ifndef CIRCA_TEST_COMMON_INCLUDED
#define CIRCA_TEST_COMMON_INCLUDED

#include "errors.h"
#include "ref_list.h"

namespace circa {

class TestFailure : public errors::CircaError
{
    virtual std::string message()
    {
        return "Test failure";
    }
};

void _test_assert_function(bool condition, int line, const char* file);
void _test_fail_function(int line, const char* file);
void _test_equals_function(ReferenceList const& a, ReferenceList const& b,
        const char* aText, const char* bText, int line, const char* file);

#define test_assert(c) _test_assert_function((c), __LINE__, __FILE__)
#define test_fail() _test_fail_function( __LINE__, __FILE__)
#define test_equals(a,b) _test_equals_function(a,b,#a,#b,__LINE__,__FILE__)

struct TestCase {
    typedef void (*TestExecuteFunction)();

    std::string name;
    TestExecuteFunction execute;

    TestCase(std::string const& _name, TestExecuteFunction _execute)
      : name(_name), execute(_execute)
    {
    }
};

extern std::vector<TestCase> gTestCases;

#define REGISTER_TEST_CASE(f) gTestCases.push_back(TestCase(#f,f))

} // namespace circa

#endif
