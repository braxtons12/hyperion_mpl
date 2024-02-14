/// @file metatypes.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Concept and type trait definitions for what consitutes various categories of
/// metaprogramming types
/// @version 0.1
/// @date 2024-02-13
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

    // TODO(braxtons12): move Pair to its own header
    template<typename TFirst, typename TSecond>
    struct Pair {
        using first = TFirst;
        using second = TSecond;

        template<typename TFunction>
            requires std::is_invocable_v<TFunction, Pair>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> std::invoke_result_t<TFunction, Pair> {
            return {};
        }
    };

    template<typename TLHSFirst, typename TLHSSecond, typename TRHSFirst, typename TRHSSecond>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Pair<TLHSFirst, TLHSSecond>& lhs,
               [[maybe_unused]] const Pair<TRHSFirst, TRHSSecond>& rhs) noexcept -> bool {
        constexpr auto is_equal = []<typename TLhs, typename TRhs>() {
            if constexpr((MetaType<TLhs> && MetaType<TRhs>) || (MetaValue<TLhs> && MetaValue<TRhs>))
            {
                return TLhs{} == TRhs{};
            }
            else {
                return false;
            }
        };

        return is_equal.template operator()<TLHSFirst, TRHSFirst>()
               && is_equal.template operator()<TLHSSecond, TRHSSecond>();
    }

    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
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

    template<typename TType>
    struct is_meta_pair : std::false_type { };

    template<typename TFirst, typename TSecond>
    struct is_meta_pair<Pair<TFirst, TSecond>> : std::true_type { };

    template<typename TType>
    static inline constexpr auto is_meta_pair_v = is_meta_pair<TType>::value;

    template<typename TType>
    concept MetaPair = is_meta_pair_v<TType>;

    namespace detail {

        template<typename TType>
        struct Type {
            using type = TType;
        };

        template<auto TValue, typename TType = decltype(TValue)>
        struct Value {
            static inline constexpr auto value = TValue;
        };

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
            requires requires { typename TTemplate<Value<true>>; }
                     && (!requires { typename TTemplate<int>; })
        struct is_instantiatable_with_type<TTemplate> : std::true_type {
            using type = Value<true>;
        };

        template<template<auto> typename TTemplate>
        struct is_instantiatable_with_value : std::false_type { };

        template<template<auto> typename TTemplate>
            requires requires { typename TTemplate<true>; }
        struct is_instantiatable_with_value<TTemplate> : std::true_type { };

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

    /// @brief A `TypeMetaFunction` is a template metafunction that accepts a single value parameter
    /// and contains either a `static constexpr` member variable, `value`, or a member using alias
    /// type, `type`
    ///
    /// @tparam TTemplate The template to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<template<auto> typename TTemplate>
    concept ValueMetaFunction
        = detail::is_value_to_type<TTemplate>::value || detail::is_value_to_value<TTemplate>::value;

    /// @brief A `MetaFunctionOf` is a callable metafunction that accepts a single value parameter
    /// of the specified type, `TType` and returns either a `MetaType` or a `MetaValue`
    ///
    /// @tparam TFunction The callable to check
    /// @tparam TType The metaprogramming type to check that `TFunction` is invocable with
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TFunction, typename TType>
    concept MetaFunctionOf = std::is_invocable_v<TFunction, TType>
                             && (MetaType<TType> || MetaValue<TType> || MetaPair<TType>);

    /// @brief A `MetaFunction` is a callable metafunction that accepts a single value parameter
    /// of `MetaType`, `MetaValue`, or `MetaPair` metaprogramming type category and returns either a
    /// `MetaType` or a `MetaValue`
    ///
    /// @tparam TFunction The callable to check
    /// @ingroup metatypes
    /// @headerfile hyperion/mpl/metatypes.h
    template<typename TFunction>
    concept MetaFunction
        = MetaFunctionOf<TFunction, detail::Type<bool>>
          || MetaFunctionOf<TFunction, detail::Value<true>>
          || MetaFunctionOf<TFunction, Pair<detail::Type<bool>, detail::Type<bool>>>;

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

    namespace _test::metatypes {
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

        constexpr auto type_to_type
            = []([[maybe_unused]] const MetaType auto& type) -> detail::Type<bool> {
            return {};
        };
        constexpr auto type_to_value
            = []([[maybe_unused]] const MetaType auto& type) -> detail::Value<true> {
            return {};
        };
        constexpr auto value_to_type
            = []([[maybe_unused]] const MetaValue auto& type) -> detail::Type<bool> {
            return {};
        };
        constexpr auto value_to_value
            = []([[maybe_unused]] const MetaValue auto& type) -> detail::Value<true> {
            return {};
        };

        static_assert(MetaType<metatype>, "hyperion::mpl::MetaType test case 1 (failing)");
        static_assert(not MetaType<not_meta>, "hyperion::mpl::MetaType test case 2 (failing)");
        static_assert(MetaType<std::add_lvalue_reference<int>>,
                      "hyperion::mpl::MetaType test case 1 (failing)");
        static_assert(!MetaType<int>, "hyperion::mpl::MetaType test case 2 (failing)");
        static_assert(!MetaType<detail::Value<1>>, "hyperion::mpl::MetaType test case 3 (failing)");

        struct not_value_type {
            int value;
        };

        static_assert(MetaValue<metavalue>, "hyperion::mpl::MetaValue test case 1 (failing)");
        static_assert(not MetaValue<not_meta>, "hyperion::mpl::MetaValue test case 2 (failing)");
        static_assert(MetaValue<detail::Value<true>>,
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
        static_assert(MetaFunction<decltype(value_to_type)>,
                      "hyperion::mpl::MetaFunction test case 3 (failing)");
        static_assert(MetaFunction<decltype(value_to_value)>,
                      "hyperion::mpl::MetaFunction test case 4 (failing)");

        static_assert(std::is_same_v<meta_result_t<decltype(type_to_type), detail::Type<bool>>,
                                     detail::Type<bool>>,
                      "hyperion::mpl::meta_result test case 1 (failing)");
        static_assert(std::is_same_v<meta_result_t<decltype(type_to_value), detail::Type<bool>>,
                                     detail::Value<true>>,
                      "hyperion::mpl::meta_result test case 2 (failing)");
        static_assert(std::is_same_v<meta_result_t<decltype(value_to_type), detail::Value<true>>,
                                     detail::Type<bool>>,
                      "hyperion::mpl::met_result test case 3 (failing)");
        static_assert(std::is_same_v<meta_result_t<decltype(value_to_value), detail::Value<true>>,
                                     detail::Value<true>>,
                      "hyperion::mpl::met_result test case 4 (failing)");

    } // namespace _test::metatypes
} // namespace hyperion::mpl

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;

#endif // HYPERION_MPL_METATYPES_H
