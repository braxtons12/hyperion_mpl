/// @file pair.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming type for operating on a pair of metaprogramming values and/or types
/// @version 0.1
/// @date 2024-02-15
///
/// MIT License
/// @copyright Copyright (c) 2024 Braxton Salyer <braxtonsalyer@gmail.com>
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

#ifndef HYPERION_MPL_PAIR_H
#define HYPERION_MPL_PAIR_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

//

#include <concepts>
#include <cstddef>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup pair Metaprogramming Pair Type
/// Hyperion provides `mpl::Pair` as a metaprogramming type for storing, communicating,
/// working with, and operating on pairs of types or values.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/pair.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto add_const = [](MetaPair auto pair) noexcept {
///     return make_pair(pair.make_first().as_const(),
///                      pair.make_second().as_const());
/// };
///
/// constexpr auto pair = Pair<int, double>{};
/// constexpr auto constified = pair.apply(add_const);
///
/// static_assert(constified == Pair<const int, const double>{});
/// @endcode
/// @headerfile hyperion/mpl/pair.h
/// @}

namespace hyperion::mpl {

    template<typename... TTypes>
    struct List;

    template<typename TFirst, typename TSecond>
    struct Pair {
        using first = detail::convert_to_meta_t<TFirst>;
        using second = detail::convert_to_meta_t<TSecond>;

        template<typename TDelay = first>
            requires std::same_as<TDelay, first>
        [[nodiscard]] constexpr auto make_first() const noexcept -> TDelay {
            return {};
        }

        template<typename TDelay = second>
            requires std::same_as<TDelay, second>
        [[nodiscard]] constexpr auto make_second() const noexcept -> TDelay {
            return {};
        }

        template<template<typename> typename TMetaFunction>
            requires requires { typename TMetaFunction<Pair>; }
                     && (MetaType<TMetaFunction<Pair>> || MetaValue<TMetaFunction<Pair>>
                         || MetaPair<TMetaFunction<Pair>>)
        [[nodiscard]] constexpr auto apply() const noexcept
            -> detail::convert_to_meta_t<TMetaFunction<Pair>> {
            return {};
        }

        template<template<typename> typename TMetaFunction>
            requires(!requires { typename TMetaFunction<Pair>; }) && requires {
                first{}.template apply<TMetaFunction>();
                second{}.template apply<TMetaFunction>();
            }
        [[nodiscard]] constexpr auto apply() const noexcept {
            return make_pair(first{}.template apply<TMetaFunction>(),
                             second{}.template apply<TMetaFunction>());
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Pair>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> meta_result_t<TFunction, Pair> {
            return {};
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, first> && MetaFunctionOf<TFunction, second>
                     && (!MetaFunctionOf<TFunction, Pair>)
        [[nodiscard]] constexpr auto apply(
            [[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept
            -> Pair<meta_result_t<TFunction, first>, meta_result_t<TFunction, second>> {
            return {};
        }

        template<template<typename> typename TPredicate>
            requires requires { typename TPredicate<Pair>; } && MetaValue<TPredicate<Pair>>
                     && std::same_as<std::remove_cvref_t<decltype(TPredicate<Pair>::value)>, bool>
        [[nodiscard]] constexpr auto
        satisfies() const noexcept -> detail::convert_to_meta_t<TPredicate<Pair>> {
            return {};
        }

        template<template<typename> typename TPredicate>
            requires(!requires { typename TPredicate<Pair>; }) && requires {
                first{}.template satisfies<TPredicate>();
                second{}.template satisfies<TPredicate>();
            }
        [[nodiscard]] constexpr auto satisfies() const noexcept {
            return make_first().template satisfies<TPredicate>()
                   && make_second().template satisfies<TPredicate>();
        }

        template<typename TPredicate>
            requires MetaFunctionOf<TPredicate, Pair> && MetaValue<meta_result_t<TPredicate, Pair>>
                     && std::same_as<
                         std::remove_cvref_t<decltype(meta_result_t<TPredicate, Pair>::value)>,
                         bool>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept -> meta_result_t<TPredicate, Pair> {
            return {};
        }

        template<typename TPredicate>
            requires MetaFunctionOf<TPredicate, first> && MetaFunctionOf<TPredicate, second>
                     && MetaValue<meta_result_t<TPredicate, first>>
                     && MetaValue<meta_result_t<TPredicate, second>>
                     && std::same_as<
                         std::remove_cvref_t<decltype(meta_result_t<TPredicate, first>::value)>,
                         bool>
                     && std::same_as<
                         std::remove_cvref_t<decltype(meta_result_t<TPredicate, second>::value)>,
                         bool>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept {
            return make_first().satisfies(TPredicate{}) && make_second().satisfies(TPredicate{});
        }

        template<typename TDelayFirst = TFirst, typename TDelaySecond = TSecond>
            requires std::same_as<TDelayFirst, TFirst> && std::same_as<TDelaySecond, TSecond>
        [[nodiscard]] constexpr auto as_list() const noexcept -> List<TDelayFirst, TDelaySecond> {
            return {};
        }

        template<std::size_t TIndex>
        // NOLINTNEXTLINE(cert-dcl58-cpp)
        [[nodiscard]] constexpr auto get() const noexcept {
            if constexpr(TIndex == 0) {
                return make_first();
            }
            else {
                return make_second();
            }
        }

        template<std::size_t TIndex>
        // NOLINTNEXTLINE(cert-dcl58-cpp)
        [[nodiscard]] constexpr auto get() noexcept {
            if constexpr(TIndex == 0) {
                return make_first();
            }
            else {
                return make_second();
            }
        }
    };
} // namespace hyperion::mpl

template<std::size_t TIndex, typename TFirst, typename TSecond>
// NOLINTNEXTLINE(cert-dcl58-cpp)
struct std::tuple_element<TIndex, hyperion::mpl::Pair<TFirst, TSecond>> {
    using type = std::conditional_t<TIndex == 0,
                                    typename hyperion::mpl::Pair<TFirst, TSecond>::first,
                                    typename hyperion::mpl::Pair<TFirst, TSecond>::second>;
};

template<typename TFirst, typename TSecond>
// NOLINTNEXTLINE(cert-dcl58-cpp)
struct std::tuple_size<hyperion::mpl::Pair<TFirst, TSecond>>
    : std::integral_constant<std::size_t, 2> { };

namespace hyperion::mpl {

    template<typename TLHSFirst, typename TLHSSecond, typename TRHSFirst, typename TRHSSecond>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Pair<TLHSFirst, TLHSSecond>& lhs,
               [[maybe_unused]] const Pair<TRHSFirst, TRHSSecond>& rhs) noexcept -> bool {
        constexpr auto is_equal = [](auto left, auto right) {
            if constexpr((MetaType<decltype(left)> && MetaType<decltype(right)>)
                         || (MetaValue<decltype(left)> && MetaValue<decltype(right)>)
                         || (MetaPair<decltype(left)> && MetaPair<decltype(right)>))
            {
                return left == right;
            }
            else {
                return false;
            }
        };

        return is_equal(lhs.make_first(), rhs.make_first())
               && is_equal(lhs.make_second(), rhs.make_second());
    }

    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<detail::convert_to_raw_t<TFirst>, detail::convert_to_raw_t<TSecond>> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, detail::convert_to_raw_t<TSecond>> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<detail::convert_to_raw_t<TFirst>, TSecond> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }
} // namespace hyperion::mpl

#include <hyperion/mpl/list.h>
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
                  "hyperion::mpl::Pair::satisfies(MetaFunctionOf<Pair>) test case 1 (failing)");
    static_assert(not Pair<const int, double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaFunctionOf<Pair>) test case 2 (failing)");
    static_assert(not Pair<int, const double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaFunctionOf<Pair>) test case 3 (failing)");
    static_assert(not Pair<int, double>{}.satisfies(is_const),
                  "hyperion::mpl::Pair::satisfies(MetaFunctionOf<Pair>) test case 4 (failing)");

    static_assert(Pair<const int, const double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaFunctionOf<Type>) test case 1 (failing)");
    static_assert(not Pair<const int, double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaFunctionOf<Type>) test case 2 (failing)");
    static_assert(not Pair<int, const double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaFunctionOf<Type>) test case 3 (failing)");
    static_assert(not Pair<int, double>{}.satisfies(is_const_inner),
                  "hyperion::mpl::Pair::satisifes(MetaFunctionOf<Type>) test case 4 (failing)");

    static_assert(
        Pair<const int, Value<1>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaFunctionOf<TypeOrValue>) test case 1 (failing)");
    static_assert(
        not Pair<int, Value<1>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaFunctionOf<TypeOrValue>) test case 2 (failing)");
    static_assert(
        not Pair<const int, Value<2>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaFunctionOf<TypeOrValue>) test case 3 (failing)");
    static_assert(
        not Pair<int, Value<2>>{}.satisfies(is_one_or_const),
        "hyperion::mpl::Pair::satisfies(MetaFunctionOf<TypeOrValue>) test case 4 (failing)");

    template<typename TPair>
    struct is_const_t
        : decltype(typename TPair::first{}.is_const() && typename TPair::second{}.is_const()) { };

    template<typename TPair>
    struct is_one_or_const_t : decltype(is_one_or_const(typename TPair::first{})
                                        && is_one_or_const(typename TPair::second{})) { };

    template<typename TType>
        requires(!MetaPair<TType>)
    struct is_const_inner_t : decltype(decltype_<TType>().is_const()) { };

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

    static_assert(Pair<int, double>{}.as_list() == List<int, double>{},
                  "hyperion::mpl::Pair::as_list test (failing)");

} // namespace hyperion::mpl::_test::pair

#endif // HYPERION_MPL_PAIR_H
