/// @file list.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming facilities for working with a list of types or values
/// @version 0.7
/// @date 2024-03-05
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
/// #include <hyperion/mpl/metapredicates.h>
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
/// static_assert(constified.all_of(is_const));
/// @endcode
/// @headerfile hyperion/mpl/list.h
///
/// # Exposition-Only Types
///
/// - `struct as_meta`: Exposition-only template metafunction that converts
/// a type, `TType`, to the corresponding metaprogramming type, exposed via
/// the member `using` alias `type`. Maps `TType` in the following manner:
///     - If `TType` is a `MetaType`, maps to
///     `Type<typename as_meta<typename TType::type>::type>`, else
///     - If `TType` is a `MetaValue`, maps to `Value<TType::value>`, else
///     - If `TType` is a `MetaPair`, maps to
///     @code{.cpp}
///     Pair<typename as_meta<typename TType::first>::type,
///          typename as_meta<typename TType::second>::type>
///     @endcode, else
///     - Maps to `Type<TType>`
/// - `struct as_raw`: Exposition-only template metafunction that converts
/// a type, `TType`, to the corresponding raw type, exposed via the member
/// `using` alias `type`. Maps `TType` in the following manner:
///     - If `TType` is a `MetaType`, maps to
///     `typename as_raw<typename TType::type>::type`, else
///     - If `TType` is a `MetaValue`, maps to `Value<TType::value>`, else
///     - If `TType` is a `MetaPair`, maps to
///     @code {.cpp}
///     Pair<typename as_raw<typename TType::first>::type,
///          typename as_raw<typename TType::second>::type>
///     @endcode, else
///     - Maps to `TType`
/// @}

#ifndef HYPERION_MPL_LIST_H
    #define HYPERION_MPL_LIST_H

namespace hyperion::mpl {

    template<typename... TTypes>
    struct List;

    template<typename TFirst, typename TSecond>
    struct Pair;

    /// @brief Tag type indicating that a searching operation in an `mpl::List`
    /// could not find a/the desired result.
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    struct not_found_tag { };

    namespace detail {
        /// @brief Represents an element at index `TIndex` in a `List`
        /// @tparam TIndex the index in the `List` of this element
        /// @tparam TType the `as_meta` type of this element
        template<usize TIndex, typename TType>
        struct element {
            [[nodiscard]] constexpr auto
            at([[maybe_unused]] Value<TIndex> index) const noexcept -> TType {
                return {};
            }
        };

        /// @brief Factory for an indexible representation of the elements of a `List`
        /// @tparam TTypes the elements of the `List`
        template<typename... TTypes>
        struct elements {
            /// @brief Returns an indexible overload set of `at` for the elements of a `List`
            template<usize... TIndices>
            [[nodiscard]] static constexpr auto
            make([[maybe_unused]] std::integer_sequence<usize, TIndices...> seq) noexcept {
                struct ret : element<TIndices, TTypes>... {
                    using element<TIndices, TTypes>::at...;
                };

                return ret{};
            }
        };

        /// @brief Returns the `index`th element of `list`
        /// @tparam TTypes the elements of the `List`
        /// @param index the index of the element of `list` to get
        /// @return the `index`th element of `list`
        template<typename... TTypes>
        [[nodiscard]] constexpr auto at(MetaValue auto index) noexcept {
            return elements<TTypes...>::make(std::make_integer_sequence<usize, sizeof...(TTypes)>{})
                .at(Value<static_cast<usize>(decltype(index)::value), usize>{});
        }

        /// @brief Removes the first element from the list, `TList`, exposing that element as
        /// the member `using` alias `front`, and the list of remaining elements from the list
        /// as the member `using` alias `remaining`
        ///
        /// @tparam TList The list to remove the first element from
        template<typename TList>
        struct pop_front;

        // specialization for removal
        template<template<typename...> typename TList, typename TFront, typename... TTypes>
        struct pop_front<TList<TFront, TTypes...>> {
            using front = TFront;
            using remaining = TList<TTypes...>;
        };

        // specialization for single element list
        template<template<typename...> typename TList, typename TFront>
        struct pop_front<TList<TFront>> {
            using front = TFront;
            using remaining = TList<>;
        };

        // specialization for empty list
        template<template<typename...> typename TList>
        struct pop_front<TList<>> {
            using front = not_found_tag;
            using remaining = TList<>;
        };

        /// @brief Implementation of `remaining` in `pop_back`.
        /// Removes the last element of the list `TList`, assuming `TIndexSequence`
        /// is the index sequence from `0` to `sizeof...(TTypes) - 1`
        ///
        /// @tparam TList The list to remove the last element from
        /// @tparam TIndexSequence The index sequence from `0` to `sizeof...(TTypes) - 1`
        /// @tparam TTypes The type stored in `TList`
        template<template<typename...> typename TList, typename TIndexSequence, typename... TTypes>
        struct pop_back_base;

        // specialization to use the index sequence to drop the last element
        template<template<typename...> typename TList, std::size_t... TIndices, typename... TTypes>
        struct pop_back_base<TList, std::index_sequence<TIndices...>, TTypes...> {
            using remaining = TList<decltype(at<TTypes...>(Value<TIndices>{}))...>;
        };

        /// @brief Removes the last element from the list, `TList`, exposing that element as
        /// the member `using` alias `back`, and the list of remaining elements from the list
        /// as the member `using` alias `remaining`
        ///
        /// @tparam TList The list to remove the last element from
        template<typename TList>
        struct pop_back;

        // specialization for removal
        template<template<typename...> typename TList, typename... TTypes>
        struct pop_back<TList<TTypes...>> {
            using back = decltype(at<TTypes...>(Value<sizeof...(TTypes) - 1>{}));
            using remaining =
                typename pop_back_base<TList,
                                       std::make_index_sequence<sizeof...(TTypes) - 1>,
                                       TTypes...>::remaining;
        };

        // specialization for single element list
        template<template<typename...> typename TList, typename TType>
        struct pop_back<TList<TType>> {
            using back = TType;
            using remaining = TList<>;
        };

        // specialization for empty list
        template<template<typename...> typename TList>
        struct pop_back<TList<>> {
            using back = not_found_tag;
            using remaining = TList<>;
        };
    } // namespace detail

    /// @brief `List` is a metaprogramming type for storing, communicating,
    /// working with, and operating on lists of types or values.
    ///
    /// # Example
    /// @code {.cpp}
    /// #include <hyperion/mpl/list.h>
    /// #include <hyperion/mpl/metapredicates.h>
    ///
    /// using namespace hyperion::mpl;
    ///
    /// constexpr auto add_const = [](MetaType auto type) noexcept {
    ///     return type.add_const();
    /// };
    ///
    /// constexpr auto const_zipped = List<int, double, float>{}
    ///                               .zip(List<u32, usize, i32>{})
    ///                               .apply(add_const);
    /// static_assert(const_zipped == List<Pair<const int, const u32>,
    ///                                  Pair<const double, const usize>,
    ///                                  Pair<const float, const i32>>{});
    /// static_assert(const_zipped.all_of(is_const));
    ///
    /// constexpr auto nums = List<Value<1>, Value<2>, Value<3>>{};
    /// static_assert(nums.accumulate(0_value) == 6_value);
    /// @endcode
    ///
    /// @tparam TTypes The types to represent in the list
    ///
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    template<typename... TTypes>
    struct List {
      private:
        // shorthand to convert the (possibly raw) type `TType` to the
        // appropriate metaprogramming type
        template<typename TType>
        using as_meta = detail::convert_to_meta_t<TType>;

        // shorthand to convert the (possibly metaprogramming) type `TType`
        // to the appropriate raw type
        template<typename TType>
        using as_raw = detail::convert_to_raw_t<TType>;

      public:
        /// @brief Returns the size of this `List`
        /// @return the size of this `List`
        [[nodiscard]] constexpr auto size() const noexcept {
            return Value<sizeof...(TTypes), usize>{};
        }

        /// @brief Returns whether this `List` is empty
        /// @return whether this `List` is empty, as a `Value` specialization
        [[nodiscard]] constexpr auto is_empty() const noexcept {
            return Value<sizeof...(TTypes) == 0, bool>{};
        }

        /// @brief Applies the metafunction `func` to the elements of this `List`,
        /// returning the result as a new `List` specialization.
        ///
        /// Using the exposition-only template metafunctions `as_meta` and `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// applies `func` to each element, `TElement`, of this `List` as if by
        /// `typename as_raw<decltype(typename as_meta<TElement>::type{}.apply(func))>::type`.
        ///
        /// # Requirements
        /// - `func` must be a metafunction invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaFunctionOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `func` must be a metafunction appliable to the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.apply(func)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        ///
        /// constexpr auto add_const = [](MetaType auto type) noexcept {
        ///     return type.add_const();
        /// };
        ///
        /// static_assert(List<int, double>{}.apply(add_const)
        ///               == List<const int, const double>{});
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply to the elements
        /// of this `List`
        /// @param func The metafunction to apply to the elements of this `List`
        /// @return a `List` specialization containing the modified elements
        template<typename TFunction>
            requires(MetaFunctionOf<TFunction, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.apply(TFunction{}), ...); }
        [[nodiscard]] constexpr auto apply(TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept {
            return List<as_raw<decltype(as_meta<TTypes>{}.apply(func))>...>{};
        }

        /// @brief Invokes the function `vis` with each element of this `List`.
        ///
        /// This is the canonical way to iterate through the elements of a `List`.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// applies `vis` to each element, `TElement`, of this `List` as if by
        /// `vis(typename as_meta<TElement>::type{})`.
        ///
        /// # Requirements
        /// - `vis` must be invocable with the corresponding metaprogramming type
        /// for each element of this `List`. This is, for each element, `TElement`,
        /// `std::invocable<TVisitor, typename as_meta<TElement>::type>` must be true.
        /// - The invoke result of `vis` for each element of this `List` must be `void`.
        ///
        /// # Example
        /// @code{.cpp}
        /// constexpr auto example = List<Value<1>, int, float, double, int>{};
        /// constexpr auto count_if = [](auto list, auto predicate) noexcept {
        ///     auto num_satisfied = 0;
        ///     list.for_each([&num_satisfied, predicate](auto element) {
        ///         if(element.satisfies(predicate)) {
        ///             num_satisfied++;
        ///         }
        ///     });
        ///     return num_satisfied;
        /// };
        ///
        /// constexpr auto num_ints = count_if(example, equal_to(decltype_<int>()));
        /// static_assert(num_ints == 2);
        /// @endcode
        ///
        /// @tparam TVisitor the type of the function to invoke with each element
        /// of this `List`
        /// @param vis the function to invoke with each element of this `List`
        template<typename TVisitor>
            requires(std::invocable<TVisitor, as_meta<TTypes>> && ...)
                    && (std::same_as<std::invoke_result_t<TVisitor, as_meta<TTypes>>, void> && ...)
        constexpr auto for_each(TVisitor&& vis) // NOLINT(*-missing-std-forward)
            const noexcept -> void {
            (vis(as_meta<TTypes>{}), ...);
        }

        /// @brief Invokes the function `vis` with the first `count` elements of this `List`.
        ///
        /// This is the canonical way to iterate through a subset of the elements of a `List`.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// applies `vis` to each element, `TElement`, of this `List` as if by
        /// `vis(typename as_meta<TElement>::type{})`.
        ///
        /// # Requirements
        /// - `vis` must be invocable with the corresponding metaprogramming type
        /// for each element of this `List`. This is, for each element, `TElement`,
        /// `std::invocable<TVisitor, typename as_meta<TElement>::type>` must be true.
        /// - The invoke result of `vis` for each element of this `List` must be `void`.
        /// - `count` must be less than or equal to the number of elements of this `List`
        ///
        /// # Example
        /// @code{.cpp}
        /// constexpr auto example = List<Value<1>, int, float, double, int>{};
        /// constexpr auto count_if_in_first_half = [](auto list, auto predicate) noexcept {
        ///     auto num_satisfied = 0;
        ///     list.for_each_n([&num_satisfied, predicate](auto element) {
        ///         if(element.satisfies(predicate)) {
        ///             num_satisfied++;
        ///         }
        ///     }, list.size() / 2_value);
        ///     return num_satisfied;
        /// };
        ///
        /// constexpr auto num_ints = count_if_in_first_half(example, equal_to(decltype_<int>()));
        /// static_assert(num_ints == 1);
        /// @endcode
        ///
        /// @tparam TVisitor the type of the function to invoke with the first `count` elements
        /// of this `List`
        /// @param vis the function to invoke with the first `count` elements of this `List`
        template<typename TVisitor, MetaValue TCount>
            requires(std::invocable<TVisitor, as_meta<TTypes>> && ...)
                    && (std::same_as<std::invoke_result_t<TVisitor, as_meta<TTypes>>, void> && ...)
                    && (TCount::value <= sizeof...(TTypes))
        constexpr auto for_each_n(TVisitor&& vis, [[maybe_unused]] TCount count) const noexcept
            -> void {
            return typename detail::
                pop_back_base<List, std::make_index_sequence<TCount::value>, TTypes...>::remaining{}
                    .for_each(std::forward<TVisitor>(vis));
        }

      private:
        /// @brief Implementation for `accumulate`.
        /// Recursively called for each type in `TTs...`, in pack order.
        ///
        /// `state` may be a different type for each invocation step
        /// in the recursive call chain.
        ///
        /// @tparam TState the type of the initial (at the top level of the call chain)
        /// or current (at any other level of the call chain) state of the accumulation
        /// @tparam TAccumulator the type of the callable to perform the accumulation with
        /// @tparam TTs the remaining metaprogramming types in the list of types to
        /// recursively accumulate
        ///
        /// @param state the initial (at the top level of the call chain) or current
        /// (at any other level of the call chain) state of the accumulation
        /// @param accumulator the callable to perform the accumulation with
        /// @param remaining the remaining metaprogramming types to recursively accumulate
        /// @return the result of the accumulation (at the top level of the call chain)
        /// or the current state of the accumulation (at any other level of the call chain)
        template<typename TState, typename TAccumulator, typename... TTs>
        [[nodiscard]] static constexpr auto
        accumulate_impl(TState&& state,
                        TAccumulator&& accumulator,
                        [[maybe_unused]] List<TTs...> remaining) noexcept {
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

        /// @brief Default accumulation operation.
        /// Performs the arithmetic sum of the two `MetaValue`s
        static constexpr auto sum = [](MetaValue auto _state, MetaValue auto current) noexcept {
            return _state + current;
        };

      public:
        /// @brief Computes the arithmetic sum of `state` and the elements of this `List`.
        ///
        /// # Requirements
        /// - All elements of this `List` must be `MetaValue`s
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.accumulate(4_value) == 10);
        /// @endcode
        ///
        /// @param state the initial state to begin the summation with
        /// @return the arithmetic sum of `state` and the elements of this `List`,
        /// as a `Value` specialization
        [[nodiscard]] constexpr auto accumulate(MetaValue auto state) const noexcept
            requires(std::invocable<decltype(sum), as_meta<decltype(state)>, as_meta<TTypes>>
                     && ...)
        {
            return accumulate_impl(as_meta<decltype(state)>{}, sum, List<as_meta<TTypes>...>{});
        }

        /// @brief Computes the accumulation of `state` and the elements of this `List`.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// and `TElement_N` to represent the type of each element in this `List`,
        /// uses the accumulation operation `accumulator`, performs the accumulation
        /// of the elements of this `List`, in order, by first invoking `accumulator` with
        /// `as_meta<decltype(state)>{}, as_meta<TElement_0>{}`, then continues to call
        /// `accumulator` for each successive `TElement`, using the result of
        /// the previous invocation as the `state` parameter for the next invocation
        /// in the sequence.
        ///
        /// # Requirements
        /// - Using `TElement_N` to represent the type of each element in this `List`
        /// and `TState` to represent both `decltype(state)` and the (possibly different)
        /// type(s) of the return value of each possible invocation of `accumulator`,
        /// `accumulator` must be a callable taking two parameters, separately invocable
        /// with parameters of types `as_meta<TState_N>, as_meta<TElement_N>`,
        /// for each of all `TElement`s and `TState`s. That is, the recursive call chain
        /// @code {.cpp}
        /// accumulator(
        ///     accumulator(
        ///         ...(
        ///             accumulator(as_meta<decltype(state)>{},
        ///                         as_meta<TElement_0>{}),
        ///             as_meta<TElement_1>{}),
        ///         ...),
        ///     as_meta<TElement_N-1>{})
        /// @endcode
        /// must be well-formed.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, double, float, int>{}
        ///               .accumulate(0_value, [](auto state, auto element) {
        ///                 // increment state if `element` is a `MetaType` representing `int`
        ///                 if constexpr(MetaType<decltype(element)>) {
        ///                     if constexpr(decltype(element){} == decltype_<int>()) {
        ///                         return state + 1_value;
        ///                     }
        ///                     else {
        ///                         return state;
        ///                     }
        ///                 }
        ///                 else {
        ///                     return state;
        ///                 }
        ///               }) == 2);
        /// @endcode
        ///
        /// @param state the initial state to begin the accumulation with
        /// @param accumulator the callable to perform the accumulation operation
        /// @return the accumulation of `state` and the elements of this `List`,
        /// according to `accumulator`
        template<typename TDelay = List<as_meta<TTypes>...>>
        [[nodiscard]] constexpr auto accumulate(auto state, auto&& accumulator) const noexcept
            requires std::same_as<TDelay, List<as_meta<TTypes>...>> && requires {
                accumulate_impl(as_meta<decltype(state)>{},
                                std::forward<decltype(accumulator)>(accumulator),
                                TDelay{});
            }
        {
            return accumulate_impl(as_meta<decltype(state)>{},
                                   std::forward<decltype(accumulator)>(accumulator),
                                   List<as_meta<TTypes>...>{});
        }

      private:
        /// @brief Returns the index of the first element of this list that satisfies `predicate`.
        ///
        /// # Requirements
        /// - The value of `index` must be less than or equal to the size of this `List`,
        /// `sizeof...(TTypes)`
        ///
        /// @param predicate the predicate to satisfy
        /// @return The index of the first element to satisfy `predicate`, or if no element
        /// satisfies `predicate`, `Value<sizeof...(TTypes)>`
        template<typename TPredicate>
        [[nodiscard]] static constexpr auto
        find_if_impl([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward))
            noexcept {
            constexpr auto values = []<auto... Indices>(std::index_sequence<Indices...>) {
                constexpr auto _at = [](MetaValue auto index) noexcept {
                    return detail::at<as_meta<TTypes>...>(index);
                };
                return std::array{
                    static_cast<bool>(_at(Value<Indices>{}).satisfies(TPredicate{}))...};
            }(std::make_index_sequence<sizeof...(TTypes)>{});
            constexpr auto first = [values]() {
                for(auto i = 0_usize; i < values.size(); ++i) {
                    if(values[i]) {
                        return i;
                    }
                }
                return values.size();
            }();
            return Value<static_cast<usize>(first), usize>{};
        }

      public:
        /// @brief Returns the first element of this `List` that satisfies the
        /// metafunction predicate `predicate`.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`,
        /// and if satisfied, returns that element.
        ///
        /// If no element satisfying `predicate` is found, returns `Type<not_found_tag>`
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        ///
        /// static_assert(List<int, const double, float>{}.find_if(is_const)
        ///               == decltype_<const double>());
        /// static_assert(List<int, const double, float>{}.find_if(is_volatile)
        ///               == decltype_<not_found_tag>());
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.find_if(is_const)
        ///               == decltype_<not_found_tag>());
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use for searcch
        /// @param predicate The metapredicate to use for search
        /// @return the first element satisfying `predicate`, or `Type<not_found_tag>`
        /// if no element satisfies `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto find_if(TPredicate&& predicate) const noexcept {
            auto result = find_if_impl(std::forward<TPredicate>(predicate));
            if constexpr(decltype(result){} == Value<sizeof...(TTypes), usize>{}) {
                return decltype_<not_found_tag>();
            }
            else {
                return at(result);
            }
        }

        /// @brief Returns the first element of this `List` that is equal to `value`.
        ///
        /// If no element equal to `value` is found, returns `Type<not_found_tag>`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, const double, float>{}.find(decltype_<const double>())
        ///               == decltype_<const double>());
        /// static_assert(List<int, const double, float>{}.find(decltype_<usize>())
        ///               == decltype_<not_found_tag>());
        /// static_assert(List<int, const double, float>{}.find(1_value)
        ///               == decltype_<not_found_tag>());
        /// static_assert(List<int, Value<1>, const double, float>{}.find(1_value)
        ///               == 1);
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.find(4_value)
        ///               == decltype_<not_found_tag>());
        /// @endcode
        ///
        /// @param value The metaprogramming value to search for
        /// @return the first element equal to `value`, or `Type<not_found_tag>`
        /// if no element equals `value`
        [[nodiscard]] constexpr auto find(auto value) const noexcept {
            return find_if(equal_to(value));
        }

        /// @brief Returns the number of elements of this `List` that satisfy the
        /// metafunction predicate `predicate`, as a `Value` specialization.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`,
        /// and if satisfied, increments the internal count.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        ///
        /// static_assert(List<int, const double, float>{}.count_if(is_const)
        ///               == 1_value);
        /// static_assert(List<int, const double, float>{}.count_if(is_volatile)
        ///               == 0_value);
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.count_if(is_const)
        ///               == 0_value);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return the number of elements satisfying `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto
        count_if([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept {
            constexpr auto accumulator = [](auto state, auto element) {
                if constexpr((decltype(element){}.satisfies(TPredicate{}))) {
                    return state + 1_value;
                }
                else {
                    return state;
                }
            };

            return accumulate(0_value, accumulator);
        }

        /// @brief Returns the number of elements of this `List` that are equal to `value`,
        /// as a `Value` specialization
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, const double, float>{}.count(decltype_<const double>())
        ///               == 1);
        /// static_assert(List<int, const double, float>{}.count(decltype_<usize>())
        ///               == 0);
        /// static_assert(List<int, const double, float, int>{}.count(decltype_<int>())
        ///               == 2);
        /// static_assert(List<int, const double, float>{}.count(1_value)
        ///               == 0);
        /// static_assert(List<int, Value<1>, const double, float>{}.count(1_value)
        ///               == 1);
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.count(4_value)
        ///               == 0);
        /// @endcode
        ///
        /// @param value The metaprogramming value to search for
        /// @return the number of elements of this `List` that are equal to `value`,
        /// as a `Value` specialization
        [[nodiscard]] constexpr auto count([[maybe_unused]] auto value) const noexcept {
            return count_if(equal_to(value));
        }

        /// @brief Returns whether this `List` contains an element equal to `value`,
        /// as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, const double, float>{}.contains(decltype_<const double>()));
        /// static_assert(not List<int, const double, float>{}.contains(decltype_<usize>()));
        /// static_assert(not List<int, const double, float>{}.contains(1_value));
        /// static_assert(List<int, Value<1>, const double, float>{}.contains(1_value));
        /// static_assert(not List<Value<1>, Value<2>, Value<3>>{}.contains(4_value));
        /// @endcode
        ///
        /// @param value The metaprogramming value to search for
        /// @return whether this `List` contains at least one element equal to `value`,
        /// as a `Value` specialization
        [[nodiscard]] constexpr auto contains(auto value) const noexcept {
            return find_if(equal_to(value)).satisfies(not_equal_to(decltype_<not_found_tag>()));
        }

        /// @brief Returns whether all elements of this `List` satisfy the
        /// metafunction predicate `predicate`, as a `Value` specialization.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<const int, const double, const float>{}.all_of(is_const));
        /// static_assert(not List<int, const double, float>{}.all_of(is_const));
        /// static_assert(not List<Value<1>, Value<2>, Value<3>>{}.all_of(is_const));
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return whether all elements satisfy `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto all_of(TPredicate&& predicate) const noexcept {
            return count_if(std::forward<TPredicate>(predicate))
                .satisfies(equal_to(Value<sizeof...(TTypes)>{}));
        }

        /// @brief Returns whether any elements of this `List` satisfy the
        /// metafunction predicate `predicate`, as a `Value` specialization.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<const int, const double, const float>{}.any_of(is_const));
        /// static_assert(List<int, const double, float>{}.any_of(is_const));
        /// static_assert(not List<int, double, float>{}.any_of(is_const));
        /// static_assert(not List<Value<1>, Value<2>, Value<3>>{}.all_of(is_const));
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return whether any elements satisfy `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto any_of(TPredicate&& predicate) const noexcept {
            return find_if(std::forward<TPredicate>(predicate))
                .satisfies(not_equal_to(decltype_<not_found_tag>()));
        }

        /// @brief Returns whether zero elements of this `List` satisfy the
        /// metafunction predicate `predicate`, as a `Value` specialization.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(not List<const int, const double, const float>{}.none_of(is_const));
        /// static_assert(not List<int, const double, float>{}.none_of(is_const));
        /// static_assert(List<int, double, float>{}.none_of(is_const));
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.none_of(is_const));
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return whether zero elements satisfy `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto none_of(TPredicate&& predicate) const noexcept {
            return find_if(std::forward<TPredicate>(predicate))
                .satisfies(equal_to(decltype_<not_found_tag>()));
        }

        /// @brief Returns the index of the first element satisfying `predicate`,
        /// as a `Value` specialization or `Value<sizeof...(TTypes)>` if no element
        /// satisfies `predicate`.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`,
        /// and if satisfied, returns the index of that element.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, const double, float>{}.index_if(is_const)
        ///               == 1_value);
        /// static_assert(List<int, double, const float>{}.index_if(is_const)
        ///               == 2_value);
        /// static_assert(List<int, double, float>{}.index_if(is_const)
        ///               == 3_value);
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.index_if(is_const)
        ///               == 3_value);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return the index of the first element to satisfy `predicate`,
        /// or `sizeof...(TTypes)` if no element satisfies `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto index_if(TPredicate&& predicate) const noexcept {
            return find_if_impl(std::forward<TPredicate>(predicate));
        }

        /// @brief Returns the index of the first element of this `List`
        /// that is equal to `value`, as a `Value` specialization
        ///
        /// If no element equal to `value` is found, returns `Value<sizeof...(TTypes)>`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, const double, float>{}.index_of(decltype_<const double>())
        ///               == 1);
        /// static_assert(List<int, const double, float>{}.index_of(decltype_<usize>())
        ///               == 3);
        /// static_assert(List<int, const double, float>{}.index_of(1_value)
        ///               == 3);
        /// static_assert(List<int, const double, float, int>{}.index_of(decltype_<int>())
        ///               == 0);
        /// static_assert(List<int, Value<1>, const double, float>{}.index_of(1_value)
        ///               == 1);
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.index_of(4_value)
        ///               == 3);
        /// @endcode
        ///
        /// @param value The metaprogramming value to search for
        /// @return the index of the first element equal to `value`,
        /// or `Value<sizeof...(TTypes)>` if no element equals `value`
        [[nodiscard]] constexpr auto index_of(auto value) const noexcept {
            return index_if(equal_to(value));
        }

        /// @brief Returns a `List` containing only the elements of this `List` that
        /// satisfy `predicate`.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`,
        /// and if satisfied, that element is included in the returned `List`.
        ///
        /// Relative ordering of elements is maintained in the returned `List`.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_value = [](MetaValue auto val) noexcept {
        ///     return Value<true>{};
        /// };
        ///
        /// static_assert(List<int, const double, float>{}.filter(is_const)
        ///               == List<const double>{});
        /// static_assert(List<int, double, float>{}.filter(is_const)
        ///               == List<>{});
        /// static_assert(List<int, double, const float>{}.filter(is_const)
        ///               == List<const float>{});
        /// static_assert(List<int, Value<1>, double, Value<2>, float>{}.filter(is_value)
        ///               == List<Value<1>, Value<2>>{});
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return the `List` containing only the elements of this `List` that
        /// satisfy `predicate`
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto
        filter([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept {
            constexpr auto append_valid = [](auto list, auto element) noexcept {
                if constexpr(decltype(element){}.satisfies(TPredicate{})) {
                    return list.push_back(element);
                }
                else {
                    return list;
                }
            };

            return accumulate(List<>{}, append_valid);
        }

        /// @brief Returns a copy of this `List`, but with all elements that satisfy
        /// `predicate` removed.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it satisfies
        /// `predicate`, as if by `typename as_meta<TElement>::type{}.satisfies(predicate)`,
        /// and if satisfied, that element is _not_ included in the returned `List`.
        ///
        /// Relative ordering of elements is maintained in the returned `List`.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_value = [](MetaValue auto val) noexcept {
        ///     return Value<true>{};
        /// };
        ///
        /// static_assert(List<int, const double, float>{}.remove_if(is_const)
        ///               == List<int, float>{});
        /// static_assert(List<int, double, float>{}.remove_if(is_const)
        ///               == List<int, double, float>{});
        /// static_assert(List<int, double, const float>{}.remove_if(is_const)
        ///               == List<int, double>{});
        /// static_assert(List<int, Value<1>, double, Value<2>, float>{}.remove_if(is_value)
        ///               == List<int, double, float>{});
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return a copy of this `List`, but with all elements that satisfy
        /// `predicate` removed
        template<typename TPredicate>
            requires(MetaPredicateOf<TPredicate, as_meta<TTypes>> && ...)
                    || requires { (as_meta<TTypes>{}.satisfies(TPredicate{}), ...); }
        [[nodiscard]] constexpr auto
        remove_if([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept {
            constexpr auto append_valid = [](auto list, auto element) noexcept {
                if constexpr(not decltype(element){}.satisfies(TPredicate{})) {
                    return list.push_back(element);
                }
                else {
                    return list;
                }
            };

            return accumulate(List<>{}, append_valid);
        }

        /// @brief Returns a copy of this `List`, but with all elements that
        /// are equal to `value` removed.
        ///
        /// Using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// checks each element, `TElement`, of this `List` to see whether it is equal to
        /// `value`, as if by `typename as_meta<TElement>::type{}.satisfies(equal_to(value))`,
        /// and if equal, that element is _not_ included in the returned `List`.
        ///
        /// Relative ordering of elements is maintained in the returned `List`.
        ///
        /// # Requirements
        /// - `predicate` must be a metapredicate invocable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `func` must be a `MetaPredicateOf<TFunction, typename as_meta<TElement>::type>`
        /// for each element,`TElement`, of this `List`
        /// - OR, `predicate` must be a metapredicate satisfiable with the corresponding
        /// metaprogramming type of each element in this `List`. That is,
        /// `typename as_meta<TElement>::type{}.satisfy(predicate)` must be well formed
        /// for each element, `TElement`, of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, const double, float>{}.remove(decltype_<const double>())
        ///               == List<int, float>{});
        /// static_assert(List<int, double, float>{}.remove(decltype_<u32>())
        ///               == List<int, double, float>{});
        /// static_assert(List<int, double, const float>{}.remove(decltype_<const float>())
        ///               == List<int, double>{});
        /// static_assert(List<int, Value<1>, double, Value<2>, float>{}.remove(1_value)
        ///               == List<int, double, Value<2>, float>{});
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metapredicate to use
        /// @param predicate The metapredicate to use
        /// @return a copy of this `List`, but with all elements that are equal to
        /// `value` removed
        [[nodiscard]] constexpr auto remove(auto value) const noexcept {
            return remove_if(equal_to(value));
        }

        /// @brief Returns a `List` containing the elements of this `List` occurring at
        /// the indices specified in `list`.
        ///
        /// Ordering of elements in the returned list follows ordering of indices
        /// specified in `list`.
        ///
        /// # Requirements
        /// - `list` must be a `MetaList` type
        /// - Every element of `list` must be a `MetaValue`
        /// - Every element of `list` must have a value in the range `[0, this->size())`
        ///
        /// # Example
        /// @code {.cpp}
        ///
        /// constexpr auto sifter = List<Value<1>, Value<2>>{};
        ///
        /// static_assert(List<int, const double, float>{}.sift(sifter)
        ///               == List<const double, float>{});
        /// static_assert(List<int, double, float>{}.sift(sifter)
        ///               == List<double, float>{});
        /// static_assert(List<int, Value<1>, double, Value<2>, float>{}.sift(sifter)
        ///               == List<Value<1>, double>{});
        /// @endcode
        ///
        /// @tparam TList The metaprogramming list class template
        /// @tparam TValues The metaprogramming value types stored in `list`
        /// @param list The metaprogramming list containing the indices of the
        /// elements of this `List` to get
        /// @return a `List` containing the elements of this `List` occurring
        /// at the indices specified in `list`
        template<template<typename...> typename TList, typename... TValues>
            requires MetaList<TList<TValues...>> && (MetaValue<TValues> && ...)
                     && ((TValues::value < sizeof...(TTypes)) && ...)
                     && ((TValues::value >= 0) && ...)
        [[nodiscard]] constexpr auto sift([[maybe_unused]] TList<TValues...> list) const noexcept {
            return List<as_raw<decltype(at(TValues{}))>...>{};
        }

        /// @brief Converts the elements of this list into a parameter pack, and invokes
        /// `func` with that pack, returning the result of the invocation.
        ///
        /// # Requirements
        /// - `func` must be invocable with the elements of this `List` as a parameter pack.
        /// That is, using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// `std::invoke(std::forward<TFunction>(func), typename as_meta<TTypes>::type{}...)`
        /// must be well-formed
        ///
        /// # Example
        /// @code{.cpp}
        /// constexpr auto sum = [](MetaValue auto... values) noexcept {
        ///     return (values + ...);
        /// };
        ///
        /// static_assert(List<Value<1>, Value<2>, Value<3>>{}.unwrap(sum) == 6);
        /// @endcode
        ///
        /// @tparam TFunction the type of the callable to invoke
        /// @param func the callable to invoke with the elements of this `List`
        /// as the parameters
        /// @return the result of invoking `func` with the elements of this `List`
        template<typename TFunction>
            requires std::invocable<TFunction, as_meta<TTypes>...>
        [[nodiscard]] constexpr auto unwrap(TFunction&& func) const noexcept
            -> std::invoke_result_t<TFunction, as_meta<TTypes>...> {
            return std::forward<TFunction>(func)(as_meta<TTypes>{}...);
        }

        /// @brief Returns the element of this `List` at index `TIndex`.
        ///
        /// Given type, `type`, being the element at index `TIndex` of this `List`, and
        /// using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns the element at index `TIndex`, converted to the corresponding `mpl`
        /// metaprogramming type, as if by `return typename as_meta<type>::type{};`
        ///
        /// # Requirements
        /// - `TIndex` must be less than the size of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, double, float>{}.at<0>() == decltype_<int>());
        /// static_assert(List<int, double, float>{}.at<1>() == decltype_<double>());
        /// static_assert(List<int, double, float>{}.at<2>() == decltype_<float>());
        /// @endcode
        ///
        /// @tparam TIndex the index of the element to access
        /// @return the element at index `TIndex`
        template<usize TIndex>
            requires(TIndex < sizeof...(TTypes))
        [[nodiscard]] constexpr auto at() const noexcept {
            return detail::at<as_meta<TTypes>...>(Value<TIndex>{});
        }

        /// @brief Returns the element of this `List` at index, `index`.
        ///
        /// Given type, `type`, being the element at index, `index`, of this `List`, and
        /// using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns the element at `index`, converted to the corresponding `mpl`
        /// metaprogramming type, as if by `return typename as_meta<type>::type{};`
        ///
        /// # Requirements
        /// - `index` must be less than the size of this `List`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, double, float>{}.at(0_value) == decltype_<int>());
        /// static_assert(List<int, double, float>{}.at(1_value) == decltype_<double>());
        /// static_assert(List<int, double, float>{}.at(2_value) == decltype_<float>());
        /// @endcode
        ///
        /// @param index the index of the element to access
        /// @return the element at index `TIndex`
        [[nodiscard]] constexpr auto at(MetaValue auto index) const noexcept
            requires(decltype(index)::value < sizeof...(TTypes))
        {
            return detail::at<as_meta<TTypes>...>(index);
        }

        /// @brief Returns the first element of this `List`
        ///
        /// Given type, `type`, being the first element of this `List`, and
        /// using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns the first element, converted to the corresponding `mpl`
        /// metaprogramming type, as if by `return typename as_meta<type>::type{};`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, double, float>{}.front() == decltype_<int>());
        /// static_assert(List<double, int, float>{}.front() == decltype_<double>());
        /// static_assert(List<float, double, int>{}.front() == decltype_<float>());
        /// @endcode
        ///
        /// @return the first element of this `List`
        [[nodiscard]] constexpr auto front() const noexcept {
            return detail::at<as_meta<TTypes>...>(0_value);
        }

        /// @brief Returns the last element of this `List`
        ///
        /// Given type, `type`, being the last element of this `List`, and
        /// using the exposition-only template metafunction `as_meta`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns the last element, converted to the corresponding `mpl`
        /// metaprogramming type, as if by `return typename as_meta<type>::type{};`
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.back() == decltype_<int>());
        /// static_assert(List<int, float, double>{}.back() == decltype_<double>());
        /// static_assert(List<int, double, float>{}.back() == decltype_<float>());
        /// @endcode
        ///
        /// @return the last element of this `List`
        [[nodiscard]] constexpr auto back() const noexcept {
            return detail::at<as_meta<TTypes>...>(Value<sizeof...(TTypes) - 1>{});
        }

        /// @brief Returns a copy of this `List`,
        /// with `type` prepended to the beginning of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `typename as_raw<decltype(type)>::type, TTypes...`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_front(decltype_<int>())
        ///               == List<int, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(decltype_<float>())
        ///               == List<float, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(decltype_usize>())
        ///               == List<usize, float, double, int>{});
        /// @endcode
        ///
        /// @param type The `Type` to prepend
        /// @return a copy of this `List`,
        /// with `type` prepended to the beginning of the `List`.
        [[nodiscard]] constexpr auto push_front(MetaType auto type) const noexcept {
            return List<as_raw<decltype(type)>, TTypes...>{};
        }

        /// @brief Returns a copy of this `List`,
        /// with `value` prepended to the beginning of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `typename as_raw<decltype(value)>::type, TTypes...`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_front(1_value)
        ///               == List<Value<1>, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(2_value)
        ///               == List<Value<2>, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(3_value)
        ///               == List<Value<3>, float, double, int>{});
        /// @endcode
        ///
        /// @param value The `Value` to prepend
        /// @return a copy of this `List`,
        /// with `value` prepended to the beginning of the `List`.
        [[nodiscard]] constexpr auto push_front(MetaValue auto value) const noexcept {
            return List<as_raw<decltype(value)>, TTypes...>{};
        }

        /// @brief Returns a copy of this `List`,
        /// with `pair` prepended to the beginning of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `typename as_raw<decltype(pair)>::type, TTypes...`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_front(Pair<int, double>{})
        ///               == List<Pair<int, double>, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(Pair<Value<1>, float>{})
        ///               == List<Pair<Value<1>, float>, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(Pair<usize, void>{})
        ///               == List<Pair<usize, void>, float, double, int>{});
        /// @endcode
        ///
        /// @param pair The `Pair` to prepend
        /// @return a copy of this `List`,
        /// with `pair` prepended to the beginning of the `List`.
        [[nodiscard]] constexpr auto push_front(MetaPair auto pair) const noexcept {
            return List<as_raw<decltype(pair)>, TTypes...>{};
        }

        /// @brief Returns a copy of this `List`,
        /// with the elements of `list` prepended to the beginning of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `TOthers..., TTypes...`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_front(List<Value<1>, int, double>{})
        ///               == List<Value<1>, int, double, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(List<usize, Pair<i32, f32>, i64>{})
        ///               == List<usize, Pair<i32, f32>, i64, float, double, int>{});
        /// static_assert(List<float, double, int>{}.push_front(List<i64, f64, Value<2>>{})
        ///               == List<i64, f64, Value<2>, float, double, int>{});
        /// @endcode
        ///
        /// @tparam TOthers the elements of `list`
        /// @param list the list of elements to prepend
        /// @return a copy of this `List`,
        /// with the elements of `list` prepended to the beginning of the `List`.
        template<typename... TOthers>
        [[nodiscard]] constexpr auto
        push_front([[maybe_unused]] List<TOthers...> list) const noexcept
            -> List<TOthers..., TTypes...> {
            return {};
        }

        /// @brief Returns a copy of this `List`,
        /// with `type` appended to the end of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `TTypes..., typename as_raw<decltype(type)>::type`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_back(decltype_<int>())
        ///               == List<float, double, int, int>{});
        /// static_assert(List<float, double, int>{}.push_back(decltype_<float>())
        ///               == List<float, double, int, float>{});
        /// static_assert(List<float, double, int>{}.push_back(decltype_usize>())
        ///               == List<float, double, int, usize>{});
        /// @endcode
        ///
        /// @param type The `Type` to append
        /// @return a copy of this `List`,
        /// with `type` appended to the end of the `List`.
        [[nodiscard]] constexpr auto push_back(MetaType auto type) const noexcept {
            return List<TTypes..., typename decltype(type)::type>{};
        }

        /// @brief Returns a copy of this `List`,
        /// with `value` appended to the end of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `TTypes..., typename as_raw<decltype(value)>::type`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_back(1_value)
        ///               == List<float, double, int, Value<1>>{});
        /// static_assert(List<float, double, int>{}.push_back(2_value)
        ///               == List<float, double, int, Value<2>>{});
        /// static_assert(List<float, double, int>{}.push_back(3_value)
        ///               == List<float, double, int, Value<3>>{});
        /// @endcode
        ///
        /// @param value The `Value` to append
        /// @return a copy of this `List`,
        /// with `value` appended to the end of the `List`.
        [[nodiscard]] constexpr auto push_back(MetaValue auto value) const noexcept {
            return List<TTypes..., decltype(value)>{};
        }

        /// @brief Returns a copy of this `List`,
        /// with `pair` appended to the end of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `TTypes..., typename as_raw<decltype(pair)>::type`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_back(Pair<int, double>{})
        ///               == List<float, double, int, Pair<int, double>>{});
        /// static_assert(List<float, double, int>{}.push_back(Pair<Value<1>, float>{})
        ///               == List<float, double, int, Pair<Value<1>, float>>{});
        /// static_assert(List<float, double, int>{}.push_back(Pair<usize, i32>{})
        ///               == List<float, double, int, Pair<usize, i32>>{});
        /// @endcode
        ///
        /// @param pair The `Pair` to append
        /// @return a copy of this `List`,
        /// with `pair` appended to the end of the `List`.
        [[nodiscard]] constexpr auto push_back(MetaPair auto pair) const noexcept {
            return List<TTypes..., decltype(pair)>{};
        }

        /// @brief Returns a copy of this `List`,
        /// with the elements of `list` appended to the end of the `List`.
        ///
        /// Using the exposition-only template metafunction `as_raw`
        /// (see the corresponding section in the @ref list module-level documentation),
        /// returns a `List` containing `TTypes..., TOthers...`.
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<float, double, int>{}.push_back(List<Value<1>, int, double>{})
        ///               == List<float, double, int, Value<1>, int, double>{});
        /// static_assert(List<float, double, int>{}.push_back(List<usize, Pair<i32, f32>, i64>{})
        ///               == List<float, double, int, usize, Pair<i32, f32>, i64>{});
        /// static_assert(List<float, double, int>{}.push_back(List<i64, f64, Value<2>>{})
        ///               == List<float, double, int, i64, f64, Value<2>>{});
        /// @endcode
        ///
        /// @tparam TOthers the elements of `list`
        /// @param list the list of elements to append
        /// @return a copy of this `List`,
        /// with the elements of `list` appended to the end of the `List`.
        template<typename... TOthers>
        [[nodiscard]] constexpr auto
        push_back([[maybe_unused]] List<TOthers...> list) const noexcept
            -> List<TTypes..., TOthers...> {
            return {};
        }

        /// @brief Returns a copy of this `List` with the first element removed
        /// @return a copy of this `List` with the first element removed
        [[nodiscard]] constexpr auto pop_front() const noexcept {
            constexpr auto to_raw = []<typename... TMetas>([[maybe_unused]] List<TMetas...> list) {
                return List<as_raw<TMetas>...>{};
            };
            return to_raw(typename detail::pop_front<List<as_meta<TTypes>...>>::remaining{});
        }

        /// @brief Returns a copy of this `List` with the last element removed
        /// @return a copy of this `List` with the last element removed
        [[nodiscard]] constexpr auto pop_back() const noexcept {
            constexpr auto to_raw = []<typename... TMetas>([[maybe_unused]] List<TMetas...> list) {
                return List<as_raw<TMetas>...>{};
            };
            return to_raw(typename detail::pop_back<List<as_meta<TTypes>...>>::remaining{});
        }

        /// @brief Converts the elements of this `List` and `rhs` into a single list
        /// of `Pair`s of elements.
        ///
        /// Returns a `List` of `Pair`s of elements, created as if by
        /// @code {.cpp}
        /// List<Pair<this->at(0), rhs.at(0)>,
        ///      Pair<this->at(1), rhs.at(1)>,
        ///      ...,
        ///      Pair<this->at(this->size() - 1), rhs.at(rhs.size() - 1)>>{}
        /// @endcode
        ///
        /// # Requirements
        /// - this `List` and `rhs` must be the same size
        ///
        /// # Example
        /// @code {.cpp}
        /// static_assert(List<int, double>{}.zip(List<u32, u64>{})
        ///               == List<Pair<int, u32>, Pair<double, u64>>{});
        /// @endcode
        ///
        /// @tparam TRHTypes the elements of `rhs`
        /// @param rhs the `List` of elements to zip with the elements of this `List`
        /// @return a `List` containing the elements of this `List` zipped with the
        /// elements of `rhs`
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

    /// @brief Equality comparison operator for `List`
    ///
    /// Checks that each element of `lhs` is equal to the corresponding (by index)
    /// element of `rhs`, as if by `equal_to(lhs.at(index))(rhs.at(index))`.
    ///
    /// # Example
    /// @code {.cpp}
    /// static_assert(List<int, double, float>{}
    ///               == List<int, double, float>{});
    /// static_assert(List<Value<1, i32>, double, float>{}
    ///               == List<Value<1, u32>, double, float>{});
    /// @endcode
    ///
    /// @tparam TLHTypes the elements of the `lhs` `List`
    /// @tparam TRHTypes the elements of the `rhs` `List`
    /// @param lhs the `List` to compare
    /// @param rhs the `List` to compare to
    /// @return whether the elements of `lhs` are equal to the elements of `rhs`
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    template<typename... TLHTypes, typename... TRHTypes>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const List<TLHTypes...>& lhs,
               [[maybe_unused]] const List<TRHTypes...>& rhs) noexcept {
        if constexpr(sizeof...(TLHTypes) != sizeof...(TRHTypes)) {
            return Value<false>{};
        }
        else {
            constexpr auto is_same
                = []<typename TFirst, typename TSecond>(Pair<TFirst, TSecond> pair)
                requires(MetaType<typename decltype(pair)::first>
                         || MetaValue<typename decltype(pair)::first>
                         || MetaPair<typename decltype(pair)::first>)
                        && (MetaType<typename decltype(pair)::second>
                            || MetaValue<typename decltype(pair)::second>
                            || MetaPair<typename decltype(pair)::second>)
            {
                return equal_to(typename decltype(pair)::first{})(
                    typename decltype(pair)::second{});
            };

            constexpr auto check_all = [](auto... results) noexcept {
                return (results && ...);
            };

            return List<TLHTypes...>{}.zip(List<TRHTypes...>{}).apply(is_same).unwrap(check_all);
        }
    }

    /// @brief Inequality comparison operator for `List`
    ///
    /// Checks that any element of `lhs` is _not_ equal to the corresponding (by index)
    /// element of `rhs`, as if by `not_equal_to(lhs.at(index))(rhs.at(index))`.
    ///
    /// # Example
    /// @code {.cpp}
    /// static_assert(List<int, double, float>{}
    ///               != List<float, double, float>{});
    /// static_assert(List<Value<1, i32>, double, float>{}
    ///               != List<Value<2, u32>, double, float>{});
    /// @endcode
    ///
    /// @tparam TLHTypes the elements of the `lhs` `List`
    /// @tparam TRHTypes the elements of the `rhs` `List`
    /// @param lhs the `List` to compare
    /// @param rhs the `List` to compare to
    /// @return whether any element of `lhs` is _not_ equal to the
    /// corresponding element of `rhs`
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    template<typename... TLHTypes, typename... TRHTypes>
    [[nodiscard]] constexpr auto
    operator!=([[maybe_unused]] const List<TLHTypes...>& lhs,
               [[maybe_unused]] const List<TRHTypes...>& rhs) noexcept {
        if constexpr(sizeof...(TLHTypes) != sizeof...(TRHTypes)) {
            return Value<true>{};
        }
        else {
            constexpr auto is_same
                = []<typename TFirst, typename TSecond>(Pair<TFirst, TSecond> pair)
                requires(MetaType<typename decltype(pair)::first>
                         || MetaValue<typename decltype(pair)::first>
                         || MetaPair<typename decltype(pair)::first>)
                        && (MetaType<typename decltype(pair)::second>
                            || MetaValue<typename decltype(pair)::second>
                            || MetaPair<typename decltype(pair)::second>)
            {
                return not_equal_to(typename decltype(pair)::first{})(
                    typename decltype(pair)::second{});
            };

            constexpr auto check_all = [](auto... results) noexcept {
                return (results || ...);
            };

            return List<TLHTypes...>{}.zip(List<TRHTypes...>{}).apply(is_same).unwrap(check_all);
        }
    }

    /// @brief Creates an `mpl::List` representing the types of the given arguments, `types`
    ///
    /// # Example
    /// @code{.cpp}
    /// static_assert(make_list(1, 1.0, 1_usize, 1_value)
    ///               == List<int, double, usize, Value<1_usize>>{});
    /// @endcode
    ///
    /// @tparam TTypes the types to store in the `mpl::List`
    /// @param types the arguments of the types to represent
    /// @return an `mpl::List` representing the types of the given arguments
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    template<typename... TTypes>
    [[nodiscard]] constexpr auto
    make_list([[maybe_unused]] TTypes&&... types) // NOLINT(*-missing-std-forward)
        noexcept {
        return List<detail::convert_to_raw_t<detail::convert_to_meta_t<TTypes>>...>{};
    }

    /// @brief Creates an `mpl::List` representing the specified types
    ///
    /// # Example
    /// @code{.cpp}
    /// static_assert(make_list<int, double, usize, Value<1>, Type<int>>()
    ///               == List<int, double, usize, Value<1>, int>{});
    /// @endcode
    ///
    /// @tparam TTypes the types to store in the `mpl::List`
    /// @return an `mpl::List` representing `TTypes...`
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    template<typename... TTypes>
    [[nodiscard]] constexpr auto make_list() noexcept {
        return List<detail::convert_to_raw_t<detail::convert_to_meta_t<TTypes>>...>{};
    }

    namespace detail {
        /// @brief Overload set to map to the Range Adaptor of the given bound
        /// (bound as in `std::bind_back`) ranges object
        template<template<typename...> typename TBoundRange, typename TAdaptor, typename TFunction>
        auto get_adaptor(TBoundRange<TAdaptor, TFunction>) -> TAdaptor;
        template<template<typename...> typename TFunctor,
                 template<typename...>
                 typename TBoundRange,
                 typename TAdaptor,
                 typename TFunction>
        auto get_adaptor(TFunctor<TBoundRange<TAdaptor, TFunction>>) -> TAdaptor;
        template<typename TType>
        auto get_adaptor(TType) -> void;

        /// @brief Overload set to map to the predicate/function of the given bound
        /// (bound as in `std::bind_back`) ranges object
        template<template<typename...> typename TBoundRange, typename TAdaptor, typename TFunction>
        auto get_function(TBoundRange<TAdaptor, TFunction>) -> TFunction;
        template<template<typename...> typename TFunctor,
                 template<typename...>
                 typename TBoundRange,
                 typename TAdaptor,
                 typename TFunction>
        auto get_function(TFunctor<TBoundRange<TAdaptor, TFunction>>) -> TFunction;
        template<template<typename...> typename TFunctor,
                 template<typename...>
                 typename TBoundRange,
                 template<typename...>
                 typename TApplicator,
                 typename TAdaptor,
                 typename TFunction>
        auto get_function(TFunctor<TBoundRange<TAdaptor, TApplicator<TFunction>>>) -> TFunction;
        template<typename TType>
        auto get_function(TType) -> void;

    #if HYPERION_PLATFORM_COMPILER_IS_MSVC
        /// @brief Used to extend the lifetime of the given value in a `constexpr` context.
        /// MSVC can be bad about assuming an object's lifetime has ended in `constexpr`
        /// contexts when it really hasn't.
        ///
        /// This is a hack to prevent that.
        template<template<typename...> typename TTemplate, typename TType, typename... TTypes>
        constexpr auto extend_constexpr_lifetime(const TTemplate<TType, TTypes...>&) {
            return TTemplate<TType, TTypes...>{TTypes{}...};
        }
        /// @brief Used to extend the lifetime of the given value in a `constexpr` context.
        /// MSVC can be bad about assuming an object's lifetime has ended in `constexpr`
        /// contexts when it really hasn't.
        ///
        /// This is a hack to prevent that.
        template<typename TType>
        constexpr auto extend_constexpr_lifetime([[maybe_unused]] const TType& value) {
            return TType{};
        }
    #endif // HYPERION_PLATFORM_COMPILER_IS_MSVC

        /// @brief Statically stack-allocated vector containing elements of type `TType`,
        /// up to `TCapacity` number of elements
        ///
        /// @note This is only a minimal implementation providing enough functionality to
        /// support a vector containing primitive types
        template<typename TType, usize TCapacity>
        class static_vector {
          public:
            template<typename TValue>
            constexpr auto push_back(TValue&& value) noexcept(
                decltype_<TType>().is_noexcept_constructible_from(decltype_<decltype(value)>()))
                -> void {
                // NOLINTNEXTLINE(*-pro-bounds-constant-array-index)
                m_values[m_size++] = std::forward<TValue>(value);
            }

            [[nodiscard]] constexpr auto operator[](auto index) const noexcept -> const TType& {
                // NOLINTNEXTLINE(*-pro-bounds-constant-array-index)
                return m_values[index];
            }

            [[nodiscard]] constexpr auto size() const noexcept -> usize {
                return m_size;
            }

            [[nodiscard]] constexpr auto begin() noexcept {
                return m_values.begin();
            }

            [[nodiscard]] constexpr auto end() noexcept {
                return m_values.begin() + m_size;
            }

            [[nodiscard]] constexpr auto begin() const noexcept {
                return m_values.begin();
            }

            [[nodiscard]] constexpr auto end() const noexcept {
                return m_values.begin() + m_size;
            }

          private:
            std::array<TType, TCapacity> m_values = {};
            usize m_size = 0_usize;
        };

        /// @brief Return a range of size `end - begin`,
        /// starting at `begin` and incrementing for each successive element,
        /// until the size is reached.
        ///
        /// @param begin The initial value
        /// @param end The one-after-the-end value
        /// @return a range of values from `begin` to `end`
        constexpr auto iota(MetaValue auto begin, MetaValue auto end) noexcept {
            std::array<std::remove_cvref_t<decltype(decltype(begin)::value)>,
                       decltype(end)::value - decltype(begin)::value>
                range{};
            auto curr = decltype(begin)::value;
            for(auto& elem : range) {
                elem = curr++;
            }
            return range;
        }

        /// @brief Converts the given range to a `static_vector<TType, TCapacity`
        /// @param range the range to convert
        /// @return a `static_vector<TType, TCapacity>` containing the elements
        /// of the `range`
        template<typename TType, usize TCapacity>
        constexpr auto to_vector(auto&& range) noexcept {
            static_vector<TType, TCapacity> arr{};
            for(const auto& elem : range) {
                arr.push_back(elem);
            }
            return arr;
        }
    } // namespace detail

    /// @brief Pipeline operator for `mpl::List`s.
    /// Provides support for piping `mpl::List`s into `std::ranges` algorithms and views.
    ///
    /// # Example
    /// @code{.cpp}
    /// constexpr auto list = List<int, const double, float>{};
    /// constexpr auto ranged
    ///     = list
    ///         | std::ranges::views::filter([](auto type) { return not type.is_const(); })
    ///         | std::ranges::views::transform([](auto type) {
    ///                 return type.as_lvalue_reference().as_volatile();
    ///           })
    ///         | std::ranges::views::reverse
    ///         | std::ranges::views::drop(1_value);
    /// static_assert(ranged == List<volatile int&>{});
    /// @endcode
    ///
    /// @tparam TTypes the types represented in the `List`
    /// @param list the list to pipe into a `std::ranges` algorithm or view
    /// @param range_object the `std::ranges` algorithm or view to pipe into
    /// @return the result of the pipeline, up to this point
    /// @ingroup list
    /// @headerfile hyperion/mpl/list.h
    template<typename... TTypes>
    [[nodiscard]] constexpr auto operator|(List<TTypes...> list, auto range_object) {
        // This is a fairly complicated sequence of metaprogramming.
        // The primary technique involves taking the results of a range algorithm,
        // converting them into a `constexpr` sequence of indices into the `list`,
        // and then using those indices to sift the `list` for the correct output elements.
        //
        // The original technique was discovered by Kris Jusiak and Daisy Hollman
        // and used in [Kris's mp library](https://github.com/boost-ext/mp),
        // which uses the [Boost Software License](http://www.boost.org/LICENSE_1_0.txt).
        //
        // It has been adapted (and reduced) here to work with `mpl::List`

        // the range adaptor type, if applicable
        using adaptor = decltype(detail::get_adaptor(range_object));
        // the function type (possibly a transform, predicate, etc), if applicable
        using function = decltype(detail::get_function(range_object));

        // case for transforms/predicates/etc (transform, filter, etc)
        if constexpr(not std::is_void_v<function>
                     && requires { (function{}(detail::convert_to_meta_t<TTypes>{}), ...); })
        {
            // case for predicates
            // we use `std::common_type` here to force conversion of
            // `Value<true || false>` to `bool` to enable storing the
            // results of invoking the predicate in a `std::array`
            if constexpr(requires {
                             std::array<std::common_type_t<decltype(function{}(
                                            detail::convert_to_meta_t<TTypes>{}))...>,
                                        sizeof...(TTypes)>{
                                 function{}(detail::convert_to_meta_t<TTypes>{})...};
                         })
            {
                // calculate the indices of the "good" elements based on the range adaptor in use
                constexpr auto indices = detail::to_vector<usize, sizeof...(TTypes)>(
                    adaptor{}([values = std::array<std::common_type_t<decltype(function{}(
                                                       detail::convert_to_meta_t<TTypes>{}))...>,
                                                   sizeof...(TTypes)>{function{}(
                                   detail::convert_to_meta_t<TTypes>{})...}](auto index) {
                        // NOLINTNEXTLINE(*-pro-bounds-constant-array-index)
                        return values[index];
                    })(detail::iota(0_value, list.size())));

                // use those indices to sift the `list`
                return [indices]<auto... TIndices>(std::index_sequence<TIndices...>, auto _list) {
                    return _list.sift(List<Value<indices[TIndices]>...>{});
                }(std::make_index_sequence<std::size(indices)>{}, list);
            }
            // case for transforms and similar
            else {
                return List<detail::convert_to_raw_t<decltype(function{}(
                    detail::convert_to_meta_t<TTypes>{}))>...>{};
            }
        }
        // all other sequence-based range adaptors (reverse, drop, take, etc)
        else {
            // calculate the indices of the "good" or "re-arranged" elements based on the
            // range adaptor in use
            constexpr auto indices = [](auto range_obj, MetaValue auto size) {
                constexpr auto to_process = detail::iota(0_value, size);
                return detail::to_vector<usize, sizeof...(TTypes)>(range_obj(to_process));
            }
#if HYPERION_PLATFORM_COMPILER_IS_MSVC
            (detail::extend_constexpr_lifetime(range_object), list.size());
#else
            (range_object, list.size());
#endif // HYPERION_PLATFORM_COMPILER_IS_MSVC

            // use those indices to sift the `list`
            return [indices]<auto... TIndices>(std::index_sequence<TIndices...>, auto _list) {
                return _list.sift(List<Value<indices[TIndices]>...>{});
            }(std::make_index_sequence<std::size(indices)>{}, list);
        }
    }

} // namespace hyperion::mpl

    #if __has_include(<ranges>)
        #include <ranges>
    #endif // __has_include(<ranges>)

namespace hyperion::mpl::_test::list {

    static_assert(List<int, double>{}.size() == 2,
                  "hyperion::mpl::List::size test case 1 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>>{}.size() == 4,
                  "hyperion::mpl::List::size test case 2 (failing)");

    static_assert(List<>{}.is_empty(), "hyperion::mpl::List::is_empty test case 1 (failing)");
    static_assert(not List<int>{}.is_empty(),
                  "hyperion::mpl::List::is_empty test case 2 (failing)");

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
    static_assert(List<int, double>{}.zip(List<float, usize>{}).apply(add_const).all_of(is_const),
                  "hyperion::mpl::List::zip test case 3 (failing)");

    static_assert(List<int, double>{} == List<int, double>{},
                  "hyperion::mpl::List operator== test case 1 (failing)");
    static_assert(List<int, double>{}.apply(add_const) == List<const int, const double>{},
                  "hyperion::mpl::List operator== test case 2 (failing)");
    static_assert(List<int, double>{}.zip(List<double, int>{})
                      == List<Pair<int, double>, Pair<double, int>>{},
                  "hyperion::mpl::List operator== test case 3 (failing)");

    constexpr auto test_metavalue = 1_value;
    constexpr auto test_metatype = decltype_<int>();
    constexpr auto test_value = "a string";

    static_assert(make_list(1, 1.0, 2_usize) == List<int, double, usize>{},
                  "hyperion::mpl::make_list test case 1 (failing)");
    static_assert(make_list(test_metavalue, test_metatype, test_value)
                      == List<Value<1_usize>, int, const char* const&>{},
                  "hyperion::mpl::make_list test case 2 (failing)");
    static_assert(make_list<int, double, Value<1>>() == List<int, double, Value<1>>{},
                  "hyperion::mpl::make_list test case 3 (failing)");
    static_assert(make_list<int, double, Type<usize>>() == List<int, double, usize>{},
                  "hyperion::mpl::make_list test case 4 (failing)");

    static_assert(List<int, double>{}.front() == decltype_<int>(),
                  "hyperion::mpl::List::front test (failing)");

    static_assert(List<int, double>{}.back() == decltype_<double>(),
                  "hyperion::mpl::List::back test (failing)");

    static_assert(List<int, double>{}.push_front(decltype_<float>()) == List<float, int, double>{},
                  "hyperion::mpl::List::push_front test case 1 (failing)");
    static_assert(List<int, double>{}.push_front(1_value) == List<Value<1>, int, double>{},
                  "hyperion::mpl::List::push_front test case 2 (failing)");
    static_assert(List<int, double>{}.push_front(Pair<int, double>{})
                      == List<Pair<int, double>, int, double>{},
                  "hyperion::mpl::List::push_front test case 3 (failing)");
    static_assert(List<int, double>{}.push_front(List<float, usize>{})
                      == List<float, usize, int, double>{},
                  "hyperion::mpl::List::push_front test case 4 (failing)");

    static_assert(List<int, double>{}.push_back(decltype_<float>()) == List<int, double, float>{},
                  "hyperion::mpl::List::push_back test case 1 (failing)");
    static_assert(List<int, double>{}.push_back(1_value) == List<int, double, Value<1>>{},
                  "hyperion::mpl::List::push_back test case 2 (failing)");
    static_assert(List<int, double>{}.push_back(Pair<int, double>{})
                      == List<int, double, Pair<int, double>>{},
                  "hyperion::mpl::List::push_back test case 3 (failing)");
    static_assert(List<int, double>{}.push_back(List<float, usize>{})
                      == List<int, double, float, usize>{},
                  "hyperion::mpl::List::push_back test case 4 (failing)");

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
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.find_if(is_const)
                      == decltype_<not_found_tag>(),
                  "hyperion::mpl::List::find_if test case 3 (failing)");

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
    static_assert(not List<Value<1>, Value<2>, Value<3>>{}.all_of(is_const),
                  "hyperion::mpl::List::all_of test case 3 (failing)");

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
    static_assert(not List<Value<1>, Value<2>, Value<3>>{}.any_of(is_const),
                  "hyperion::mpl::List::any_of test case 3 (failing)");

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
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.none_of(is_const),
                  "hyperion::mpl::List::none_of test case 3 (failing)");

    static_assert(List<Value<1>, Value<2>, Value<3>>{}.accumulate(0_value) == 6_value,
                  "hyperion::mpl::List::accumulate test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.accumulate(0_value,
                                                                  [](auto state, auto val) {
                                                                      return state + val;
                                                                  })
                      == 8_value,
                  "hyperion::mpl::List::accumulate test case 2 (failing)");

    static_assert(List<Value<1>, Value<2>, Value<3>>{}.accumulate(0_value) == 6_value,
                  "hyperion::mpl::List::accumulate test case 3 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.accumulate(0_value,
                                                                  [](auto state, auto val) {
                                                                      return state + val;
                                                                  })
                      == 8_value,
                  "hyperion::mpl::List::accumulate test case 4 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count_if([](auto val) {
        return val == 3_value;
    }) == 2_value,
                  "hyperion::mpl::List::count_if test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count_if([](auto val) {
        return val == 4_value;
    }) == 0_value,
                  "hyperion::mpl::List::count_if test case 2 (failing)");
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.count_if(is_const) == 0_value,
                  "hyperion::mpl::List::count_if test case 3 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count(3_value) == 2_value,
                  "hyperion::mpl::List::count test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count(4_value) == 0_value,
                  "hyperion::mpl::List::count test case 2 (failing)");

    static_assert(List<Value<3>, int, double, Value<2>>{}.contains(2_value),
                  "hyperion::mpl::List::contains test case 1 (failing)");
    static_assert(List<Value<3>, int, double, Value<2>>{}.contains(decltype_<int>()),
                  "hyperion::mpl::List::contains test case 2 (failing)");
    static_assert(not List<Value<3>, int, double, Value<2>>{}.contains(decltype_<float>()),
                  "hyperion::mpl::List::contains test case 3 (failing)");
    static_assert(not List<Value<3>, int, double, Value<2>>{}.contains(4_value),
                  "hyperion::mpl::List::contains test case 4 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<4>>{}.index_if(greater_than(3_value)) == 2_value,
                  "hyperion::mpl::List::index_if test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<4>>{}.index_if(less_than(3_value)) == 1_value,
                  "hyperion::mpl::List::index_if test case 2 (failing)");
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.index_if(is_const) == 3_value,
                  "hyperion::mpl::List::count_if test case 3 (failing)");

    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(decltype_<double>()) == 2_value,
                  "hyperion::mpl::List::index_of test case 1 (failing)");
    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(decltype_<int>()) == 1_value,
                  "hyperion::mpl::List::index_of test case 2 (failing)");
    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(4_value) == 3_value,
                  "hyperion::mpl::List::index_of test case 3 (failing)");

    constexpr auto is_value = []([[maybe_unused]] MetaValue auto value) noexcept {
        return Value<true>{};
    };

    static_assert(List<int, const double, float>{}.filter(is_const) == List<const double>{},
                  "hyperion::mpl::List::filter test case 1 (failing)");
    static_assert(List<int, double, float>{}.filter(is_const) == List<>{},
                  "hyperion::mpl::List::filter test case 2 (failing)");
    static_assert(List<int, double, const float>{}.filter(is_const) == List<const float>{},
                  "hyperion::mpl::List::filter test case 3 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>, float>{}.filter(is_value)
                      == List<Value<1>, Value<2>>{},
                  "hyperion::mpl::List::filter test case 4 (failing)");

    static_assert(List<int, const double, float>{}.remove_if(is_const) == List<int, float>{},
                  "hyperion::mpl::List::remove_if test case 1 (failing)");
    static_assert(List<int, double, float>{}.remove_if(is_const) == List<int, double, float>{},
                  "hyperion::mpl::List::remove_if test case 2 (failing)");
    static_assert(List<int, double, const float>{}.remove_if(is_const) == List<int, double>{},
                  "hyperion::mpl::List::remove_if test case 3 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>, float>{}.remove_if(is_value)
                      == List<int, double, float>{},
                  "hyperion::mpl::List::remove_if test case 4 (failing)");

    static_assert(List<int, const double, float>{}.remove(decltype_<const double>())
                      == List<int, float>{},
                  "hyperion::mpl::List::remove test case 1 (failing)");
    static_assert(List<int, double, float>{}.remove(decltype_<u32>()) == List<int, double, float>{},
                  "hyperion::mpl::List::remove test case 2 (failing)");
    static_assert(List<int, double, const float>{}.remove(decltype_<const float>())
                      == List<int, double>{},
                  "hyperion::mpl::List::remove test case 3 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>, float>{}.remove(1_value)
                      == List<int, double, Value<2>, float>{},
                  "hyperion::mpl::List::remove test case 4 (failing)");

    static_assert(List<int, const double, float>{}.sift(List<Value<1>, Value<2>>{})
                      == List<const double, float>{},
                  "hyperion::mpl::List::sift test case 1 (failing)");
    static_assert(List<int, double, float>{}.sift(List<Value<0>, Value<2>>{}) == List<int, float>{},
                  "hyperion::mpl::List::sift test case 2 (failing)");
    static_assert(
        List<int, Value<1>, double, Value<2>, float>{}.sift(List<Value<1>, Value<2>, Value<4>>{})
            == List<Value<1>, double, float>{},
        "hyperion::mpl::List::sift test case 3 (failing)");

    static constexpr auto sum = [](MetaValue auto... value) noexcept {
        return (value + ...);
    };

    static constexpr auto num_ints = [](auto... value) noexcept {
        constexpr auto to_value = [](auto val) {
            if constexpr(MetaType<decltype(val)>) {
                if constexpr(detail::convert_to_meta_t<decltype(val)>{} == decltype_<int>()) {
                    return 1_value;
                }
                else {
                    return 0_value;
                }
            }
            else {
                return 0_value;
            }
        };

        return (to_value(value) + ...);
    };

    static_assert(List<Value<1>, Value<2>, Value<3>, Value<4>>{}.unwrap(sum) == 10_value,
                  "hyperion::mpl::List::unwrap test case 1 (failing)");
    static_assert(List<Value<3>, Value<4>, Value<3>, Value<4>>{}.unwrap(sum) == 14_value,
                  "hyperion::mpl::List::unwrap test case 2 (failing)");
    static_assert(List<Value<3>, Value<4>, Value<3>, Value<4>>{}.unwrap(sum) == 14_value,
                  "hyperion::mpl::List::unwrap test case 3 (failing)");
    static_assert(List<int, Value<1>, int, Value<2>, int>{}.unwrap(num_ints) == 3_value,
                  "hyperion::mpl::List::unwrap test case 4 (failing)");

    #if __cpp_lib_ranges >= 202110L

    static constexpr auto test_ranges1() noexcept {
        constexpr auto list = List<int, double, float>{};
        constexpr auto manipped
            = list
              | std::ranges::views::transform([](MetaType auto type) { return type.as_const(); })
              | std::ranges::views::drop(1_value) | std::ranges::views::reverse;
        return manipped == List<const float, const double>{};
    }

    static constexpr auto test_ranges2() noexcept {
        constexpr auto list = List<int, const double, float>{};
        constexpr auto manipped
            = list | std::ranges::views::filter([](auto type) { return not type.is_const(); })
              | std::ranges::views::reverse;
        return manipped == List<float, int>{};
    }

    static constexpr auto test_ranges3() noexcept {
        constexpr auto list = List<int, const double, float>{};
        constexpr auto manipped
            = list | std::ranges::views::filter([](auto type) { return not type.is_const(); })
              | std::ranges::views::transform(
                  [](auto type) { return type.as_lvalue_reference().as_volatile(); })
              | std::ranges::views::reverse | std::ranges::views::drop(1_value);

        return manipped == List<volatile int&>{};
    }

    static constexpr auto test_ranges4() noexcept {
        constexpr auto list = List<int, Value<1>, const double, Value<2>, float>{};
        constexpr auto manipped
            = list
              | std::ranges::views::filter([](auto elem) { return not MetaValue<decltype(elem)>; })
              | std::ranges::views::transform(
                  [](auto type) { return type.as_lvalue_reference().as_volatile(); })
              | std::ranges::views::reverse | std::ranges::views::drop(1_value);

        return manipped == List<const volatile double&, volatile int&>{};
    }

    static_assert(test_ranges1(), "hyperion::mpl::List ranges support test case 1 (failing)");
    static_assert(test_ranges2(), "hyperion::mpl::List ranges support test case 2 (failing)");
    static_assert(test_ranges3(), "hyperion::mpl::List ranges support test case 3 (failing)");
    static_assert(test_ranges4(), "hyperion::mpl::List ranges support test case 4 (failing)");

    #endif // __cpp_lib_ranges >= 202110L
} // namespace hyperion::mpl::_test::list

#endif // HYPERION_MPL_LIST_H
