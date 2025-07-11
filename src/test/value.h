/// @file value.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Test for value.h
/// @version 0.5.1
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

#ifndef HYPERION_MPL_VALUE_TEST_H
#define HYPERION_MPL_VALUE_TEST_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>

namespace hyperion::mpl::_test::value {

    static_assert(value_of(Value<3>{}) == 3, "hyperion::mpl::value_of test case 1 (failing)");
    static_assert(value_of(std::integral_constant<int, 3>{}) == 3,
                  "hyperion::mpl::value_of test case 2 (failing)");
    static_assert(Value<3>{} == 3, "hyperion::mpl::Value implicit conversion test case (failing)");

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
    static_assert(!Value<true>{} == false, "hyperion::mpl::operator!(Value) test case 1 (failing)");
    // NOLINTNEXTLINE(readability-simplify-boolean-expr)
    static_assert(!Value<false>{} == true, "hyperion::mpl::operator!(Value) test case 2 (failing)");

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

    template<MetaValue TValue>
    struct add_one_meta {
        static inline constexpr auto value = TValue::value + 1;
    };

    template<MetaValue TValue>
    struct times_two_meta {
        static inline constexpr auto value = TValue::value * 2;
    };

    template<MetaValue TValue>
    struct add_one_meta_typed {
        using type = Value<TValue::value + 1>;
    };

    template<MetaValue TValue>
    struct times_two_meta_typed {
        using type = Value<TValue::value * 2>;
    };

    template<auto TValue>
    struct add_one_value {
        static inline constexpr auto value = TValue + 1;
    };

    template<auto TValue>
    struct times_two_value {
        static inline constexpr auto value = TValue * 2;
    };

    template<auto TValue>
    struct add_one_value_typed {
        using type = Value<TValue + 1>;
    };

    template<auto TValue>
    struct times_two_value_typed {
        using type = Value<TValue * 2>;
    };

    static_assert((1_value).apply<add_one_meta>() == 2,
                  "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                  "(MetaValue<MetaFunction<MetaValue>>) test case 1 (failing)");
    static_assert((2_value).apply<times_two_meta>() == 4_value,
                  "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                  "(MetaValue<MetaFunction<MetaValue>>) test case 2 (failing)");
    static_assert((2_value).apply<times_two_meta>().apply<add_one_meta>().apply<times_two_meta>()
                      == 10_value,
                  "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                  "(MetaValue<MetaFunction<MetaValue>>) test case 3 (failing)");

    static_assert((1_value).apply<add_one_meta_typed>() == 2,
                  "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                  "(MetaType<MetaFunction<MetaValue>>) test case 1 (failing)");
    static_assert((2_value).apply<times_two_meta_typed>() == 4_value,
                  "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                  "(MetaType<MetaFunction<MetaValue>>) test case 2 (failing)");
    static_assert((2_value)
                          .apply<times_two_meta_typed>()
                          .apply<add_one_meta_typed>()
                          .apply<times_two_meta_typed>()
                      == 10_value,
                  "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                  "(MetaType<MetaFunction<MetaValue>>) test case 3 (failing)");

    static_assert((1_value).apply<add_one_value>() == 2,
                  "hyperion::mpl::Value::apply<MetaFunction<value>> "
                  "(MetaValue<MetaFunction<value>>) test case 1 (failing)");
    static_assert((2_value).apply<times_two_value>() == 4_value,
                  "hyperion::mpl::Value::apply<MetaFunction<value>> "
                  "(MetaValue<MetaFunction<value>>) test case 2 (failing)");
    static_assert((2_value).apply<times_two_value>().apply<add_one_value>().apply<times_two_value>()
                      == 10_value,
                  "hyperion::mpl::Value::apply<MetaFunction<value>> "
                  "(MetaValue<MetaFunction<value>>) test case 3 (failing)");

    static_assert((1_value).apply<add_one_value_typed>() == 2,
                  "hyperion::mpl::Value::apply<MetaFunction<value>> "
                  "(MetaType<MetaFunction<value>>) test case 1 (failing)");
    static_assert((2_value).apply<times_two_value_typed>() == 4_value,
                  "hyperion::mpl::Value::apply<MetaFunction<value>> "
                  "(MetaType<MetaFunction<value>>) test case 2 (failing)");
    static_assert((2_value)
                          .apply<times_two_value_typed>()
                          .apply<add_one_value_typed>()
                          .apply<times_two_value_typed>()
                      == 10_value,
                  "hyperion::mpl::Value::apply<MetaFunction<value>> "
                  "(MetaType<MetaFunction<value>>) test case 3 (failing)");

    constexpr auto add1 = [](MetaValue auto value) {
        return value + 1_value;
    };

    constexpr auto times2 = [](MetaValue auto value) {
        return value * 2_value;
    };

    static_assert((1_value).apply(add1) == 2,
                  "hyperion::mpl::Value::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                  "1 (failing)");
    static_assert((2_value).apply(times2) == 4,
                  "hyperion::mpl::Value::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                  "2 (failing)");
    static_assert((2_value).apply(times2).apply(add1).apply(times2) == 10_value,
                  "hyperion::mpl::Value::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                  "3 (failing)");

    template<auto TValue>
    struct is_true_value : public std::bool_constant<bool(TValue)> { };

    static_assert((1_value).satisfies<is_true_value>(),
                  "hyperion::mpl::Value::satisfies<ValueMetaFunction> -> MetaValue test case 1 "
                  "(failing)");
    static_assert(!(0_value).satisfies<is_true_value>(),
                  "hyperion::mpl::Value::satisfies<ValueMetaFunction> -> MetaValue test case 2 "
                  "(failing)");

    template<MetaValue TValue>
    struct is_true_meta : public std::bool_constant<bool(TValue::value)> { };

    static_assert((1_value).satisfies<is_true_meta>(),
                  "hyperion::mpl::Value::satisfies<TypeMetaFunction> -> MetaValue test case 1 "
                  "(failing)");
    static_assert(!(0_value).satisfies<is_true_meta>(),
                  "hyperion::mpl::Value::satisfies<TypeMetaFunction> -> MetaValue test case 2 "
                  "(failing)");

    constexpr auto is_true
        = [](MetaValue auto value) noexcept -> Value<decltype(value)::value, bool> {
        return {};
    };

    static_assert((1_value).satisfies(is_true),
                  "hyperion::mpl::Value::satisfies(MetaPredicate) -> MetaValue test case 1 "
                  "(failing)");
    static_assert(not(0_value).satisfies(is_true),
                  "hyperion::mpl::Value::satisfies(MetaPredicate) -> MetaValue test case 2 "
                  "(failing)");

    constexpr auto is_int = [](MetaType auto type) noexcept
        -> Value<std::is_same_v<int, typename decltype(type)::type>, bool> {
        return {};
    };

    static_assert(
        not(1_value).satisfies(is_int),
        "hyperion::mpl::Value::satisfies(MetaPredicate) -> MetaValue test case 3 (failing)");
} // namespace hyperion::mpl::_test::value

#endif // HYPERION_MPL_VALUE_TEST_H
