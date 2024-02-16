/// @file list.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming facilities for working with a list of types or values
/// @version 0.1
/// @date 2024-02-15
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

#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

//

#include <concepts>
#include <functional>
#include <type_traits>

#ifndef HYPERION_MPL_LIST_H
    #define HYPERION_MPL_LIST_H

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
/// // or as an alternative spelling, `list.apply(add_const)`
/// constexpr auto constified = list.for_each(add_const);
///
/// static_assert(constified == List<const int, const double, const float>{});
/// @endcode
/// @headerfile hyperion/mpl/list.h
/// @}

namespace hyperion::mpl {

    template<typename... TTypes>
    struct List;

    template<typename TFirst, typename TSecond>
    struct Pair;

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
    } // namespace detail

    template<typename... TTypes>
    struct List {
        template<typename TType>
        using as_meta = typename detail::convert_to_meta<TType>::type;

        template<typename TType>
        using as_raw = typename detail::convert_to_raw<TType>::type;

        [[nodiscard]] constexpr auto size() const noexcept -> Value<sizeof...(TTypes), usize> {
            return {};
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Type<detail::any_tag>>
                     && MetaType<meta_result_t<TFunction, Type<detail::any_tag>>>
                     && (MetaType<detail::convert_to_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto for_each(TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> List<as_raw<decltype(as_meta<TTypes>{}.apply(func))>...> {
            return {};
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Pair<Type<detail::any_tag>, Type<detail::any_tag>>>
                     && MetaValue<meta_result_t<TFunction,
                                                Pair<Type<detail::any_tag>, Type<detail::any_tag>>>>
                     && (MetaPair<typename detail::convert_to_meta<TTypes>::type> && ...)
        [[nodiscard]] constexpr auto for_each(TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> List<as_raw<decltype(as_meta<TTypes>{}.apply(func))>...> {
            return {};
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Type<detail::any_tag>>
                     && MetaType<meta_result_t<TFunction, Type<detail::any_tag>>>
                     && (MetaType<detail::convert_to_meta<TTypes>> && ...)
        [[nodiscard]] constexpr auto apply(TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> List<as_raw<decltype(as_meta<TTypes>{}.apply(func))>...> {
            return {};
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

        [[nodiscard]] constexpr auto push_front(MetaType auto type) const noexcept
            -> List<typename decltype(type)::type, TTypes...> {
            return {};
        }

        template<typename... TOthers>
        [[nodiscard]] constexpr auto
        push_front([[maybe_unused]] List<TOthers...> list) const noexcept
            -> List<TOthers..., TTypes...> {
            return {};
        }

        [[nodiscard]] constexpr auto push_back(MetaType auto type) const noexcept
            -> List<TTypes..., typename decltype(type)::type> {
            return {};
        }

        template<typename... TOthers>
        [[nodiscard]] constexpr auto
        push_back([[maybe_unused]] List<TOthers...> list) const noexcept
            -> List<TTypes..., TOthers...> {
            return {};
        }

        template<typename... TRHTypes>
            requires(sizeof...(TRHTypes) == sizeof...(TTypes))
        [[nodiscard]] constexpr auto zip([[maybe_unused]] List<TRHTypes...> rhs) const noexcept {
            return std::invoke(
                []<usize... TIndices>(std::index_sequence<TIndices...>) {
                    constexpr auto _list = decltype(rhs){};

                    return mpl::List<
                        as_raw<decltype(make_pair(List{}.template at<TIndices>(),
                                                  _list.template at<TIndices>()))>...>{};
                },
                std::index_sequence_for<TTypes...>{});
        }
    };

    template<typename... TLHTypes, typename... TRHTypes>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const List<TLHTypes...>& lhs,
               [[maybe_unused]] const List<TRHTypes...>& rhs) noexcept -> bool {
        constexpr auto is_same = []<typename TFirst, typename TSecond>(Pair<TFirst, TSecond> pair) {
            if constexpr((MetaType<typename decltype(pair)::first>
                          && MetaType<typename decltype(pair)::second>)
                         || (MetaValue<typename decltype(pair)::first>
                             && MetaValue<typename decltype(pair)::second>)
                         || (MetaPair<typename decltype(pair)::first>
                             && MetaPair<typename decltype(pair)::second>))
            {
                return Value<(typename decltype(pair)::first{} ==
                              typename decltype(pair)::second{}),
                             bool>{};
            }
            else {
                return Value<false, bool>{};
            }
        };

        constexpr auto check_all
            = []<typename... TResults>([[maybe_unused]] List<TResults...> results) {
                  return (TResults{} && ...);
              };

        return check_all(List<TLHTypes...>{}.zip(List<TRHTypes...>{}).for_each(is_same));
    }
} // namespace hyperion::mpl

    #include <hyperion/mpl/pair.h>

namespace hyperion::mpl::_test::list {

    static constexpr auto add_const = [](MetaType auto type) {
        return type.as_const();
    };

    static_assert(std::same_as<decltype(List<int, double>{}.for_each(add_const)),
                               List<const int, const double>>,
                  "hyperion::mpl::List::for_each test case 1 (failing)");

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

    static_assert(List<int, double>{} == List<int, double>{},
                  "hyperion::mpl::List operator== test case 1 (failing)");
    static_assert(List<int, double>{}.for_each(add_const) == List<const int, const double>{},
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
    static_assert(List<int, double>{}.push_front(List<float, usize>{})
                      == List<float, usize, int, double>{},
                  "hyperion::mpl::List::push_front test case 2 (failing)");

    static_assert(List<int, double>{}.push_back(decltype_<float>()) == List<int, double, float>{},
                  "hyperion::mpl::List::push_back test case 1 (failing)");
    static_assert(List<int, double>{}.push_back(List<float, usize>{})
                      == List<int, double, float, usize>{},
                  "hyperion::mpl::List::push_back test case 2 (failing)");

} // namespace hyperion::mpl::_test::list

#endif // HYPERION_MPL_LIST_H
