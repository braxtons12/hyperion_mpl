/// @file metatypes.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Tests for metatypes.h
/// @version 0.6.1
/// @date 2025-07-08
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

#ifndef HYPERION_MPL_METATYPES_TEST_H
#define HYPERION_MPL_METATYPES_TEST_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/pair.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>

namespace hyperion::mpl::_test::metatypes {
    struct not_meta { };

    struct metatype {
        using type = void;
    };

    struct metavalue {
        static inline constexpr auto value = 42;
    };

    template<typename TType>
    struct metatype_typefunction {
        using type = std::add_const_t<TType>;
    };

    template<typename TType>
    struct metatype_valuefunction {
        static inline constexpr auto value = std::is_const_v<TType>;
    };

    template<auto TValue>
    struct metavalue_typefunction {
        using type = std::conditional_t<TValue, int, void>;
    };

    template<auto TValue>
    struct metavalue_valuefunction {
        static inline constexpr auto value = TValue * 2;
    };

    constexpr auto type_to_type = []([[maybe_unused]] const MetaType auto& type) -> Type<bool> {
        return {};
    };
    constexpr auto type_to_value = []([[maybe_unused]] const MetaType auto& type) -> Value<true> {
        return {};
    };
    constexpr auto type_to_pair
        = []([[maybe_unused]] const MetaType auto& type) -> Pair<Type<bool>, Type<bool>> {
        return {};
    };

    template<typename... TTypes>
    struct TestList { };

    constexpr auto type_to_list
        = []([[maybe_unused]] const MetaType auto& type) -> TestList<bool, bool> {
        return {};
    };

    constexpr auto value_to_type = []([[maybe_unused]] const MetaValue auto& type) -> Type<bool> {
        return {};
    };
    constexpr auto value_to_value = []([[maybe_unused]] const MetaValue auto& type) -> Value<true> {
        return {};
    };
    constexpr auto value_to_pair
        = []([[maybe_unused]] const MetaValue auto& type) -> Pair<Type<bool>, Type<bool>> {
        return {};
    };
    constexpr auto value_to_list
        = []([[maybe_unused]] const MetaValue auto& type) -> TestList<bool, bool> {
        return {};
    };

    constexpr auto pair_to_type = []([[maybe_unused]] const MetaPair auto& pair) -> Type<bool> {
        return {};
    };
    constexpr auto pair_to_value = []([[maybe_unused]] const MetaPair auto& pair) -> Value<true> {
        return {};
    };
    constexpr auto pair_to_pair
        = []([[maybe_unused]] const MetaPair auto& pair) -> Pair<Type<bool>, Type<bool>> {
        return {};
    };
    constexpr auto pair_to_list
        = []([[maybe_unused]] const MetaPair auto& pair) -> TestList<bool, bool> {
        return {};
    };

    constexpr auto list_to_type = []([[maybe_unused]] const MetaList auto& list) -> Type<bool> {
        return {};
    };
    constexpr auto list_to_value = []([[maybe_unused]] const MetaList auto& list) -> Value<true> {
        return {};
    };
    constexpr auto list_to_pair
        = []([[maybe_unused]] const MetaList auto& list) -> Pair<Type<bool>, Type<bool>> {
        return {};
    };
    constexpr auto list_to_list
        = []([[maybe_unused]] const MetaList auto& list) -> TestList<bool, bool> {
        return {};
    };

    static_assert(MetaType<metatype>, "hyperion::mpl::MetaType test case 1 (failing)");
    static_assert(not MetaType<not_meta>, "hyperion::mpl::MetaType test case 2 (failing)");
    static_assert(MetaType<std::add_lvalue_reference<int>>,
                  "hyperion::mpl::MetaType test case 1 (failing)");
    static_assert(!MetaType<int>, "hyperion::mpl::MetaType test case 2 (failing)");
    static_assert(!MetaType<Value<1>>, "hyperion::mpl::MetaType test case 3 (failing)");

    struct not_value_type {
        int value;
    };

    static_assert(MetaValue<metavalue>, "hyperion::mpl::MetaValue test case 1 (failing)");
    static_assert(not MetaValue<not_meta>, "hyperion::mpl::MetaValue test case 2 (failing)");
    static_assert(MetaValue<Value<true>>,
                  "hyperion::mpl::MetaValue not satisfied by hyperion::mpl::Value "
                  "(implementation failing)");
    static_assert(MetaValue<std::bool_constant<true>>,
                  "hyperion::mpl::MetaValue not satisfied by std::bool_constant "
                  "(implementation failing)");
    static_assert(!MetaValue<not_value_type>,
                  "hyperion::mpl::MetaValue not satisfied by _test::not_value_type "
                  "(implementation failing)");

    static_assert(TypeMetaFunction<metatype_typefunction>,
                  "hyperion::mpl::TypeMetaFunction test case 1 (failing)");
    static_assert(TypeMetaFunction<metatype_valuefunction>,
                  "hyperion::mpl::TypeMetaFunction test case 2 (failing)");

    static_assert(ValueMetaFunction<metavalue_typefunction>,
                  "hyperion::mpl::ValueMetaFunction test case 1 (failing)");
    static_assert(ValueMetaFunction<metavalue_valuefunction>,
                  "hyperion::mpl::ValueMetaFunction test case 2 (failing)");

    static_assert(MetaFunction<decltype(type_to_type)>,
                  "hyperion::mpl::MetaFunction test case 1 (failing)");
    static_assert(MetaFunction<decltype(type_to_value)>,
                  "hyperion::mpl::MetaFunction test case 2 (failing)");
    static_assert(MetaFunction<decltype(type_to_pair)>,
                  "hyperion::mpl::MetaFunction test case 3 (failing)");
    static_assert(MetaFunction<decltype(type_to_list)>,
                  "hyperion::mpl::MetaFunction test case 4 (failing)");

    static_assert(MetaFunction<decltype(value_to_type)>,
                  "hyperion::mpl::MetaFunction test case 5 (failing)");
    static_assert(MetaFunction<decltype(value_to_value)>,
                  "hyperion::mpl::MetaFunction test case 6 (failing)");
    static_assert(MetaFunction<decltype(value_to_pair)>,
                  "hyperion::mpl::MetaFunction test case 7 (failing)");
    static_assert(MetaFunction<decltype(value_to_list)>,
                  "hyperion::mpl::MetaFunction test case 8 (failing)");

    static_assert(MetaFunction<decltype(pair_to_type)>,
                  "hyperion::mpl::MetaFunction test case 9 (failing)");
    static_assert(MetaFunction<decltype(pair_to_value)>,
                  "hyperion::mpl::MetaFunction test case 10 (failing)");
    static_assert(MetaFunction<decltype(pair_to_pair)>,
                  "hyperion::mpl::MetaFunction test case 11 (failing)");
    static_assert(MetaFunction<decltype(pair_to_list)>,
                  "hyperion::mpl::MetaFunction test case 12 (failing)");

    static_assert(MetaFunction<decltype(list_to_type)>,
                  "hyperion::mpl::MetaFunction test case 13 (failing)");
    static_assert(MetaFunction<decltype(list_to_value)>,
                  "hyperion::mpl::MetaFunction test case 14 (failing)");
    static_assert(MetaFunction<decltype(list_to_pair)>,
                  "hyperion::mpl::MetaFunction test case 15 (failing)");
    static_assert(MetaFunction<decltype(list_to_list)>,
                  "hyperion::mpl::MetaFunction test case 16 (failing)");

    static_assert(std::is_same_v<meta_result_t<decltype(type_to_type), Type<bool>>, Type<bool>>,
                  "hyperion::mpl::meta_result test case 1 (failing)");
    static_assert(std::is_same_v<meta_result_t<decltype(type_to_value), Type<bool>>, Value<true>>,
                  "hyperion::mpl::meta_result test case 2 (failing)");
    static_assert(std::is_same_v<meta_result_t<decltype(value_to_type), Value<true>>, Type<bool>>,
                  "hyperion::mpl::met_result test case 3 (failing)");
    static_assert(std::is_same_v<meta_result_t<decltype(value_to_value), Value<true>>, Value<true>>,
                  "hyperion::mpl::met_result test case 4 (failing)");

} // namespace hyperion::mpl::_test::metatypes

#endif // HYPERION_MPL_METATYPES_TEST_H
