/// @file value.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming value type
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

namespace hyperion::mpl {

    // clang-format off

    template<typename TType>
    struct is_value_type 
        : std::bool_constant<requires {
                                 TType::value;
                             }>
    {};

    template<typename TType>
    static inline constexpr auto is_value_type_v = is_value_type<TType>::value;

    template<typename TType>
    concept ValueType = is_value_type_v<TType>;
    // clang-format on

    template<auto TValue, typename TType = decltype(TValue)>
    struct Value {
        static inline constexpr auto value = TValue;

        // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
        [[nodiscard]] constexpr operator decltype(value)() noexcept {
            return value;
        }
    };

    template<char... TChars>
    [[nodiscard]] static inline constexpr auto operator""_value() noexcept {
        constexpr auto parsed = detail::parse_literal<usize>(detail::string_literal<TChars...>{});
        detail::check_literal_status<parsed.status>();
        return Value<parsed.value>{};
    }

    template<auto TValue>
    [[nodiscard]] constexpr auto
    value_of([[maybe_unused]] const Value<TValue>& value) noexcept -> decltype(TValue) {
        return TValue;
    }

    template<ValueType TType>
        requires(!std::same_as<TType, Value<TType::value>>)
    [[nodiscard]] constexpr auto
    value_of([[maybe_unused]] const TType& value) noexcept -> decltype(TType::value) {
        return TType::value;
    }

    template<ValueType TType>
        requires(!std::same_as<TType, Value<TType::value>>)
    [[nodiscard]] constexpr auto
    as_value([[maybe_unused]] const TType& value) noexcept -> Value<TType::value> {
        return {};
    }

    template<auto TLhs>
        requires concepts::UnaryPlusable<decltype(TLhs)>
    [[nodiscard]] constexpr auto
    operator+([[maybe_unused]] const Value<TLhs>& value) noexcept -> Value<TLhs> {
        return {};
    }

    template<auto TLhs>
        requires concepts::UnaryMinusable<decltype(TLhs)>
    [[nodiscard]] constexpr auto
    operator-([[maybe_unused]] const Value<TLhs>& value) noexcept -> Value<-TLhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::Addable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator+([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs + TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::Subtractable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator-([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs - TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::Dividible<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator/([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs / TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::Multipliable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator*([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs * TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::BooleanAndable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator&&([[maybe_unused]] const Value<TLhs>& lhs,
               [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs && TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::BooleanOrable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator||([[maybe_unused]] const Value<TLhs>& lhs,
               [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs || TRhs> {
        return {};
    }

    template<auto TLhs>
        requires concepts::BooleanNotable<decltype(TLhs)>
    [[nodiscard]] constexpr auto
    operator!([[maybe_unused]] const Value<TLhs>& lhs) noexcept -> Value<!TLhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::BinaryAndable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator&([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs & TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::BinaryOrable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator|([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs | TRhs> {
        return {};
    }

    template<auto TLhs>
        requires concepts::BinaryNotable<decltype(TLhs)>
    [[nodiscard]] constexpr auto
    operator~([[maybe_unused]] const Value<TLhs>& lhs) noexcept -> Value<~TLhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::EqualityComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Value<TLhs>& lhs,
               [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs == TRhs> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::InequalityComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator!=([[maybe_unused]] const Value<TLhs>& lhs,
               [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs != TRhs> {
        return {};
    }

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    template<auto TLhs, auto TRhs>
        requires concepts::ThreeWayComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator<=>([[maybe_unused]] const Value<TLhs>& lhs,
                [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<TLhs <=> TRhs> {
        return {};
    }

#else

    template<auto TLhs, auto TRhs>
        requires concepts::LessThanComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator<([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<(TLhs < TRhs)> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::LessThanOrEqualComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator<=([[maybe_unused]] const Value<TLhs>& lhs,
               [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<(TLhs <= TRhs)> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::GreaterThanComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator>([[maybe_unused]] const Value<TLhs>& lhs,
              [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<(TLhs > TRhs)> {
        return {};
    }

    template<auto TLhs, auto TRhs>
        requires concepts::GreaterThanOrEqualComparable<decltype(TLhs), decltype(TRhs)>
    [[nodiscard]] constexpr auto
    operator>=([[maybe_unused]] const Value<TLhs>& lhs,
               [[maybe_unused]] const Value<TRhs>& rhs) noexcept -> Value<(TLhs >= TRhs)> {
        return {};
    }

#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    namespace _test {

        static_assert(ValueType<Value<true>>,
                      "hyperion::mpl::ValueType not satisfied by hyperion::mpl::Value "
                      "(implementation failing)");
        static_assert(ValueType<std::bool_constant<true>>,
                      "hyperion::mpl::ValueType not satisfied by std::bool_constant "
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

        static_assert(Value<1>{} + Value<2>{} == 3,
                      "hyperion::mpl::operator+(Value, Value) test case 1 (failing)");
        static_assert(Value<1>{} + Value<-2>{} == -1,
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

#endif // HYPERION_MPL_VALUE_H
