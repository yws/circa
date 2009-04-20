// Copyright 2008 Andrew Fischer

#include "common_headers.h"

#include "circa.h"
#include "testing.h"

using namespace circa;

int circa_main(std::vector<std::string> args)
{
    initialize();

    // load args into kernel
    /*List *cl_args = new List();
    
    for (unsigned int i=0; i < args.size(); i++) {
        cl_args->append(args[i]);
    }

    import_value(*KERNEL, LIST_TYPE, cl_args, "cl-args");*/

    args.erase(args.begin());

    bool justPrintBranch = false;
    bool justPrintSource = false;

    if (args.size() == 0) {
        run_all_tests();
        shutdown();
        return 0;
    }

    if (args[0] == "-e") {
        args.erase(args.begin());
        std::stringstream command;
        bool firstArg = true;
        while (!args.empty()) {
            if (!firstArg) command << " ";
            command << args[0];
            args.erase(args.begin());
            firstArg = false;
        }

        Branch workspace;
        Term* result = workspace.eval(command.str());
        std::cout << result->toString() << std::endl;
        shutdown();
        return 0;
    }

    if (args[0] == "--list-tests") {
        std::vector<std::string> testNames = list_all_test_names();

        std::vector<std::string>::const_iterator it;
        for (it = testNames.begin(); it != testNames.end(); ++it) {
            std::cout << *it << std::endl;
        }
        shutdown();
        return 0;
    }

    if (args[0] == "-p") {
        justPrintBranch = true;
        args.erase(args.begin());
    }

    if (args[0] == "-s") {
        justPrintSource = true;
        args.erase(args.begin());

        if (justPrintBranch) {
            // todo: fatal
        }
    }

    Branch workspace;
    string_value(&workspace, args[0], "filename");

    Term* evaluated_file = workspace.eval("evaluate_file(filename)");
    if (evaluated_file->hasError()) {
        std::cout << "Parsing error: " << evaluated_file->getErrorMessage()
            << std::endl;
    }
    Branch& branch = as_branch(evaluated_file);

    if (justPrintBranch) {
        std::cout << branch_to_string_raw(branch);
    }

    else if (justPrintSource) {
        std::cout << get_branch_source(branch) << std::endl;
    }

    else if (has_compile_errors(branch)) {
        print_compile_errors(branch, std::cout);
    } else {

        Term* error_listener = new Term();

        evaluate_branch(branch, error_listener);

        if (error_listener->hasError()) {
            std::cout << "Error occured: " << error_listener->getErrorMessage() << std::endl;
        }
    }

    shutdown();
    return 0;
}

int main(int nargs, const char * args[])
{
    std::vector<std::string> args_vector;

    for (int i = 0; i < nargs; i++)
        args_vector.push_back(args[i]);

    return circa_main(args_vector);
}
