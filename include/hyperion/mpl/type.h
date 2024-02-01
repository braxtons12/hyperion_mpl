/// @file type.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Metaprogramming type wrapper for use as metafunction parameter and return type
/// @version 0.1
/// @date 2024-01-31
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

#include <hyperion/mpl/value.h>

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

    template<typename TType>
    concept Metatype = requires { typename TType::type; };

    template<typename TType>
    struct is_metatype : std::bool_constant<Metatype<TType>> { };

    template<typename TType>
    static inline constexpr auto is_metatype_v = is_metatype<TType>::value;

    template<typename TType>
    struct Type {
        using type = TType;

        template<template<typename> typename TPredicate>
            requires ValueType<TPredicate<type>>
                     && std::same_as<std::remove_const_t<decltype(TPredicate<type>::value)>, bool>
        [[nodiscard]] constexpr auto satisfies() noexcept -> Value<TPredicate<type>::value> {
            return {};
        }

        template<template<typename> typename TMetaFunction>
            requires ValueType<TMetaFunction<type>>
        [[nodiscard]] constexpr auto apply() noexcept -> Value<TMetaFunction<type>::value> {
            return {};
        }

        template<template<typename> typename TMetaFunction>
            requires Metatype<TMetaFunction<type>>
        [[nodiscard]] constexpr auto apply() noexcept -> Type<typename TMetaFunction<type>::type> {
            return {};
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is([[maybe_unused]] const Type<TRhs>& rhs) noexcept -> Value<std::same_as<type, TRhs>> {
            return {};
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_qualification_of([[maybe_unused]] const Type<TRhs>& rhs) noexcept {
            return Type<TRhs>{}.is(Type<std::remove_cvref_t<type>>{});
        }

        [[nodiscard]] constexpr auto is_const() noexcept
            -> Value<std::is_const_v<type> || std::is_const_v<std::remove_reference_t<type>>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_lvalue_reference() noexcept -> Value<std::is_lvalue_reference_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_rvalue_reference() noexcept -> Value<std::is_rvalue_reference_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_volatile() noexcept -> Value<std::is_volatile_v<type>
                                        || std::is_volatile_v<std::remove_reference_t<type>>> {
            return {};
        }

        [[nodiscard]] constexpr auto as_const() noexcept {
            using constified = std::add_const_t<std::remove_reference_t<type>>;
            if constexpr(is_lvalue_reference()) {
                return Type<std::add_lvalue_reference_t<constified>>{};
            }
            else if constexpr(is_rvalue_reference()) {
                return Type<std::add_rvalue_reference_t<constified>>{};
            }
            else {
                return Type<constified>{};
            }
        }

        [[nodiscard]] constexpr auto as_lvalue_reference() noexcept {
            using base_type = std::remove_reference_t<type>;
            if constexpr(is_lvalue_reference()) {
                return Type<type>{};
            }
            else {
                return Type<std::add_lvalue_reference_t<base_type>>{};
            }
        }

        [[nodiscard]] constexpr auto as_rvalue_reference() noexcept {
            using base_type = std::remove_reference_t<type>;
            if constexpr(is_rvalue_reference()) {
                return Type<type>{};
            }
            else {
                return Type<std::add_rvalue_reference_t<base_type>>{};
            }
        }

        [[nodiscard]] constexpr auto as_volatile() noexcept {
            using volatilified = std::add_volatile_t<std::remove_reference_t<type>>;
            if constexpr(is_lvalue_reference()) {
                return Type<std::add_lvalue_reference_t<volatilified>>{};
            }
            else if constexpr(is_rvalue_reference()) {
                return Type<std::add_rvalue_reference_t<volatilified>>{};
            }
            else {
                return Type<volatilified>{};
            }
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_convertible_to([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::convertible_to<type, TRhs>> {
            return {};
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_derived_from([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::derived_from<type, TRhs>> {
            return {};
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto is_base_of([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::derived_from<TRhs, type>> {
            return {};
        }

        template<template<typename...> typename TList, typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const TList<TTypes...>& list) noexcept
            -> Value<std::is_constructible_v<type, TTypes...>> {
            return {};
        }

        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const Type<TTypes>&... list) noexcept
            -> Value<std::is_constructible_v<type, TTypes...>> {
            return {};
        }

        template<template<typename...> typename TList, typename... TTypes>
        [[nodiscard]] constexpr auto
        is_noexcept_constructible_from([[maybe_unused]] const TList<TTypes...>& list) noexcept
            -> Value<std::is_nothrow_constructible_v<type, TTypes...>> {
            return {};
        }

        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_noexcept_constructible_from([[maybe_unused]] const Type<TTypes>&... list) noexcept
            -> Value<std::is_nothrow_constructible_v<type, TTypes...>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_default_constructible() noexcept -> Value<std::is_default_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_noexcept_default_constructible() noexcept
            -> Value<std::is_nothrow_default_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_trivially_default_constructible() noexcept
            -> Value<std::is_trivially_default_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_copy_constructible() noexcept -> Value<std::is_copy_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_noexcept_copy_constructible() noexcept
            -> Value<std::is_nothrow_copy_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_trivially_copy_constructible() noexcept
            -> Value<std::is_trivially_copy_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_move_constructible() noexcept -> Value<std::is_move_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_noexcept_move_constructible() noexcept
            -> Value<std::is_nothrow_move_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_trivially_move_constructible() noexcept
            -> Value<std::is_trivially_move_constructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_copy_assignable() noexcept -> Value<std::is_copy_assignable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_noexcept_copy_assignable() noexcept -> Value<std::is_nothrow_copy_assignable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_trivially_copy_assignable() noexcept
            -> Value<std::is_trivially_copy_assignable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_move_assignable() noexcept -> Value<std::is_move_assignable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_noexcept_move_assignable() noexcept -> Value<std::is_nothrow_move_assignable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_trivially_move_assignable() noexcept
            -> Value<std::is_trivially_move_assignable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_destructible() noexcept -> Value<std::is_destructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_noexcept_destructible() noexcept -> Value<std::is_nothrow_destructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_trivially_destructible() noexcept -> Value<std::is_trivially_destructible_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto is_swappable() noexcept -> Value<std::is_swappable_v<type>> {
            return {};
        }

        [[nodiscard]] constexpr auto
        is_noexcept_swappable() noexcept -> Value<std::is_nothrow_swappable_v<type>> {
            return {};
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_swappable_with([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::is_swappable_with_v<type, TRhs>> {
            return {};
        }

        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_noexcept_swappable_with([[maybe_unused]] const Type<TRhs>& rhs) noexcept
            -> Value<std::is_nothrow_swappable_with_v<type, TRhs>> {
            return {};
        }
    };

    template<typename TType>
        requires std::is_reference_v<TType>
    [[nodiscard]] constexpr auto
    decltype_([[maybe_unused]] TType&& type) // NOLINT(*-missing-std-forward)
        noexcept -> Type<decltype(type)> {
        return {};
    }

    template<typename TType>
        requires(!std::is_reference_v<TType>)
    [[nodiscard]] constexpr auto
    decltype_([[maybe_unused]] TType&& type) // NOLINT(*-missing-std-forward)
        noexcept -> Type<TType> {
        return {};
    }

    namespace _test {

        constexpr int test_val = 1;
        static_assert(std::same_as<int, typename decltype(decltype_(1))::type>,
                      "hyperion::mpl::decltype_ test case 1 (failing)");
        static_assert(std::same_as<const int&, typename decltype(decltype_(test_val))::type>,
                      "hyperion::mpl::decltype_ test case 2 (failing)");

    } // namespace _test
} // namespace hyperion::mpl

#endif // HYPERION_MPL_TYPE_H
