/// @file pair.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming type for operating on a pair of metaprogramming values and/or types
/// @version 0.1
/// @date 2024-02-23
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

#include <hyperion/mpl/metatypes.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

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

#ifndef HYPERION_MPL_PAIR_H
    #define HYPERION_MPL_PAIR_H

namespace hyperion::mpl {

    template<typename... TTypes>
    struct List;

    /// @brief `Pair` is Hyperion's metaprogramming type for operating on pairs of types or values.
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
    ///
    /// @tparam TFirst The first type to store in the pair
    /// @tparam TSecond The second type to store in the pair
    /// @ingroup pair
    /// @headerfile hyperion/mpl/pair.h
    template<typename TFirst, typename TSecond>
    struct Pair {
        /// @brief The first type to store in the pair, converted to the appropriate metaprogramming
        /// type.
        ///
        /// If `TFirst` is a specialization of `mpl::Type`, `mpl::Pair`, or `mpl::Value`,
        /// this aliases to `TFirst`. Otherwise, wraps it in `mpl::Type`.
        using first = detail::convert_to_meta_t<TFirst>;
        /// @brief The second type to store in the pair, converted to the appropriate
        /// metaprogramming type.
        ///
        /// If `TSecond` is a specialization of `mpl::Type`, `mpl::Pair`, or `mpl::Value`,
        /// this aliases to `TSecond`. Otherwise, wraps it in `mpl::Type`.
        using second = detail::convert_to_meta_t<TSecond>;

        /// @brief Returns an instance of `first`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto first = Pair<int, double>{}.make_first();
        /// static_assert(first == decltype_<int>());
        /// @endcode
        ///
        /// @return an instance of `first`
        template<typename TDelay = first>
            requires std::same_as<TDelay, first>
        [[nodiscard]] constexpr auto make_first() const noexcept -> TDelay {
            return {};
        }

        /// @brief Returns an instance of `second`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto second = Pair<int, double>{}.make_second();
        /// static_assert(second == decltype_<double>());
        /// @endcode
        ///
        /// @return an instance of `second`
        template<typename TDelay = second>
            requires std::same_as<TDelay, second>
        [[nodiscard]] constexpr auto make_second() const noexcept -> TDelay {
            return {};
        }

        /// @brief Applies the template metafunction `TMetaFunction` to this `Pair`.
        ///
        /// Applies `TMetaFunction` to this `Pair`,
        /// and returns the result as a metaprogramming type (a specialization of `mpl::Type`,
        /// `mpl::Value`, or `mpl::Pair`).
        ///
        /// # Requirements
        /// - `TMetaFunction` must be instantiatable with this specialization of `Pair`
        /// (i.e. `TMetaFunction<Pair>` must be well-formed).
        /// - The specialization of `TMetaFunction` for `Pair`, `TMetaFunction<Pair>`,
        /// must be a metaprogramming type (i.e. it must be a `MetaType`, `MetaValue`,
        /// or `MetaPair`)
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename TType>
        /// requires (!MetaPair<TType>)
        /// struct add_one_or_const {
        ///     using type = std::conditional_t<MetaValue<TType>,
        ///                                     decltype(TType{} + 1_value),
        ///                                     decltype(decltype_<TType>().as_const())>;
        /// };
        ///
        /// template<typename TPair>
        /// struct add_one_or_const_to_pair {
        ///     using first = typename add_one_or_const<typename TPair::first>::type;
        ///     using second = typename add_one_or_const<typename TPair::second>::type;
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.apply<add_one_or_const_to_pair>()
        ///               == Pair<const int, Value<2>>{});
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply
        /// @return The result of applying `TMetaFunction` to this `Pair`, as a metaprogramming type
        template<template<typename> typename TMetaFunction>
            requires requires { typename TMetaFunction<Pair>; }
                     && (MetaType<TMetaFunction<Pair>> || MetaValue<TMetaFunction<Pair>>
                         || MetaPair<TMetaFunction<Pair>>)
        [[nodiscard]] constexpr auto apply() const noexcept
            -> detail::convert_to_meta_t<TMetaFunction<Pair>> {
            return {};
        }

        /// @brief Applies the template metafunction `TMetaFunction` to the types represented in
        /// this `Pair`.
        ///
        /// Applies `TMetaFunction` to `first` and `second` individually, returning the results
        /// together as a specialization of `Pair`.
        /// Applies `TMetaFunction` to each of `first` and `second` as if by
        /// `make().template apply<TMetaFunction>()`, where `make` is `make_first` or
        /// `make_second`, respectively.
        ///
        /// # Requirements
        /// - `TMetaFunction` _must not_ be instantiatable with this specialization of `Pair`.
        /// I.E. `TMetaFunction<Pair>` _must not_ be well-formed (this is most easily achievable by
        /// adding a requirement clause of `requires (!MetaPair<TType>)` to your `TMetaFunction`,
        /// where `TType` is the type parameter of the template).
        /// - `TMetaFunction` must be applicable to both `first` and `second`. I.E.,
        /// `make().template apply<TMetaFunction>()`, where `make` is `make_first` or `make_second`,
        /// respectively, must be well formed.
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename TType>
        /// requires (!MetaPair<TType>)
        /// struct add_one_or_const {
        ///     using type = std::conditional_t<MetaValue<TType>,
        ///                                     decltype(TType{} + 1_value),
        ///                                     decltype(decltype_<TType>().as_const())>;
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.apply<add_one_or_const>()
        ///               == Pair<const int, Value<2>>{});
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply
        /// @return The result of applying `TMetaFunction` to this `Pair`, as a metaprogramming type
        template<template<typename> typename TMetaFunction>
            requires(!requires { typename TMetaFunction<Pair>; }) && requires {
                first{}.template apply<TMetaFunction>();
                second{}.template apply<TMetaFunction>();
            }
        [[nodiscard]] constexpr auto apply() const noexcept {
            return make_pair(first{}.template apply<TMetaFunction>(),
                             second{}.template apply<TMetaFunction>());
        }

        /// @brief Applies the metafunction `TFunction` to this `Pair`.
        ///
        /// Applies `TFunction` to this `Pair`, as if by `std::forward<TFunction>(func)(Pair{})`,
        /// and returns the resulting metaprogramming type.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<Pair>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto add_one_or_const = [](MetaPair auto pair) noexcept {
        ///     constexpr auto apply_to_inner = [](auto inner) noexcept {
        ///         if constexpr(MetaType<decltype(inner)>) {
        ///             return inner.as_const();
        ///         }
        ///         else if constexpr(MetaValue<decltype(inner)>) {
        ///             return inner + 1_value;
        ///         }
        ///     };
        ///
        ///     return make_pair(pair.make_first().apply(apply_to_inner),
        ///                      pair.make_second().apply(apply_to_inner));
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.apply(add_one_or_const)
        ///               == Pair<const int, Value<2>>{});
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `TFunction` to this `Pair`
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Pair>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> meta_result_t<TFunction, Pair> {
            return {};
        }

        /// @brief Applies the metafunction `TFunction` to the types represented in this `Pair`.
        ///
        /// Applies `TFunction` to `first` and `second` individually, returning the results
        /// together as a specialization of `Pair`.
        /// Applies `TFunction` to each of `first` and `second` as if by
        /// `make().apply(std::forward<TFunction>(func))`, where `make` is `make_first` or
        /// `make_second`, respectively.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<first>`.
        /// - `TFunction` must be a `MetaFunctionOf<second>`.
        /// - `TFunction` _must not_ be a `MetaFunctionOf<Pair>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto add_one_or_const = [](auto val) noexcept
        ///     requires (!MetaPair<decltype(val)>)
        /// {
        ///     if constexpr(MetaType<decltype(val)>) {
        ///         return val.as_const();
        ///     }
        ///     else if constexpr(MetaValue<decltype(val)>) {
        ///         return val + 1_value;
        ///     }
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.apply(add_one_or_const)
        ///               == Pair<const int, Value<2>>{});
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `TFunction` to the types represented in this `Pair`
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, first> && MetaFunctionOf<TFunction, second>
                     && (!MetaFunctionOf<TFunction, Pair>)
        [[nodiscard]] constexpr auto apply(
            [[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept
            -> Pair<meta_result_t<TFunction, first>, meta_result_t<TFunction, second>> {
            return {};
        }

        /// @brief Checks that this `Pair` satisfies the template metafunction predicate,
        /// `TPredicate`.
        ///
        /// Checks that this `Pair` satisfies `TPredicate` and returns the result as an
        /// `mpl:Value<result, bool>`.
        ///
        /// # Requirements
        /// - `TPredicate` must be instantiatable with this specialization of `Pair`
        /// (i.e. `TPredicate<Pair>` must be well-formed).
        /// - The specialization of `TMetaFunction` for `Pair`, `TMetaFunction<Pair>`,
        /// must be a `MetaValue`
        /// - The value of the specialization of `TMetaFunction` for `Pair`,
        /// `TMetaFunction<Pair>::value`, must be of type (possibly cv-ref qualified) `bool`
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename TType>
        /// requires (!MetaPair<TType>)
        /// struct is_one_or_const {
        ///     static inline constexpr auto value = false;
        /// };
        ///
        /// template<typename TType>
        /// requires MetaValue<TType>
        /// struct is_one_or_const<TType> {
        ///     static inline constexpr auto value = TType::value == 1;
        /// };
        ///
        /// template<typename TType>
        /// requires MetaType<TType>
        /// struct is_one_or_const<TType> {
        ///     static inline constexpr auto value = std::is_const_v<typename TType::type>;
        /// };
        ///
        /// template<typename TPair>
        /// struct is_one_or_const_in_pair {
        ///     static inline constexpr auto value
        ///         = is_one_or_const<typename TPair::first>::value
        ///           && is_one_or_const<typename TPair::second>::value;
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.satisfies<is_one_or_const_in_pair>());
        /// @endcode
        ///
        /// @tparam TPredicate The template metafunction predicate to validate
        /// @return whether this `Pair` satisfies `TPredicate`, as an `mpl::Value`
        template<template<typename> typename TPredicate>
            requires requires { typename TPredicate<Pair>; } && MetaValue<TPredicate<Pair>>
                     && std::same_as<std::remove_cvref_t<decltype(TPredicate<Pair>::value)>, bool>
        [[nodiscard]] constexpr auto
        satisfies() const noexcept -> detail::convert_to_meta_t<TPredicate<Pair>> {
            return {};
        }

        /// @brief Checks that the types represented by this `Pair` satisfy the template
        /// metafunction predicate, `TPredicate`.
        ///
        /// Checks that `first` and `second` both satisfy `TPredicate`, and returns the result as an
        /// `mpl:Value<result, bool>`.
        /// Checks that `first` and `second` both satisfy `TPredicate` as if by
        /// `make().template satisfies<TPredicate>()` where `make` is `make_first`
        /// or `make_second`, respectively.
        ///
        /// # Requirements
        /// - `TPredicate` _must not_ be instantiatable with this specialization of `Pair`.
        /// I.E., `TPredicate<Pair>` _must not_ be well-formed (this is most easily achievable by
        /// adding a requirement clause of `requires (!MetaPair<TType>)` to your `TPredicate`,
        /// where `TType` is the type parameter of the template).
        /// - `TPredicate` must be a template metafunction predicate _capable_ of being satisfied
        /// by both `first` and `second`. I.E., `TMetaFunction<first>`
        /// and `TMetaFunction<second>` must be well-formed.
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename TType>
        /// requires (!MetaPair<TType>)
        /// struct is_one_or_const {
        ///     static inline constexpr auto value = false;
        /// };
        ///
        /// template<typename TType>
        /// requires MetaValue<TType>
        /// struct is_one_or_const<TType> {
        ///     static inline constexpr auto value = TType::value == 1;
        /// };
        ///
        /// template<typename TType>
        /// requires MetaType<TType>
        /// struct is_one_or_const<TType> {
        ///     static inline constexpr auto value = std::is_const_v<typename TType::type>;
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.satisfies<is_one_or_const>());
        /// @endcode
        ///
        /// @tparam TPredicate The template metafunction predicate to validate
        /// @return whether this `Pair` satisfies `TPredicate`, as an `mpl::Value`
        template<template<typename> typename TPredicate>
            requires(!requires { typename TPredicate<Pair>; }) && requires {
                first{}.template satisfies<TPredicate>();
                second{}.template satisfies<TPredicate>();
            }
        [[nodiscard]] constexpr auto satisfies() const noexcept {
            return make_first().template satisfies<TPredicate>()
                   && make_second().template satisfies<TPredicate>();
        }

        /// @brief Checks that this `Pair` satisfies the metafunction predicate, `predicate`.
        ///
        /// Checks that this `Pair` satisfies `predicate` and returns the result as an
        /// `mpl:Value<result, bool>`.
        ///
        /// # Requirements
        /// - `TPredicate` must be a `MetaPredicateOf<Pair>`.
        /// - `TPredicate` _must not_ be a `MetaPredicateOf<first>`.
        /// - `TPredicate` _must not_ be a `MetaPredicateOf<second>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_one_or_const = [](MetaPair auto pair) noexcept {
        ///     constexpr auto satisfies_inner = [](auto val) noexcept
        ///         requires (!MetaPair<decltype(val)>)
        ///     {
        ///         if constexpr(MetaType<decltype(val)>) {
        ///             return val.is_const();
        ///         }
        ///         else if constexpr(MetaValue<decltype(val)>) {
        ///             return Value<decltype(val)::value == 1, bool>{};
        ///         }
        ///     }
        ///
        ///     return pair.make_first().satisfies(satisfies_inner)
        ///            && pair.make_second().satisfies(satisfies_inner);
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.satisfies(is_one_or_const));
        /// @endcode
        ///
        /// @tparam TPredicate The metafunction predicate to validate
        /// @return whether this `Pair` satisfies `predicate`, as an `mpl::Value`
        template<typename TPredicate>
            requires MetaPredicateOf<TPredicate, Pair> && (!MetaPredicateOf<TPredicate, first>)
                     && (!MetaPredicateOf<TPredicate, second>)
        [[nodiscard]] constexpr auto satisfies(
            [[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept -> meta_result_t<TPredicate, Pair> {
            return {};
        }

        /// @brief Checks that the types represented by this `Pair` both satisfy the metafunction
        /// predicate, `predicate`.
        ///
        /// Checks that `first` and `second` both satisfy `predicate` and returns the result as an
        /// `mpl:Value<result, bool>`.
        /// Checks that `first` and `second` both satisfy `TPredicate` as if by
        /// `make().template satisfies(std::forward<TPredicate>(predicate))` where `make` is
        /// `make_first` or `make_second`, respectively.
        ///
        /// # Requirements
        /// - `TPredicate` must be a `MetaPredicateOf<first>`
        /// - `TPredicate` must be a `MetaPredicateOf<second>`.
        /// - `TPredicate` _must not_ be a `MetaPredicateOf<Pair>`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_one_or_const = [](auto pair) noexcept {
        ///     requires (!MetaPair<decltype(val)>)
        /// {
        ///     if constexpr(MetaType<decltype(val)>) {
        ///         return val.is_const();
        ///     }
        ///     else if constexpr(MetaValue<decltype(val)>) {
        ///         return Value<decltype(val)::value == 1, bool>{};
        ///     }
        /// };
        ///
        /// static_assert(Pair<int, Value<1>>{}.satisfies(is_one_or_const));
        /// @endcode
        ///
        /// @tparam TPredicate The metafunction predicate to validate
        /// @return whether the types represented by this `Pair` satisfy `predicate`,
        /// as an `mpl::Value`
        template<typename TPredicate>
            requires MetaPredicateOf<TPredicate, first> && MetaPredicateOf<TPredicate, second>
                     && (!MetaPredicateOf<TPredicate, Pair>)
        [[nodiscard]] constexpr auto satisfies(
            [[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept {
            return make_first().satisfies(TPredicate{}) && make_second().satisfies(TPredicate{});
        }

        /// @brief Converts this `Pair` to an `mpl::List<TFirst, TSecond>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto as_list = Pair<int, double>{}.as_list();
        /// static_assert(as_list == List<int, double>{});
        /// @endcode
        /// @return an `mpl::List` representing `TFirst` and `TSecond`
        template<typename TDelayFirst = TFirst, typename TDelaySecond = TSecond>
            requires std::same_as<TDelayFirst, TFirst> && std::same_as<TDelaySecond, TSecond>
        [[nodiscard]] constexpr auto as_list() const noexcept {
            return List<TDelayFirst, TDelaySecond>{};
        }

        /// @brief Unwraps this `Pair` into `first` and `second`, and invokes `func`
        /// with them.
        ///
        /// Decomposes `this` into its constituent `first` and `second` metaprogramming
        /// elements and invokes `func`, passing `first` and `second` as the invocation
        /// arguments, returning the result of the invocation.
        ///
        /// # Requirements
        /// - `func` must be invocable with `first, second`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto sum = [](MetaValue auto lhs, MetaValue auto rhs) noexcept {
        ///     return lhs + rhs;
        /// };
        ///
        /// static_assert(Pair<Value<1>, Value<2>>{}.unpack(sum) == 3_value);
        /// @endcode
        ///
        /// @tparam TFunction the type of the function to invoke with `first` and
        /// `second` as arguments
        /// @param func the function to invoke with `first` and `second` as arguments
        /// @return The result of invoking `func` with `first` and `second` as arguments
        template<typename TFunction>
            requires std::invocable<TFunction, first, second>
        [[nodiscard]] constexpr auto
        unwrap(TFunction&& func) const noexcept -> std::invoke_result_t<TFunction, first, second> {
            return std::forward<TFunction>(func)(make_first(), make_second());
        }

        /// @brief Returns the metaprogramming type at `TIndex` of this `Pair`
        ///
        /// If `TIndex` is `0`, returns `first`.
        /// If `TIndex` is `1`, returns `second`.
        ///
        /// # Requirements
        /// - `TIndex` must be `0`, or `1`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto first = Pair<int, double>{}.template get<0>();
        /// static_assert(first == decltype_<int>());
        /// @endcode
        ///
        /// @note This, combined with specializations of `std::tuple_element` and
        /// `std::tuple_size` enable structured binding destructuring of this pair
        /// into `first` and `second` individually, as in:
        /// @code {.cpp}
        /// // we can't use structured bindings outside of function-scope, so we wrap our example
        /// // in a `constexpr` function
        /// constexpr auto example() {
        ///     auto [first, second] = Pair<int, double>{};
        ///     return first == decltype_<int>() && second == decltype_<double>();
        /// }
        ///
        /// static_assert(example());
        /// @endcode
        ///
        /// @tparam TIndex The index of the type to get
        /// @return The metaprogramming type at `TIndex` of this `Pair`
        template<std::size_t TIndex>
            requires(TIndex == 0 || TIndex == 1)
        // NOLINTNEXTLINE(cert-dcl58-cpp)
        [[nodiscard]] constexpr auto get() const noexcept {
            if constexpr(TIndex == 0) {
                return make_first();
            }
            else {
                return make_second();
            }
        }

        /// @brief Returns the metaprogramming type at `TIndex` of this `Pair`
        ///
        /// If `TIndex` is `0`, returns `first`.
        /// If `TIndex` is `1`, returns `second`.
        ///
        /// # Requirements
        /// - `TIndex` must be `0`, or `1`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto first = Pair<int, double>{}.template get<0>();
        /// static_assert(first == decltype_<int>());
        /// @endcode
        ///
        /// @note This, combined with specializations of `std::tuple_element` and
        /// `std::tuple_size` enable structured binding destructuring of this pair
        /// into `first` and `second` individually, as in:
        /// @code {.cpp}
        /// // we can't use structured bindings outside of function-scope, so we wrap our example
        /// // in a `constexpr` function
        /// constexpr auto example() {
        ///     auto [first, second] = Pair<int, double>{};
        ///     return first == decltype_<int>() && second == decltype_<double>();
        /// }
        ///
        /// static_assert(example());
        /// @endcode
        ///
        /// @tparam TIndex The index of the type to get
        /// @return The metaprogramming type at `TIndex` of this `Pair`
        template<std::size_t TIndex>
            requires(TIndex == 0 || TIndex == 1)
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

    /// @brief Equality comparison operator between two `Pair` specializations
    ///
    /// Checks that `lhs` and `rhs` both represent the same metaprogramming types and values.
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto first = Pair<int, Value<1, std::uint32_t>>{};
    /// constexpr auto second = Pair<int, Value<1, std::uint64_t>>{};
    ///
    /// static_assert(first == second);
    /// @endcode
    ///
    /// @tparam TLHSFirst The first type of the left-hand `Pair` of the comparison
    /// @tparam TLHSSecond The second type of the left-hand `Pair` of the comparison
    /// @tparam TRHSFirst The first type of the right-hand `Pair` of the comparison
    /// @tparam TRHSSecond The second type of the right-hand `Pair` of the comparison
    /// @param lhs The left-hand `Pair` to compare
    /// @param rhs The right-hand `Pair` to compare with
    /// @return whether `lhs` represents the same metaprogramming types and values as `rhs`
    /// @ingroup pair
    /// @headerfile hyperion/mpl/pair.h
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

    /// @brief Constructs an `mpl::Pair` representing the types of the given arguments.
    ///
    /// If `TFirst` or `TSecond` are metaprogramming types, first converts them to their
    /// raw type before storing in the `Pair`. (e.g. if either are `mpl::Type<some_type>`,
    /// the returned pair would be `Pair<some_type1, some_type2>`,
    /// not `Pair<Type<some_type1>, Type<some_type2>>`).
    ///
    /// This overload does not accept reference qualified parameters.
    ///
    /// # Requirements
    /// - `TFirst` is not reference qualified
    /// - `TSecond` is not reference qualified
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto pair = make_pair(decltype_<int>(), decltype_<double>());
    /// static_assert(pair == Pair<int, double>{});
    /// @endcode
    ///
    /// @tparam TFirst The first type to store in a `Pair`
    /// @tparam TSecond The second type to store in a `Pair`
    /// @param first The first value to store the type of
    /// @param second The second value to store the type of
    /// @return a Pair representing the raw types of `TFirst` and `TSecond`
    /// @ingroup pair
    /// @headerfile hyperion/mpl/pair.h
    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<detail::convert_to_raw_t<TFirst>, detail::convert_to_raw_t<TSecond>> {
        return {};
    }

    /// @brief Constructs an `mpl::Pair` representing the types of the given arguments.
    ///
    /// If `TFirst` or `TSecond` are metaprogramming types, first converts them to their
    /// raw type before storing in the `Pair`. (e.g. if either are `mpl::Type<some_type>`,
    /// the returned pair would be `Pair<some_type1, some_type2>`,
    /// not `Pair<Type<some_type1>, Type<some_type2>>`).
    ///
    /// This overload accepts reference qualified parameters for `first`, but not `second`.
    ///
    /// # Requirements
    /// - `TFirst` is reference qualified
    /// - `TSecond` is not reference qualified
    ///
    /// # Example
    /// @code {.cpp}
    /// int i = 0;
    /// constexpr auto pair = make_pair(i, decltype_<double>());
    /// static_assert(pair == Pair<int&, double>{});
    /// @endcode
    ///
    /// @tparam TFirst The first type to store in a `Pair`
    /// @tparam TSecond The second type to store in a `Pair`
    /// @param first The first value to store the type of
    /// @param second The second value to store the type of
    /// @return a Pair representing the raw types of `TFirst` and `TSecond`
    /// @ingroup pair
    /// @headerfile hyperion/mpl/pair.h
    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, detail::convert_to_raw_t<TSecond>> {
        return {};
    }

    /// @brief Constructs an `mpl::Pair` representing the types of the given arguments.
    ///
    /// If `TFirst` or `TSecond` are metaprogramming types, first converts them to their
    /// raw type before storing in the `Pair`. (e.g. if either are `mpl::Type<some_type>`,
    /// the returned pair would be `Pair<some_type1, some_type2>`,
    /// not `Pair<Type<some_type1>, Type<some_type2>>`).
    ///
    /// This overload accepts reference qualified parameters for `second`, but not `first`.
    ///
    /// # Requirements
    /// - `TFirst` is not reference qualified
    /// - `TSecond` is reference qualified
    ///
    /// # Example
    /// @code {.cpp}
    /// const double j = 0.0;
    /// constexpr auto pair = make_pair(decltype_<int>(), j);
    /// static_assert(pair == Pair<int, const double&>{});
    /// @endcode
    ///
    /// @tparam TFirst The first type to store in a `Pair`
    /// @tparam TSecond The second type to store in a `Pair`
    /// @param first The first value to store the type of
    /// @param second The second value to store the type of
    /// @return a Pair representing the raw types of `TFirst` and `TSecond`
    /// @ingroup pair
    /// @headerfile hyperion/mpl/pair.h
    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<detail::convert_to_raw_t<TFirst>, TSecond> {
        return {};
    }

    /// @brief Constructs an `mpl::Pair` representing the types of the given arguments.
    ///
    /// If `TFirst` or `TSecond` are metaprogramming types, first converts them to their
    /// raw type before storing in the `Pair`. (e.g. if either are `mpl::Type<some_type>`,
    /// the returned pair would be `Pair<some_type1, some_type2>`,
    /// not `Pair<Type<some_type1>, Type<some_type2>>`).
    ///
    /// This overload accepts reference qualified parameters for both `first` and `second`.
    ///
    /// # Requirements
    /// - `TFirst` is reference qualified
    /// - `TSecond` is reference qualified
    ///
    /// # Example
    /// @code {.cpp}
    /// int i = 0;
    /// const double j = 0.0;
    /// constexpr auto pair = make_pair(i, j);
    /// static_assert(pair == Pair<int&, const double&>{});
    /// @endcode
    ///
    /// @tparam TFirst The first type to store in a `Pair`
    /// @tparam TSecond The second type to store in a `Pair`
    /// @param first The first value to store the type of
    /// @param second The second value to store the type of
    /// @return a Pair representing the raw types of `TFirst` and `TSecond`
    /// @ingroup pair
    /// @headerfile hyperion/mpl/pair.h
    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }

} // namespace hyperion::mpl

// specialize `std::tuple_element` for `Pair` for structured bindings support

template<std::size_t TIndex, typename TFirst, typename TSecond>
// NOLINTNEXTLINE(cert-dcl58-cpp)
struct std::tuple_element<TIndex, hyperion::mpl::Pair<TFirst, TSecond>> {
    using type = std::conditional_t<TIndex == 0,
                                    typename hyperion::mpl::Pair<TFirst, TSecond>::first,
                                    typename hyperion::mpl::Pair<TFirst, TSecond>::second>;
};

// specialize `std::tuple_csize` for `Pair` for structured bindings support

template<typename TFirst, typename TSecond>
// NOLINTNEXTLINE(cert-dcl58-cpp)
struct std::tuple_size<hyperion::mpl::Pair<TFirst, TSecond>>
    : std::integral_constant<std::size_t, 2> { };

// NOLINTNEXTLINE(misc-header-include-cycle)
    #include <hyperion/mpl/type.h>
// NOLINTNEXTLINE(misc-header-include-cycle)
    #include <hyperion/mpl/value.h>
// NOLINTNEXTLINE(misc-header-include-cycle)
    #include <hyperion/mpl/list.h>

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

    static_assert(Pair<int, double>{}.as_list() == List<int, double>{},
                  "hyperion::mpl::Pair::as_list test (failing)");

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

#endif // HYPERION_MPL_PAIR_H
