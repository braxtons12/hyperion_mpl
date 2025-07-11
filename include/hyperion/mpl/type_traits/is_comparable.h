/// @file is_comparable.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming type traits to determine if two types are comparable
/// in various ways
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

#ifndef HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_H
#define HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_H

#include <hyperion/platform/def.h>
//
#include <hyperion/mpl/concepts/comparable.h>

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
    #include <compare>
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

#include <type_traits>

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

/// @ingroup type_traits
/// @{
/// @defgroup comparison_operator_detection Comparison Operator Detection
/// Assortment of type traits to determine if two types are comparable.
///
/// # Example:
/// @code {.cpp}
/// #include <hyperion/mpl/type_traits/is_comparable.h>
///
/// using namespace hyperion::mpl::type_traits;
/// struct not_comparable {};
///
/// static_assert(is_equality_comparable_v<int, int>);
/// static_assert(!is_equality_comparable_v<not_comparable, not_comparable>);
///
/// @endcode
/// @headerfile hyperion/mpl/type_traits/is_comparable.h
/// @}

namespace hyperion::mpl::type_traits {

    namespace detail { }

    // clang-format off

    /// @brief Type trait to determine whether a value of type `TLhs`
    /// is equality comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_equality_comparable
        : std::bool_constant<concepts::EqualityComparable<TLhs, TRhs>>
    {};

    // clang-format on

    /// @brief Value of the type trait `is_equality_comparable`.
    /// Used to determine whether a value of type `TLhs` is equality
    /// comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_equality_comparable_v
        = is_equality_comparable<TLhs, TRhs>::value;
    // clang-format off

    /// @brief Type trait to determine whether a value of type `TLhs`
    /// is inequality comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_inequality_comparable
        : std::bool_constant<concepts::InequalityComparable<TLhs, TRhs>>
    {};

    // clang-format on

    /// @brief Value of the type trait `is_inequality_comparable`.
    /// Used to determine whether a value of type `TLhs` is inequality
    /// comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_inequality_comparable_v
        = is_inequality_comparable<TLhs, TRhs>::value;
    // clang-format off

    /// @brief Type trait to determine whether a value of type `TLhs` is
    /// less-than comparable to a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_less_than_comparable
        : std::bool_constant<concepts::LessThanComparable<TLhs, TRhs>>
    {};

    // clang-format on

    /// @brief Value of the type trait `is_less_than_comparable`.
    /// Used to determine whether a value of type `TLhs` is less-than
    /// comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_less_than_comparable_v
        = is_less_than_comparable<TLhs, TRhs>::value;
    // clang-format off

    /// @brief Type trait to determine whether a value of type `TLhs` is
    /// less-than-or-equal comparable to a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_less_than_or_equal_comparable
        : std::bool_constant<concepts::LessThanOrEqualComparable<TLhs, TRhs>>
    {};

    // clang-format on

    /// @brief Value of the type trait `is_less_than_or_equal_comparable`.
    /// Used to determine whether a value of type `TLhs` is less-than-or-equal
    /// comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_less_than_or_equal_comparable_v
        = is_less_than_or_equal_comparable<TLhs, TRhs>::value;
    // clang-format off

    /// @brief Type trait to determine whether a value of type `TLhs` is
    /// greater-than comparable to a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_greater_than_comparable
        : std::bool_constant<concepts::GreaterThanComparable<TLhs, TRhs>>
    {};

    // clang-format on

    /// @brief Value of the type trait `is_greater_than_comparable`.
    /// Used to determine whether a value of type `TLhs` is greater-than
    /// comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_greater_than_comparable_v
        = is_greater_than_comparable<TLhs, TRhs>::value;
    // clang-format off

    /// @brief Type trait to determine whether a value of type `TLhs` is
    /// greater-than-or-equal comparable to a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_greater_than_or_equal_comparable
        : std::bool_constant<concepts::GreaterThanOrEqualComparable<TLhs, TRhs>>
    {};

    // clang-format on

    /// @brief Value of the type trait `is_greater_than_or_equal_comparable`.
    /// Used to determine whether a value of type `TLhs` is greater-than-or-equal
    /// comparable with a value of type `TRhs`.
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_greater_than_or_equal_comparable_v
        = is_greater_than_or_equal_comparable<TLhs, TRhs>::value;
    // clang-format off

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    /// @brief Type trait to determine whether a value of type `TLhs` is
    /// three-way comparable to a value of type `TRhs`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` is three-way comparable with a `TRhs`, also provides
    /// the member `typedef` type `result_type` equal to the type of the returned
    /// result of three-way comparing a `TLhs` with a `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() <=> std::declval<TRhs>())`
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_three_way_comparable<TLhs, TRHs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    struct is_three_way_comparable : std::false_type {
        using result_type = void;
    };

    // specialization for the true case

    template<typename TLhs, typename TRhs>
        requires concepts::ThreeWayComparable<TLhs, TRhs>
    struct is_three_way_comparable<TLhs, TRhs> : std::true_type {
        using result_type = std::compare_three_way_result_t<TLhs, TRhs>;
    };
    // clang-format on

    /// @brief Value of the type trait `is_three_way_comparable`.
    /// Used to determine whether a value of type `TLhs` is
    /// three-way comparable to a value of type `TRhs`.
    ///
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    static inline constexpr auto is_three_way_comparable_v
        = is_three_way_comparable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member typedef of `is_three_way_comparable`.
    /// Used to determine the type of the returned result of three-way comparing
    /// a `TLhs` with a `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() <=> std::declval<TRhs>())`
    ///
    /// @tparam TLhs The left-hand side argument of the comparison
    /// @tparam TRhs The right-hand side argument of the comparison
    /// @ingroup comparison_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_comparable.h
    template<typename TLhs, typename TRhs>
    using three_way_compare_result_t = typename is_three_way_comparable<TLhs, TRhs>::result_type;

#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
} // namespace hyperion::mpl::type_traits

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;
#endif // HYPERION_MPL_TYPE_TRAITS_IS_COMPARABLE_H
