
// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

// This file is generated during the build process by ca-prebuild.py .
// You should probably not edit this file manually.

#include "../common_headers.h"

#include "../branch.h"

namespace circa {

namespace abs_function { void setup(Branch* kernel); }
namespace add_function { void setup(Branch* kernel); }
namespace any_true_function { void setup(Branch* kernel); }
namespace branch_function { void setup(Branch* kernel); }
namespace cast_function { void setup(Branch* kernel); }
namespace comment_function { void setup(Branch* kernel); }
namespace comparison_function { void setup(Branch* kernel); }
namespace concat_function { void setup(Branch* kernel); }
namespace cond_function { void setup(Branch* kernel); }
namespace container_set_function { void setup(Branch* kernel); }
namespace copy_function { void setup(Branch* kernel); }
namespace cppbuild_function { void setup(Branch* kernel); }
namespace declared_state_function { void setup(Branch* kernel); }
namespace div_function { void setup(Branch* kernel); }
namespace equals_function { void setup(Branch* kernel); }
namespace error_function { void setup(Branch* kernel); }
namespace extra_output_function { void setup(Branch* kernel); }
namespace feedback_function { void setup(Branch* kernel); }
namespace filter_function { void setup(Branch* kernel); }
namespace for_function { void setup(Branch* kernel); }
namespace get_field_function { void setup(Branch* kernel); }
namespace get_index_function { void setup(Branch* kernel); }
namespace if_block_function { void setup(Branch* kernel); }
namespace implicit_state_function { void setup(Branch* kernel); }
namespace include_function { void setup(Branch* kernel); }
namespace increment_function { void setup(Branch* kernel); }
namespace input_explicit_function { void setup(Branch* kernel); }
namespace internal_debug_function { void setup(Branch* kernel); }
namespace list_function { void setup(Branch* kernel); }
namespace logical_function { void setup(Branch* kernel); }
namespace lookup_branch_ref_function { void setup(Branch* kernel); }
namespace make_function { void setup(Branch* kernel); }
namespace math_function { void setup(Branch* kernel); }
namespace meta_function { void setup(Branch* kernel); }
namespace mult_function { void setup(Branch* kernel); }
namespace namespace_function { void setup(Branch* kernel); }
namespace neg_function { void setup(Branch* kernel); }
namespace print_function { void setup(Branch* kernel); }
namespace rand_function { void setup(Branch* kernel); }
namespace range_function { void setup(Branch* kernel); }
namespace rpath_function { void setup(Branch* kernel); }
namespace set_field_function { void setup(Branch* kernel); }
namespace set_index_function { void setup(Branch* kernel); }
namespace set_methods_function { void setup(Branch* kernel); }
namespace set_union_function { void setup(Branch* kernel); }
namespace static_error_function { void setup(Branch* kernel); }
namespace sub_function { void setup(Branch* kernel); }
namespace swap_function { void setup(Branch* kernel); }
namespace switch_function { void setup(Branch* kernel); }
namespace term_to_source_function { void setup(Branch* kernel); }
namespace to_string_function { void setup(Branch* kernel); }
namespace trig_function { void setup(Branch* kernel); }
namespace type_check_function { void setup(Branch* kernel); }
namespace unique_id_function { void setup(Branch* kernel); }
namespace unknown_function_function { void setup(Branch* kernel); }
namespace unknown_identifier_function { void setup(Branch* kernel); }
namespace unrecognized_expr_function { void setup(Branch* kernel); }
namespace write_text_file_function { void setup(Branch* kernel); }

void setup_builtin_functions(Branch* kernel)
{
    abs_function::setup(kernel);
    add_function::setup(kernel);
    any_true_function::setup(kernel);
    branch_function::setup(kernel);
    cast_function::setup(kernel);
    comment_function::setup(kernel);
    comparison_function::setup(kernel);
    concat_function::setup(kernel);
    cond_function::setup(kernel);
    container_set_function::setup(kernel);
    copy_function::setup(kernel);
    cppbuild_function::setup(kernel);
    declared_state_function::setup(kernel);
    div_function::setup(kernel);
    equals_function::setup(kernel);
    error_function::setup(kernel);
    extra_output_function::setup(kernel);
    feedback_function::setup(kernel);
    filter_function::setup(kernel);
    for_function::setup(kernel);
    get_field_function::setup(kernel);
    get_index_function::setup(kernel);
    if_block_function::setup(kernel);
    implicit_state_function::setup(kernel);
    include_function::setup(kernel);
    increment_function::setup(kernel);
    input_explicit_function::setup(kernel);
    internal_debug_function::setup(kernel);
    list_function::setup(kernel);
    logical_function::setup(kernel);
    lookup_branch_ref_function::setup(kernel);
    make_function::setup(kernel);
    math_function::setup(kernel);
    meta_function::setup(kernel);
    mult_function::setup(kernel);
    namespace_function::setup(kernel);
    neg_function::setup(kernel);
    print_function::setup(kernel);
    rand_function::setup(kernel);
    range_function::setup(kernel);
    rpath_function::setup(kernel);
    set_field_function::setup(kernel);
    set_index_function::setup(kernel);
    set_methods_function::setup(kernel);
    set_union_function::setup(kernel);
    static_error_function::setup(kernel);
    sub_function::setup(kernel);
    swap_function::setup(kernel);
    switch_function::setup(kernel);
    term_to_source_function::setup(kernel);
    to_string_function::setup(kernel);
    trig_function::setup(kernel);
    type_check_function::setup(kernel);
    unique_id_function::setup(kernel);
    unknown_function_function::setup(kernel);
    unknown_identifier_function::setup(kernel);
    unrecognized_expr_function::setup(kernel);
    write_text_file_function::setup(kernel);
}

} // namespace circa