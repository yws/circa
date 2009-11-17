
class TestCase(object):
    def __init__(self, name, func):
        self.name = name
        self.func = func

class TestFailure(object):
    def __init__(self, name, message):
        self.name = name
        self.message = message

class TestSuite(object):
    def __init__(self):
        self.tests = []

    def append(self, name, func):
        self.tests.append(TestCase(name,func))

    def run_and_print_results(self):
        test_count = 0
        failures = []

        for test in self.tests:
            test_count += 1
            result = test.func()

            if result is not True:
                failures.append(TestFailure(test.name, result))

        if not failures:
            print "Ran %i tests. All tests passed." % test_count
        else:
            print "Ran %i tests. %i failures.." % (test_count, len(failures))

            for failure in failures:
                comment = ""
                if failure.message: comment = ": "+failure.message
                print "%s failed%s" % (failure.name, comment)
