/// @file value.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming value type
/// @version 0.1
/// @date 2024-02-24
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
#include <hyperion/platform/types.h>
//
#include <hyperion/mpl/concepts/comparable.h>
#include <hyperion/mpl/concepts/operator_able.h>
#include <hyperion/mpl/metatypes.h>

#if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
    #include <compare>
#endif // HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

#include <concepts>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup value Metaprogramming Value Type
/// Hyperion provides `mpl::Value` as a metaprogramming type for storing, communicating,
/// working with, and calculating compile-time values.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/value.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto val1 = Value<4>{};
/// constexpr auto val2 = Value<2>{};
///
/// constexpr auto meaning_of_life = (val1 * 10_value) + val2;
///
/// static_assert(meaning_of_life == 42);
/// @endcode
/// @headerfile hyperion/mpl/value.h
/// @}

#ifndef HYPERION_MPL_VALUE_H
    #define HYPERION_MPL_VALUE_H

namespace hyperion::mpl {
    template<typename TType>
    struct Type;

    template<auto TValue, typename TType>
    struct Value;

    /// @brief `Value` is Hyperion's preferred metaprogramming value type.
    ///
    /// `Value` represents a compile time value, storing that value in its
    /// `static constexpr` member variable, `value`.
    ///
    /// @tparam TValue The value of this `Value`
    /// @tparam TType The type of `TValue`. This can be manually specified to enforce
    /// that `TValue` is coerced to `TType` (for example, in order to force `TValue`
    /// to be an `int`, instead of `std::size_t`)
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = std::remove_cvref_t<decltype(TValue)>>
    struct Value {
        /// @brief The value of this metaprogramming value.
        static inline constexpr auto value = static_cast<TType>(TValue);
        /// @brief The type of `value`
        using value_type = TType;

        /// @brief Conversion operator to the `value`.
        /// @return The `value` of this specialization of `Value`
        [[nodiscard]] constexpr operator TType() const noexcept { // NOLINT
            return value;
        }

        /// @brief Call operator to get the `value`.
        /// @return The `value` of this specialization of `Value`
        [[nodiscard]] constexpr auto operator()() const noexcept -> TType {
            return value;
        }

        /// @brief Conversion operator to `bool`.
        /// Converts the `value` of this specialization of `Value` to `bool`.
        ///
        /// # Requirements
        /// - `TType` must be convertible to `bool`
        /// - `TType` must not be `bool`
        ///
        /// @return the `value` of this specialization of `Value`, converted to `bool`
        [[nodiscard]] explicit constexpr operator bool() const noexcept
            requires std::convertible_to<TType, bool> && (!std::same_as<TType, bool>)
        {
            return value;
        }

        /// @brief Returns the `value` of this `Value` specialization.
        /// @return `value`
        [[nodiscard]] constexpr auto value_of() const -> TType {
            return value;
        }

        /// @brief Applies the specified template metafunction to this specialization of `Value`.
        ///
        /// Applies `TMetaFunction` to this specialization of `Value` and returns the calculated
        /// result as a metaprogramming type.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `TypeMetaFunction`:
        ///     - It must be a template taking a single type parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename TValue>
        /// struct add_one_typed {
        ///     using type = Value<TValue::value + 1>;
        /// };
        ///
        /// // `two` is `Value<2, usize>`
        /// constexpr auto two = (1_value).apply<add_one>();
        /// static_assert(two == 2_value);
        ///
        /// template<typename TValue>
        /// struct add_one_typed {
        ///     using type = Type<Value<TValue::value + 1>>;
        /// };
        ///
        /// // `two_typed` is `Value<2, usize>`
        /// constexpr auto two_typed = (1_value).apply<add_one_typed>().inner();
        /// static_assert(two_typed == 2_value);
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply to this `Value`
        /// @return The result of applying `TMetaFunction`
        template<template<typename> typename TMetaFunction>
            requires TypeMetaFunction<TMetaFunction>
        [[nodiscard]] constexpr auto apply() const noexcept
            -> detail::unwrap_inner_t<detail::convert_to_meta_t<TMetaFunction<Value>>> {
            return {};
        }

        /// @brief Applies the specified template metafunction to this specialization of `Value`.
        ///
        /// Applies `TMetaFunction` to this specialization of `Value` and returns the calculated
        /// result as a metaprogramming type.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `ValueMetaFunction`:
        ///     - It must be a template taking a single value parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        ///
        /// # Example
        /// @code {.cpp}
        /// template<auto TValue>
        /// struct add_one {
        ///     static inline constexpr auto value = TValue + 1>;
        /// };
        ///
        /// constexpr auto two = (1_value).apply<add_one>(); // `two` is `Value<2, usize>`
        /// static_assert(two == 2_value);
        ///
        /// template<auto TValue>
        /// struct add_one_typed {
        ///     using type = Type<Value<TValue + 1>>;
        /// };
        ///
        /// // `two_typed` is `Value<2, usize>`
        /// constexpr auto two_typed = (1_value).apply<add_one_typed>().inner();
        /// static_assert(two_typed == 2_value);
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply to this `Value`
        /// @return The result of applying `TMetaFunction`
        template<template<auto> typename TMetaFunction>
            requires ValueMetaFunction<TMetaFunction>
        [[nodiscard]] constexpr auto apply() const noexcept
            -> detail::unwrap_inner_t<detail::convert_to_meta_t<TMetaFunction<value>>> {
            return {};
        }

        /// @brief Applies the given metafunction to this specialization of `Value`.
        ///
        /// Applies `func` to this specialization of `Value` and returns the calculated result as a
        /// metaprogramming type.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<Value>` type
        ///     - It must be a callable with an overload taking a single `Value` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto add_one = [](MetaValue auto value) {
        ///     return value + 1_value;
        /// };
        ///
        /// // `two` is `Value<2, usize>`
        /// constexpr auto two = (1_value).apply(add_one_typed);
        /// static_assert(two == 2_value);
        ///
        /// constexpr auto add_one_typed = [](MetaValue auto value) {
        ///     return decltype_(value + 1_value);
        /// };
        ///
        /// // `two` is `Value<2, usize>`
        /// constexpr auto two_typed = (1_value).apply(add_one_typed).inner();
        /// static_assert(two_typed == 2_value);
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Value` specialization
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Value>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept
            -> detail::unwrap_inner_t<detail::convert_to_meta_t<meta_result_t<TFunction, Value>>> {
            return {};
        }

        /// @brief Checks to see if this `Value` specialization satisfies the given metafunction
        /// predicate, `TPredicate`
        ///
        /// # Requirements
        /// - `TPredicate` must be a `ValueMetaFunction`
        ///     - It must be a template taking a single value parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `TPredicate<value>` must be a `MetaValue`
        /// - The type of the value of `TPredicate`, `TPredicate<value>::value`, must be
        /// (possibly cv-ref qualified) `bool`)
        ///
        /// # Example
        /// @code {.cpp}
        /// template<auto TValue>
        /// struct is_two {
        ///     static inline constexpr auto value = TValue == 2;
        /// };
        ///
        /// constexpr auto not_two = (1_value).satisfies<is_two>(); // `not_two` is
        ///                                                         // `Value<false, bool>`
        /// constexpr auto was_two = (2_value).satisfies<is_two>(); // `was_two` is
        ///                                                         // `Value<true, bool>`
        /// @endcode
        /// @tparam TPredicate The metafunction predicate to check with
        /// @return The result of checking this `Value` specialization against `TPredicate`, as a
        /// `Value` specialization
        template<template<auto> typename TPredicate>
            requires ValueMetaFunction<TPredicate> && MetaValue<TPredicate<value>>
                     && std::same_as<std::remove_const_t<decltype(TPredicate<value>::value)>, bool>
        [[nodiscard]] constexpr auto satisfies() const noexcept -> Value<TPredicate<value>::value> {
            return {};
        }

        /// @brief Checks to see if this `Value` specialization satisfies the given metafunction
        /// predicate, `TPredicate`
        ///
        /// # Requirements
        /// - `TPredicate` must be a `TypeMetaFunction`
        ///     - It must be a template taking a single type parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `TPredicate<Value>` must be a `MetaValue`
        /// - The type of the value of `TPredicate`, `TPredicate<Value>::value`, must be
        /// (possibly cv-ref qualified) `bool`)
        ///
        /// # Example
        /// @code {.cpp}
        /// template<MetaValue TValue>
        /// struct is_two {
        ///     static inline constexpr auto value = TValue::value == 2;
        /// };
        ///
        /// constexpr auto not_two = (1_value).satisfies<is_two>(); // `not_two` is
        ///                                                         // `Value<false, bool>`
        /// constexpr auto was_two = (2_value).satisfies<is_two>(); // `was_two` is
        ///                                                         // `Value<true, bool>`
        /// @endcode
        /// @tparam TPredicate The metafunction predicate to check with
        /// @return The result of checking this `Value` specialization against `TPredicate`, as a
        /// `Value` specialization
        template<template<typename> typename TPredicate>
            requires TypeMetaFunction<TPredicate> && MetaValue<TPredicate<Value>>
                     && std::same_as<std::remove_const_t<decltype(TPredicate<Value>::value)>, bool>
        [[nodiscard]] constexpr auto satisfies() const noexcept -> Value<TPredicate<Value>::value> {
            return {};
        }

        /// @brief Checks to see if this `Value` specialization satisfies the given metafunction
        /// predicate, `predicate`
        ///
        /// If `predicate` is not a `MetaPredicateOf<Value>` type, returns `Value<false>`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_two = [](MetaValue auto value)
        ///     -> Value<decltype(value)::value == 2>
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto not_two = (1_value).satisfies(is_two); // `not_two` is
        ///                                                       // `Value<false, bool>`
        /// constexpr auto was_two = (2_value).satisfies(is_two); // `was_two` is
        ///                                                       // `Value<true, bool>`
        /// @endcode
        /// @tparam TPredicate The metafunction predicate to check with
        /// @return The result of checking this `Value` specialization against `TPredicate`, as a
        /// `Value` specialization
        template<typename TPredicate>
        [[nodiscard]] constexpr auto satisfies(TPredicate&& predicate) const noexcept {
            if constexpr(MetaPredicateOf<TPredicate, Value>) {
                return std::forward<TPredicate>(predicate)(Value{});
            }
            else {
                return Value<false>{};
            }
        }
    };

    /// @brief Numeric literal operator to create a compile-time `Value`.
    ///
    /// `Value` represents a compile time value, storing that value in its
    /// `static constexpr` member variable, `value`.
    ///
    /// @tparam TChars The characters of the numeric literal
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<char... TChars>
    [[nodiscard]] static inline constexpr auto operator""_value() noexcept {
        constexpr auto parsed
            = hyperion::detail::parse_literal<usize>(hyperion::detail::string_literal<TChars...>{});
        hyperion::detail::check_literal_status<parsed.status>();
        return Value<parsed.value>{};
    }

    /// @brief Returns the compile-time value of the given `mpl::Value`
    ///
    /// @tparam TValue The compile-time value of `value`
    /// @tparam TType The type of the compile-time value
    /// @param value The `mpl::Value` to get the compile-time value of
    /// @return the compile-time value of the given `mpl::Value`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
    [[nodiscard]] constexpr auto
    value_of([[maybe_unused]] const Value<TValue, TType>& value) noexcept -> decltype(TValue) {
        return TValue;
    }

    /// @brief Returns the compile-time value of the given `mpl::Value`
    ///
    /// # Requirements
    /// - `TType` must be an `mpl::MetaValue`
    /// - `TType` must not be a specialization of `mpl::Value`
    ///
    /// @tparam TType The `mpl::MetaValue` storing the compile-time value
    /// @param value The `mpl::MetaValue` to get the compile-time value of
    /// @return the compile-time value of the given `mpl::Value`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<MetaValue TType>
        requires(
            !std::same_as<TType, Value<TType::value, std::remove_cvref_t<decltype(TType::value)>>>)
    [[nodiscard]] constexpr auto
    value_of([[maybe_unused]] const TType& value) noexcept -> decltype(TType::value) {
        return TType::value;
    }

    /// @brief Returns the given `mpl::MetaValue` into an `mpl::Value`
    ///
    /// # Requirements
    /// - `TType` must be an `mpl::MetaValue`
    /// - `TType` must not be a specialization of `mpl::Value`
    ///
    /// @tparam TType The `mpl::MetaValue` storing the compile-time value
    /// @param value The `mpl::MetaValue` to get the compile-time value of
    /// @return the compile-time value of the given `mpl::Value`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<MetaValue TType>
    [[nodiscard]] constexpr auto as_value([[maybe_unused]] const TType& value) noexcept
        -> Value<TType::value, std::remove_cvref_t<decltype(TType::value)>> {
        return {};
    }

    /// @brief Unary plus operator for `mpl::Value`.
    ///
    /// # Requirements
    /// - The type of `TValue` must be `UnaryPlusable` (it must support unary `operator+`).
    ///
    /// @tparam TValue The value of this `Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to +
    /// @return The `mpl::Value` representing `+TValue`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::UnaryPlusable<decltype(TValue)>
    [[nodiscard]] constexpr auto
    operator+([[maybe_unused]] const Value<TValue, TType>& value) noexcept -> Value<TValue> {
        return {};
    }

    /// @brief Unary minus operator for `mpl::Value`.
    ///
    /// # Requirements
    /// - The type of `TValue` must be `UnaryMinusable` (it must support unary `operator-`).
    ///
    /// @tparam TValue The value of this `Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to -
    /// @return The `mpl::Value` representing `-TValue`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::UnaryMinusable<decltype(TValue)>
    [[nodiscard]] constexpr auto
    operator-([[maybe_unused]] const Value<TValue, TType>& value) noexcept -> Value<-TValue> {
        return {};
    }

    /// @brief Adds the two given `mpl::Value`s and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be addable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to add
    /// @param rhs The right-hand `mpl::Value` to add
    /// @return The `mpl::Value` representing `TLhs + TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Addable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator+([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs + TRhs> {
        return {};
    }

    /// @brief Subtracts `rhs` from `lhs` and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be subtractable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to subtract from
    /// @param rhs The right-hand `mpl::Value` to subtract
    /// @return The `mpl::Value` representing `TLhs - TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Subtractable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator-([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs - TRhs> {
        return {};
    }

    /// @brief Multiplies the two given `mpl::Value`s and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be multipliable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to add
    /// @param rhs The right-hand `mpl::Value` to add
    /// @return The `mpl::Value` representing `TLhs * TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Multipliable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator*([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs * TRhs> {
        return {};
    }

    /// @brief Divides `lhs` by `rhs` and returns the result as another `mpl::Value`
    /// specialization.
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be dividible
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to divide
    /// @param rhs The right-hand `mpl::Value` to divide by
    /// @return The `mpl::Value` representing `TLhs / TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::Dividible<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator/([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs / TRhs> {
        return {};
    }

    /// @brief Performs the boolean and of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be boolean andable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to and
    /// @param rhs The right-hand `mpl::Value` to and
    /// @return The `mpl::Value` representing `TLhs && TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BooleanAndable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator&&([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs && TRhs> {
        return {};
    }

    /// @brief Performs the boolean or of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be boolean orable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to or
    /// @param rhs The right-hand `mpl::Value` to or
    /// @return The `mpl::Value` representing `TLhs || TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BooleanOrable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator||([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs || TRhs> {
        return {};
    }

    /// @brief Performs the boolean not of `value`
    ///
    /// # Requirements
    /// - `TValue` boolean notable
    ///
    /// @tparam TValue The value of the `mpl::Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to not
    /// @return The `mpl::Value` representing `!TLhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::BooleanNotable<TType>
    [[nodiscard]] constexpr auto
    operator!([[maybe_unused]] const Value<TValue, TType>& value) noexcept
        -> Value<!TValue, TType> {
        return {};
    }

    /// @brief Performs the binary and of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be binary andable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to and
    /// @param rhs The right-hand `mpl::Value` to and
    /// @return The `mpl::Value` representing `TLhs & TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BinaryAndable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator&([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs & TRhs> {
        return {};
    }

    /// @brief Performs the binary or of `lhs` and `rhs`
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be binary orable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to or
    /// @param rhs The right-hand `mpl::Value` to or
    /// @return The `mpl::Value` representing `TLhs | TRhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::BinaryOrable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator|([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs | TRhs> {
        return {};
    }

    /// @brief Performs the binary not of `value`
    ///
    /// # Requirements
    /// - `TValue` binary notable
    ///
    /// @tparam TValue The value of the `mpl::Value`
    /// @tparam TType The type of `TValue`
    /// @param value The `mpl::Value` to not
    /// @return The `mpl::Value` representing `!TLhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TValue, typename TType = decltype(TValue)>
        requires concepts::BinaryNotable<TType>
    [[nodiscard]] constexpr auto
    operator~([[maybe_unused]] const Value<TValue, TType>& value) noexcept
        -> Value<~TValue, TType> {
        return {};
    }

    /// @brief Equality comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be equality comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return Whether the value of `lhs` is equal to the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::EqualityComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs == TRhs> {
        return {};
    }

    /// @brief Inequality comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be inequality comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return Whether the value of `lhs` is _not_ equal to the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::InequalityComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator!=([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<TLhs != TRhs> {
        return {};
    }

    #if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    /// @brief Three-way comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be three-way comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return The result of three-way comparing the value of `lhs` with the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::ThreeWayComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator<=>([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
                [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept
        -> Value<TLhs <=> TRhs> {
        return {};
    }

    #endif // HYPERION?HYPERION_PLATFORM_STD_LIB_HAS_COMPARE

    /// @brief Less-than comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be less-than comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return Whether the value of `lhs` is less than the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::LessThanComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator<([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<(TLhs < TRhs)> {
        return {};
    }

    /// @brief Less-than-or-equal comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be less-than-or-equal comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return Whether the value of `lhs` is less than or equal to the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::LessThanOrEqualComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator<=([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept
        -> Value<(TLhs <= TRhs)> {
        return {};
    }

    /// @brief Greater-than comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be greater-than comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return Whether the value of `lhs` is greater than the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::GreaterThanComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator>([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
              [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept -> Value<(TLhs > TRhs)> {
        return {};
    }

    /// @brief Greater-than-or-equal comparison operator between two `mpl::Value`s
    ///
    /// # Requirements
    /// - `TLhs` and `TRhs` must be greater-than-or-equal comparable
    ///
    /// @tparam TLhs The value of the left-hand `mpl::Value`
    /// @tparam TRhs The value of the right-hand` mpl::Value`
    /// @tparam TTypeLhs The type of `TLhs`
    /// @tparam TTypeRhs The type of `RLhs`
    /// @param lhs The left-hand `mpl::Value` to compare
    /// @param rhs The right-hand `mpl::Value` to compare with
    /// @return Whether the value of `lhs` is greater than or equal to the value of `rhs`
    /// @ingroup value
    /// @headerfile hyperion/mpl/value.h
    template<auto TLhs,
             auto TRhs,
             typename TTypeLhs = decltype(TLhs),
             typename TTypeRhs = decltype(TRhs)>
        requires concepts::GreaterThanOrEqualComparable<TTypeLhs, TTypeRhs>
    [[nodiscard]] constexpr auto
    operator>=([[maybe_unused]] const Value<TLhs, TTypeLhs>& lhs,
               [[maybe_unused]] const Value<TRhs, TTypeRhs>& rhs) noexcept
        -> Value<(TLhs >= TRhs)> {
        return {};
    }

    namespace _test::value {

        static_assert(value_of(Value<3>{}) == 3, "hyperion::mpl::value_of test case 1 (failing)");
        static_assert(value_of(std::integral_constant<int, 3>{}) == 3,
                      "hyperion::mpl::value_of test case 2 (failing)");
        static_assert(Value<3>{} == 3,
                      "hyperion::mpl::Value implicit conversion test case (failing)");

        static_assert(std::same_as<decltype(as_value(std::integral_constant<int, 3>{})), Value<3>>,
                      "hyperion::mpl::as_value test case 1 (failing)");

        static_assert(Value<3>{} == 3_value,
                      "hyperion::mpl::operator"
                      "_value test case 1 (failing)");

        static_assert(+Value<3>{} == 3, "hyperion::mpl::operator+(Value) test case 1 (failing)");
        static_assert(-Value<3>{} == -3, "hyperion::mpl::operator-(Value) test case 1 (failing)");

        constexpr auto test_value1 = Value<4>{};
        constexpr auto test_value2 = Value<2>{};

        constexpr auto test_value_result = 42;

        static_assert(Value<1>{} + Value<2>{} == 3,
                      "hyperion::mpl::operator+(Value, Value) test case 1 (failing)");
        static_assert(Value<1>{} + Value<-2>{} == -1,
                      "hyperion::mpl::operator+(Value, Value) test case 2 (failing)");
        static_assert((test_value1 * 10_value) + test_value2 == test_value_result,
                      "hyperion::mpl::operator+(Value, Value) test case 2 (failing)");

        static_assert(Value<1>{} - Value<2>{} == -1,
                      "hyperion::mpl::operator-(Value, Value) test case 1 (failing)");
        static_assert(Value<1>{} - Value<-2>{} == 3,
                      "hyperion::mpl::operator-(Value, Value) test case 2 (failing)");

        static_assert(Value<2>{} * Value<2>{} == 4,
                      "hyperion::mpl::operator*(Value, Value) test case 1 (failing)");
        static_assert(Value<2>{} * Value<-2>{} == -4,
                      "hyperion::mpl::operator*(Value, Value) test case 2 (failing)");

        static_assert(Value<2>{} / Value<2>{} == 1,
                      "hyperion::mpl::operator*(Value, Value) test case 1 (failing)");
        static_assert(Value<2>{} / Value<-2>{} == -1,
                      "hyperion::mpl::operator*(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} && Value<false>{}) == false,
                      "hyperion::mpl::operator&&(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} && Value<true>{}) == true,
                      "hyperion::mpl::operator&&(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} || Value<false>{}) == true,
                      "hyperion::mpl::operator||(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<false>{} || Value<false>{}) == false,
                      "hyperion::mpl::operator||(Value, Value) test case 2 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert((Value<true>{} || Value<true>{}) == true,
                      "hyperion::mpl::operator||(Value, Value) test case 3 (failing)");

        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert(!Value<true>{} == false,
                      "hyperion::mpl::operator!(Value) test case 1 (failing)");
        // NOLINTNEXTLINE(readability-simplify-boolean-expr)
        static_assert(!Value<false>{} == true,
                      "hyperion::mpl::operator!(Value) test case 2 (failing)");

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b1100_usize>{} & Value<0b0011_usize>{}) == 0,
                      "hyperion::mpl::operator&(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b0110_usize>{} & Value<0b1100_usize>{}) == 0b0100_usize,
                      "hyperion::mpl::operator&(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b1100_usize>{} | Value<0b0011_usize>{}) == 0b1111_usize,
                      "hyperion::mpl::operator&(Value, Value) test case 1 (failing)");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert((Value<0b0110_usize>{} | Value<0b1100_usize>{}) == 0b1110_usize,
                      "hyperion::mpl::operator&(Value, Value) test case 2 (failing)");

        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert(~Value<0b0011_usize>{} == (~0b0011_usize),
                      "hyperion::mpl::operator~(Value) test case 1 (failing)");
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
        static_assert(~Value<0b1100_usize>{} == (~0b1100_usize),
                      "hyperion::mpl::operator~(Value) test case 2 (failing)");

        static_assert(Value<1>{} == Value<1>{},
                      "hyperion::mpl::operator==(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} == Value<2>{}),
                      "hyperion::mpl::operator==(Value, Value) test case 2 failing");

        static_assert(Value<1>{} != Value<2>{},
                      "hyperion::mpl::operator!=(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} != Value<1>{}),
                      "hyperion::mpl::operator!=(Value, Value) test case 2 failing");

        static_assert(Value<1>{} < Value<2>{},
                      "hyperion::mpl::operator<(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} < Value<1>{}),
                      "hyperion::mpl::operator<(Value, Value) test case 2 failing");

        static_assert(Value<1>{} <= Value<1>{},
                      "hyperion::mpl::operator<=(Value, Value) test case 1 failing");
        static_assert(Value<1>{} <= Value<2>{},
                      "hyperion::mpl::operator<=(Value, Value) test case 2 failing");
        static_assert(!(Value<1>{} <= Value<0>{}),
                      "hyperion::mpl::operator<=(Value, Value) test case 3 failing");

        static_assert(Value<2>{} > Value<1>{},
                      "hyperion::mpl::operator>(Value, Value) test case 1 failing");
        static_assert(!(Value<1>{} > Value<1>{}),
                      "hyperion::mpl::operator>(Value, Value) test case 2 failing");

        static_assert(Value<1>{} >= Value<1>{},
                      "hyperion::mpl::operator>=(Value, Value) test case 1 failing");
        static_assert(Value<2>{} >= Value<1>{},
                      "hyperion::mpl::operator>=(Value, Value) test case 2 failing");
        static_assert(!(Value<0>{} >= Value<1>{}),
                      "hyperion::mpl::operator>=(Value, Value) test case 3 failing");

    #if HYPERION_PLATFORM_STD_LIB_HAS_COMPARE
        static_assert((Value<1>{} <=> Value<1>{}) == std::strong_ordering::equal,
                      "hyperion::mpl::operator<=>(Value, Value) test case 1 failing");
        static_assert(Value<2>{} <=> Value<1>{} == std::strong_ordering::greater,
                      "hyperion::mpl::operator<=>(Value, Value) test case 2 failing");
        static_assert(Value<0>{} <=> Value<1>{} == std::strong_ordering::less,
                      "hyperion::mpl::operator<=>(Value, Value) test case 3 failing");
    #endif

        template<MetaValue TValue>
        struct add_one_meta {
            static inline constexpr auto value = TValue::value + 1;
        };

        template<MetaValue TValue>
        struct times_two_meta {
            static inline constexpr auto value = TValue::value * 2;
        };

        template<MetaValue TValue>
        struct add_one_meta_typed {
            using type = Value<TValue::value + 1>;
        };

        template<MetaValue TValue>
        struct times_two_meta_typed {
            using type = Value<TValue::value * 2>;
        };

        template<auto TValue>
        struct add_one_value {
            static inline constexpr auto value = TValue + 1;
        };

        template<auto TValue>
        struct times_two_value {
            static inline constexpr auto value = TValue * 2;
        };

        template<auto TValue>
        struct add_one_value_typed {
            using type = Value<TValue + 1>;
        };

        template<auto TValue>
        struct times_two_value_typed {
            using type = Value<TValue * 2>;
        };

        static_assert((1_value).apply<add_one_meta>() == 2,
                      "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                      "(MetaValue<MetaFunction<MetaValue>>) test case 1 (failing)");
        static_assert((2_value).apply<times_two_meta>() == 4_value,
                      "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                      "(MetaValue<MetaFunction<MetaValue>>) test case 2 (failing)");
        static_assert(
            (2_value).apply<times_two_meta>().apply<add_one_meta>().apply<times_two_meta>()
                == 10_value,
            "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
            "(MetaValue<MetaFunction<MetaValue>>) test case 3 (failing)");

        static_assert((1_value).apply<add_one_meta_typed>() == 2,
                      "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                      "(MetaType<MetaFunction<MetaValue>>) test case 1 (failing)");
        static_assert((2_value).apply<times_two_meta_typed>() == 4_value,
                      "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                      "(MetaType<MetaFunction<MetaValue>>) test case 2 (failing)");
        static_assert((2_value)
                              .apply<times_two_meta_typed>()
                              .apply<add_one_meta_typed>()
                              .apply<times_two_meta_typed>()
                          == 10_value,
                      "hyperion::mpl::Value::apply<MetaFunction<MetaValue>> "
                      "(MetaType<MetaFunction<MetaValue>>) test case 3 (failing)");

        static_assert((1_value).apply<add_one_value>() == 2,
                      "hyperion::mpl::Value::apply<MetaFunction<value>> "
                      "(MetaValue<MetaFunction<value>>) test case 1 (failing)");
        static_assert((2_value).apply<times_two_value>() == 4_value,
                      "hyperion::mpl::Value::apply<MetaFunction<value>> "
                      "(MetaValue<MetaFunction<value>>) test case 2 (failing)");
        static_assert(
            (2_value).apply<times_two_value>().apply<add_one_value>().apply<times_two_value>()
                == 10_value,
            "hyperion::mpl::Value::apply<MetaFunction<value>> "
            "(MetaValue<MetaFunction<value>>) test case 3 (failing)");

        static_assert((1_value).apply<add_one_value_typed>() == 2,
                      "hyperion::mpl::Value::apply<MetaFunction<value>> "
                      "(MetaType<MetaFunction<value>>) test case 1 (failing)");
        static_assert((2_value).apply<times_two_value_typed>() == 4_value,
                      "hyperion::mpl::Value::apply<MetaFunction<value>> "
                      "(MetaType<MetaFunction<value>>) test case 2 (failing)");
        static_assert((2_value)
                              .apply<times_two_value_typed>()
                              .apply<add_one_value_typed>()
                              .apply<times_two_value_typed>()
                          == 10_value,
                      "hyperion::mpl::Value::apply<MetaFunction<value>> "
                      "(MetaType<MetaFunction<value>>) test case 3 (failing)");

        constexpr auto add1 = [](MetaValue auto value) {
            return value + 1_value;
        };

        constexpr auto times2 = [](MetaValue auto value) {
            return value * 2_value;
        };

        static_assert((1_value).apply(add1) == 2,
                      "hyperion::mpl::Value::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                      "1 (failing)");
        static_assert((2_value).apply(times2) == 4,
                      "hyperion::mpl::Value::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                      "2 (failing)");
        static_assert((2_value).apply(times2).apply(add1).apply(times2) == 10_value,
                      "hyperion::mpl::Value::apply(MetaFunction(MetaValue)) -> MetaValue test case "
                      "3 (failing)");

        template<auto TValue>
        struct is_true_value : public std::bool_constant<bool(TValue)> { };

        static_assert((1_value).satisfies<is_true_value>(),
                      "hyperion::mpl::Value::satisfies<ValueMetaFunction> -> MetaValue test case 1 "
                      "(failing)");
        static_assert(!(0_value).satisfies<is_true_value>(),
                      "hyperion::mpl::Value::satisfies<ValueMetaFunction> -> MetaValue test case 2 "
                      "(failing)");

        template<MetaValue TValue>
        struct is_true_meta : public std::bool_constant<bool(TValue::value)> { };

        static_assert((1_value).satisfies<is_true_meta>(),
                      "hyperion::mpl::Value::satisfies<TypeMetaFunction> -> MetaValue test case 1 "
                      "(failing)");
        static_assert(!(0_value).satisfies<is_true_meta>(),
                      "hyperion::mpl::Value::satisfies<TypeMetaFunction> -> MetaValue test case 2 "
                      "(failing)");

        constexpr auto is_true
            = [](MetaValue auto value) noexcept -> Value<decltype(value)::value, bool> {
            return {};
        };

        static_assert((1_value).satisfies(is_true),
                      "hyperion::mpl::Value::satisfies(MetaPredicate) -> MetaValue test case 1 "
                      "(failing)");
        static_assert(not(0_value).satisfies(is_true),
                      "hyperion::mpl::Value::satisfies(MetaPredicate) -> MetaValue test case 2 "
                      "(failing)");

        constexpr auto is_int = [](MetaType auto type) noexcept
            -> Value<std::is_same_v<int, typename decltype(type)::type>, bool> {
            return {};
        };

        static_assert(
            not(1_value).satisfies(is_int),
            "hyperion::mpl::Value::satisfies(MetaPredicate) -> MetaValue test case 3 (failing)");

    } // namespace _test::value
} // namespace hyperion::mpl

#endif // HYPERION_MPL_VALUE_H
