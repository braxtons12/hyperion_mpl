/// @file list.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming facilities for working with a list of types or values
/// @version 0.1
/// @date 2024-02-23
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

#include <hyperion/platform/def.h>
#include <hyperion/platform/ignore.h>
#include <hyperion/platform/types.h>
//
#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>
//
#include <hyperion/mpl/metapredicates.h>

#include <concepts>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup list Metaprogramming List Type
/// Hyperion provides `mpl::List` as a metaprogramming type for storing, communicating,
/// working with, and operating on lists of types or values.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/list.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto add_const = [](MetaType auto type) noexcept {
///     return type.add_const();
/// };
///
/// constexpr auto list = List<int, double, float>{};
/// constexpr auto zipped = list.zip(List<u32, usize, i32>{});
/// constexpr auto constified = zipped.apply(add_const);
///
/// static_assert(constified == List<Pair<const int, const u32>,
///                                  Pair<const double, const usize>,
///                                  Pair<const float, const i32>>{});
/// @endcode
/// @headerfile hyperion/mpl/list.h
/// @}

#ifndef HYPERION_MPL_LIST_H
    #define HYPERION_MPL_LIST_H

namespace hyperion::mpl {

    template<typename... TTypes>
    struct List;

    template<typename TFirst, typename TSecond>
    struct Pair;

    struct not_found_tag { };

    namespace detail {
        struct any_tag { };

        static inline constexpr auto any_value = 0;

        template<usize TIndex, usize TCurrent, typename TList>
        struct at;

        template<usize TIndex,
                 usize TCurrent,
                 template<typename...>
                 typename TList,
                 typename THead,
                 typename... TTails>
        struct at<TIndex, TCurrent, TList<THead, TTails...>> {
    #if HYPERION_COMPILER_HAS_TYPE_PACK_ELEMENT
            using type = __type_pack_element<TIndex, THead, TTails...>;
    #else
            using type = std::conditional_t<
                TIndex == TCurrent,
                THead,
                typename at<TIndex, TCurrent + 1_usize, TList<TTails...>>::type>;
    #endif
        };

        template<usize TIndex, usize TCurrent, template<typename...> typename TList>
        struct at<TIndex, TCurrent, TList<>> {
            using type = void;
        };

        template<typename TList>
        struct pop_front;

        template<template<typename...> typename TList, typename TFront, typename... TTypes>
        struct pop_front<TList<TFront, TTypes...>> {
            using front = TFront;
            using remaining = TList<TTypes...>;
        };

        template<template<typename...> typename TList, typename TFront>
        struct pop_front<TList<TFront>> {
            using front = TFront;
            using remaining = TList<>;
        };

        template<template<typename...> typename TList>
        struct pop_front<TList<>> {
            using front = not_found_tag;
            using remaining = TList<>;
        };

        template<template<typename...> typename TList, typename TIndexSequence, typename... TTypes>
        struct pop_back_base;

        template<template<typename...> typename TList, std::size_t... TIndices, typename... TTypes>
        struct pop_back_base<TList, std::index_sequence<TIndices...>, TTypes...> {
            using remaining = TList<typename at<TIndices, 0_usize, List<TTypes...>>::type...>;
        };

        template<typename TList>
        struct pop_back;

        template<template<typename...> typename TList, typename... TTypes>
        struct pop_back<TList<TTypes...>> {
            using back = typename at<sizeof...(TTypes) - 1, 0_usize, TList<TTypes...>>::type;
            using remaining =
                typename pop_back_base<TList,
                                       std::make_index_sequence<sizeof...(TTypes) - 1>,
                                       TTypes...>::remaining;
        };

        template<template<typename...> typename TList>
        struct pop_back<TList<>> {
            using back = not_found_tag;
            using remaining = TList<>;
        };
    } // namespace detail

    template<typename... TTypes>
    struct List {
        template<typename TType>
        using as_meta = detail::convert_to_meta_t<TType>;

        template<typename TType>
        using as_raw = detail::convert_to_raw_t<TType>;

        [[nodiscard]] constexpr auto size() const noexcept -> Value<sizeof...(TTypes), usize> {
            return {};
        }

        template<typename TFunction>
            requires(MetaFunctionOf<TFunction, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.apply(TFunction{}), ...); }
        [[nodiscard]] constexpr auto apply(TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept {
            return List<as_raw<decltype(as_meta<TTypes>{}.apply(func))>...>{};
        }

        template<typename TVisitor>
            requires(std::invocable<TVisitor, as_meta<TTypes>> && ...)
                    && (std::same_as<std::invoke_result_t<TVisitor, as_meta<TTypes>>, void> && ...)
        constexpr auto for_each(TVisitor&& vis) const noexcept -> void {
            (std::forward<TVisitor>(vis)(as_meta<TTypes>{}), ...);
        }

        template<typename TVisitor>
            requires(std::invocable<TVisitor, as_meta<TTypes>> && ...)
                    && (std::same_as<std::invoke_result_t<TVisitor, as_meta<TTypes>>, void> && ...)
        constexpr auto for_each_n(TVisitor&& vis, auto count) const noexcept -> void {
            return typename detail::pop_back_base<List,
                                                  std::make_index_sequence<decltype(count)::value>,
                                                  TTypes...>::remaining{}
                .for_each(std::forward<TVisitor>(vis));
        }

      private:
        template<typename TState, typename TAccumulator, typename... TTs>
            requires(std::invocable<TAccumulator, TState, as_meta<TTs>> && ...)
        [[nodiscard]] constexpr auto
        accumulate_impl(TState&& state,
                        TAccumulator&& accumulator,
                        [[maybe_unused]] List<TTs...> remaining) const noexcept {
            using popped = detail::pop_front<List<TTs...>>;
            using front = typename popped::front;
            using remainder = typename popped::remaining;

            if constexpr(std::same_as<front, not_found_tag>) {
                return std::forward<TState>(state);
            }
            else {
                return accumulate_impl(
                    std::forward<TAccumulator>(accumulator)(std::forward<TState>(state), front{}),
                    std::forward<TAccumulator>(accumulator),
                    remainder{});
            }
        }

      public:
        template<typename TState, typename TAccumulator>
            requires(std::invocable<TAccumulator, TState, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto
        accumulate(TState&& state, TAccumulator&& accumulator) const noexcept {
            return accumulate_impl(std::forward<TState>(state),
                                   std::forward<TAccumulator>(accumulator),
                                   List<as_meta<TTypes>...>{});
        }

      private:
        template<typename TPredicate>
        [[nodiscard]] constexpr auto
        find_if_impl([[maybe_unused]] TPredicate&& predicate, // NOLINT(*-missing-std-forward))
                     [[maybe_unused]] MetaValue auto index) const noexcept
            requires(decltype(index)::value <= sizeof...(TTypes))
        {
            if constexpr(meta_result_t<TPredicate,
                                       typename detail::at<decltype(index)::value,
                                                           0_usize,
                                                           List<as_meta<TTypes>...>>::type>{})
            {
                return index;
            }
            else if constexpr(decltype(index){} + 1_value < sizeof...(TTypes)) {
                return find_if_impl(std::forward<TPredicate>(predicate), index + 1_value);
            }
            else {
                return Value<sizeof...(TTypes), usize>{};
            }
        }

      public:
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto find_if(TPredicate&& predicate) const noexcept {
            auto result = find_if_impl(std::forward<TPredicate>(predicate), 0_value);
            if constexpr(result == Value<sizeof...(TTypes), usize>{}) {
                return decltype_<not_found_tag>();
            }
            else {
                return at(result);
            }
        }

        [[nodiscard]] constexpr auto find([[maybe_unused]] auto value) const noexcept {
            return find_if(equal_to(value));
        }

        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto
        count_if([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept {
            constexpr auto accumulator = [](auto state, auto element) {
                if constexpr(TPredicate{}(decltype(element){})) {
                    return state + 1_value;
                }
                else {
                    return state;
                }
            };

            return accumulate(0_value, accumulator);
        }

        [[nodiscard]] constexpr auto count([[maybe_unused]] auto value) const noexcept {
            return count_if(equal_to(value));
        }

        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto all_of(TPredicate&& predicate) const noexcept {
            return count_if(std::forward<TPredicate>(predicate)) == sizeof...(TTypes);
        }

        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto any_of(TPredicate&& predicate) const noexcept {
            return find_if_impl(std::forward<TPredicate>(predicate), 0_value)
                   != Value<sizeof...(TTypes), usize>{};
        }

        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto none_of(TPredicate&& predicate) const noexcept {
            return find_if_impl(std::forward<TPredicate>(predicate), 0_value)
                   == Value<sizeof...(TTypes), usize>{};
        }

        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto index_if(TPredicate&& predicate) const noexcept {
            return find_if_impl(std::forward<TPredicate>(predicate), 0_value);
        }

        template<typename TValue>
            requires((!MetaPredicateOf<TValue, as_meta<TTypes>>) || ...)
        [[nodiscard]] constexpr auto index_of([[maybe_unused]] TValue value) const noexcept {
            return index_if(equal_to(value));
        }

        template<usize TIndex>
            requires(TIndex < sizeof...(TTypes))
        [[nodiscard]] constexpr auto at() const noexcept ->
            typename detail::at<TIndex, 0_usize, List<as_meta<TTypes>...>>::type {
            return {};
        }

        [[nodiscard]] constexpr auto at(MetaValue auto index) const noexcept ->
            typename detail::at<decltype(index)::value, 0_usize, List<as_meta<TTypes>...>>::type
            requires(decltype(index)::value < sizeof...(TTypes))
        {
            return {};
        }

        [[nodiscard]] constexpr auto front() const noexcept ->
            typename detail::at<0_usize, 0_usize, List<as_meta<TTypes>...>>::type {
            return {};
        }

        [[nodiscard]] constexpr auto back() const noexcept -> typename detail::
            at<sizeof...(TTypes) - 1_usize, 0_usize, List<as_meta<TTypes>...>>::type {
            return {};
        }

        [[nodiscard]] constexpr auto push_front(MetaType auto type) const noexcept {
            return List<typename decltype(type)::type, TTypes...>{};
        }

        [[nodiscard]] constexpr auto push_front(MetaValue auto value) const noexcept {
            return List<decltype(value), TTypes...>{};
        }

        template<typename... TOthers>
        [[nodiscard]] constexpr auto
        push_front([[maybe_unused]] List<TOthers...> list) const noexcept
            -> List<TOthers..., TTypes...> {
            return {};
        }

        [[nodiscard]] constexpr auto push_back(MetaType auto type) const noexcept {
            return List<TTypes..., typename decltype(type)::type>{};
        }

        [[nodiscard]] constexpr auto push_back(MetaValue auto value) const noexcept {
            return List<TTypes..., decltype(value)>{};
        }

        template<typename... TOthers>
        [[nodiscard]] constexpr auto
        push_back([[maybe_unused]] List<TOthers...> list) const noexcept
            -> List<TTypes..., TOthers...> {
            return {};
        }

        [[nodiscard]] constexpr auto
        pop_front() const noexcept -> typename detail::pop_front<List>::remaining {
            return {};
        }

        [[nodiscard]] constexpr auto
        pop_back() const noexcept -> typename detail::pop_back<List>::remaining {
            return {};
        }

        template<typename... TRHTypes>
            requires(sizeof...(TRHTypes) == sizeof...(TTypes))
        [[nodiscard]] constexpr auto zip([[maybe_unused]] List<TRHTypes...> rhs) const noexcept {
            return []<usize... TIndices>(std::index_sequence<TIndices...>) {
                constexpr auto _list = decltype(rhs){};

                return mpl::List<as_raw<decltype(make_pair(List{}.template at<TIndices>(),
                                                           _list.template at<TIndices>()))>...>{};
            }(std::index_sequence_for<TTypes...>{});
        }
    };
} // namespace hyperion::mpl

    #include <hyperion/mpl/pair.h>

namespace hyperion::mpl {
    template<typename... TLHTypes, typename... TRHTypes>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const List<TLHTypes...>& lhs,
               [[maybe_unused]] const List<TRHTypes...>& rhs) noexcept -> bool {
        constexpr auto is_same = []<typename TFirst, typename TSecond>(Pair<TFirst, TSecond> pair)
            requires(MetaType<typename decltype(pair)::first>
                     || MetaValue<typename decltype(pair)::first>
                     || MetaPair<typename decltype(pair)::first>)
                    && (MetaType<typename decltype(pair)::second>
                        || MetaValue<typename decltype(pair)::second>
                        || MetaPair<typename decltype(pair)::second>)
        {
            return equal_to(typename decltype(pair)::first{})(typename decltype(pair)::second{});
        };

        constexpr auto check_all
            = []<typename... TResults>([[maybe_unused]] List<TResults...> results) {
                  return (TResults{} && ...);
              };

        return check_all(List<TLHTypes...>{}.zip(List<TRHTypes...>{}).apply(is_same));
    }
} // namespace hyperion::mpl

namespace hyperion::mpl::_test::list {

    static constexpr auto add_const = [](MetaType auto type) {
        return type.as_const();
    };

    static_assert(
        std::same_as<decltype(List<int, double>{}.apply(add_const)), List<const int, const double>>,
        "hyperion::mpl::List::apply test case 1 (failing)");

    static_assert(List<int, double>{}.at<0>() == decltype_<int>(),
                  "hyperion::mpl::List::at test case 1 (failing)");
    static_assert(List<int, double>{}.at<1>() == decltype_<double>(),
                  "hyperion::mpl::List::at test case 2 (failing)");

    static_assert(std::same_as<decltype(List<int, double>{}.zip(List<double, int>{})),
                               List<Pair<int, double>, Pair<double, int>>>,
                  "hyperion::mpl::List::zip test case 1 (failing)");
    static_assert(List<int, double>{}.zip(List<float, usize>{}).apply(add_const)
                      == List<Pair<const int, const float>, Pair<const double, const usize>>{},
                  "hyperion::mpl::List::zip test case 2 (failing)");

    static_assert(List<int, double>{} == List<int, double>{},
                  "hyperion::mpl::List operator== test case 1 (failing)");
    static_assert(List<int, double>{}.apply(add_const) == List<const int, const double>{},
                  "hyperion::mpl::List operator== test case 2 (failing)");
    static_assert(List<int, double>{}.zip(List<double, int>{})
                      == List<Pair<int, double>, Pair<double, int>>{},
                  "hyperion::mpl::List operator== test case 3 (failing)");

    static_assert(List<int, double>{}.front() == decltype_<int>(),
                  "hyperion::mpl::List::front test (failing)");

    static_assert(List<int, double>{}.back() == decltype_<double>(),
                  "hyperion::mpl::List::back test (failing)");

    static_assert(List<int, double>{}.push_front(decltype_<float>()) == List<float, int, double>{},
                  "hyperion::mpl::List::push_front test case 1 (failing)");
    static_assert(List<int, double>{}.push_front(1_value) == List<Value<1>, int, double>{},
                  "hyperion::mpl::List::push_front test case 2 (failing)");
    static_assert(List<int, double>{}.push_front(List<float, usize>{})
                      == List<float, usize, int, double>{},
                  "hyperion::mpl::List::push_front test case 3 (failing)");

    static_assert(List<int, double>{}.push_back(decltype_<float>()) == List<int, double, float>{},
                  "hyperion::mpl::List::push_back test case 1 (failing)");
    static_assert(List<int, double>{}.push_back(1_value) == List<int, double, Value<1>>{},
                  "hyperion::mpl::List::push_back test case 2 (failing)");
    static_assert(List<int, double>{}.push_back(List<float, usize>{})
                      == List<int, double, float, usize>{},
                  "hyperion::mpl::List::push_back test case 3 (failing)");

    static_assert(List<int, double, float>{}.pop_front() == List<double, float>{},
                  "hyperion::mpl::List::pop_front test case 1 (failing)");
    static_assert(List<Value<1>, int, double>{}.pop_front() == List<int, double>{},
                  "hyperion::mpl::List::pop_front test case 2 (failing)");

    static_assert(List<int, double, float>{}.pop_back() == List<int, double>{},
                  "hyperion::mpl::List::pop_back test case 1 (failing)");
    static_assert(List<int, double, Value<1>>{}.pop_back() == List<int, double>{},
                  "hyperion::mpl::List::pop_back test case 2 (failing)");

    [[nodiscard]] constexpr auto test_for_each() noexcept -> bool {
        constexpr auto list = List<int, double, float, int>{};

        auto num_ints = 0;
        list.for_each([&num_ints](auto type) {
            if constexpr(MetaType<decltype(type)>) {
                if(type == decltype_<int>()) {
                    num_ints++;
                }
            }
        });

        return num_ints == 2;
    }

    static_assert(test_for_each(), "hyperion::mpl::List::for_each test (failing)");

    [[nodiscard]] constexpr auto test_for_each_n1() noexcept -> bool {
        constexpr auto list = List<int, double, float, int>{};

        auto num_ints = 0;
        list.for_each_n(
            [&num_ints](auto type) {
                if constexpr(MetaType<decltype(type)>) {
                    if(type == decltype_<int>()) {
                        num_ints++;
                    }
                }
            },
            3_value);

        return num_ints == 1;
    }

    [[nodiscard]] constexpr auto test_for_each_n2() noexcept -> bool {
        constexpr auto list = List<int, double, float, int>{};

        auto num_ints = 0;
        list.for_each_n(
            [&num_ints](auto type) {
                if constexpr(MetaType<decltype(type)>) {
                    if(type == decltype_<int>()) {
                        num_ints++;
                    }
                }
            },
            0_value);

        return num_ints == 0;
    }

    static_assert(test_for_each_n1(), "hyperion::mpl::List::for_each_n test case 1 (failing)");
    static_assert(test_for_each_n2(), "hyperion::mpl::List::for_each_n test case 2 (failing)");

    static_assert(List<int, double, float>{}.find_if([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<double>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }) == decltype_<double>(),
                  "hyperion::mpl::List::find_if test case 1 (failing)");
    static_assert(List<int, double, float>{}.find_if([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<usize>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }) == decltype_<not_found_tag>(),
                  "hyperion::mpl::List::find_if test case 2 (failing)");

    static_assert(List<int, double, float>{}.all_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<std::is_arithmetic_v<typename decltype(type)::type>, bool>{};
        }
        else {
            return Value<false, bool>{};
        }
    }),
                  "hyperion::mpl::List::all_of test case 1 (failing)");
    static_assert(not List<int, double, float, Value<1>>{}.all_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<std::is_arithmetic_v<typename decltype(type)::type>, bool>{};
        }
        else {
            return Value<false, bool>{};
        }
    }),
                  "hyperion::mpl::List::all_of test case 2 (failing)");

    static_assert(List<int, double, float>{}.any_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<double>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::any_of test case 1 (failing)");
    static_assert(not List<int, double, float>{}.any_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<usize>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::any_of test case 2 (failing)");

    static_assert(List<int, double, float>{}.none_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<usize>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::none_of test case 1 (failing)");
    static_assert(not List<int, double, float>{}.none_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<double>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::none_of test case 2 (failing)");

    static_assert(List<Value<1>, Value<2>, Value<3>>{}.accumulate(0_value,
                                                                  [](auto state, auto val) {
                                                                      return state + val;
                                                                  })
                      == 6_value,
                  "hyperion::mpl::List::accumulate test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.accumulate(0_value,
                                                                  [](auto state, auto val) {
                                                                      return state + val;
                                                                  })
                      == 8_value,
                  "hyperion::mpl::List::accumulate test case 2 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count_if([](auto val) {
        return val == 3_value;
    }) == 2_value,
                  "hyperion::mpl::List::count_if test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count_if([](auto val) {
        return val == 4_value;
    }) == 0_value,
                  "hyperion::mpl::List::count_if test case 2 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count(3_value) == 2_value,
                  "hyperion::mpl::List::count test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count(4_value) == 0_value,
                  "hyperion::mpl::List::count test case 2 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<4>>{}.index_if(greater_than(3_value)) == 2_value,
                  "hyperion::mpl::List::index_if test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<4>>{}.index_if(less_than(3_value)) == 1_value,
                  "hyperion::mpl::List::index_if test case 2 (failing)");

    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(decltype_<double>()) == 2_value,
                  "hyperion::mpl::List::index_of test case 1 (failing)");
    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(decltype_<int>()) == 1_value,
                  "hyperion::mpl::List::index_of test case 2 (failing)");
    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(4_value) == 3_value,
                  "hyperion::mpl::List::index_of test case 3 (failing)");
} // namespace hyperion::mpl::_test::list

#endif // HYPERION_MPL_LIST_H
