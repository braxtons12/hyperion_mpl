/// @file pair.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Tests for pair.h
/// @version 0.1.1
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

#ifndef HYPERION_MPL_PAIR_TEST_H
#define HYPERION_MPL_PAIR_TEST_H

#include <hyperion/mpl/pair.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>

namespace hyperion::mpl::_test::pair {

    constexpr auto add_const = [](MetaPair auto pair) noexcept {
        return make_pair(pair.make_first().as_const(), pair.make_second().as_const());
    };

    constexpr auto add_const_inner = [](MetaType auto type) noexcept {
        return type.as_const();
    };

    constexpr auto add_one_or_const = [](auto val) noexcept
        requires(!MetaPair<decltype(val)>)
    {
        if constexpr(MetaType<decltype(val)>) {
            return val.as_const();
        }
        else {
            return val + 1_value;
        }
    };

    static_assert(Pair<int, double>{}.apply(add_const) == Pair<const int, const double>{},
                  "hyperion::mpl::Pair::apply test case 1 (failing)");
    static_assert(Pair<int, double>{}.apply(add_const_inner) == Pair<const int, const double>{},
                  "hyperion::mpl::Pair::apply test case 2 (failing)");
    static_assert(Pair<int, Value<1>>{}.apply(add_one_or_const) == Pair<const int, Value<2>>{},
                  "hyperion::mpl::Pair::apply test case 3 (failing)");

    template<typename TPair>
    struct add_const_t {
        using first = decltype(typename TPair::first{}.as_const());
        using second = decltype(typename TPair::second{}.as_const());
    };

    template<typename TPair>
    struct add_one_or_const_t {
        using first = decltype(add_one_or_const(typename TPair::first{}));
        using second = decltype(add_one_or_const(typename TPair::second{}));
    };

    template<typename TType>
        requires(!MetaPair<TType>)
    struct add_const_inner_t {
        using type = decltype(decltype_<TType>().as_const());
    };

    template<typename TType>
        requires(!MetaPair<TType>)
    struct add_one_or_const_inner_t {
        using type = std::conditional_t<MetaValue<TType>,
                                        decltype(TType{} + 1_value),
                                        decltype(decltype_<TType>().as_const())>;
    };

    static_assert(Pair<int, double>{}.apply<add_const_t>() == Pair<const int, const double>{},
                  "hyperion::mpl::Pair::apply<> test case 1 (failing)");
    static_assert(Pair<int, Value<1>>{}.apply<add_one_or_const_t>() == Pair<const int, Value<2>>{},
                  "hyperion::mpl::Pair::apply<> test case 2 (failing)");
    static_assert(Pair<int, double>{}.apply<add_const_inner_t>() == Pair<const int, const double>{},
                  "hyperion::mpl::Pair::apply<> test case 3 (failing)");
    static_assert(Pair<int, Value<1>>{}.apply<add_one_or_const_inner_t>()
                      == Pair<const int, Value<2>>{},
                  "hyperion::mpl::Pair::apply<> test case 4 (failing)");

    constexpr auto is_const = [](MetaPair auto pair) noexcept {
        return pair.make_first().is_const() && pair.make_second().is_const();
    };

    constexpr auto is_const_inner = [](MetaType auto type) noexcept {
        return type.is_const();
    };

    constexpr auto is_one_or_const = [](auto val) noexcept
        requires(!MetaPair<decltype(val)>)
    {
        if constexpr(MetaType<decltype(val)>) {
            return val.is_const();
        }
        else if constexpr(MetaValue<decltype(val)>) {
            return Value<decltype(val)::value == 1_value, bool>{};
        }
    };

    static_assert(Pair<const int, const double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaPredicateOf<Pair>) test case 1 (failing)");
    static_assert(not Pair<const int, double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaPredicateOf<Pair>) test case 2 (failing)");
    static_assert(not Pair<int, const double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaPredicateOf<Pair>) test case 3 (failing)");
    static_assert(not Pair<int, double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaPredicateOf<Pair>) test case 4 (failing)");

    static_assert(Pair<const int, const double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaPredicateOf<Type>) test case 1 (failing)");
    static_assert(not Pair<const int, double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaPredicateOf<Type>) test case 2 (failing)");
    static_assert(not Pair<int, const double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaPredicateOf<Type>) test case 3 (failing)");
    static_assert(not Pair<int, double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaPredicateOf<Type>) test case 4 (failing)");

    static_assert(
        Pair<const int, Value<1>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaPredicateOf<TypeOrValue>) test case 1 (failing)");
    static_assert(
        not Pair<int, Value<1>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaPredicateOf<TypeOrValue>) test case 2 (failing)");
    static_assert(
        not Pair<const int, Value<2>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaPredicateOf<TypeOrValue>) test case 3 (failing)");
    static_assert(
        not Pair<int, Value<2>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaPredicateOf<TypeOrValue>) test case 4 (failing)");

    template<typename TPair>
    struct is_const_t
        : decltype(typename TPair::first{}.is_const() && typename TPair::second{}.is_const()){};

    template<typename TPair>
    struct is_one_or_const_t : decltype(is_one_or_const(typename TPair::first{})
                                        && is_one_or_const(typename TPair::second{})){};

    template<typename TType>
        requires(!MetaPair<TType>)
    struct is_const_inner_t : decltype(decltype_<TType>().is_const()){};

    template<typename TType>
        requires(!MetaPair<TType>)
    struct is_one_or_const_inner_t : std::conditional_t<MetaValue<TType>,
                                                        decltype(TType{} == 1_value),
                                                        decltype(decltype_<TType>().is_const())> {
    };

    static_assert(
        Pair<const int, const double>{}.satisfies<is_const_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<Pair<Type, Type>>> test case 1 (failing)");
    static_assert(
        not Pair<const int, double>{}.satisfies<is_const_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<Pair<Type, Type>>> test case 2 (failing)");
    static_assert(
        not Pair<int, const double>{}.satisfies<is_const_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<Pair<Type, Type>>> test case 3 (failing)");
    static_assert(
        not Pair<int, double>{}.satisfies<is_const_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<Pair<Type, Type>> test case 4 (failing)");

    static_assert(
        Pair<const int, Value<1>>{}.satisfies<is_one_or_const_t>(),
        "hyperion::mpl::Pair::apply<MetaFunction<Pair<Type, Value>>> test case 1 (failing)");
    static_assert(
        not Pair<int, Value<1>>{}.satisfies<is_one_or_const_t>(),
        "hyperion::mpl::Pair::apply<MetaFunction<Pair<Type, Value>>> test case 2 (failing)");
    static_assert(
        not Pair<const int, Value<2>>{}.satisfies<is_one_or_const_t>(),
        "hyperion::mpl::Pair::apply<MetaFunction<Pair<Type, Value>>> test case 3 (failing)");
    static_assert(
        not Pair<int, Value<2>>{}.satisfies<is_one_or_const_t>(),
        "hyperion::mpl::Pair::apply<MetaFunction<Pair<Type, Value>>> test case 4 (failing)");

    static_assert(Pair<const int, const double>{}.satisfies<is_const_inner_t>(),
                  "hyperion::mpl::Pair::satisfies<MetaFunction<Type>> test case 1 (failing)");
    static_assert(not Pair<const int, double>{}.satisfies<is_const_inner_t>(),
                  "hyperion::mpl::Pair::satisfies<MetaFunction<Type>> test case 2 (failing)");
    static_assert(not Pair<int, const double>{}.satisfies<is_const_inner_t>(),
                  "hyperion::mpl::Pair::satisfies<MetaFunction<Type>> test case 3 (failing)");
    static_assert(not Pair<int, double>{}.satisfies<is_const_inner_t>(),
                  "hyperion::mpl::Pair::satisfies<MetaFunction<Type>> test case 4 (failing)");

    static_assert(
        Pair<const int, Value<1>>{}.satisfies<is_one_or_const_inner_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<TypeOrValue>> test case 1 (failing)");
    static_assert(
        not Pair<const int, Value<2>>{}.satisfies<is_one_or_const_inner_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<TypeOrValue>> test case 2 (failing)");
    static_assert(
        not Pair<int, Value<1>>{}.satisfies<is_one_or_const_inner_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<TypeOrValue>> test case 3 (failing)");
    static_assert(
        not Pair<int, Value<2>>{}.satisfies<is_one_or_const_inner_t>(),
        "hyperion::mpl::Pair::satisfies<MetaFunction<TypeOrValue>> test case 4 (failing)");

    constexpr auto sum = [](MetaValue auto lhs, MetaValue auto rhs) noexcept {
        return lhs + rhs;
    };

    static_assert(Pair<Value<1>, Value<2>>{}.unwrap(sum) == 3_value,
                  "hyperion::mpl::Pair::unwrap test case 1 (failing)");
    static_assert(Pair<Value<3>, Value<2>>{}.unwrap(sum) == 5_value,
                  "hyperion::mpl::Pair::unwrap test case 2 (failing)");

    static_assert(Pair<int, double>{} == Pair<int, double>{},
                  "hyperion::mpl::Pair operator== test case 1 (failing)");
    static_assert(Pair<double, int>{} == Pair<double, int>{},
                  "hyperion::mpl::Pair operator== test case 2 (failing)");
    static_assert(Pair<int, double>{} != Pair<double, int>{},
                  "hyperion::mpl::Pair operator== test case 3 (failing)");
    static_assert(Pair<double, int>{} != Pair<int, double>{},
                  "hyperion::mpl::Pair operator== test case 4 (failing)");

    constexpr auto test_pair_decomposition() -> bool {
        constexpr auto test_pair = Pair<int, double>{};
        auto [test_int_t, test_double_t] = test_pair;

        return test_int_t == decltype_<int>() && test_double_t == decltype_<double>();
    }

    static_assert(test_pair_decomposition(),
                  "hyperion::mpl::Pair structured binding test (failing)");

    constexpr auto test_make_pair() -> bool {
        int first = 0;
        const double second = 0.0;

        constexpr auto pair_no_refs = make_pair(decltype_<int>(), decltype_<double>());
        constexpr auto pair_first_ref = make_pair(first, decltype_<double>());
        constexpr auto pair_second_ref = make_pair(decltype_<int>(), second);
        constexpr auto pair_refs = make_pair(first, second);

        constexpr auto no_refs_ok = pair_no_refs == Pair<int, double>{};
        constexpr auto first_ref_ok = pair_first_ref == Pair<int&, double>{};
        constexpr auto second_ref_ok = pair_second_ref == Pair<int, const double&>{};
        constexpr auto refs_ok = pair_refs == Pair<int&, const double&>{};

        return no_refs_ok && first_ref_ok && second_ref_ok && refs_ok;
    }

    static_assert(test_make_pair(), "hyperion::mpl::make_pair test (failing)");

} // namespace hyperion::mpl::_test::pair

#endif // HYPERION_MPL_PAIR_TEST_H
