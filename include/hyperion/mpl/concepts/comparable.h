/// @file comparable.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming C++20 concepts to determine if two types are comparable
/// in various ways
/// @version 0.1
/// @date 2024-01-27
///
/// MIT License
/// @copyright Copyright (c) 2024 Braxton Salyer <braxtonsalyer@gmail.com>
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

#ifndef HYPERION_MPL_CONCEPTS_COMPARABLE_H
#define HYPERION_MPL_CONCEPTS_COMPARABLE_H

#include <hyperion/mpl/type_traits/is_comparable.h>

namespace hyperion::mpl::concepts {

    template<typename TLhs, typename TRhs>
    concept EqualityComparable = type_traits::is_equality_comparable_v<TLhs, TRhs>;

    template<typename TLhs, typename TRhs>
    concept InequalityComparable = type_traits::is_inequality_comparable_v<TLhs, TRhs>;

    template<typename TLhs, typename TRhs>
    concept LessThanComparable = type_traits::is_less_than_comparable_v<TLhs, TRhs>;

    template<typename TLhs, typename TRhs>
    concept LessThanOrEqualComparable = type_traits::is_less_than_or_equal_comparable_v<TLhs, TRhs>;

    template<typename TLhs, typename TRhs>
    concept GreaterThanComparable = type_traits::is_greater_than_comparable_v<TLhs, TRhs>;

    template<typename TLhs, typename TRhs>
    concept GreaterThanOrEqualComparable
        = type_traits::is_greater_than_or_equal_comparable_v<TLhs, TRhs>;

    template<typename TLhs, typename TRhs>
    concept ThreeWayComparable = type_traits::is_three_way_comparable_v<TLhs, TRhs>;

    namespace _test {

        struct not_comparable { };

        static_assert(EqualityComparable<int, int>,
                      "hyperion::mpl::concepts::EqualityComparable test case 1 failing");
        static_assert(EqualityComparable<int, double>,
                      "hyperion::mpl::concepts::EqualityComparable test case 2 failing");
        static_assert(!EqualityComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::EqualityComparable test case 3 failing");

        static_assert(InequalityComparable<int, int>,
                      "hyperion::mpl::concepts::InequalityComparable test case 1 failing");
        static_assert(InequalityComparable<int, double>,
                      "hyperion::mpl::concepts::InequalityComparable test case 2 failing");
        static_assert(!InequalityComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::InequalityComparable test case 3 failing");

        static_assert(LessThanComparable<int, int>,
                      "hyperion::mpl::concepts::LessThanComparable test case 1 failing");
        static_assert(LessThanComparable<int, double>,
                      "hyperion::mpl::concepts::LessThanComparable test case 2 failing");
        static_assert(!LessThanComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::LessThanComparable test case 3 failing");

        static_assert(LessThanOrEqualComparable<int, int>,
                      "hyperion::mpl::concepts::LessThanOrEqualComparable test case 1 failing");
        static_assert(LessThanOrEqualComparable<int, double>,
                      "hyperion::mpl::concepts::LessThanOrEqualComparable test case 2 failing");
        static_assert(!LessThanOrEqualComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::LessThanOrEqualComparable test case 3 failing");

        static_assert(GreaterThanComparable<int, int>,
                      "hyperion::mpl::concepts::GreaterThanComparable test case 1 failing");
        static_assert(GreaterThanComparable<int, double>,
                      "hyperion::mpl::concepts::GreaterThanComparable test case 2 failing");
        static_assert(!GreaterThanComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::GreaterThanComparable test case 3 failing");

        static_assert(GreaterThanOrEqualComparable<int, int>,
                      "hyperion::mpl::concepts::GreaterThanOrEqualComparable test case 1 failing");
        static_assert(GreaterThanOrEqualComparable<int, double>,
                      "hyperion::mpl::concepts::GreaterThanOrEqualComparable test case 2 failing");
        static_assert(!GreaterThanOrEqualComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::GreaterThanOrEqualComparable test case 3 failing");

        static_assert(ThreeWayComparable<int, int>,
                      "hyperion::mpl::concepts::ThreeWayComparable test case 1 failing");
        static_assert(ThreeWayComparable<int, double>,
                      "hyperion::mpl::concepts::ThreeWayComparable test case 2 failing");
        static_assert(!ThreeWayComparable<int, not_comparable>,
                      "hyperion::mpl::concepts::ThreeWayComparable test case 3 failing");
    } // namespace _test
} // namespace hyperion::mpl::concepts

#endif // HYPERION_MPL_CONCEPTS_IS_COMPARABLE_H
