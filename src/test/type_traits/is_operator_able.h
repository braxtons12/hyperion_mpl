/// @file is_operator_able.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Tests for is_operator_able.h
/// support an operator
/// @version 0.1.1
/// @date 2025-07-08
///
/// MIT License
/// @copyright Copyright (c) 2025 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

#ifndef HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_TEST_H
#define HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_TEST_H

#include <hyperion/mpl/type_traits/is_operator_able.h>

namespace hyperion::mpl::type_traits::_test {

    struct nothing_able {
        // NOLINTNEXTLINE(google-runtime-operator)
        auto operator&() = delete;
    };

    static_assert(is_unary_plusable_v<int>,
                  "hyperion::mpl::type_traits::is_unary_plusable test case 1 (failing)");
    static_assert(is_unary_plusable_v<double>,
                  "hyperion::mpl::type_traits::is_unary_plusable test case 2 (failing)");
    static_assert(!is_unary_plusable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_unary_plusable test case 3 (failing)");
    static_assert(std::same_as<unary_plus_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_unary_plusable test case 4 (failing)");
    static_assert(std::same_as<unary_plus_result_t<double>, double>,
                  "hyperion::mpl::type_traits::is_unary_plusable test case 5 (failing)");
    static_assert(std::same_as<unary_plus_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_unary_plusable test case 6 (failing)");

    static_assert(is_unary_minusable_v<int>,
                  "hyperion::mpl::type_traits::is_unary_minusable test case 1 (failing)");
    static_assert(is_unary_minusable_v<double>,
                  "hyperion::mpl::type_traits::is_unary_minusable test case 2 (failing)");
    static_assert(!is_unary_minusable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_unary_minusable test case 3 (failing)");
    static_assert(std::same_as<unary_minus_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_unary_minusable test case 4 (failing)");
    static_assert(std::same_as<unary_minus_result_t<double>, double>,
                  "hyperion::mpl::type_traits::is_unary_minusable test case 5 (failing)");
    static_assert(std::same_as<unary_minus_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_unary_minusable test case 6 (failing)");

    static_assert(is_binary_notable_v<int>,
                  "hyperion::mpl::type_traits::is_binary_notable test case 1 (failing)");
    static_assert(!is_binary_notable_v<double>,
                  "hyperion::mpl::type_traits::is_binary_notable test case 2 (failing)");
    static_assert(!is_binary_notable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_binary_notable test case 3 (failing)");
    static_assert(std::same_as<binary_not_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_binary_notable test case 4 (failing)");
    static_assert(std::same_as<binary_not_result_t<double>, void>,
                  "hyperion::mpl::type_traits::is_binary_notable test case 5 (failing)");
    static_assert(std::same_as<binary_not_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_binary_notable test case 6 (failing)");

    static_assert(is_boolean_notable_v<int>,
                  "hyperion::mpl::type_traits::is_boolean_notable test case 1 (failing)");
    static_assert(is_boolean_notable_v<double>,
                  "hyperion::mpl::type_traits::is_boolean_notable test case 2 (failing)");
    static_assert(!is_boolean_notable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_boolean_notable test case 3 (failing)");
    static_assert(std::same_as<boolean_not_result_t<int>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_notable test case 4 (failing)");
    static_assert(std::same_as<boolean_not_result_t<double>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_notable test case 5 (failing)");
    static_assert(std::same_as<boolean_not_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_boolean_notable test case 6 (failing)");

    static_assert(is_addressable_v<int>,
                  "hyperion::mpl::type_traits::is_addressable test case 1 (failing)");
    static_assert(is_addressable_v<double>,
                  "hyperion::mpl::type_traits::is_addressable test case 2 (failing)");
    static_assert(!is_addressable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_addressable test case 3 (failing)");
    static_assert(std::same_as<address_result_t<int>, int*>,
                  "hyperion::mpl::type_traits::is_addressable test case 4 (failing)");
    static_assert(std::same_as<address_result_t<double>, double*>,
                  "hyperion::mpl::type_traits::is_addressable test case 5 (failing)");
    static_assert(std::same_as<address_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_addressable test case 6 (failing)");

    struct arrow_able {
        auto operator->() -> int*;
        auto operator->() const -> const int*;
    };

    static_assert(is_arrowable_v<arrow_able>,
                  "hyperion::mpl::type_traits::is_arrowable test case 1 (failing)");
    static_assert(!is_arrowable_v<int>,
                  "hyperion::mpl::type_traits::is_arrowable test case 2 (failing)");
    static_assert(!is_arrowable_v<double>,
                  "hyperion::mpl::type_traits::is_arrowable test case 3 (failing)");
    static_assert(!is_arrowable_v<int*>,
                  "hyperion::mpl::type_traits::is_arrowable test case 4 (failing)");
    static_assert(!is_arrowable_v<double*>,
                  "hyperion::mpl::type_traits::is_arrowable test case 5 (failing)");
    static_assert(!is_arrowable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_arrowable test case 6 (failing)");
    static_assert(std::same_as<arrow_result_t<arrow_able>, int*>,
                  "hyperion::mpl::type_traits::is_arrowable test case 7 (failing)");
    static_assert(std::same_as<arrow_result_t<int>, void>,
                  "hyperion::mpl::type_traits::is_arrowable test case 8 (failing)");
    static_assert(std::same_as<arrow_result_t<double>, void>,
                  "hyperion::mpl::type_traits::is_arrowable test case 9 (failing)");
    static_assert(std::same_as<arrow_result_t<int*>, void>,
                  "hyperion::mpl::type_traits::is_arrowable test case 10 (failing)");
    static_assert(std::same_as<arrow_result_t<double*>, void>,
                  "hyperion::mpl::type_traits::is_arrowable test case 11 (failing)");
    static_assert(std::same_as<arrow_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_arrowable test case 12 (failing)");

    static_assert(!is_dereferencible_v<int>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 2 (failing)");
    static_assert(!is_dereferencible_v<double>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 3 (failing)");
    static_assert(is_dereferencible_v<int*>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 4 (failing)");
    static_assert(is_dereferencible_v<double*>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 5 (failing)");
    static_assert(!is_dereferencible_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 6 (failing)");
    static_assert(std::same_as<dereference_result_t<int>, void>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 8 (failing)");
    static_assert(std::same_as<dereference_result_t<double>, void>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 9 (failing)");
    static_assert(std::same_as<dereference_result_t<int*>, int&>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 10 (failing)");
    static_assert(std::same_as<dereference_result_t<double*>, double&>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 11 (failing)");
    static_assert(std::same_as<dereference_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_dereferencible test case 12 (failing)");

    static_assert(is_addable_v<int>,
                  "hyperion::mpl::type_traits::is_addable test case 1 (failing)");
    static_assert(is_addable_v<double>,
                  "hyperion::mpl::type_traits::is_addable test case 2 (failing)");
    static_assert(is_addable_v<int, double>,
                  "hyperion::mpl::type_traits::is_addable test case 3 (failing)");
    static_assert(is_addable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_addable test case 4 (failing)");
    static_assert(!is_addable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_addable test case 5 (failing)");
    static_assert(std::same_as<add_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_addable test case 6 (failing)");
    static_assert(std::same_as<add_result_t<double>, double>,
                  "hyperion::mpl::type_traits::is_addable test case 7 (failing)");
    static_assert(std::same_as<add_result_t<int, double>, double>,
                  "hyperion::mpl::type_traits::is_addable test case 8 (failing)");
    static_assert(std::same_as<add_result_t<double, bool>, double>,
                  "hyperion::mpl::type_traits::is_addable test case 9 (failing)");
    static_assert(std::same_as<add_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_addable test case 10 (failing)");

    static_assert(is_subtractable_v<int>,
                  "hyperion::mpl::type_traits::is_subtractable test case 1 (failing)");
    static_assert(is_subtractable_v<double>,
                  "hyperion::mpl::type_traits::is_subtractable test case 2 (failing)");
    static_assert(is_subtractable_v<int, double>,
                  "hyperion::mpl::type_traits::is_subtractable test case 3 (failing)");
    static_assert(is_subtractable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_subtractable test case 4 (failing)");
    static_assert(!is_subtractable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_subtractable test case 5 (failing)");
    static_assert(std::same_as<subtract_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_subtractable test case 6 (failing)");
    static_assert(std::same_as<subtract_result_t<double>, double>,
                  "hyperion::mpl::type_traits::is_subtractable test case 7 (failing)");
    static_assert(std::same_as<subtract_result_t<int, double>, double>,
                  "hyperion::mpl::type_traits::is_subtractable test case 8 (failing)");
    static_assert(std::same_as<subtract_result_t<double, bool>, double>,
                  "hyperion::mpl::type_traits::is_subtractable test case 9 (failing)");
    static_assert(std::same_as<subtract_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_subtractable test case 10 (failing)");

    static_assert(is_multipliable_v<int>,
                  "hyperion::mpl::type_traits::is_multipliable test case 1 (failing)");
    static_assert(is_multipliable_v<double>,
                  "hyperion::mpl::type_traits::is_multipliable test case 2 (failing)");
    static_assert(is_multipliable_v<int, double>,
                  "hyperion::mpl::type_traits::is_multipliable test case 3 (failing)");
    static_assert(is_multipliable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_multipliable test case 4 (failing)");
    static_assert(!is_multipliable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_multipliable test case 5 (failing)");
    static_assert(std::same_as<multiply_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_multipliable test case 6 (failing)");
    static_assert(std::same_as<multiply_result_t<double>, double>,
                  "hyperion::mpl::type_traits::is_multipliable test case 7 (failing)");
    static_assert(std::same_as<multiply_result_t<int, double>, double>,
                  "hyperion::mpl::type_traits::is_multipliable test case 8 (failing)");
    static_assert(std::same_as<multiply_result_t<double, bool>, double>,
                  "hyperion::mpl::type_traits::is_multipliable test case 9 (failing)");
    static_assert(std::same_as<multiply_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_multipliable test case 10 (failing)");

    static_assert(is_dividible_v<int>,
                  "hyperion::mpl::type_traits::is_dividible test case 1 (failing)");
    static_assert(is_dividible_v<double>,
                  "hyperion::mpl::type_traits::is_dividible test case 2 (failing)");
    static_assert(is_dividible_v<int, double>,
                  "hyperion::mpl::type_traits::is_dividible test case 3 (failing)");
    static_assert(is_dividible_v<double, bool>,
                  "hyperion::mpl::type_traits::is_dividible test case 4 (failing)");
    static_assert(!is_dividible_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_dividible test case 5 (failing)");
    static_assert(std::same_as<divide_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_dividible test case 6 (failing)");
    static_assert(std::same_as<divide_result_t<double>, double>,
                  "hyperion::mpl::type_traits::is_dividible test case 7 (failing)");
    static_assert(std::same_as<divide_result_t<int, double>, double>,
                  "hyperion::mpl::type_traits::is_dividible test case 8 (failing)");
    static_assert(std::same_as<divide_result_t<double, bool>, double>,
                  "hyperion::mpl::type_traits::is_dividible test case 9 (failing)");
    static_assert(std::same_as<divide_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_dividible test case 10 (failing)");

    static_assert(is_binary_andable_v<int>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 1 (failing)");
    static_assert(!is_binary_andable_v<double>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 2 (failing)");
    static_assert(!is_binary_andable_v<int, double>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 3 (failing)");
    static_assert(!is_binary_andable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 4 (failing)");
    static_assert(!is_binary_andable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 5 (failing)");
    static_assert(std::same_as<binary_and_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 6 (failing)");
    static_assert(std::same_as<binary_and_result_t<double>, void>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 7 (failing)");
    static_assert(std::same_as<binary_and_result_t<int, double>, void>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 8 (failing)");
    static_assert(std::same_as<binary_and_result_t<double, bool>, void>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 9 (failing)");
    static_assert(std::same_as<binary_and_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_binary_andable test case 10 (failing)");

    static_assert(is_binary_orable_v<int>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 1 (failing)");
    static_assert(!is_binary_orable_v<double>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 2 (failing)");
    static_assert(!is_binary_orable_v<int, double>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 3 (failing)");
    static_assert(!is_binary_orable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 4 (failing)");
    static_assert(!is_binary_orable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 5 (failing)");
    static_assert(std::same_as<binary_or_result_t<int>, int>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 6 (failing)");
    static_assert(std::same_as<binary_or_result_t<double>, void>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 7 (failing)");
    static_assert(std::same_as<binary_or_result_t<int, double>, void>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 8 (failing)");
    static_assert(std::same_as<binary_or_result_t<double, bool>, void>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 9 (failing)");
    static_assert(std::same_as<binary_or_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_binary_orable test case 10 (failing)");

    static_assert(is_boolean_andable_v<int>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 1 (failing)");
    static_assert(is_boolean_andable_v<double>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 2 (failing)");
    static_assert(is_boolean_andable_v<int, double>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 3 (failing)");
    static_assert(is_boolean_andable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 4 (failing)");
    static_assert(!is_boolean_andable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 5 (failing)");
    static_assert(std::same_as<boolean_and_result_t<int>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 6 (failing)");
    static_assert(std::same_as<boolean_and_result_t<double>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 7 (failing)");
    static_assert(std::same_as<boolean_and_result_t<int, double>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 8 (failing)");
    static_assert(std::same_as<boolean_and_result_t<double, bool>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 9 (failing)");
    static_assert(std::same_as<boolean_and_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_boolean_andable test case 10 (failing)");

    static_assert(is_boolean_orable_v<int>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 1 (failing)");
    static_assert(is_boolean_orable_v<double>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 2 (failing)");
    static_assert(is_boolean_orable_v<int, double>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 3 (failing)");
    static_assert(is_boolean_orable_v<double, bool>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 4 (failing)");
    static_assert(!is_boolean_orable_v<nothing_able>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 5 (failing)");
    static_assert(std::same_as<boolean_or_result_t<int>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 6 (failing)");
    static_assert(std::same_as<boolean_or_result_t<double>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 7 (failing)");
    static_assert(std::same_as<boolean_or_result_t<int, double>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 8 (failing)");
    static_assert(std::same_as<boolean_or_result_t<double, bool>, bool>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 9 (failing)");
    static_assert(std::same_as<boolean_or_result_t<nothing_able>, void>,
                  "hyperion::mpl::type_traits::is_boolean_orable test case 10 (failing)");

} // namespace hyperion::mpl::type_traits::_test

#endif // HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_TEST_H
