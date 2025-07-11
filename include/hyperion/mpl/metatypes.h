/// @file metatypes.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Concept and type trait definitions for what consitutes various categories of
/// metaprogramming types
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

#ifndef HYPERION_MPL_METATYPES_H
    #define HYPERION_MPL_METATYPES_H

/// @ingroup mpl
/// @{
/// @defgroup metatypes Metaprogramming Type Requirements
/// Hyperion provides `concept` and type trait definitions outlining the requirements for various
/// categories of metaprogramming types.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/metatypes.h>
/// #include <hyperion/mpl/value.h>
/// #include <hyperion/mpl/type.h>
///
/// using namespace hyperion::mpl;
///
/// struct not_meta {};
///
/// struct metatype {
///     using type = void;
/// };
///
/// struct metavalue {
///     static inline constexpr auto value = 42;
/// };
///
/// template<typename TType>
/// struct metatype_typefunction {
///     using type = std::add_const_t<TType>;
/// };
///
/// template<typename TType>
/// struct metatype_valuefunction {
///     static inline constexpr auto value = std::is_const_v<TType>;
/// };
///
/// template<auto TValue>
/// struct metavalue_typefunction {
///     using type = std::conditional_t<TValue, int, void>;
/// };
///
/// template<auto TValue>
/// struct metavalue_valuefunction {
///     static inline constexpr auto value = TValue * 2;
/// };
///
/// constexpr auto type_to_type = []([[maybe_unused]] const MetaType auto& type) -> Type<bool> {
///     return {};
/// };
/// constexpr auto type_to_value
///     = []([[maybe_unused]] const MetaType auto& type) -> Value<true> {
///     return {};
/// };
/// constexpr auto value_to_type
///     = []([[maybe_unused]] const MetaValue auto& type) -> Type<bool> {
///     return {};
/// };
/// constexpr auto value_to_value
///     = []([[maybe_unused]] const MetaValue auto& type) -> Value<true> {
///     return {};
/// };
///
/// static_assert(MetaType<metatype>);
/// static_assert(not MetaType<not_meta>);
///
/// static_assert(MetaValue<metavalue>);
/// static_assert(not MetaValue<not_meta>);
///
/// static_assert(TypeMetaFunction<metatype_typefunction>);
/// static_assert(TypeMetaFunction<metatype_valuefunction>);
///
/// static_assert(ValueMetaFunction<metavalue_typefunction>);
/// static_assert(ValueMetaFunction<metavalue_valuefunction>);
///
/// static_assert(MetaFunction<decltype(type_to_type)>);
/// static_assert(MetaFunction<decltype(type_to_value)>);
/// static_assert(MetaFunction<decltype(value_to_type)>);
/// static_assert(MetaFunction<decltype(value_to_value)>);
/// @endcode
/// @headerfile hyperion/mpl/metatypes.h
/// @}

#include <hyperion/platform/def.h>

#include <concepts>
#include <type_traits>

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

namespace hyperion::mpl {

    /// @brief Concept specifying the requirements for a metaprogramming
    /// value type.
    ///
    /// A metaprogramming value type is any type that has a `static constexpr` member
    /// variable named `value`.
    ///
    /// # Requirements
    /// - `TType` must have a `static constexpr` member variable named `value`.
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    concept MetaValue = requires {
        {
            TType::value
        };
        {
            std::integral_constant<decltype(TType::value), TType::value>{}
        };
    };

    /// @brief Type trait to determine whether type `TType` is a metaprogramming value type.
    ///
    /// A metaprogramming value type is any type that has a `static constexpr` member
    /// variable named `value`.
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    struct is_meta_value : std::bool_constant<MetaValue<TType>> { };

    /// @brief Value of the type trait `is_meta_value`.
    /// Used to determine whether type `TType` is a metaprogramming value type.
    ///
    /// A metaprogramming value type is any type that has a `static constexpr` member
    /// variable named `value`.
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/value.h
    template<typename TType>
    static inline constexpr auto is_meta_value_v = is_meta_value<TType>::value;

    /// @brief Concept specifying the requirements for a metaprogramming type wrapper type.
    ///
    /// A metaprogramming type wrapper type is any type that represents another type, via
    /// a member using alias type `type` (e.g. `using type = bool;`).
    ///
    /// # Requirements
    /// - `TType` must have a member using alias type `type` (e.g. `using type = bool;`).
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    concept MetaType = requires { typename TType::type; };

    /// @brief Type trait to determine whether type `TType` is a metaprogramming type wrapper type.
    ///
    /// A metaprogramming type wrapper type is any type that represents another type, via
    /// a member using alias type `type` (e.g. `using type = bool;`).
    ///
    /// # Requirements
    /// - `TType` must have a member using alias type `type` (e.g. `using type = bool;`).
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    struct is_meta_type : std::bool_constant<MetaType<TType>> { };

    /// @brief Value of the type trait `is_meta_type`.
    /// Used to determine whether type `TType` is a metaprogramming type wrapper type.
    ///
    /// A metaprogramming type wrapper type is any type that represents another type, via
    /// a member using alias type `type` (e.g. `using type = bool;`).
    ///
    /// # Requirements
    /// - `TType` must have a member using alias type `type` (e.g. `using type = bool;`).
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    static inline constexpr auto is_meta_type_v = is_meta_type<TType>::value;

    /// @brief Concept specifying the requirements for a metaprogramming pair.
    ///
    /// A metaprogramming pair type is any type that represents a 2 element combination
    /// (a pair) of (potentially different) `MetaType` and/or `MetaValue`,
    /// representing each via the member `using` aliases `first` and `second`, respectively.
    ///
    /// # Requirements
    /// - `TType` must have a member `using` alias type `first` (e.g. `using type = bool;`).
    /// - `TType` must have a member `using` alias type `second` (e.g. `using type = bool;`).
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    concept MetaPair = requires {
        typename TType::first;
        typename TType::second;
    };

    /// @brief Type trait to determine whether type `TType` is a metaprogramming pair type.
    ///
    /// A metaprogramming pair type is any type that represents a 2 element combination
    /// (a pair) of (potentially different) `MetaType` and/or `MetaValue`,
    /// representing each via the member `using` aliases `first` and `second`, respectively.
    ///
    /// - `TType` must have a member `using` alias type `first` (e.g. `using type = bool;`).
    /// - `TType` must have a member `using` alias type `second` (e.g. `using type = bool;`).
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    struct is_meta_pair : std::bool_constant<MetaPair<TType>> { };

    /// @brief Value of the type trait `is_meta_pair`.
    /// Used to determine whether type `TType` is a metaprogramming pair type.
    ///
    /// A metaprogramming pair type is any type that represents a 2 element combination
    /// (a pair) of (potentially different) `MetaType` and/or `MetaValue`,
    /// representing each via the member `using` aliases `first` and `second`, respectively.
    ///
    /// - `TType` must have a member `using` alias type `first` (e.g. `using type = bool;`).
    /// - `TType` must have a member `using` alias type `second` (e.g. `using type = bool;`).
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    static inline constexpr auto is_meta_pair_v = is_meta_pair<TType>::value;

    /// @brief Type trait to determine whether type `TType` is a metaprogramming list type.
    ///
    /// A metaprogramming list type is any type that represents a list of types and is not
    /// a `MetaPair`.
    ///
    /// - `TType` must be a class/struct template accepting a variadic list of types.
    /// I.E. it must be a type of the form
    /// `template<typename... TTypes> struct type { /** contents **/ };`
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    struct is_meta_list : std::false_type { };

    // specialization for the true case
    template<template<typename...> typename TList, typename... TTypes>
    struct is_meta_list<TList<TTypes...>>
        : std::conditional_t<MetaPair<TList<TTypes...>>, std::false_type, std::true_type> { };

    /// @brief Value of the type trait `is_meta_list`.
    /// Used to determine whether type `TType` is a metaprogramming list type.
    ///
    /// A metaprogramming list type is any type that represents a list of types and is not
    /// a `MetaPair`.
    ///
    /// - `TType` must be a class/struct template accepting a variadic list of types.
    /// I.E. it must be a type of the form
    /// `template<typename... TTypes> struct type { /** contents **/ };`
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    static inline constexpr auto is_meta_list_v = is_meta_list<TType>::value;

    /// @brief Concept specifying the requirements for a metaprogramming list type.
    ///
    /// A metaprogramming list type is any type that represents a list of types and is not
    /// a `MetaPair`.
    ///
    /// - `TType` must be a class/struct template accepting a variadic list of types.
    /// I.E. it must be a type of the form
    /// `template<typename... TTypes> struct type { /** contents **/ };`
    ///
    /// @tparam TType The type to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TType>
    concept MetaList = is_meta_list_v<TType>;

    template<typename TType>
    struct Type;

    template<auto TValue, typename TType>
    struct Value;

    template<typename TFirst, typename TSecond>
    struct Pair;

    template<typename... TTypes>
    struct List;

    namespace detail {
        template<typename TType>
        struct convert_to_meta {
            using type = mpl::Type<TType>;
        };

        template<typename TType>
            requires MetaValue<std::remove_cvref_t<TType>>
        struct convert_to_meta<TType> {
            using type
                = mpl::Value<std::remove_cvref_t<TType>::value,
                             std::remove_cvref_t<decltype(std::remove_cvref_t<TType>::value)>>;
        };

        template<typename TType>
            requires MetaType<std::remove_cvref_t<TType>>
        struct convert_to_meta<TType> {
            // NOLINTNEXTLINE(modernize-type-traits)
            using type = Type<typename std::remove_cvref_t<TType>::type>;
        };

        template<typename TType>
            requires MetaType<std::remove_cvref_t<TType>>
                     && MetaType<typename std::remove_cvref_t<TType>::type>
        struct convert_to_meta<TType> {
            using type = Type<typename std::remove_cvref_t<TType>::type::type>;
        };

        template<typename TType>
            requires MetaPair<std::remove_cvref_t<TType>>
        struct convert_to_meta<TType> {
            using type = mpl::Pair<
                typename convert_to_meta<typename std::remove_cvref_t<TType>::first>::type,
                typename convert_to_meta<typename std::remove_cvref_t<TType>::second>::type>;
        };

        template<template<typename...> typename TList, typename... TTypes>
            requires MetaList<TList<TTypes...>> && std::same_as<List<TTypes...>, TList<TTypes...>>
        struct convert_to_meta<TList<TTypes...>> {
            using type = TList<TTypes...>;
        };

        template<typename TType>
        using convert_to_meta_t = typename convert_to_meta<TType>::type;

        template<typename TType>
        struct convert_to_raw;

        template<typename TType>
            requires(!MetaType<std::remove_cvref_t<TType>>)
                    && (!MetaValue<std::remove_cvref_t<TType>>)
                    && (!MetaPair<std::remove_cvref_t<TType>>)
        struct convert_to_raw<TType> {
            using type = TType;
        };

        template<typename TType>
            requires MetaValue<std::remove_cvref_t<TType>>
        struct convert_to_raw<TType> {
            using type
                = mpl::Value<std::remove_cvref_t<TType>::value,
                             std::remove_cvref_t<decltype(std::remove_cvref_t<TType>::value)>>;
        };

        template<typename TType>
            requires MetaType<std::remove_cvref_t<TType>>
        struct convert_to_raw<TType> {
            using type = typename convert_to_raw<typename std::remove_cvref_t<TType>::type>::type;
        };

        template<typename TType>
            requires MetaPair<std::remove_cvref_t<TType>>
        struct convert_to_raw<TType> {
            using type = mpl::Pair<
                typename convert_to_raw<typename std::remove_cvref_t<TType>::first>::type,
                typename convert_to_raw<typename std::remove_cvref_t<TType>::second>::type>;
        };

        template<typename TType>
        using convert_to_raw_t = typename convert_to_raw<TType>::type;

        template<typename TType>
        struct unwrap_inner {
            using type = TType;
        };

        template<typename TType>
            requires MetaType<std::remove_cvref_t<TType>>
                     && MetaValue<typename std::remove_cvref_t<TType>::type>
        struct unwrap_inner<TType> {
            using type = Value<std::remove_cvref_t<TType>::type::value,
                               decltype(std::remove_cvref_t<TType>::type::value)>;
        };

        template<typename TType>
        using unwrap_inner_t = typename unwrap_inner<TType>::type;

        template<template<typename> typename TTemplate>
        struct is_instantiatable_with_type : std::false_type {
            using type = void;
        };

        template<template<typename> typename TTemplate>
            requires requires { typename TTemplate<int>; }
        struct is_instantiatable_with_type<TTemplate> : std::true_type {
            using type = int;
        };

        template<template<typename> typename TTemplate>
            requires requires { typename TTemplate<Value<true, bool>>; }
                     && (!requires { typename TTemplate<int>; })
        struct is_instantiatable_with_type<TTemplate> : std::true_type {
            using type = Value<true, bool>;
        };

        template<template<auto> typename TTemplate>
        struct is_instantiatable_with_value : std::false_type { };

        template<template<auto> typename TTemplate>
            requires requires { typename TTemplate<true>; }
        struct is_instantiatable_with_value<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_instantiatable_with_pair : std::false_type { };

        template<template<typename> typename TTemplate>
            requires requires { typename TTemplate<Pair<int, bool>>; }
        struct is_instantiatable_with_pair<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_instantiatable_with_list : std::false_type { };

        template<template<typename> typename TTemplate>
            requires requires { typename TTemplate<List<int, bool>>; }
        struct is_instantiatable_with_list<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_type_to_type : std::false_type { };

        template<template<typename> typename TTemplate>
            requires is_instantiatable_with_type<TTemplate>::value
                     && MetaType<TTemplate<typename is_instantiatable_with_type<TTemplate>::type>>
        struct is_type_to_type<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_type_to_value : std::false_type { };

        template<template<typename> typename TTemplate>
            requires is_instantiatable_with_type<TTemplate>::value
                     && MetaValue<TTemplate<typename is_instantiatable_with_type<TTemplate>::type>>
        struct is_type_to_value<TTemplate> : std::true_type { };

        template<template<auto> typename TTemplate>
        struct is_value_to_type : std::false_type { };

        template<template<auto> typename TTemplate>
            requires is_instantiatable_with_value<TTemplate>::value && MetaType<TTemplate<true>>
        struct is_value_to_type<TTemplate> : std::true_type { };

        template<template<auto> typename TTemplate>
        struct is_value_to_value : std::false_type { };

        template<template<auto> typename TTemplate>
            requires is_instantiatable_with_value<TTemplate>::value && MetaValue<TTemplate<true>>
        struct is_value_to_value<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_pair_to_type : std::false_type { };

        template<template<typename> typename TTemplate>
            requires is_instantiatable_with_pair<TTemplate>::value && MetaType<TTemplate<Pair<int, bool>>>
        struct is_pair_to_type<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_pair_to_value : std::false_type { };

        template<template<typename> typename TTemplate>
            requires is_instantiatable_with_pair<TTemplate>::value && MetaValue<TTemplate<Pair<int, bool>>>
        struct is_pair_to_value<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_list_to_type : std::false_type { };

        template<template<typename> typename TTemplate>
            requires is_instantiatable_with_list<TTemplate>::value && MetaType<TTemplate<List<int, bool>>>
        struct is_list_to_type<TTemplate> : std::true_type { };

        template<template<typename> typename TTemplate>
        struct is_list_to_value : std::false_type { };

        template<template<typename> typename TTemplate>
            requires is_instantiatable_with_list<TTemplate>::value && MetaValue<TTemplate<List<int, bool>>>
        struct is_list_to_value<TTemplate> : std::true_type { };
    } // namespace detail

    /// @brief A `TypeMetaFunction` is a template metafunction that accepts a single type parameter
    /// and contains either a `static constexpr` member variable, `value`, or a member using alias
    /// type, `type`
    ///
    /// @tparam TTemplate The template to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<template<typename> typename TTemplate>
    concept TypeMetaFunction
        = detail::is_type_to_type<TTemplate>::value || detail::is_type_to_value<TTemplate>::value;

    /// @brief A `ValueMetaFunction` is a template metafunction that accepts a single value
    /// parameter and contains either a `static constexpr` member variable, `value`, or a member
    /// using alias type, `type`
    ///
    /// @tparam TTemplate The template to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<template<auto> typename TTemplate>
    concept ValueMetaFunction
        = detail::is_value_to_type<TTemplate>::value || detail::is_value_to_value<TTemplate>::value;

    /// @brief A `PairMetaFunction` is a template metafunction that accepts a single pair parameter
    /// and contains either a `static constexpr` member variable, `value`, or a member using alias
    /// type, `type`
    ///
    /// @tparam TTemplate The template to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<template<typename> typename TTemplate>
    concept PairMetaFunction
        = detail::is_pair_to_type<TTemplate>::value || detail::is_pair_to_value<TTemplate>::value;

    /// @brief A `ListMetaFunction` is a template metafunction that accepts a single list parameter
    /// and contains either a `static constexpr` member variable, `value`, or a member using alias
    /// type, `type`
    ///
    /// @tparam TTemplate The template to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<template<typename> typename TTemplate>
    concept ListMetaFunction
        = detail::is_list_to_type<TTemplate>::value || detail::is_list_to_value<TTemplate>::value;

    // clang-format off

    /// @brief A `MetaFunctionOf` is a callable metafunction that accepts a single value parameter
    /// of the specified type, `TType` and returns either a `MetaType` or a `MetaValue`
    ///
    /// @tparam TFunction The callable to check
    /// @tparam TType The metaprogramming type to check that `TFunction` is invocable with
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TFunction, typename TType>
    concept MetaFunctionOf = std::is_invocable_v<TFunction, TType>
                             && (MetaType<TType> || MetaValue<TType> || MetaPair<TType> || MetaList<TType>)
                             && (MetaType<std::invoke_result_t<TFunction, TType>>
                                 || MetaValue<std::invoke_result_t<TFunction, TType>>
                                 || MetaPair<std::invoke_result_t<TFunction, TType>>
                                 || MetaList<std::invoke_result_t<TFunction, TType>>);

    // clang-format on

    /// @brief A `MetaFunction` is a callable metafunction that accepts a single value parameter
    /// of `MetaType`, `MetaValue`, or `MetaPair` metaprogramming type category and returns either a
    /// `MetaType` or a `MetaValue`
    ///
    /// @tparam TFunction The callable to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TFunction>
    concept MetaFunction
        = MetaFunctionOf<TFunction, Type<bool>> || MetaFunctionOf<TFunction, Value<true, bool>>
          || MetaFunctionOf<TFunction, Pair<Type<bool>, Type<bool>>>
          || MetaFunctionOf<TFunction, List<Type<bool>, Type<float>>>;

    /// @brief `meta_result` is a type trait representing the invoke result of a callable
    /// metafunction, `TFunction`, with a metaprogramming type, `TType`
    ///
    /// If `TFunction` is a `MetaFunctionOf<TType>`, contains the member using alias type, `type`,
    /// that is the resulting type of invoking a `TFunction` with a `TType`.
    /// If `TFunction` is not a `MetaFunctionOf<TType>`, the program is ill-formed.
    ///
    /// # Requirements
    /// - `TFunction` must be a `MetaFunctionOf<TType>`
    ///     - It must be a callable invocable with `TType`
    ///     - The result of invoking `TFunction` with `TType` must be either a `MetaType` or a
    ///     `MetaValue`
    ///
    /// @tparam TFunction The callable metafunction to get the invoke result of
    /// @tparam TType The metaprogramming type to invoke `TFunction` with
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TFunction, typename TType>
        requires MetaFunctionOf<TFunction, TType>
    struct meta_result {
        using type = std::invoke_result_t<TFunction, TType>;
    };

    /// @brief Shorthand alias to the member using alias type, `type`, of the type trait
    /// `meta_result`.
    /// Represents the invoke result of a callable metafunction, `TFunction`, with a metaprogramming
    /// type, `TType`
    ///
    /// If `TFunction` is a `MetaFunctionOf<TType>`, using aliases to the resulting type of invoking
    /// a `TFunction` with a `TType`.
    /// If `TFunction` is not a `MetaFunctionOf<TType>`, the program is ill-formed.
    ///
    /// # Requirements
    /// - `TFunction` must be a `MetaFunctionOf<TType>`
    ///     - It must be a callable invocable with `TType`
    ///     - The result of invoking `TFunction` with `TType` must be either a `MetaType` or a
    ///     `MetaValue`
    ///
    /// @tparam TFunction The callable metafunction to get the invoke result of
    /// @tparam TType The metaprogramming type to invoke `TFunction` with
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TFunction, typename TType>
    using meta_result_t = typename meta_result<TFunction, TType>::type;

    /// @brief A `MetaPredicateOf` is a callable metafunction predicate that
    /// accepts a single value parameter of the specified type, `TType`,
    /// and returns a `MetaValue` with value type of `bool`.
    ///
    /// @tparam TFunction The callable to check
    /// @tparam TType The metaprogramming type to check that `TPredicate` is invocable with
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TPredicate, typename TType>
    concept MetaPredicateOf
        = MetaFunctionOf<TPredicate, TType> && MetaValue<meta_result_t<TPredicate, TType>>
          && std::same_as<std::remove_cvref_t<decltype(meta_result_t<TPredicate, TType>::value)>,
                          bool>;
} // namespace hyperion::mpl

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;

#endif // HYPERION_MPL_METATYPES_H
