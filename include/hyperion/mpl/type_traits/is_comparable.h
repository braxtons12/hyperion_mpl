/// @file is_comparable.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming type traits to determine if two types are comparable
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

#ifndef HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_H
#define HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_H

#include <hyperion/platform/def.h>

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
    #include <compare>
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

#include <type_traits>

namespace hyperion::mpl::type_traits {

    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_equality_comparable
        : std::bool_constant<requires(const TLhs& lhs, const TRhs& rhs) {
            lhs == rhs;
            rhs == lhs;
          }>
    {};

    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_equality_comparable_v
        = is_equality_comparable<TLhs, TRhs>::value;
    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_inequality_comparable
        : std::bool_constant<requires(const TLhs& lhs, const TRhs& rhs) {
            lhs != rhs;
            rhs != lhs;
          }>
    {};

    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_inequality_comparable_v
        = is_inequality_comparable<TLhs, TRhs>::value;
    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_less_than_comparable
        : std::bool_constant<requires(const TLhs& lhs, const TRhs& rhs) {
            lhs < rhs;
            rhs < lhs;
          }>
    {};

    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_less_than_comparable_v
        = is_less_than_comparable<TLhs, TRhs>::value;
    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_less_than_or_equal_comparable
        : std::bool_constant<requires(const TLhs& lhs, const TRhs& rhs) {
            lhs <= rhs;
            rhs <= lhs;
          }>
    {};

    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_less_than_or_equal_comparable_v
        = is_less_than_or_equal_comparable<TLhs, TRhs>::value;
    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_greater_than_comparable
        : std::bool_constant<requires(const TLhs& lhs, const TRhs& rhs) {
            lhs > rhs;
            rhs > lhs;
          }>
    {};

    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_greater_than_comparable_v
        = is_greater_than_comparable<TLhs, TRhs>::value;
    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_greater_than_or_equal_comparable
        : std::bool_constant<requires(const TLhs& lhs, const TRhs& rhs) {
            lhs >= rhs;
            rhs >= lhs;
          }>
    {};

    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_greater_than_or_equal_comparable_v
        = is_greater_than_or_equal_comparable<TLhs, TRhs>::value;
    // clang-format off

    template<typename TLhs, typename TRhs>
    struct is_three_way_comparable : std::false_type {
#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
        using result_type = void;
#endif
    };

    template<typename TLhs, typename TRhs>
        requires std::three_way_comparable_with<TLhs, TRhs>
    struct is_three_way_comparable<TLhs, TRhs> : std::true_type {
#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
        using result_type = std::compare_three_way_result_t<TLhs, TRhs>;
#endif
    };
    // clang-format on

    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_three_way_comparable_v
        = is_three_way_comparable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs>
    using three_way_compare_result_t = typename is_three_way_comparable<TLhs, TRhs>::result_type;

    namespace _test {

        struct not_comparable { };

        static_assert(is_equality_comparable_v<int, int>,
                      "hyperion::mpl::type_traits::is_equality_comparable test case 1 failing");
        static_assert(is_equality_comparable_v<int, double>,
                      "hyperion::mpl::type_traits::is_equality_comparable test case 2 failing");
        static_assert(!is_equality_comparable_v<int, not_comparable>,
                      "hyperion::mpl::type_traits::is_equality_comparable test case 3 failing");

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

        static_assert(is_three_way_comparable_v<int, int>,
                      "hyperion::mpl::type_traits::is_three_way_comparable test case 1 failing");
        static_assert(is_three_way_comparable_v<int, double>,
                      "hyperion::mpl::type_traits::is_three_way_comparable test case 2 failing");
        static_assert(!is_three_way_comparable_v<int, not_comparable>,
                      "hyperion::mpl::type_traits::is_three_way_comparable test case 3 failing");

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
        static_assert(std::same_as<three_way_compare_result_t<int, int>, std::strong_ordering>,
                      "hyperion::mpl::type_traits::is_three_way_comparable test case 1 failing");
        static_assert(std::same_as<three_way_compare_result_t<int, double>, std::partial_ordering>,
                      "hyperion::mpl::type_traits::is_three_way_comparable test case 2 failing");
        static_assert(std::same_as<three_way_compare_result_t<int, not_comparable>, void>,
                      "hyperion::mpl::type_traits::is_three_way_comparable test case 3 failing");
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
    }  // namespace _test
} // namespace hyperion::mpl::type_traits

#endif // HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_H
