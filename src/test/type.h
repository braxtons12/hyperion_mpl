/// @file type.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Test for type.h
/// @version 0.3.0
/// @date 2025-11-24
///
/// MIT License
/// @copyright Copyright (c) 2025 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#ifndef HYPERION_MPL_TYPE_TEST_H
#define HYPERION_MPL_TYPE_TEST_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/type.h>

namespace hyperion::mpl::_test::type {
    constexpr int test_val = 1;

    static_assert(std::same_as<int, typename decltype(decltype_(1))::type>,
                  "hyperion::mpl::decltype_ test case 1 (failing)");
    static_assert(std::same_as<const int&, typename decltype(decltype_(test_val))::type>,
                  "hyperion::mpl::decltype_ test case 2 (failing)");
    static_assert(std::same_as<const int&, typename decltype(decltype_<const int&>())::type>,
                  "hyperion::mpl::decltype_ test case 3 (failing)");

    static_assert(Type<int>{} == Type<int>{},
                  "hyperion::mpl::Type::operator== test case 1 (failing)");
    static_assert(Type<const int>{} == Type<const int>{},
                  "hyperion::mpl::Type::operator== test case 2 (failing)");
    static_assert(!(Type<const int>{} == Type<int>{}),
                  "hyperion::mpl::Type::operator== test case 3 (failing)");
    static_assert(!(Type<double>{} == Type<int>{}),
                  "hyperion::mpl::Type::operator== test case 4 (failing)");

    static_assert(Type<const int>{} != Type<int>{},
                  "hyperion::mpl::Type::operator!= test case 1 (failing)");
    static_assert(Type<double>{} != Type<int>{},
                  "hyperion::mpl::Type::operator!= test case 2 (failing)");
    static_assert(!(Type<int>{} != Type<int>{}),
                  "hyperion::mpl::Type::operator!= test case 3 (failing)");

    struct not_copy_or_move {
        not_copy_or_move() = default;
        ~not_copy_or_move() = default;
        not_copy_or_move(const not_copy_or_move&) = delete;
        not_copy_or_move(not_copy_or_move&&) = default;

        auto operator=(const not_copy_or_move&) -> not_copy_or_move& = delete;
        auto operator=(not_copy_or_move&&) -> not_copy_or_move& = default;
    };

    static_assert(decltype_<int>().satisfies<std::is_integral>(),
                  "hyperion::mpl::Type::satisfies<TypeMetaFunction> test case 1 (failing)");
    static_assert(!decltype_<double>().satisfies<std::is_integral>(),
                  "hyperion::mpl::Type::satisfies<TypeMetaFunction> test case 2 (failing)");

    constexpr auto is_const_type_type = [](MetaType auto type) noexcept {
        return decltype_(type.is_const());
    };

    constexpr auto is_const_type_value = [](MetaType auto type) noexcept {
        return type.is_const();
    };

    constexpr auto is_true_value_type
        = [](MetaValue auto value) noexcept -> Type<Value<decltype(value)::value, bool>> {
        return {};
    };

    constexpr auto is_true_value_value
        = [](MetaValue auto value) noexcept -> Value<decltype(value)::value, bool> {
        return {};
    };

    static_assert(decltype_<const int>().satisfies(is_const_type_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<type>) -> Type<Value> "
                  "test case 1 (failing)");
    static_assert(!decltype_<int>().satisfies(is_const_type_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<type>) -> Type<Value> "
                  "test case 2 (failing)");

    static_assert(decltype_<const int>().satisfies(is_const_type_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<type>) -> Value test "
                  "case 1 (failing)");
    static_assert(!decltype_<int>().satisfies(is_const_type_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<type>) -> Value test "
                  "case 2 (failing)");

    static_assert(decltype_(decltype_<const int>()).satisfies(is_const_type_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Type<type>>) -> Type<Value> "
                  "test case 1 (failing)");
    static_assert(!decltype_(decltype_<int>()).satisfies(is_const_type_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<type<type>>) -> Type<Value> "
                  "test case 2 (failing)");

    static_assert(decltype_(decltype_<const int>()).satisfies(is_const_type_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Type<type>>) -> Value test "
                  "case 1 (failing)");
    static_assert(!decltype_(decltype_<int>()).satisfies(is_const_type_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Type<type>>) -> Value test "
                  "case 2 (failing)");

    static_assert(decltype_(1_value).satisfies(is_true_value_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Type<Value> "
                  "test case 1 (failing)");
    static_assert(!decltype_(0_value).satisfies(is_true_value_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Type<Value> "
                  "test case 2 (failing)");
    static_assert(not decltype_<int>().satisfies(is_true_value_type),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Type<Value> "
                  "test case 3 (failing)");

    static_assert(decltype_(1_value).satisfies(is_true_value_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Value "
                  "test case 1 (failing)");
    static_assert(!decltype_(0_value).satisfies(is_true_value_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Value "
                  "test case 2 (failing)");
    static_assert(not decltype_<int>().satisfies(is_true_value_value),
                  "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Type<Value> "
                  "test case 3 (failing)");

    template<MetaValue TValue>
    struct add_one {
        static inline constexpr auto value = TValue::value + 1;
    };

    template<MetaValue TValue>
    struct times_two {
        static inline constexpr auto value = TValue::value * 2;
    };

    static_assert(decltype_(1_value).apply<add_one>() == 2,
                  "hyperion::mpl::Type::apply<MetaFunction<MetaValue>> test case 1 (failing)");
    static_assert(decltype_(2_value).apply<times_two>() == 4_value,
                  "hyperion::mpl::Type::apply<MetaFunction<MetaValue>> test case 3 (failing)");
    static_assert(decltype_(2_value).apply<times_two>().apply<add_one>().apply<times_two>()
                      == 10_value,
                  "hyperion::mpl::Type::apply<MetaFunction<MetaValue>> test case 3 (failing)");

    constexpr auto add1 = [](MetaValue auto value) {
        return value + 1_value;
    };

    constexpr auto times2 = [](MetaValue auto value) {
        return value * 2_value;
    };

    static_assert(decltype_(1_value).apply(add1) == 2,
                  "hyperion::mpl::Type::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                  "1 (failing)");
    static_assert(decltype_(2_value).apply(times2) == 4,
                  "hyperion::mpl::Type::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                  "2 (failing)");
    static_assert(decltype_(2_value).apply(times2).apply(add1).apply(times2) == 10_value,
                  "hyperion::mpl::Type::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                  "3 (failing)");

    constexpr auto add1_semi_typely = [](MetaType auto type)
        requires MetaValue<typename decltype(type)::type>
    {
        return type.inner() + 1_value;
    };

    constexpr auto times2_semi_typely = [](MetaType auto type)
        requires MetaValue<typename decltype(type)::type>
    {
        return type.inner() * 2_value;
    };

    static_assert(decltype_(1_value).apply(add1_semi_typely) == 2,
                  "hyperion::mpl::Type::apply(MetaFunction(Type)) -> MetaValue test case "
                  "1 (failing)");
    static_assert(decltype_(2_value).apply(times2_semi_typely) == 4,
                  "hyperion::mpl::Type::apply(MetaFunction(Type)) -> MetaValue test case "
                  "2 (failing)");
    static_assert(decltype_(2_value).apply(times2_semi_typely).apply(add1).apply(times2)
                      == 10_value,
                  "hyperion::mpl::Type::apply(MetaFunction(Type)) -> MetaValue test case "
                  "3 (failing)");

    constexpr auto add1_fully_typely = [](MetaType auto type)
        requires MetaValue<typename decltype(type)::type>
    {
        return decltype_(type.inner() + 1_value);
    };

    constexpr auto times2_fully_typely = [](MetaType auto type)
        requires MetaValue<typename decltype(type)::type>
    {
        return decltype_(type.inner() * 2_value);
    };

    static_assert(decltype_(1_value).apply(add1_fully_typely).inner() == 2,
                  "hyperion::mpl::Type::apply(MetaFunction(Type)) -> Type<MetaValue> test case "
                  "1 (failing)");
    static_assert(decltype_(2_value).apply(times2_fully_typely).inner() == 4,
                  "hyperion::mpl::Type::apply(MetaFunction(Type)) -> Type<MetaValue> test case "
                  "2 (failing)");
    static_assert(decltype_(2_value)
                          .apply(times2_fully_typely)
                          .apply(add1_fully_typely)
                          .apply(times2_fully_typely)
                          .inner()
                      == 10_value,
                  "hyperion::mpl::Type::apply(MetaFunction(Type)) -> Type<MetaValue> test case "
                  "3 (failing)");

    static_assert(decltype_(1).apply<std::add_lvalue_reference>() == Type<int&>{},
                  "hyperion::mpl::Type::apply<MetaFunction<type>> test case 1 (failing)");
    static_assert(decltype_(1).apply<std::add_const>() == Type<const int>{},
                  "hyperion::mpl::Type::apply<MetaFunction<type>> test case 2 (failing)");
    static_assert(decltype_(test_val)
                          .apply<std::remove_reference>()
                          .apply<std::remove_const>()
                          .apply<std::add_rvalue_reference>()
                      == decltype_<int&&>(),
                  "hyperion::mpl::Type::apply<MetaFunction<type>> test case 3 (failing)");

    constexpr auto add_const
        = [](MetaType auto type) -> std::add_const<typename decltype(type)::type> {
        return {};
    };

    constexpr auto add_lvalue_reference
        = [](MetaType auto type) -> std::add_lvalue_reference<typename decltype(type)::type> {
        return {};
    };

    constexpr auto remove_reference
        = [](MetaType auto type) -> std::remove_reference<typename decltype(type)::type> {
        return {};
    };

    static_assert(decltype_<int>().apply(add_const) == decltype_<const int>(),
                  "hyperion::mpl::Type::apply(MetaFunction(Type<type>)) -> Type<type> test "
                  "case 1 (failing)");
    static_assert(decltype_<int>().apply(add_const).apply(add_lvalue_reference)
                      == decltype_<const int&>(),
                  "hyperion::mpl::Type::apply(MetaFunction(Type<type>)) -> Type<type> test "
                  "case 2 (failing)");
    static_assert(
        decltype_<int&&>().apply(remove_reference).apply(add_const).apply(add_lvalue_reference)
            == decltype_<const int&>(),
        "hyperion::mpl::Type::apply(MetaFunction(Type<type>)) -> Type<type> test case 3 "
        "(failing)");

    static_assert(
        decltype_(decltype_<int>()).apply(add_const) == decltype_<const int>(),
        "hyperion::mpl::Type::apply(MetaFunction(type)) -> Type<type> test case 1 (failing)");
    static_assert(
        decltype_(decltype_<int>()).apply(add_const).apply(add_lvalue_reference)
            == decltype_<const int&>(),
        "hyperion::mpl::Type::apply(MetaFunction(type)) -> Type<type> test case 2 (failing)");
    static_assert(
        decltype_(decltype_<int&&>())
                .apply(remove_reference)
                .apply(add_const)
                .apply(add_lvalue_reference)
            == decltype_<const int&>(),
        "hyperion::mpl::Type::apply(MetaFunction(type)) -> Type<type> test case 3 (failing)");

    constexpr auto add_const_typed = [](MetaType auto type) {
        return type.as_const();
    };

    constexpr auto add_lvalue_reference_typed = [](MetaType auto type) {
        return type.as_lvalue_reference();
    };

    constexpr auto remove_reference_typed
        = [](MetaType auto type) -> Type<std::remove_reference_t<typename decltype(type)::type>> {
        return {};
    };

    static_assert(decltype_<int>().apply(add_const_typed) == decltype_<const int>(),
                  "hyperion::mpl::Type::apply(MetaFunction(Type<type>)) -> Type<type> test "
                  "case 1 (failing)");
    static_assert(decltype_<int>().apply(add_const_typed).apply(add_lvalue_reference_typed)
                      == decltype_<const int&>(),
                  "hyperion::mpl::Type::apply(MetaFunction(Type<type>)) -> Type<type> test "
                  "case 2 (failing)");
    static_assert(decltype_<int&&>()
                          .apply(remove_reference_typed)
                          .apply(add_const_typed)
                          .apply(add_lvalue_reference_typed)
                      == decltype_<const int&>(),
                  "hyperion::mpl::Type::apply(MetaFunction(Type<type>)) -> Type<type> test "
                  "case 3 (failing)");

    static_assert(
        decltype_(decltype_<int>()).apply(add_const_typed) == decltype_<const int>(),
        "hyperion::mpl::Type::apply(MetaFunction(type)) -> Type<type> test case 1 (failing)");
    static_assert(
        decltype_(decltype_<int>()).apply(add_const_typed).apply(add_lvalue_reference_typed)
            == decltype_<const int&>(),
        "hyperion::mpl::Type::apply(MetaFunction(type)) -> Type<type> test case 2 (failing)");
    static_assert(
        decltype_(decltype_<int&&>())
                .apply(remove_reference_typed)
                .apply(add_const_typed)
                .apply(add_lvalue_reference_typed)
            == decltype_<const int&>(),
        "hyperion::mpl::Type::apply(MetaFunction(type)) -> Type<type> test case 3 (failing)");

    static_assert(decltype_<int>().is(decltype_<int>()),
                  "hyperion::mpl::Type::is test case 1 (failing)");
    static_assert(!decltype_<int>().is(decltype_<double>()),
                  "hyperion::mpl::Type::is test case 2 (failing)");
    static_assert(!decltype_<int>().is(decltype_<const int>()),
                  "hyperion::mpl::Type::is test case 2 (failing)");

    static_assert(decltype_<int>().is_qualification_of(decltype_<int>()),
                  "hyperion::mpl::Type::is_qualification_of test case 1 (failing)");
    static_assert(decltype_<const int>().is_qualification_of(decltype_<int>()),
                  "hyperion::mpl::Type::is_qualification_of test case 2 (failing)");
    static_assert(decltype_<int&>().is_qualification_of(decltype_<int>()),
                  "hyperion::mpl::Type::is_qualification_of test case 3 (failing)");
    static_assert(!decltype_<int>().is_qualification_of(decltype_<double>()),
                  "hyperion::mpl::Type::is_qualification_of test case 4 (failing)");
    static_assert(!decltype_<double>().is_qualification_of(decltype_<int>()),
                  "hyperion::mpl::Type::is_qualification_of test case 5 (failing)");

    static_assert(decltype_<const int>().is_const(),
                  "hyperion::mpl::Type::is_const test case 1 (failing)");
    static_assert(decltype_<const int&>().is_const(),
                  "hyperion::mpl::Type::is_const test case 2 (failing)");
    static_assert(!decltype_<int>().is_const(),
                  "hyperion::mpl::Type::is_const test case 3 (failing)");

    static_assert(decltype_<int&>().is_lvalue_reference(),
                  "hyperion::mpl::Type::is_lvalue_reference test case 1 (failing)");
    static_assert(decltype_<const int&>().is_lvalue_reference(),
                  "hyperion::mpl::Type::is_lvalue_reference test case 2 (failing)");
    static_assert(!decltype_<int>().is_lvalue_reference(),
                  "hyperion::mpl::Type::is_lvalue_reference test case 3 (failing)");

    static_assert(decltype_<volatile int>().is_volatile(),
                  "hyperion::mpl::Type::is_volatile test case 1 (failing)");
    static_assert(decltype_<volatile int&>().is_volatile(),
                  "hyperion::mpl::Type::is_volatile test case 2 (failing)");
    static_assert(!decltype_<int>().is_volatile(),
                  "hyperion::mpl::Type::is_volatile test case 3 (failing)");

    struct empty { };

    static_assert(decltype_<empty>().is_empty(),
                  "hyperion::mpl::Type::is_empty test case 1 (failing)");
    static_assert(not decltype_<int>().is_empty(),
                  "hyperion::mpl::Type::is_empty test case 2 (failing)");

    struct not_trivial {
        not_trivial(const not_trivial&);
    };
    static_assert(decltype_<empty>().is_trivial(),
                  "hyperion::mpl::Type::is_trivial test case 1 (failing)");
    static_assert(decltype_<int>().is_trivial(),
                  "hyperion::mpl::Type::is_trivial test case 2 (failing)");
    static_assert(not decltype_<not_trivial>().is_trivial(),
                  "hyperion::mpl::Type::is_trivial test case 3 (failing)");

    static_assert(decltype_<int&&>().is_rvalue_reference(),
                  "hyperion::mpl::Type::is_rvalue_reference test case 1 (failing)");
    static_assert(decltype_<const int&&>().is_rvalue_reference(),
                  "hyperion::mpl::Type::is_rvalue_reference test case 2 (failing)");
    static_assert(!decltype_<int>().is_rvalue_reference(),
                  "hyperion::mpl::Type::is_rvalue_reference test case 3 (failing)");

    static_assert(decltype_<int&>().is_reference(),
                  "hyperion::mpl::Type::is_reference test case 1 (failing)");
    static_assert(decltype_<const int&>().is_reference(),
                  "hyperion::mpl::Type::is_reference test case 2 (failing)");
    static_assert(decltype_<int&&>().is_reference(),
                  "hyperion::mpl::Type::is_reference test case 3 (failing)");
    static_assert(decltype_<const int&&>().is_reference(),
                  "hyperion::mpl::Type::is_reference test case 4 (failing)");
    static_assert(not decltype_<int>().is_reference(),
                  "hyperion::mpl::Type::is_reference test case 5 (failing)");

    static_assert(not decltype_<int>().is_pointer(),
                  "hyperion::mpl::Type::is_pointer test case 1 (failing)");
    static_assert(decltype_<int*>().is_pointer(),
                  "hyperion::mpl::Type::is_pointer test case 2 (failing)");
    static_assert(decltype_<int* const>().is_pointer(),
                  "hyperion::mpl::Type::is_pointer test case 3 (failing)");
    static_assert(decltype_<const int*>().is_pointer(),
                  "hyperion::mpl::Type::is_pointer test case 4 (failing)");
    static_assert(decltype_<const int* const>().is_pointer(),
                  "hyperion::mpl::Type::is_pointer test case 5 (failing)");

    static_assert(decltype_<int>().as_const() == decltype_<const int>(),
                  "hyperion::mpl::Type::as_const test case 1 (failing)");
    static_assert(decltype_<int&>().as_const() == decltype_<const int&>(),
                  "hyperion::mpl::Type::as_const test case 2 (failing)");
    static_assert(decltype_<int>().as_const() != decltype_<int>(),
                  "hyperion::mpl::Type::as_const test case 3 (failing)");

    static_assert(decltype_<int>().as_lvalue_reference() == decltype_<int&>(),
                  "hyperion::mpl::Type::as_lvalue_reference test case 1 (failing)");
    static_assert(decltype_<const int>().as_lvalue_reference() == decltype_<const int&>(),
                  "hyperion::mpl::Type::as_lvalue_reference test case 2 (failing)");
    static_assert(decltype_<int>().as_lvalue_reference() != decltype_<int>(),
                  "hyperion::mpl::Type::as_lvalue_reference test case 3 (failing)");

    static_assert(decltype_<int>().as_volatile() == decltype_<volatile int>(),
                  "hyperion::mpl::Type::as_volatile test case 1 (failing)");
    static_assert(decltype_<int&>().as_volatile() == decltype_<volatile int&>(),
                  "hyperion::mpl::Type::as_volatile test case 2 (failing)");
    static_assert(decltype_<int>().as_volatile() != decltype_<int>(),
                  "hyperion::mpl::Type::as_volatile test case 3 (failing)");

    static_assert(decltype_<int>().as_rvalue_reference() == decltype_<int&&>(),
                  "hyperion::mpl::Type::as_rvalue_reference test case 1 (failing)");
    static_assert(decltype_<const int>().as_rvalue_reference() == decltype_<const int&&>(),
                  "hyperion::mpl::Type::as_rvalue_reference test case 2 (failing)");
    static_assert(decltype_<int>().as_rvalue_reference() != decltype_<int>(),
                  "hyperion::mpl::Type::as_rvalue_reference test case 3 (failing)");

    static_assert(decltype_<int>().as_pointer() == decltype_<int*>(),
                  "hyperion::mpl::Type::as_pointer test case 1 (failing)");
    static_assert(decltype_<const int>().as_pointer() == decltype_<const int*>(),
                  "hyperion::mpl::Type::as_pointer test case 2 (failing)");
    static_assert(decltype_<int&>().as_pointer() == decltype_<int*>(),
                  "hyperion::mpl::Type::as_pointer test case 3 (failing)");
    static_assert(decltype_<int*>().as_pointer() == decltype_<int**>(),
                  "hyperion::mpl::Type::as_pointer test case 4 (failing)");

    static_assert(decltype_<int>().remove_lvalue_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_lvalue_reference test case 1 (failing)");
    static_assert(decltype_<const int>().remove_lvalue_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_lvalue_reference test case 2 (failing)");
    static_assert(decltype_<int&>().remove_lvalue_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_lvalue_reference test case 3 (failing)");
    static_assert(decltype_<const int&>().remove_lvalue_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_lvalue_reference test case 4 (failing)");
    static_assert(decltype_<int&&>().remove_lvalue_reference() == decltype_<int&&>(),
                  "hyperion::mpl::Type::remove_lvalue_reference test case 5 (failing)");
    static_assert(decltype_<const int&&>().remove_lvalue_reference() == decltype_<const int&&>(),
                  "hyperion::mpl::Type::remove_lvalue_reference test case 6 (failing)");

    static_assert(decltype_<int>().remove_rvalue_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_rvalue_reference test case 1 (failing)");
    static_assert(decltype_<const int>().remove_rvalue_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_rvalue_reference test case 2 (failing)");
    static_assert(decltype_<int&>().remove_rvalue_reference() == decltype_<int&>(),
                  "hyperion::mpl::Type::remove_rvalue_reference test case 3 (failing)");
    static_assert(decltype_<const int&>().remove_rvalue_reference() == decltype_<const int&>(),
                  "hyperion::mpl::Type::remove_rvalue_reference test case 4 (failing)");
    static_assert(decltype_<int&&>().remove_rvalue_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_rvalue_reference test case 5 (failing)");
    static_assert(decltype_<const int&&>().remove_rvalue_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_rvalue_reference test case 6 (failing)");

    static_assert(decltype_<int>().remove_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_reference  test case 1 (failing)");
    static_assert(decltype_<const int>().remove_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_reference  test case 2 (failing)");
    static_assert(decltype_<int&>().remove_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_reference  test case 3 (failing)");
    static_assert(decltype_<const int&>().remove_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_reference  test case 4 (failing)");
    static_assert(decltype_<int&&>().remove_reference() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_reference  test case 5 (failing)");
    static_assert(decltype_<const int&&>().remove_reference() == decltype_<const int>(),
                  "hyperion::mpl::Type::remove_reference  test case 6 (failing)");

    static_assert(decltype_<int>().remove_const() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_const  test case 1 (failing)");
    static_assert(decltype_<const int>().remove_const() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_const  test case 2 (failing)");
    static_assert(decltype_<int&>().remove_const() == decltype_<int&>(),
                  "hyperion::mpl::Type::remove_const  test case 3 (failing)");
    static_assert(decltype_<const int&>().remove_const() == decltype_<int&>(),
                  "hyperion::mpl::Type::remove_const  test case 4 (failing)");
    static_assert(decltype_<int&&>().remove_const() == decltype_<int&&>(),
                  "hyperion::mpl::Type::remove_const  test case 5 (failing)");
    static_assert(decltype_<const int&&>().remove_const() == decltype_<int&&>(),
                  "hyperion::mpl::Type::remove_const  test case 6 (failing)");
    static_assert(decltype_<const int*>().remove_const() == decltype_<const int*>(),
                  "hyperion::mpl::Type::remove_const  test case 7 (failing)");
    static_assert(decltype_<const int* const>().remove_const() == decltype_<const int*>(),
                  "hyperion::mpl::Type::remove_const  test case 8 (failing)");

    static_assert(decltype_<int>().remove_volatile() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_volatile  test case 1 (failing)");
    static_assert(decltype_<volatile int>().remove_volatile() == decltype_<int>(),
                  "hyperion::mpl::Type::remove_volatile  test case 2 (failing)");
    static_assert(decltype_<int&>().remove_volatile() == decltype_<int&>(),
                  "hyperion::mpl::Type::remove_volatile  test case 3 (failing)");
    static_assert(decltype_<volatile int&>().remove_volatile() == decltype_<int&>(),
                  "hyperion::mpl::Type::remove_volatile  test case 4 (failing)");
    static_assert(decltype_<int&&>().remove_volatile() == decltype_<int&&>(),
                  "hyperion::mpl::Type::remove_volatile  test case 5 (failing)");
    static_assert(decltype_<volatile int&&>().remove_volatile() == decltype_<int&&>(),
                  "hyperion::mpl::Type::remove_volatile  test case 6 (failing)");
    static_assert(decltype_<volatile int*>().remove_volatile() == decltype_<volatile int*>(),
                  "hyperion::mpl::Type::remove_volatile  test case 7 (failing)");
    static_assert(decltype_<volatile int* volatile>().remove_volatile()
                      == decltype_<volatile int*>(),
                  "hyperion::mpl::Type::remove_volatile  test case 8 (failing)");

    static_assert(decltype_<int>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 1 (failing)");
    static_assert(decltype_<const int>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 2 (failing)");
    static_assert(decltype_<int&>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 3 (failing)");
    static_assert(decltype_<const int&>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 4 (failing)");
    static_assert(decltype_<int&&>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 5 (failing)");
    static_assert(decltype_<const int&&>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 6 (failing)");
    static_assert(decltype_<const int*>().unqualified() == decltype_<const int*>(),
                  "hyperion::mpl::Type::unqualified  test case 7 (failing)");
    static_assert(decltype_<const int* const>().unqualified() == decltype_<const int*>(),
                  "hyperion::mpl::Type::unqualified  test case 8 (failing)");
    static_assert(decltype_<volatile int>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 9 (failing)");
    static_assert(decltype_<volatile int&>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 10 (failing)");
    static_assert(decltype_<volatile int&&>().unqualified() == decltype_<int>(),
                  "hyperion::mpl::Type::unqualified  test case 11 (failing)");
    static_assert(decltype_<volatile int*>().unqualified() == decltype_<volatile int*>(),
                  "hyperion::mpl::Type::unqualified  test case 12 (failing)");
    static_assert(decltype_<volatile int* volatile>().unqualified() == decltype_<volatile int*>(),
                  "hyperion::mpl::Type::unqualified  test case 13 (failing)");

    struct not_convertible { };

    static_assert(decltype_<int>().is_convertible_to(decltype_<double>()),
                  "hyperion::mpl::Type::is_convertible_to test case 1 (failing)");
    static_assert(decltype_<float>().is_convertible_to(decltype_<double>()),
                  "hyperion::mpl::Type::is_convertible_to test case 2 (failing)");
    static_assert(!decltype_<int>().is_convertible_to(decltype_<not_convertible>()),
                  "hyperion::mpl::Type::is_convertible_to test case 3 (failing)");

    struct base_type { };

    struct derived_type : base_type { };

    static_assert(decltype_<derived_type>().is_derived_from(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_derived_from test case 1 (failing)");
    static_assert(!decltype_<int>().is_derived_from(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_derived_from test case 2 (failing)");
    static_assert(!decltype_<base_type>().is_derived_from(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_derived_from test case 3 (failing)");
    static_assert(!decltype_<base_type>().is_derived_from(decltype_<derived_type>()),
                  "hyperion::mpl::Type::is_derived_from test case 4 (failing)");

    static_assert(decltype_<base_type>().is_base_of(decltype_<derived_type>()),
                  "hyperion::mpl::Type::is_base_of test case 1 (failing)");
    static_assert(!decltype_<int>().is_base_of(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_base_of test case 2 (failing)");
    static_assert(!decltype_<base_type>().is_base_of(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_base_of test case 3 (failing)");
    static_assert(!decltype_<derived_type>().is_base_of(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_base_of test case 4 (failing)");

    struct constructible_from_int {
        explicit constructible_from_int(int) noexcept;
    };

    struct constructible_from_anything {
        template<typename TType>
            requires(!bool(
                decltype_<TType>().is_qualification_of(decltype_<constructible_from_anything>())))
        explicit constructible_from_anything(TType&&);

        template<typename... TTypes>
            requires(sizeof...(TTypes) > 1)
        explicit constructible_from_anything(TTypes&&...) noexcept;
    };

    template<typename... TTypes>
    struct test_list { };

    static_assert(decltype_<constructible_from_int>().is_constructible_from(decltype_<int>()),
                  "hyperion::mpl::Type::is_constructible_from(MetaType) test case 1 (failing)");
    static_assert(decltype_<constructible_from_int>().is_constructible_from(decltype_<double>()),
                  "hyperion::mpl::Type::is_constructible_from(MetaType) test case 2 (failing)");
    static_assert(!decltype_<constructible_from_int>().is_constructible_from(decltype_<void*>()),
                  "hyperion::mpl::Type::is_constructible_from(MetaType) test case 3 (failing)");
    static_assert(decltype_<constructible_from_anything>().is_constructible_from(decltype_<int>()),
                  "hyperion::mpl::Type::is_constructible_from(MetaType) test case 4 (failing)");
    static_assert(
        decltype_<constructible_from_anything>().is_constructible_from(decltype_<void*>()),
        "hyperion::mpl::Type::is_constructible_from(MetaType) test case 5 (failing)");

    static_assert(decltype_<constructible_from_int>().is_constructible_from(test_list<int>{}),
                  "hyperion::mpl::Type::is_constructible_from(List) test case 1 (failing)");
    static_assert(decltype_<constructible_from_int>().is_constructible_from(test_list<double>{}),
                  "hyperion::mpl::Type::is_constructible_from(List) test case 2 (failing)");
    static_assert(!decltype_<constructible_from_int>().is_constructible_from(test_list<void*>{}),
                  "hyperion::mpl::Type::is_constructible_from(List) test case 3 (failing)");
    static_assert(
        !decltype_<constructible_from_int>().is_constructible_from(test_list<int, double>{}),
        "hyperion::mpl::Type::is_constructible_from(List) test case 4 (failing)");
    static_assert(
        decltype_<constructible_from_anything>().is_constructible_from(test_list<void*>{}),
        "hyperion::mpl::Type::is_constructible_from(List) test case 5 (failing)");
    static_assert(
        decltype_<constructible_from_anything>().is_constructible_from(test_list<int, double>{}),
        "hyperion::mpl::Type::is_constructible_from(List) test case 6 (failing)");

    static_assert(
        decltype_<constructible_from_int>().is_noexcept_constructible_from(decltype_<int>()),
        "hyperion::mpl::Type::is_noexcept_constructible_from(MetaType) test case 1 (failing)");
    static_assert(
        decltype_<constructible_from_int>().is_noexcept_constructible_from(decltype_<double>()),
        "hyperion::mpl::Type::is_noexcept_constructible_from(MetaType) test case 2 (failing)");
    static_assert(
        !decltype_<constructible_from_int>().is_noexcept_constructible_from(decltype_<void*>()),
        "hyperion::mpl::Type::is_noexcept_constructible_from(MetaType) test case 3 (failing)");
    static_assert(
        !decltype_<constructible_from_anything>().is_noexcept_constructible_from(decltype_<int>()),
        "hyperion::mpl::Type::is_noexcept_constructible_from(MetaType) test case 4 (failing)");
    static_assert(
        !decltype_<constructible_from_anything>().is_noexcept_constructible_from(
            decltype_<void*>()),
        "hyperion::mpl::Type::is_noexcept_constructible_from(MetaType) test case 5 (failing)");

    static_assert(
        decltype_<constructible_from_int>().is_noexcept_constructible_from(test_list<int>{}),
        "hyperion::mpl::Type::is_noexcept_constructible_from(List) test case 1 (failing)");
    static_assert(
        decltype_<constructible_from_int>().is_noexcept_constructible_from(test_list<double>{}),
        "hyperion::mpl::Type::is_noexcept_constructible_from(List) test case 2 (failing)");
    static_assert(
        !decltype_<constructible_from_int>().is_noexcept_constructible_from(test_list<void*>{}),
        "hyperion::mpl::Type::is_noexcept_constructible_from(List) test case 3 (failing)");
    static_assert(
        !decltype_<constructible_from_int>().is_noexcept_constructible_from(
            test_list<int, double>{}),
        "hyperion::mpl::Type::is_noexcept_constructible_from(List) test case 4 (failing)");
    static_assert(
        !decltype_<constructible_from_anything>().is_noexcept_constructible_from(
            test_list<void*>{}),
        "hyperion::mpl::Type::is_noexcept_constructible_from(List) test case 5 (failing)");
    static_assert(
        decltype_<constructible_from_anything>().is_noexcept_constructible_from(
            test_list<int, double>{}),
        "hyperion::mpl::Type::is_noexcept_constructible_from(List) test case 6 (failing)");

    struct not_default_constructible {
        not_default_constructible() = delete;
    };

    struct default_constructible {
        default_constructible() = default;
    };

    struct noexcept_default_constructible {
        noexcept_default_constructible() noexcept = default;
    };

    struct not_trivially_default_constructible {
        [[noreturn]] not_trivially_default_constructible() noexcept(false) {
            // NOLINTNEXTLINE(hicpp-exception-baseclass)
            throw 0;
        }
    };

    static_assert(decltype_<int>().is_default_constructible(),
                  "hyperion::mpl::Type::is_default_constructible test case 1 (failing)");
    static_assert(decltype_<default_constructible>().is_default_constructible(),
                  "hyperion::mpl::Type::is_default_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_default_constructible>().is_default_constructible(),
                  "hyperion::mpl::Type::is_default_constructible test case 3 (failing)");
    static_assert(!decltype_<not_default_constructible>().is_default_constructible(),
                  "hyperion::mpl::Type::is_default_constructible test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_default_constructible(),
                  "hyperion::mpl::Type::is_noexcept_default_constructible test case 1 (failing)");
    static_assert(
        !decltype_<not_trivially_default_constructible>().is_noexcept_default_constructible(),
        "hyperion::mpl::Type::is_noexcept_default_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_default_constructible>().is_noexcept_default_constructible(),
                  "hyperion::mpl::Type::is_noexcept_default_constructible test case 3 (failing)");
    static_assert(!decltype_<not_default_constructible>().is_noexcept_default_constructible(),
                  "hyperion::mpl::Type::is_noexcept_default_constructible test case 4 (failing)");

    static_assert(decltype_<int>().is_trivially_default_constructible(),
                  "hyperion::mpl::Type::is_trivially_default_constructible test case 1 (failing)");
    static_assert(decltype_<default_constructible>().is_trivially_default_constructible(),
                  "hyperion::mpl::Type::is_trivially_default_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_default_constructible>().is_trivially_default_constructible(),
                  "hyperion::mpl::Type::is_trivially_default_constructible test case 3 (failing)");
    static_assert(!decltype_<not_default_constructible>().is_trivially_default_constructible(),
                  "hyperion::mpl::Type::is_trivially_default_constructible test case 4 (failing)");
    static_assert(
        !decltype_<not_trivially_default_constructible>().is_trivially_default_constructible(),
        "hyperion::mpl::Type::is_trivially_default_constructible test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_copy_constructible {
        not_copy_constructible(const not_copy_constructible&) = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct copy_constructible {
        copy_constructible(const copy_constructible&) = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_copy_constructible {
        noexcept_copy_constructible(const noexcept_copy_constructible&) noexcept = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_trivially_copy_constructible {
        [[noreturn]] not_trivially_copy_constructible(
            [[maybe_unused]] const not_trivially_copy_constructible& copy) noexcept(false) {
            // NOLINTNEXTLINE(hicpp-exception-baseclass)
            throw 0;
        }
    };

    static_assert(decltype_<int>().is_copy_constructible(),
                  "hyperion::mpl::Type::is_copy_constructible test case 1 (failing)");
    static_assert(decltype_<copy_constructible>().is_copy_constructible(),
                  "hyperion::mpl::Type::is_copy_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_constructible>().is_copy_constructible(),
                  "hyperion::mpl::Type::is_copy_constructible test case 3 (failing)");
    static_assert(!decltype_<not_copy_constructible>().is_copy_constructible(),
                  "hyperion::mpl::Type::is_copy_constructible test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_copy_constructible(),
                  "hyperion::mpl::Type::is_noexcept_copy_constructible test case 1 (failing)");
    static_assert(!decltype_<not_trivially_copy_constructible>().is_noexcept_copy_constructible(),
                  "hyperion::mpl::Type::is_noexcept_copy_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_constructible>().is_noexcept_copy_constructible(),
                  "hyperion::mpl::Type::is_noexcept_copy_constructible test case 3 (failing)");
    static_assert(!decltype_<not_copy_constructible>().is_noexcept_copy_constructible(),
                  "hyperion::mpl::Type::is_noexcept_copy_constructible test case 4 (failing)");

    static_assert(decltype_<int>().is_trivially_copy_constructible(),
                  "hyperion::mpl::Type::is_trivially_copy_constructible test case 1 (failing)");
    static_assert(decltype_<copy_constructible>().is_trivially_copy_constructible(),
                  "hyperion::mpl::Type::is_trivially_copy_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_constructible>().is_trivially_copy_constructible(),
                  "hyperion::mpl::Type::is_trivially_copy_constructible test case 3 (failing)");
    static_assert(!decltype_<not_copy_constructible>().is_trivially_copy_constructible(),
                  "hyperion::mpl::Type::is_trivially_copy_constructible test case 4 (failing)");
    static_assert(!decltype_<not_trivially_copy_constructible>().is_trivially_copy_constructible(),
                  "hyperion::mpl::Type::is_trivially_copy_constructible test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_move_constructible {
        not_move_constructible(not_move_constructible&&) = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct move_constructible {
        move_constructible(move_constructible&&) = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_move_constructible {
        noexcept_move_constructible(noexcept_move_constructible&&) noexcept = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_trivially_move_constructible {
        [[noreturn]] not_trivially_move_constructible(
            [[maybe_unused]] not_trivially_move_constructible&& move) noexcept(false) {
            // NOLINTNEXTLINE(hicpp-exception-baseclass)
            throw 0;
        }
    };

    static_assert(decltype_<int>().is_move_constructible(),
                  "hyperion::mpl::Type::is_move_constructible test case 1 (failing)");
    static_assert(decltype_<move_constructible>().is_move_constructible(),
                  "hyperion::mpl::Type::is_move_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_move_constructible>().is_move_constructible(),
                  "hyperion::mpl::Type::is_move_constructible test case 3 (failing)");
    static_assert(!decltype_<not_move_constructible>().is_move_constructible(),
                  "hyperion::mpl::Type::is_move_constructible test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_move_constructible(),
                  "hyperion::mpl::Type::is_noexcept_move_constructible test case 1 (failing)");
    static_assert(!decltype_<not_trivially_move_constructible>().is_noexcept_move_constructible(),
                  "hyperion::mpl::Type::is_noexcept_move_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_move_constructible>().is_noexcept_move_constructible(),
                  "hyperion::mpl::Type::is_noexcept_move_constructible test case 3 (failing)");
    static_assert(!decltype_<not_move_constructible>().is_noexcept_move_constructible(),
                  "hyperion::mpl::Type::is_noexcept_move_constructible test case 4 (failing)");

    static_assert(decltype_<int>().is_trivially_move_constructible(),
                  "hyperion::mpl::Type::is_trivially_move_constructible test case 1 (failing)");
    static_assert(decltype_<move_constructible>().is_trivially_move_constructible(),
                  "hyperion::mpl::Type::is_trivially_move_constructible test case 2 (failing)");
    static_assert(decltype_<noexcept_move_constructible>().is_trivially_move_constructible(),
                  "hyperion::mpl::Type::is_trivially_move_constructible test case 3 (failing)");
    static_assert(!decltype_<not_move_constructible>().is_trivially_move_constructible(),
                  "hyperion::mpl::Type::is_trivially_move_constructible test case 4 (failing)");
    static_assert(!decltype_<not_trivially_move_constructible>().is_trivially_move_constructible(),
                  "hyperion::mpl::Type::is_trivially_move_constructible test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_copy_assignable {
        auto operator=(const not_copy_assignable&) -> not_copy_assignable& = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct copy_assignable {
        auto operator=(const copy_assignable&) -> copy_assignable& = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_copy_assignable {
        auto
        operator=(const noexcept_copy_assignable&) noexcept -> noexcept_copy_assignable& = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_trivially_copy_assignable {
        auto operator=([[maybe_unused]] const not_trivially_copy_assignable& copy) noexcept(false)
            -> not_trivially_copy_assignable& {
            if(this == &copy) {
                // NOLINTNEXTLINE(hicpp-exception-baseclass)
                throw 0;
                HYPERION_UNREACHABLE();
            }

            return *this;
        }
    };

    static_assert(decltype_<int>().is_copy_assignable(),
                  "hyperion::mpl::Type::is_copy_assignable test case 1 (failing)");
    static_assert(decltype_<copy_assignable>().is_copy_assignable(),
                  "hyperion::mpl::Type::is_copy_assignable test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_assignable>().is_copy_assignable(),
                  "hyperion::mpl::Type::is_copy_assignable test case 3 (failing)");
    static_assert(!decltype_<not_copy_assignable>().is_copy_assignable(),
                  "hyperion::mpl::Type::is_copy_assignable test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_copy_assignable(),
                  "hyperion::mpl::Type::is_noexcept_copy_assignable test case 1 (failing)");
    static_assert(!decltype_<not_trivially_copy_assignable>().is_noexcept_copy_assignable(),
                  "hyperion::mpl::Type::is_noexcept_copy_assignable test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_assignable>().is_noexcept_copy_assignable(),
                  "hyperion::mpl::Type::is_noexcept_copy_assignable test case 3 (failing)");
    static_assert(!decltype_<not_copy_assignable>().is_noexcept_copy_assignable(),
                  "hyperion::mpl::Type::is_noexcept_copy_assignable test case 4 (failing)");

    static_assert(decltype_<int>().is_trivially_copy_assignable(),
                  "hyperion::mpl::Type::is_trivially_copy_assignable test case 1 (failing)");
    static_assert(decltype_<copy_assignable>().is_trivially_copy_assignable(),
                  "hyperion::mpl::Type::is_trivially_copy_assignable test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_assignable>().is_trivially_copy_assignable(),
                  "hyperion::mpl::Type::is_trivially_copy_assignable test case 3 (failing)");
    static_assert(!decltype_<not_copy_assignable>().is_trivially_copy_assignable(),
                  "hyperion::mpl::Type::is_trivially_copy_assignable test case 4 (failing)");
    static_assert(!decltype_<not_trivially_copy_assignable>().is_trivially_copy_assignable(),
                  "hyperion::mpl::Type::is_trivially_copy_assignable test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_move_assignable {
        auto operator=(not_move_assignable&&) -> not_move_assignable& = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct move_assignable {
        auto operator=(move_assignable&&) -> move_assignable& = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_move_assignable {
        auto operator=(noexcept_move_assignable&&) noexcept -> noexcept_move_assignable& = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_trivially_move_assignable {
        [[noreturn]] auto
        operator=([[maybe_unused]] not_trivially_move_assignable&& move) noexcept(false)
            -> not_trivially_move_assignable& {
            // NOLINTNEXTLINE(hicpp-exception-baseclass)
            throw 0;
        }
    };

    static_assert(decltype_<int>().is_move_assignable(),
                  "hyperion::mpl::Type::is_move_assignable test case 1 (failing)");
    static_assert(decltype_<move_assignable>().is_move_assignable(),
                  "hyperion::mpl::Type::is_move_assignable test case 2 (failing)");
    static_assert(decltype_<noexcept_move_assignable>().is_move_assignable(),
                  "hyperion::mpl::Type::is_move_assignable test case 3 (failing)");
    static_assert(!decltype_<not_move_assignable>().is_move_assignable(),
                  "hyperion::mpl::Type::is_move_assignable test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_move_assignable(),
                  "hyperion::mpl::Type::is_noexcept_move_assignable test case 1 (failing)");
    static_assert(!decltype_<not_trivially_move_assignable>().is_noexcept_move_assignable(),
                  "hyperion::mpl::Type::is_noexcept_move_assignable test case 2 (failing)");
    static_assert(decltype_<noexcept_move_assignable>().is_noexcept_move_assignable(),
                  "hyperion::mpl::Type::is_noexcept_move_assignable test case 3 (failing)");
    static_assert(!decltype_<not_move_assignable>().is_noexcept_move_assignable(),
                  "hyperion::mpl::Type::is_noexcept_move_assignable test case 4 (failing)");

    static_assert(decltype_<int>().is_trivially_move_assignable(),
                  "hyperion::mpl::Type::is_trivially_move_assignable test case 1 (failing)");
    static_assert(decltype_<move_assignable>().is_trivially_move_assignable(),
                  "hyperion::mpl::Type::is_trivially_move_assignable test case 2 (failing)");
    static_assert(decltype_<noexcept_move_assignable>().is_trivially_move_assignable(),
                  "hyperion::mpl::Type::is_trivially_move_assignable test case 3 (failing)");
    static_assert(!decltype_<not_move_assignable>().is_trivially_move_assignable(),
                  "hyperion::mpl::Type::is_trivially_move_assignable test case 4 (failing)");
    static_assert(!decltype_<not_trivially_move_assignable>().is_trivially_move_assignable(),
                  "hyperion::mpl::Type::is_trivially_move_assignable test case 5 (failing)");

    template<typename T>
    struct assignable_from {
        auto operator=(T) -> assignable_from& {
            return *this;
        }
    };

    template<typename T>
    struct noexcept_assignable_from {
        auto operator=(T) -> noexcept_assignable_from& {
            return *this;
        }
    };

    static_assert(decltype_<assignable_from<int>>().is_assignable_from(decltype_<int>()),
                  "hyperion::mpl::Type::is_assignable_from test case 1 (failing)");
    static_assert(
        decltype_<assignable_from<int>>().is_assignable_from(decltype_<assignable_from<int>>()),
        "hyperion::mpl::Type::is_assignable_from test case 2 (failing)");
    static_assert(not decltype_<assignable_from<int>>().is_assignable_from(decltype_<base_type>()),
                  "hyperion::mpl::Type::is_assignable_from test case 3 (failing)");

    static_assert(decltype_<assignable_from<int>>().is_noexcept_assignable_from(
                      decltype_<assignable_from<int>>()),
                  "hyperion::mpl::Type::is_noexcept_assignable_from test case 1 (failing)");
    static_assert(
        not decltype_<assignable_from<int>>().is_noexcept_assignable_from(decltype_<int>()),
        "hyperion::mpl::Type::is_noexcept_assignable_from test case 2 (failing)");
    static_assert(
        not decltype_<assignable_from<int>>().is_noexcept_assignable_from(decltype_<base_type>()),
        "hyperion::mpl::Type::is_noexcept_assignable_from test case 3 (failing)");

    static_assert(decltype_<assignable_from<int>>().is_trivially_assignable_from(
                      decltype_<assignable_from<int>>()),
                  "hyperion::mpl::Type::is_trivially_assignable_from test case 1 (failing)");
    static_assert(
        not decltype_<assignable_from<int>>().is_trivially_assignable_from(decltype_<int>()),
        "hyperion::mpl::Type::is_trivially_assignable_from test case 2 (failing)");
    static_assert(
        not decltype_<assignable_from<int>>().is_trivially_assignable_from(decltype_<base_type>()),
        "hyperion::mpl::Type::is_trivially_assignable_from test case 3 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_destructible {
        ~not_destructible() = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct destructible {
        ~destructible() = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_destructible {
        ~noexcept_destructible() noexcept = default;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_trivially_destructible {
        [[noreturn]] ~not_trivially_destructible() noexcept(false) {
            // NOLINTNEXTLINE(hicpp-exception-baseclass)
            throw 0;
        }
    };

    static_assert(decltype_<int>().is_destructible(),
                  "hyperion::mpl::Type::is_destructible test case 1 (failing)");
    static_assert(decltype_<destructible>().is_destructible(),
                  "hyperion::mpl::Type::is_destructible test case 2 (failing)");
    static_assert(decltype_<noexcept_destructible>().is_destructible(),
                  "hyperion::mpl::Type::is_destructible test case 3 (failing)");
    static_assert(!decltype_<not_destructible>().is_destructible(),
                  "hyperion::mpl::Type::is_destructible test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_destructible(),
                  "hyperion::mpl::Type::is_noexcept_destructible test case 1 (failing)");
    static_assert(!decltype_<not_trivially_destructible>().is_noexcept_destructible(),
                  "hyperion::mpl::Type::is_noexcept_destructible test case 2 (failing)");
    static_assert(decltype_<noexcept_destructible>().is_noexcept_destructible(),
                  "hyperion::mpl::Type::is_noexcept_destructible test case 3 (failing)");
    static_assert(!decltype_<not_destructible>().is_noexcept_destructible(),
                  "hyperion::mpl::Type::is_noexcept_destructible test case 4 (failing)");

    static_assert(decltype_<int>().is_trivially_destructible(),
                  "hyperion::mpl::Type::is_trivially_destructible test case 1 (failing)");
    static_assert(decltype_<destructible>().is_trivially_destructible(),
                  "hyperion::mpl::Type::is_trivially_destructible test case 2 (failing)");
    static_assert(decltype_<noexcept_destructible>().is_trivially_destructible(),
                  "hyperion::mpl::Type::is_trivially_destructible test case 3 (failing)");
    static_assert(!decltype_<not_destructible>().is_trivially_destructible(),
                  "hyperion::mpl::Type::is_trivially_destructible test case 4 (failing)");
    static_assert(!decltype_<not_trivially_destructible>().is_trivially_destructible(),
                  "hyperion::mpl::Type::is_trivially_destructible test case 5 (failing)");

    struct swappable {
        friend void swap(swappable& lhs, swappable& rhs) noexcept(false);
    };

    struct not_swappable {
        friend void swap(not_swappable& lhs, not_swappable& rhs) = delete;
    };

    struct noexcept_swappable {
        friend void swap(noexcept_swappable& lhs, noexcept_swappable& rhs) noexcept;
    };

    static_assert(decltype_<int>().is_swappable(),
                  "hyperion::mpl::Type::is_swappable test case 1 (failing)");
    static_assert(decltype_<swappable>().is_swappable(),
                  "hyperion::mpl::Type::is_swappable test case 2 (failing)");
    static_assert(decltype_<noexcept_swappable>().is_swappable(),
                  "hyperion::mpl::Type::is_swappable test case 3 (failing)");
    static_assert(!decltype_<not_swappable>().is_swappable(),
                  "hyperion::mpl::Type::is_swappable test case 4 (failing)");

    static_assert(decltype_<int>().is_noexcept_swappable(),
                  "hyperion::mpl::Type::is_noexcept_swappable test case 1 (failing)");
    static_assert(decltype_<noexcept_swappable>().is_noexcept_swappable(),
                  "hyperion::mpl::Type::is_noexcept_swappable test case 2 (failing)");
    static_assert(!decltype_<swappable>().is_noexcept_swappable(),
                  "hyperion::mpl::Type::is_noexcept_swappable test case 3 (failing)");
    static_assert(!decltype_<not_swappable>().is_noexcept_swappable(),
                  "hyperion::mpl::Type::is_noexcept_swappable test case 3 (failing)");

    struct swappable_with {
        friend void swap(swappable_with& lhs, int& rhs) noexcept(false);
        friend void swap(int& lhs, swappable_with& rhs) noexcept(false);
    };

    struct noexcept_swappable_with {
        friend void swap(noexcept_swappable_with& lhs, int& rhs) noexcept;
        friend void swap(int& lhs, noexcept_swappable_with& rhs) noexcept;
    };

    static_assert(decltype_<int>().is_swappable_with(),
                  "hyperion::mpl::Type::is_swappable_with test case 1 (failing)");
    static_assert(decltype_<swappable>().is_swappable_with(),
                  "hyperion::mpl::Type::is_swappable_with test case 2 (failing)");
    static_assert(decltype_<noexcept_swappable>().is_swappable_with(),
                  "hyperion::mpl::Type::is_swappable_with test case 3 (failing)");
    static_assert(!decltype_<not_swappable>().is_swappable_with(),
                  "hyperion::mpl::Type::is_swappable_with test case 4 (failing)");
    static_assert(decltype_<int>().is_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_swappable_with test case 5 (failing)");
    static_assert(decltype_<swappable_with>().is_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_swappable_with test case 6 (failing)");
    static_assert(decltype_<noexcept_swappable_with>().is_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_swappable_with test case 7 (failing)");
    static_assert(!decltype_<not_swappable>().is_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_swappable_with test case 8 (failing)");

    static_assert(decltype_<int>().is_noexcept_swappable_with(),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 1 (failing)");
    static_assert(decltype_<noexcept_swappable>().is_noexcept_swappable_with(),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 2 (failing)");
    static_assert(!decltype_<swappable>().is_noexcept_swappable_with(),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 3 (failing)");
    static_assert(!decltype_<not_swappable>().is_noexcept_swappable_with(),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 4 (failing)");
    static_assert(decltype_<int>().is_noexcept_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 5 (failing)");
    static_assert(decltype_<noexcept_swappable_with>().is_noexcept_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 6 (failing)");
    static_assert(!decltype_<swappable_with>().is_noexcept_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 7 (failing)");
    static_assert(!decltype_<not_swappable>().is_noexcept_swappable_with(decltype_<int>()),
                  "hyperion::mpl::Type::is_noexcept_swappable_with test case 8 (failing)");

    static_assert(decltype_<int>().sizeof_() == 4_usize,
                  "hyperion::mpl::Type::sizeof_ test case 1 (failing)");
    static_assert(decltype_<double>().sizeof_() == 8_usize,
                  "hyperion::mpl::Type::sizeof_ test case 2 (failing)");
    static_assert(decltype_<char>().sizeof_() == 1_usize,
                  "hyperion::mpl::Type::sizeof_ test case 3 (failing)");
} // namespace hyperion::mpl::_test::type

#endif // HYPERION_MPL_TYPE_TEST_H
