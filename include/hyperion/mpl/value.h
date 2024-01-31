/// @file value.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming value type
/// @version 0.1
/// @date 2024-01-30
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

#ifndef HYPERION_MPL_VALUE_H
#define HYPERION_MPL_VALUE_H

#include <hyperion/mpl/concepts/comparable.h>
#include <hyperion/mpl/concepts/operator_able.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
    #include <compare>
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

#include <concepts>
#include <type_traits>

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

/// @ingroup mpl
/// @{
/// @defgroup value Metaprogramming Value Type
/// Hyperion provides `mpl::Value` as a metaprogramming type for storing, communicating,
/// working with, and calculating compile-time values.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/value.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto val1 = Value<4>{};
/// constexpr auto val2 = Value<2>{};
///
/// constexpr auto meaning_of_life = (val1 * 10_value) + val2;
///
/// static_assert(meaning_of_life == 42);
/// @endcode
/// @headerfile hyperion/mpl/type_traits.h
/// @}

namespace hyperion::mpl {

    namespace detail {

        template<typename TType, int = 0>
        struct has_static_constexpr_value : std::false_type { };

        HYPERION_IGNORE_COMMA_MISUSE_WARNING_START;
        HYPERION_IGNORE_UNUSED_VALUES_WARNING_START;
        template<typename TType>
        struct has_static_constexpr_value<TType, (int(TType::value), 0)> : std::true_type { };
        HYPERION_IGNORE_UNUSED_VALUES_WARNING_STOP;
        HYPERION_IGNORE_COMMA_MISUSE_WARNING_STOP;

        template<typename TType>
        concept HasStaticConstexprValue = has_static_constexpr_value<TType>::value;

    } // namespace detail

    /// @brief Concept specifying the requirements for a metaprogramming
    /// value type.
    ///
    /// A metaprogramming value type is any type that has a `static constexpr` member
    /// variable named `value`.
    ///
    /// # Requirements
    /// - `TType` must have a `static constexpr` member variable named `value`.
    ///
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<typename TType>
    concept ValueType = requires { TType::value; } && detail::HasStaticConstexprValue<TType>;

    /// @brief Type trait to determine whether type `TType` is a metaprogramming value type.
    ///
    /// A metaprogramming value type is any type that has a `static constexpr` member
    /// variable named `value`.
    ///
    /// @tparam TType The type to check
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<typename TType>
    struct is_value_type : std::bool_constant<ValueType<TType>> { };

    /// @brief Value of the type trait `is_value_type`.
    /// Used to determine whether type `TType` is a metaprogramming value type.
    ///
    /// A metaprogramming value type is any type that has a `static constexpr` member
    /// variable named `value`.
    ///
    /// @tparam TType The type to check
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<typename TType>
    static inline constexpr auto is_value_type_v = is_value_type<TType>::value;

    /// @brief `Value` is Hyperion's preferred metaprogramming value type.
    ///
    /// `Value` represents a compile time value, storing that value in its
    /// `static constexpr` member variable, `value`.
    ///
    /// @tparam TValue The value of this `Value`
    /// @tparam TType The type of `TValue`. This can be manually specified to enforce
    /// that `TValue` is coerced to `TType` (for example, in order to force `TValue`
    /// to be an `int`, instead of `std::size_t`)
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
    struct Value {
        /// @brief The value of this metaprogramming value
        static inline constexpr auto value = static_cast<TType>(TValue);

        /// @brief Conversion operator to the `value`
        [[nodiscard]] constexpr operator TType() const noexcept { // NOLINT
            return value;
        }

        /// @brief Returns the `value` of this `Value` specialization
        /// @return `value`
        [[nodiscard]] constexpr auto value_of() const -> TType {
            return value;
        }
    };

    /// @brief Numeric literal operator to create a compile-time `Value`.
    ///
    /// `Value` represents a compile time value, storing that value in its
    /// `static constexpr` member variable, `value`.
    ///
    /// @tparam TChars The characters of the numeric literal
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<char... TChars>
    [[nodiscard]] static inline constexpr auto operator""_value() noexcept {
        constexpr auto parsed
            = hyperion::detail::parse_literal<usize>(hyperion::detail::string_literal<TChars...>{});
        hyperion::detail::check_literal_status<parsed.status>();
        return Value<parsed.value>{};
    }

    /// @brief Returns the compile-time value of the given `mpl::Value`
    ///
    /// @tparam TValue The compile-time value of `value`
    /// @tparam TType The type of the compile-time value
    /// @param value The `mpl::Value` to get the compile-time value of
    /// @return the compile-time value of the given `mpl::Value`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
    [[nodiscard]] constexpr auto
    value_of([[maybe_unused]] const Value<TValue, TType>& value) noexcept -> decltype(TValue) {
        return TValue;
    }

    /// @brief Returns the compile-time value of the given `mpl::Value`
    ///
    /// # Requirements
    /// - `TType` must be an `mpl::ValueType`
    /// - `TType` must not be a specialization of `mpl::Value`
    ///
    /// @tparam TType The `mpl::ValueType` storing the compile-time value
    /// @param value The `mpl::ValueType` to get the compile-time value of
    /// @return the compile-time value of the given `mpl::Value`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<ValueType TType>
        requires(
            !std::same_as<TType, Value<TType::value, std::remove_cvref_t<decltype(TType::value)>>>)
    [[nodiscard]] constexpr auto
    value_of([[maybe_unused]] const TType& value) noexcept -> decltype(TType::value) {
        return TType::value;
    }

    /// @brief Returns the given `mpl::ValueType` into an `mpl::Value`
    ///
    /// # Requirements
    /// - `TType` must be an `mpl::ValueType`
    /// - `TType` must not be a specialization of `mpl::Value`
    ///
    /// @tparam TType The `mpl::ValueType` storing the compile-time value
    /// @param value The `mpl::ValueType` to get the compile-time value of
    /// @return the compile-time value of the given `mpl::Value`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<ValueType TType>
        requires(
            !std::same_as<TType, Value<TType::value, std::remove_cvref_t<decltype(TType::value)>>>)
    [[nodiscard]] constexpr auto as_value([[maybe_unused]] const TType& value) noexcept
        -> Value<TType::value, std::remove_cvref_t<decltype(TType::value)>> {
        return {};
    }

    /// @brief Unary plus operator for `mpl::Value`.
    ///
    /// # Requirements
    /// - The type of `TValue` must be `UnaryPlusable` (it must support unary `operator+`).
    ///
    /// @tparam TValue The value of this `Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to +
    /// @return The `mpl::Value` representing `+TValue`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::UnaryPlusable<decltype(TValue)>
    [[nodiscard]] constexpr auto
    operator+([[maybe_unused]] const Value<TValue, TType>& value) noexcept -> Value<TValue> {
        return {};
    }

    /// @brief Unary minus operator for `mpl::Value`.
    ///
    /// # Requirements
    /// - The type of `TValue` must be `UnaryMinusable` (it must support unary `operator-`).
    ///
    /// @tparam TValue The value of this `Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to -
    /// @return The `mpl::Value` representing `-TValue`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::UnaryMinusable<decltype(TValue)>
    [[nodiscard]] constexpr auto
    operator-([[maybe_unused]] const Value<TValue, TType>& value) noexcept -> Value<-TValue> {
        return {};
    }

    /// @brief Adds the two given `mpl::Value`s and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be addable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to add
    /// @param rhs The right-hand `mpl::Value` to add
    /// @return The `mpl::Value` representing `TLhs + TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Addable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator+([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs + TRhs> {
        return {};
    }

    /// @brief Subtracts `rhs` from `lhs` and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be subtractable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to subtract from
    /// @param rhs The right-hand `mpl::Value` to subtract
    /// @return The `mpl::Value` representing `TLhs - TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Subtractable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator-([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs - TRhs> {
        return {};
    }

    /// @brief Multiplies the two given `mpl::Value`s and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be multipliable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to add
    /// @param rhs The right-hand `mpl::Value` to add
    /// @return The `mpl::Value` representing `TLhs * TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Multipliable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator*([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs * TRhs> {
        return {};
    }

    /// @brief Divides `lhs` by `rhs` and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be dividible
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to divide
    /// @param rhs The right-hand `mpl::Value` to divide by
    /// @return The `mpl::Value` representing `TLhs / TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Dividible<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator/([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs / TRhs> {
        return {};
    }

    /// @brief Performs the boolean and of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be boolean andable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to and
    /// @param rhs The right-hand `mpl::Value` to and
    /// @return The `mpl::Value` representing `TLhs && TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BooleanAndable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator&&([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs && TRhs> {
        return {};
    }

    /// @brief Performs the boolean or of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be boolean orable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to or
    /// @param rhs The right-hand `mpl::Value` to or
    /// @return The `mpl::Value` representing `TLhs || TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BooleanOrable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator||([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs || TRhs> {
        return {};
    }

    /// @brief Performs the boolean not of `value`
    ///
    /// # Requirements
    /// - `TValue` boolean notable
    ///
    /// @tparam TValue The value of the `mpl::Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to not
    /// @return The `mpl::Value` representing `!TLhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::BooleanNotable<TType>
    [[nodiscard]] constexpr auto
    operator!([[maybe_unused]] const Value<TValue, TType>& value) noexcept
        -> Value<!TValue, TType> {
        return {};
    }

    /// @brief Performs the binary and of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be binary andable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to and
    /// @param rhs The right-hand `mpl::Value` to and
    /// @return The `mpl::Value` representing `TLhs & TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BinaryAndable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator&([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs & TRhs> {
        return {};
    }

    /// @brief Performs the binary or of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be binary orable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to or
    /// @param rhs The right-hand `mpl::Value` to or
    /// @return The `mpl::Value` representing `TLhs | TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BinaryOrable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator|([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs | TRhs> {
        return {};
    }

    /// @brief Performs the binary not of `value`
    ///
    /// # Requirements
    /// - `TValue` binary notable
    ///
    /// @tparam TValue The value of the `mpl::Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to not
    /// @return The `mpl::Value` representing `!TLhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::BinaryNotable<TType>
    [[nodiscard]] constexpr auto
    operator~([[maybe_unused]] const Value<TValue, TType>& value) noexcept
        -> Value<~TValue, TType> {
        return {};
    }

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::EqualityComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs == TRhs> {
        return {};
    }

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::InequalityComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator!=([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs != TRhs> {
        return {};
    }

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::ThreeWayComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator<=>([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
                [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept
        -> Value<TLhs <=> TRhs> {
        return {};
    }

#else

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::LessThanComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator<([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<(TLhs < TRhs)> {
        return {};
    }

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::LessThanOrEqualComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator<=([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept
        -> Value<(TLhs <= TRhs)> {
        return {};
    }

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::GreaterThanComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator>([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<(TLhs > TRhs)> {
        return {};
    }

    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::GreaterThanOrEqualComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator>=([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept
        -> Value<(TLhs >= TRhs)> {
        return {};
    }

#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    namespace _test {

        struct not_value_type {
            int value;
        };

        static_assert(ValueType<Value<true>>,
                      "hyperion::mpl::ValueType not satisfied by hyperion::mpl::Value "
                      "(implementation failing)");
        static_assert(ValueType<std::bool_constant<true>>,
                      "hyperion::mpl::ValueType not satisfied by std::bool_constant "
                      "(implementation failing)");
        static_assert(!ValueType<not_value_type>,
                      "hyperion::mpl::ValueType not satisfied by _test::not_value_type "
                      "(implementation failing)");

        static_assert(value_of(Value<3>{}) == 3, "hyperion::mpl::value_of test case 1 (failing)");
        static_assert(value_of(std::integral_constant<int, 3>{}) == 3,
                      "hyperion::mpl::value_of test case 2 (failing)");
        static_assert(Value<3>{} == 3,
                      "hyperion::mpl::Value implicit conversion test case (failing)");

        static_assert(std::same_as<decltype(as_value(std::integral_constant<int, 3>{})), Value<3>>,
                      "hyperion::mpl::as_value test case 1 (failing)");

        static_assert(Value<3>{} == 3_value,
                      "hyperion::mpl::operator"
                      "_value test case 1 (failing)");

        static_assert(+Value<3>{} == 3, "hyperion::mpl::operator+(Value) test case 1 (failing)");
        static_assert(-Value<3>{} == -3, "hyperion::mpl::operator-(Value) test case 1 (failing)");

        constexpr auto test_value1 = Value<4>{};
        constexpr auto test_value2 = Value<2>{};

        constexpr auto test_value_result = 42;

        static_assert(Value<1>{} + Value<2>{} == 3,
                      "hyperion::mpl::operator+(Value, Value) test case 1 (failing)");
        static_assert(Value<1>{} + Value<-2>{} == -1,
                      "hyperion::mpl::operator+(Value, Value) test case 2 (failing)");
        static_assert((test_value1 * 10_value) + test_value2 == test_value_result,
                      "hyperion::mpl::operator+(Value, Value) test case 2 (failing)");

        static_assert(Value<1>{} - Value<2>{} == -1,
                      "hyperion::mpl::operator-(Value, Value) test case 1 (failing)");
        static_assert(Value<1>{} - Value<-2>{} == 3,
                      "hyperion::mpl::operator-(Value, Value) test case 2 (failing)");

        static_assert(Value<2>{} * Value<2>{} == 4,
                      "hyperion::mpl::operator*(Value, Value) test case 1 (failing)");
        static_assert(Value<2>{} * Value<-2>{} == -4,
                      "hyperion::mpl::operator*(Value, Value) test case 2 (failing)");

        static_assert(Value<2>{} / Value<2>{} == 1,
                      "hyperion::mpl::operator*(Value, Value) test case 1 (failing)");
        static_assert(Value<2>{} / Value<-2>{} == -1,
                      "hyperion::mpl::operator*(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} && Value<false>{}) == false,
                      "hyperion::mpl::operator&&(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} && Value<true>{}) == true,
                      "hyperion::mpl::operator&&(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} || Value<false>{}) == true,
                      "hyperion::mpl::operator||(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<false>{} || Value<false>{}) == false,
                      "hyperion::mpl::operator||(Value, Value) test case 2 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} || Value<true>{}) == true,
                      "hyperion::mpl::operator||(Value, Value) test case 3 (failing)");

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert(!Value<true>{} == false,
                      "hyperion::mpl::operator!(Value) test case 1 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert(!Value<false>{} == true,
                      "hyperion::mpl::operator!(Value) test case 2 (failing)");

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b1100_usize>{} & Value<0b0011_usize>{}) == 0,
                      "hyperion::mpl::operator&(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b0110_usize>{} & Value<0b1100_usize>{}) == 0b0100_usize,
                      "hyperion::mpl::operator&(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b1100_usize>{} | Value<0b0011_usize>{}) == 0b1111_usize,
                      "hyperion::mpl::operator&(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b0110_usize>{} | Value<0b1100_usize>{}) == 0b1110_usize,
                      "hyperion::mpl::operator&(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert(~Value<0b0011_usize>{} == (~0b0011_usize),
                      "hyperion::mpl::operator~(Value) test case 1 (failing)");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert(~Value<0b1100_usize>{} == (~0b1100_usize),
                      "hyperion::mpl::operator~(Value) test case 2 (failing)");

        static_assert(Value<1>{} == Value<1>{},
                      "hyperion::mpl::operator==(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} == Value<2>{}),
                      "hyperion::mpl::operator==(Value, Value) test case 2 failing");

        static_assert(Value<1>{} != Value<2>{},
                      "hyperion::mpl::operator!=(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} != Value<1>{}),
                      "hyperion::mpl::operator!=(Value, Value) test case 2 failing");

        static_assert(Value<1>{} < Value<2>{},
                      "hyperion::mpl::operator<(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} < Value<1>{}),
                      "hyperion::mpl::operator<(Value, Value) test case 2 failing");

        static_assert(Value<1>{} <= Value<1>{},
                      "hyperion::mpl::operator<=(Value, Value) test case 1 failing");
        static_assert(Value<1>{} <= Value<2>{},
                      "hyperion::mpl::operator<=(Value, Value) test case 2 failing");
        static_assert(!(Value<1>{} <= Value<0>{}),
                      "hyperion::mpl::operator<=(Value, Value) test case 3 failing");

        static_assert(Value<2>{} > Value<1>{},
                      "hyperion::mpl::operator>(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} > Value<1>{}),
                      "hyperion::mpl::operator>(Value, Value) test case 2 failing");

        static_assert(Value<1>{} >= Value<1>{},
                      "hyperion::mpl::operator>=(Value, Value) test case 1 failing");
        static_assert(Value<2>{} >= Value<1>{},
                      "hyperion::mpl::operator>=(Value, Value) test case 2 failing");
        static_assert(!(Value<0>{} >= Value<1>{}),
                      "hyperion::mpl::operator>=(Value, Value) test case 3 failing");

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
        static_assert((Value<1>{} <=> Value<1>{}) == std::strong_ordering::equal,
                      "hyperion::mpl::operator<=>(Value, Value) test case 1 failing");
        static_assert(Value<2>{} <=> Value<1>{} == std::strong_ordering::greater,
                      "hyperion::mpl::operator<=>(Value, Value) test case 2 failing");
        static_assert(Value<0>{} <=> Value<1>{} == std::strong_ordering::less,
                      "hyperion::mpl::operator<=>(Value, Value) test case 3 failing");
#endif
    } // namespace _test
} // namespace hyperion::mpl

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;

#endif // HYPERION_MPL_VALUE_H
