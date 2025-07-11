/// @file is_comparable.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief tests for is_comparable.h
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

#ifndef HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_TEST_H
#define HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_TEST_H

#include <hyperion/mpl/type_traits/is_comparable.h>

namespace hyperion::mpl::type_traits::_test {

    struct not_comparable { };

    static_assert(is_equality_comparable_v<int, int>,
                  "hyperion::mpl::type_traits::is_equality_comparable test case 1 failing");
    static_assert(is_equality_comparable_v<int, double>,
                  "hyperion::mpl::type_traits::is_equality_comparable test case 2 failing");
    static_assert(!is_equality_comparable_v<int, not_comparable>,
                  "hyperion::mpl::type_traits::is_equality_comparable test case 3 failing");
    static_assert(!is_equality_comparable_v<not_comparable, not_comparable>,
                  "hyperion::mpl::type_traits::is_equality_comparable test case 4 failing");

    static_assert(is_inequality_comparable_v<int, int>,
                  "hyperion::mpl::type_traits::is_inequality_comparable test case 1 failing");
    static_assert(is_inequality_comparable_v<int, double>,
                  "hyperion::mpl::type_traits::is_inequality_comparable test case 2 failing");
    static_assert(!is_inequality_comparable_v<int, not_comparable>,
                  "hyperion::mpl::type_traits::is_inequality_comparable test case 3 failing");

    static_assert(is_less_than_comparable_v<int, int>,
                  "hyperion::mpl::type_traits::is_less_than_comparable test case 1 failing");
    static_assert(is_less_than_comparable_v<int, double>,
                  "hyperion::mpl::type_traits::is_less_than_comparable test case 2 failing");
    static_assert(!is_less_than_comparable_v<int, not_comparable>,
                  "hyperion::mpl::type_traits::is_less_than_comparable test case 3 failing");

    static_assert(
        is_less_than_or_equal_comparable_v<int, int>,
        "hyperion::mpl::type_traits::is_less_than_or_equal_comparable test case 1 failing");
    static_assert(
        is_less_than_or_equal_comparable_v<int, double>,
        "hyperion::mpl::type_traits::is_less_than_or_equal_comparable test case 2 failing");
    static_assert(
        !is_less_than_or_equal_comparable_v<int, not_comparable>,
        "hyperion::mpl::type_traits::is_less_than_or_equal_comparable test case 3 failing");

    static_assert(is_greater_than_comparable_v<int, int>,
                  "hyperion::mpl::type_traits::is_greater_than_comparable test case 1 failing");
    static_assert(is_greater_than_comparable_v<int, double>,
                  "hyperion::mpl::type_traits::is_greater_than_comparable test case 2 failing");
    static_assert(!is_greater_than_comparable_v<int, not_comparable>,
                  "hyperion::mpl::type_traits::is_greater_than_comparable test case 3 failing");

    static_assert(
        is_greater_than_or_equal_comparable_v<int, int>,
        "hyperion::mpl::type_traits::is_greater_than_or_equal_comparable test case 1 failing");
    static_assert(
        is_greater_than_or_equal_comparable_v<int, double>,
        "hyperion::mpl::type_traits::is_greater_than_or_equal_comparable test case 2 failing");
    static_assert(
        !is_greater_than_or_equal_comparable_v<int, not_comparable>,
        "hyperion::mpl::type_traits::is_greater_than_or_equal_comparable test case 3 failing");

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    static_assert(is_three_way_comparable_v<int, int>,
                  "hyperion::mpl::type_traits::is_three_way_comparable test case 1 failing");
    static_assert(is_three_way_comparable_v<int, double>,
                  "hyperion::mpl::type_traits::is_three_way_comparable test case 2 failing");
    static_assert(!is_three_way_comparable_v<int, not_comparable>,
                  "hyperion::mpl::type_traits::is_three_way_comparable test case 3 failing");

    static_assert(std::same_as<three_way_compare_result_t<int, int>, std::strong_ordering>,
                  "hyperion::mpl::type_traits::is_three_way_comparable test case 1 failing");
    static_assert(std::same_as<three_way_compare_result_t<int, double>, std::partial_ordering>,
                  "hyperion::mpl::type_traits::is_three_way_comparable test case 2 failing");
    static_assert(std::same_as<three_way_compare_result_t<int, not_comparable>, void>,
                  "hyperion::mpl::type_traits::is_three_way_comparable test case 3 failing");

#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
} // namespace hyperion::mpl::type_traits::_test

#endif // HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_TEST_H
