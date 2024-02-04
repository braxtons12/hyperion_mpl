/// @file type.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Metaprogramming type wrapper for use as metafunction parameter and return type
/// @version 0.1
/// @date 2024-02-03
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

#ifndef HYPERION_MPL_TYPE_H
#define HYPERION_MPL_TYPE_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/platform/def.h>

//

#include <concepts>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup type Metaprogramming Type Wrapper
/// Hyperion provides `mpl::Type` as a metaprogramming type for storing, communicating,
/// working with, and operating on types.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/type.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto type1 = Type<int>{};
/// constexpr auto type2 = Value<double>{};
///
/// static_assert(not type1.is(type2));
/// @endcode
/// @headerfile hyperion/mpl/type_traits.h
/// @}

namespace hyperion::mpl {
    template<auto TValue, typename TType>
    struct Value;

    template<typename TType>
    struct Type {
        using type = TType;

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic push");
        _Pragma("GCC diagnostic ignored \"-Wdeprecated-volatile\"");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        /// @brief Returns the inner `MetaType` or `MetaValue` `type` of `this` `Type`
        ///
        /// # Requirements
        /// - `type` is default constructible
        /// - `type` is a `MetaType` or a `MetaValue`
        ///
        /// @return The inner `MetaType` or `MetaValue` of `this` `Type`
        ///
        /// @ingroup type
        /// @headerfile hyperion/mpl/type.h
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto inner() noexcept -> TDelay
            requires std::is_default_constructible_v<TDelay> && MetaValue<TDelay>
        {
            return {};
        }

#if HYPERION_PLATFORM_COMPILER_IS_CLANG
        _Pragma("GCC diagnostic pop");
#endif // HYPERION_PLATFORM_COMPILER_IS_CLANG

        /// @brief Returns whether the `type` of this `Type` is also a `MetaType` or `MetaValue`
        /// (in which case, you may call `this->inner()` to obtain that `MetaType` or `MetaValue`).
        /// @return Whether the `type` of this `Type` is also a `MetaType` or `MetaValue`
        /// @ingroup type
        /// @headerfile hyperion/mpl/type.h
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto has_inner() noexcept -> bool {
            return MetaValue<TDelay>;
        }

        template<template<typename> typename TMetaFunction>
        [[nodiscard]] constexpr auto apply() noexcept -> std::enable_if_t<
            TypeMetaFunction<TMetaFunction> && MetaValue<TMetaFunction<type>>,
            Value<TMetaFunction<type>::value, decltype(TMetaFunction<type>::value)>>;

        template<template<typename> typename TMetaFunction>
            requires TypeMetaFunction<TMetaFunction> && MetaType<TMetaFunction<type>>
                     && (!MetaValue<typename TMetaFunction<type>::type>)
                     && (!MetaType<typename TMetaFunction<type>::type>)
        [[nodiscard]] constexpr auto apply()
            // NOLINTNEXTLINE(modernize-type-traits)
            noexcept -> Type<typename TMetaFunction<type>::type> {
            return {};
        }

        template<template<typename> typename TMetaFunction>
            requires TypeMetaFunction<TMetaFunction> && MetaType<TMetaFunction<type>>
                     && (MetaValue<typename TMetaFunction<type>::type>
                         || MetaType<typename TMetaFunction<type>::type>)
        [[nodiscard]] constexpr auto apply()
            // NOLINTNEXTLINE(modernize-type-traits)
            noexcept {
            if constexpr(MetaValue<typename TMetaFunction<type>::type>) {
                return Value<TMetaFunction<type>::type::value,
                             std::remove_cvref_t<decltype(TMetaFunction<type>::type::value)>>{};
            }
            else {
                return Type<typename TMetaFunction<type>::type::type>{};
            }
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Type<type>>
                     && MetaType<meta_result_t<TFunction, Type<type>>>
                     && (!MetaType<typename meta_result_t<TFunction, Type<type>>::type>)
        [[nodiscard]] constexpr auto apply(
            [[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
                                               // NOLINTNEXTLINE(modernize-type-traits)
            noexcept -> Type<typename meta_result_t<TFunction, Type<type>>::type> {
            return {};
        }

        template<typename TFunction>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            noexcept -> std::enable_if_t<
                MetaFunctionOf<TFunction, Type<type>>
                    && MetaValue<meta_result_t<TFunction, Type<type>>>,
                Value<meta_result_t<TFunction, Type<type>>::value,
                      std::remove_cvref_t<decltype(meta_result_t<TFunction, Type<type>>::value)>>>;

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, type> && MetaType<meta_result_t<TFunction, type>>
                     && MetaValue<typename meta_result_t<TFunction, type>::type> && MetaValue<type>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            noexcept {
            return type{}.apply(TFunction{});
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, type> && MetaType<meta_result_t<TFunction, type>>
                     && (!MetaValue<typename meta_result_t<TFunction, type>::type>)
                     && (!MetaType<typename meta_result_t<TFunction, type>::type>)
                     && MetaValue<type>
        [[nodiscard]] constexpr auto apply(
            [[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
                                               // NOLINTNEXTLINE(modernize-type-traits)
            noexcept {
            return type{}.apply(TFunction{});
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, type> && MetaValue<meta_result_t<TFunction, type>>
                     && MetaValue<type>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            noexcept {
            return type{}.apply(TFunction{});
        }

        template<template<typename> typename TPredicate>
        [[nodiscard]] constexpr auto satisfies() noexcept -> std::enable_if_t<
            MetaValue<TPredicate<type>>
                && std::same_as<std::remove_const_t<decltype(TPredicate<type>::value)>, bool>,
            Value<TPredicate<type>::value, bool>>;

        template<typename TPredicate>
            requires MetaFunctionOf<TPredicate, Type<type>>
                     && MetaValue<meta_result_t<TPredicate, Type<type>>>
                     && std::same_as<std::remove_cvref_t<
                                         decltype(meta_result_t<TPredicate, Type<type>>::value)>,
                                     bool>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            noexcept -> meta_result_t<TPredicate, Type<type>> {
            return {};
        }

        template<typename TPredicate>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            noexcept -> std::enable_if_t<
                MetaFunctionOf<TPredicate, Type<type>>
                    && MetaType<meta_result_t<TPredicate, Type<type>>>
                    && MetaValue<typename meta_result_t<TPredicate, Type<type>>::type>
                    && std::same_as<
                        std::remove_cvref_t<
                            decltype(meta_result_t<TPredicate, Type<type>>::type::value)>,
                        bool>,
                Value<meta_result_t<TPredicate, Type<type>>::type::value, bool>>;

        template<typename TPredicate>
            requires MetaFunctionOf<TPredicate, type> && MetaValue<meta_result_t<TPredicate, type>>
                     && std::same_as<
                         std::remove_cvref_t<decltype(meta_result_t<TPredicate, type>::value)>,
                         bool>
                     && MetaValue<type>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-forward)
            noexcept -> meta_result_t<TPredicate, type> {
            return {};
        }

        template<typename TPredicate>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-forward)
            noexcept -> std::enable_if_t<
                MetaFunctionOf<TPredicate, type> && MetaType<meta_result_t<TPredicate, type>>
                    && MetaValue<typename meta_result_t<TPredicate, type>::type>
                    && std::same_as<
                        std::remove_cvref_t<decltype(meta_result_t<TPredicate, type>::type::value)>,
                        bool>
                    && MetaValue<type>,
                Value<meta_result_t<TPredicate, type>::type::value, bool>>;

        template<typename TRhs>
        [[nodiscard]] constexpr auto is([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::same_as<type, TRhs>, bool>;

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_qualification_of([[maybe_unused]] const Type<TRhs>& rhs) noexcept {
            return Type<TRhs>{}.is(Type<std::remove_cvref_t<type>>{});
        }

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_const() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_const_v<std::remove_reference_t<TDelay>>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_lvalue_reference() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_lvalue_reference_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_rvalue_reference() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_rvalue_reference_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_volatile() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_volatile_v<std::remove_reference_t<TDelay>>, bool>>;

        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_const() noexcept {
            using constified = std::add_const_t<std::remove_reference_t<TDelay>>;
            if constexpr(Type<TDelay>{}.is_lvalue_reference()) {
                return Type<std::add_lvalue_reference_t<constified>>{};
            }
            else if constexpr(Type<TDelay>{}.is_rvalue_reference()) {
                return Type<std::add_rvalue_reference_t<constified>>{};
            }
            else {
                return Type<constified>{};
            }
        }

        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_lvalue_reference() noexcept {
            using base_type = std::remove_reference_t<TDelay>;
            if constexpr(Type<TDelay>{}.is_lvalue_reference()) {
                return Type<TDelay>{};
            }
            else {
                return Type<std::add_lvalue_reference_t<base_type>>{};
            }
        }

        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_rvalue_reference() noexcept {
            using base_type = std::remove_reference_t<TDelay>;
            if constexpr(Type<TDelay>{}.is_rvalue_reference()) {
                return Type<TDelay>{};
            }
            else {
                return Type<std::add_rvalue_reference_t<base_type>>{};
            }
        }

        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_volatile() noexcept {
            using volatilified = std::add_volatile_t<std::remove_reference_t<TDelay>>;
            if constexpr(Type<TDelay>{}.is_lvalue_reference()) {
                return Type<std::add_lvalue_reference_t<volatilified>>{};
            }
            else if constexpr(Type<TDelay>{}.is_rvalue_reference()) {
                return Type<std::add_rvalue_reference_t<volatilified>>{};
            }
            else {
                return Type<volatilified>{};
            }
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_convertible_to([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::convertible_to<type, TRhs>, bool>;

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_derived_from([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::derived_from<type, TRhs> && !std::same_as<type, TRhs>, bool>;

        template<typename TRhs>
        [[nodiscard]] constexpr auto is_base_of([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::derived_from<TRhs, type> && !std::same_as<type, TRhs>, bool>;

        template<template<typename...> typename TList, typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const TList<TTypes...>& list) noexcept
            -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                                Value<std::is_constructible_v<type, TTypes...>, bool>>;

        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const Type<TTypes>&... list) noexcept
            -> Value<std::is_constructible_v<type, TTypes...>, bool>;

        template<template<typename...> typename TList, typename... TTypes>
        [[nodiscard]] constexpr auto
        is_noexcept_constructible_from([[maybe_unused]] const TList<TTypes...>& list) noexcept
            -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                                Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>>;

        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_noexcept_constructible_from([[maybe_unused]] const Type<TTypes>&... list) noexcept
            -> Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_default_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_default_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_default_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,

                                Value<std::is_nothrow_default_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_default_constructible() noexcept

            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_default_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_copy_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_copy_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_copy_constructible() noexcept

            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_copy_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_copy_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_copy_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_move_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_move_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_move_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_move_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_move_constructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_move_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_copy_assignable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_copy_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_copy_assignable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_copy_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_copy_assignable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_copy_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_move_assignable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_move_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_move_assignable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_move_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_move_assignable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_move_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto
        is_destructible() noexcept -> std::enable_if_t<std::same_as<TDelay, type>,
                                                       Value<std::is_destructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_destructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_destructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_destructible() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_destructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto
        is_swappable() noexcept -> std::enable_if_t<std::same_as<TDelay, type>,
                                                    Value<std::is_swappable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_swappable() noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_swappable_v<TDelay>, bool>>;

        template<typename TRhs = std::conditional_t<std::is_reference_v<type>,
                                                    type,
                                                    std::add_lvalue_reference_t<type>>>
        [[nodiscard]] constexpr auto
        is_swappable_with([[maybe_unused]] const Type<TRhs>& rhs = Type<TRhs>{}) noexcept
            -> Value<std::is_swappable_with_v<std::conditional_t<std::is_reference_v<type>,
                                                                 type,
                                                                 std::add_lvalue_reference_t<type>>,
                                              TRhs>,
                     bool>;

        template<typename TRhs = std::conditional_t<std::is_reference_v<type>,
                                                    type,
                                                    std::add_lvalue_reference_t<type>>>
        [[nodiscard]] constexpr auto
        is_noexcept_swappable_with([[maybe_unused]] const Type<TRhs>& rhs = Type<TRhs>{}) noexcept
            -> Value<std::is_nothrow_swappable_with_v<
                         std::conditional_t<std::is_reference_v<type>,
                                            type,
                                            std::add_lvalue_reference_t<type>>,
                         TRhs>,
                     bool>;
    };

    template<typename TType>
        requires std::is_reference_v<TType>
    [[nodiscard]] constexpr auto
    decltype_([[maybe_unused]] TType&& type) // NOLINT(*-missing-std-forward)
        noexcept -> std::conditional_t<MetaType<std::remove_cvref_t<TType>>,
                                       std::remove_cvref_t<TType>,
                                       Type<decltype(type)>> {
        return {};
    }

    template<typename TType>
        requires(!std::is_reference_v<TType>)
    [[nodiscard]] constexpr auto
    decltype_([[maybe_unused]] TType&& type) // NOLINT(*-missing-std-forward)
        noexcept -> std::conditional_t<MetaType<TType>, TType, Type<TType>> {
        return {};
    }

    template<typename TType>
    [[nodiscard]] constexpr auto
    decltype_() noexcept -> std::conditional_t<MetaType<TType>, TType, Type<TType>> {
        return {};
    }

    template<typename TLhs, typename TRhs>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Type<TLhs>& lhs,
               [[maybe_unused]] const Type<TRhs>& rhs) noexcept -> bool {
        return Type<TLhs>{}.is(Type<TRhs>{});
    }

    template<typename TLhs, typename TRhs>
    [[nodiscard]] constexpr auto
    operator!=([[maybe_unused]] const Type<TLhs>& lhs,
               [[maybe_unused]] const Type<TRhs>& rhs) noexcept -> bool {
        return !(Type<TLhs>{}.is(Type<TRhs>{}));
    }
} // namespace hyperion::mpl

// NOLINTNEXTLINE(misc-header-include-cycle)
#include <hyperion/mpl/value.h>

namespace hyperion::mpl {

    template<typename TType>
    template<template<typename> typename TMetaFunction>
    [[nodiscard]] constexpr auto Type<TType>::apply() noexcept -> std::enable_if_t<
        TypeMetaFunction<TMetaFunction> && MetaValue<TMetaFunction<type>>,
        Value<TMetaFunction<type>::value, decltype(TMetaFunction<type>::value)>> {
        return {};
    }

    template<typename TType>
    template<typename TFunction>
    [[nodiscard]] constexpr auto
    Type<TType>::apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
        noexcept -> std::enable_if_t<
            MetaFunctionOf<TFunction, Type<type>>
                && MetaValue<meta_result_t<TFunction, Type<type>>>,
            Value<meta_result_t<TFunction, Type<type>>::value,
                  std::remove_cvref_t<decltype(meta_result_t<TFunction, Type<type>>::value)>>> {
        return as_value(meta_result_t<TFunction, Type<type>>{});
    }

    template<typename TType>
    template<template<typename> typename TPredicate>
    [[nodiscard]] constexpr auto Type<TType>::satisfies() noexcept -> std::enable_if_t<
        MetaValue<TPredicate<type>>
            && std::same_as<std::remove_const_t<decltype(TPredicate<type>::value)>, bool>,
        Value<TPredicate<type>::value, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TPredicate>
    [[nodiscard]] constexpr auto
    Type<TType>::satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
        noexcept -> std::enable_if_t<
            MetaFunctionOf<TPredicate, Type<type>>
                && MetaType<meta_result_t<TPredicate, Type<type>>>
                && MetaValue<typename meta_result_t<TPredicate, Type<type>>::type>
                && std::same_as<std::remove_cvref_t<
                                    decltype(meta_result_t<TPredicate, Type<type>>::type::value)>,
                                bool>,
            Value<meta_result_t<TPredicate, Type<type>>::type::value, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TPredicate>
    [[nodiscard]] constexpr auto
    Type<TType>::satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-forward)
        noexcept -> std::enable_if_t<
            MetaFunctionOf<TPredicate, type> && MetaType<meta_result_t<TPredicate, type>>
                && MetaValue<typename meta_result_t<TPredicate, type>::type>
                && std::same_as<
                    std::remove_cvref_t<decltype(meta_result_t<TPredicate, type>::type::value)>,
                    bool>
                && MetaValue<type>,
            Value<meta_result_t<TPredicate, type>::type::value, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto Type<TType>::is([[maybe_unused]] const Type<TRhs>& rhs) noexcept
        -> Value<std::same_as<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_const() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_const_v<std::remove_reference_t<TDelay>>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_lvalue_reference() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_lvalue_reference_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_rvalue_reference() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_rvalue_reference_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_volatile() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_volatile_v<std::remove_reference_t<TDelay>>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_convertible_to([[maybe_unused]] const Type<TRhs>& rhs) noexcept
        -> Value<std::convertible_to<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_derived_from([[maybe_unused]] const Type<TRhs>& rhs) noexcept
        -> Value<std::derived_from<type, TRhs> && !std::same_as<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_base_of([[maybe_unused]] const Type<TRhs>& rhs) noexcept
        -> Value<std::derived_from<TRhs, type> && !std::same_as<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<template<typename...> typename TList, typename... TTypes>
    [[nodiscard]] constexpr auto
    Type<TType>::is_constructible_from([[maybe_unused]] const TList<TTypes...>& list) noexcept
        -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                            Value<std::is_constructible_v<type, TTypes...>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename... TTypes>
    [[nodiscard]] constexpr auto
    Type<TType>::is_constructible_from([[maybe_unused]] const Type<TTypes>&... list) noexcept
        -> Value<std::is_constructible_v<type, TTypes...>, bool> {
        return {};
    }

    template<typename TType>
    template<template<typename...> typename TList, typename... TTypes>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_constructible_from(
        [[maybe_unused]] const TList<TTypes...>& list) noexcept
        -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                            Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename... TTypes>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_constructible_from(
        [[maybe_unused]] const Type<TTypes>&... list) noexcept
        -> Value<std::is_nothrow_constructible_v<type, TTypes...>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_default_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_default_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_default_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_default_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_default_constructible() noexcept

        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_default_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_copy_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_copy_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_copy_constructible() noexcept

        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_copy_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_copy_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_copy_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_move_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_move_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_move_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_move_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_move_constructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_move_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_copy_assignable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_copy_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_copy_assignable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_copy_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_copy_assignable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_copy_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_move_assignable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_move_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_move_assignable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_move_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_move_assignable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_move_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_destructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_destructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_destructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_destructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_destructible() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_destructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_swappable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>, Value<std::is_swappable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_swappable() noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_swappable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_swappable_with([[maybe_unused]] const Type<TRhs>& rhs) noexcept -> Value<
        std::is_swappable_with_v<
            std::conditional_t<std::is_reference_v<type>, type, std::add_lvalue_reference_t<type>>,
            TRhs>,
        bool> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_noexcept_swappable_with([[maybe_unused]] const Type<TRhs>& rhs) noexcept
        -> Value<
            std::is_nothrow_swappable_with_v<std::conditional_t<std::is_reference_v<type>,
                                                                type,
                                                                std::add_lvalue_reference_t<type>>,
                                             TRhs>,
            bool> {
        return {};
    }

    namespace _test::type {
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

        constexpr auto is_const_type_type = [](const MetaType auto& type) noexcept
            -> Type<Value<(std::remove_cvref_t<decltype(type)>{}).is_const(), bool>> {
            return {};
        };

        constexpr auto is_const_type_value = [](const MetaType auto& type) noexcept
            -> Value<(std::remove_cvref_t<decltype(type)>{}).is_const(), bool> {
            return {};
        };

        constexpr auto is_true_value_type = [](const MetaValue auto& value) noexcept
            -> Type<Value<std::remove_cvref_t<decltype(value)>::value, bool>> {
            return {};
        };

        constexpr auto is_true_value_value = [](const MetaValue auto& value) noexcept
            -> Value<std::remove_cvref_t<decltype(value)>::value, bool> {
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
                      "test case 3 (failing)");
        static_assert(!decltype_(0_value).satisfies(is_true_value_type),
                      "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Type<Value> "
                      "test case 4 (failing)");

        static_assert(decltype_(1_value).satisfies(is_true_value_value),
                      "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Value "
                      "test case 3 (failing)");
        static_assert(!decltype_(0_value).satisfies(is_true_value_value),
                      "hyperion::mpl::Type::satisfies(MetaFunctionOf<Value>) -> Value "
                      "test case 4 (failing)");

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

        constexpr auto add1 = [](const MetaValue auto& value)
            -> Value<std::remove_cvref_t<decltype(value)>::value + 1> {
            return {};
        };

        constexpr auto times2 = [](const MetaValue auto& value)
            -> Value<std::remove_cvref_t<decltype(value)>::value * 2> {
            return {};
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

        constexpr auto add1_semi_typely = [](const MetaType auto& type)
            -> Value<std::remove_cvref_t<decltype(type)>::type::value + 1>
            requires MetaValue<typename std::remove_cvref_t<decltype(type)>::type>
        {
            return {};
        };

        constexpr auto times2_semi_typely = [](const MetaType auto& type)
            -> Value<std::remove_cvref_t<decltype(type)>::type::value * 2>
            requires MetaValue<typename std::remove_cvref_t<decltype(type)>::type>
        {
            return {};
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

        constexpr auto add1_fully_typely = [](const MetaType auto& type)
            -> Type<Value<std::remove_cvref_t<decltype(type)>::type::value + 1>>
            requires MetaValue<typename std::remove_cvref_t<decltype(type)>::type>
        {
            return {};
        };

        constexpr auto times2_fully_typely = [](const MetaType auto& type)
            -> Type<Value<std::remove_cvref_t<decltype(type)>::type::value * 2>>
            requires MetaValue<typename std::remove_cvref_t<decltype(type)>::type>
        {
            return {};
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

        constexpr auto add_const = [](const MetaType auto& type)
            -> std::add_const<typename std::remove_cvref_t<decltype(type)>::type> {
            return {};
        };

        constexpr auto add_lvalue_reference = [](const MetaType auto& type)
            -> std::add_lvalue_reference<typename std::remove_cvref_t<decltype(type)>::type> {
            return {};
        };

        constexpr auto remove_reference = [](const MetaType auto& type)
            -> std::remove_reference<typename std::remove_cvref_t<decltype(type)>::type> {
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

        constexpr auto add_const_typed = [](const MetaType auto& type)
            -> Type<std::add_const_t<typename std::remove_cvref_t<decltype(type)>::type>> {
            return {};
        };

        constexpr auto add_lvalue_reference_typed = [](const MetaType auto& type)
            -> Type<
                std::add_lvalue_reference_t<typename std::remove_cvref_t<decltype(type)>::type>> {
            return {};
        };

        constexpr auto remove_reference_typed = [](const MetaType auto& type)
            -> Type<std::remove_reference_t<typename std::remove_cvref_t<decltype(type)>::type>> {
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

        static_assert(decltype_<int&&>().is_rvalue_reference(),
                      "hyperion::mpl::Type::is_rvalue_reference test case 1 (failing)");
        static_assert(decltype_<const int&&>().is_rvalue_reference(),
                      "hyperion::mpl::Type::is_rvalue_reference test case 2 (failing)");
        static_assert(!decltype_<int>().is_rvalue_reference(),
                      "hyperion::mpl::Type::is_rvalue_reference test case 3 (failing)");

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
                requires(!bool(decltype_<TType>().is_qualification_of(
                    decltype_<constructible_from_anything>())))
            explicit constructible_from_anything(TType&&);

            template<typename... TTypes>
                requires(sizeof...(TTypes) > 1)
            explicit constructible_from_anything(TTypes&&...) noexcept;
        };

        template<typename... TTypes>
        struct test_list { };

        static_assert(decltype_<constructible_from_int>().is_constructible_from(decltype_<int>()),
                      "hyperion::mpl::Type::is_constructible_from(MetaType) test case 1 (failing)");
        static_assert(
            decltype_<constructible_from_int>().is_constructible_from(decltype_<double>()),
            "hyperion::mpl::Type::is_constructible_from(MetaType) test case 2 (failing)");
        static_assert(
            !decltype_<constructible_from_int>().is_constructible_from(decltype_<void*>()),
            "hyperion::mpl::Type::is_constructible_from(MetaType) test case 3 (failing)");
        static_assert(
            decltype_<constructible_from_anything>().is_constructible_from(decltype_<int>()),
            "hyperion::mpl::Type::is_constructible_from(MetaType) test case 4 (failing)");
        static_assert(
            decltype_<constructible_from_anything>().is_constructible_from(decltype_<void*>()),
            "hyperion::mpl::Type::is_constructible_from(MetaType) test case 5 (failing)");

        static_assert(decltype_<constructible_from_int>().is_constructible_from(test_list<int>{}),
                      "hyperion::mpl::Type::is_constructible_from(List) test case 1 (failing)");
        static_assert(
            decltype_<constructible_from_int>().is_constructible_from(test_list<double>{}),
            "hyperion::mpl::Type::is_constructible_from(List) test case 2 (failing)");
        static_assert(
            !decltype_<constructible_from_int>().is_constructible_from(test_list<void*>{}),
            "hyperion::mpl::Type::is_constructible_from(List) test case 3 (failing)");
        static_assert(
            !decltype_<constructible_from_int>().is_constructible_from(test_list<int, double>{}),
            "hyperion::mpl::Type::is_constructible_from(List) test case 4 (failing)");
        static_assert(
            decltype_<constructible_from_anything>().is_constructible_from(test_list<void*>{}),
            "hyperion::mpl::Type::is_constructible_from(List) test case 5 (failing)");
        static_assert(decltype_<constructible_from_anything>().is_constructible_from(
                          test_list<int, double>{}),
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
            !decltype_<constructible_from_anything>().is_noexcept_constructible_from(
                decltype_<int>()),
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

        static_assert(
            decltype_<int>().is_noexcept_default_constructible(),
            "hyperion::mpl::Type::is_noexcept_default_constructible test case 1 (failing)");
        static_assert(
            !decltype_<not_trivially_default_constructible>().is_noexcept_default_constructible(),
            "hyperion::mpl::Type::is_noexcept_default_constructible test case 2 (failing)");
        static_assert(
            decltype_<noexcept_default_constructible>().is_noexcept_default_constructible(),
            "hyperion::mpl::Type::is_noexcept_default_constructible test case 3 (failing)");
        static_assert(
            !decltype_<not_default_constructible>().is_noexcept_default_constructible(),
            "hyperion::mpl::Type::is_noexcept_default_constructible test case 4 (failing)");

        static_assert(
            decltype_<int>().is_trivially_default_constructible(),
            "hyperion::mpl::Type::is_trivially_default_constructible test case 1 (failing)");
        static_assert(
            decltype_<default_constructible>().is_trivially_default_constructible(),
            "hyperion::mpl::Type::is_trivially_default_constructible test case 2 (failing)");
        static_assert(
            decltype_<noexcept_default_constructible>().is_trivially_default_constructible(),
            "hyperion::mpl::Type::is_trivially_default_constructible test case 3 (failing)");
        static_assert(
            !decltype_<not_default_constructible>().is_trivially_default_constructible(),
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
        static_assert(
            !decltype_<not_trivially_copy_constructible>().is_noexcept_copy_constructible(),
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
        static_assert(
            !decltype_<not_trivially_copy_constructible>().is_trivially_copy_constructible(),
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
        static_assert(
            !decltype_<not_trivially_move_constructible>().is_noexcept_move_constructible(),
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
        static_assert(
            !decltype_<not_trivially_move_constructible>().is_trivially_move_constructible(),
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
            auto operator=(const noexcept_copy_assignable&) noexcept
                -> noexcept_copy_assignable& = default;
        };

        // NOLINTNEXTLINE(*-special-member-functions)
        struct not_trivially_copy_assignable {
            auto
            operator=([[maybe_unused]] const not_trivially_copy_assignable& copy) noexcept(false)
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
            auto
            operator=(noexcept_move_assignable&&) noexcept -> noexcept_move_assignable& = default;
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
        };

        struct noexcept_swappable_with {
            friend void swap(noexcept_swappable_with& lhs, int& rhs) noexcept;
        };

        static_assert(decltype_<int>().is_swappable_with(),
                      "hyperion::mpl::Type::is_swappable_with test case 1 (failing)");
        static_assert(decltype_<swappable>().is_swappable_with(),
                      "hyperion::mpl::Type::is_swappable_with test case 2 (failing)");
        static_assert(decltype_<noexcept_swappable>().is_swappable_with(),
                      "hyperion::mpl::Type::is_swappable_with test case 3 (failing)");
        static_assert(!decltype_<not_swappable>().is_swappable_with(),
                      "hyperion::mpl::Type::is_swappable_with test case 4 (failing)");

        static_assert(decltype_<int>().is_noexcept_swappable_with(),
                      "hyperion::mpl::Type::is_noexcept_swappable_with test case 1 (failing)");
        static_assert(decltype_<noexcept_swappable>().is_noexcept_swappable_with(),
                      "hyperion::mpl::Type::is_noexcept_swappable_with test case 2 (failing)");
        static_assert(!decltype_<swappable>().is_noexcept_swappable_with(),
                      "hyperion::mpl::Type::is_noexcept_swappable_with test case 3 (failing)");
        static_assert(!decltype_<not_swappable>().is_noexcept_swappable_with(),
                      "hyperion::mpl::Type::is_noexcept_swappable_with test case 3 (failing)");

    } // namespace _test::type
} // namespace hyperion::mpl

#endif // HYPERION_MPL_TYPE_H
