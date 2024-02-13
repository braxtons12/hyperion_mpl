/// @file type.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Metaprogramming type wrapper for use as metafunction parameter and return type
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

#ifndef HYPERION_MPL_TYPE_H
#define HYPERION_MPL_TYPE_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

//

#include <concepts>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup type Metaprogramming Type Manipulation Type
/// Hyperion provides `mpl::Type` as a metaprogramming type for storing, communicating,
/// working with, and operating on types.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/type.h>
///
/// using namespace hyperion::mpl;
///
/// // alternatively, use `decltype_<int>()`
/// constexpr auto type1 = Type<int>{};
/// constexpr auto type2 = Type<double>{};
///
/// static_assert(not type1.is(type2));
/// @endcode
/// @headerfile hyperion/mpl/type.h
/// @}

namespace hyperion::mpl {
    template<auto TValue, typename TType>
    struct Value;

    /// @brief `Type` is a metaprogramming type wrapper used for storing, communicating,
    /// operating on, and otherwis working with types.
    ///
    /// # Example
    /// @code {.cpp}
    /// #include <hyperion/mpl/type.h>
    ///
    /// using namespace hyperion::mpl;
    ///
    /// // alternatively, use `decltype_<int>()`
    /// constexpr auto type1 = Type<int>{};
    /// constexpr auto type2 = Type<double>{};
    ///
    /// static_assert(not type1.is(type2));
    /// @endcode
    ///
    /// @tparam TType The type to metaprogram against
    /// @ingroup type
    /// @headerfile hyperion/mpl/type.h
    template<typename TType>
    struct Type {
        /// @brief The type that this `Type` is a metaprogramming wrapper for
        using type = TType;

        /// @brief Returns another instance of this `specialization` of `Type`
        /// @return another instance of this `Type` specialization
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto self() const noexcept -> Type<TDelay> {
            return {};
        }

        /// @brief Returns the inner ``MetaValue` `type` of `this` `Type`,
        /// if `this` `Type` represents a `MetaValue`
        ///
        /// # Requirements
        /// - `type` is trivially default constructible
        /// - `type` is a `MetaValue`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto one_typed = delctype_(1_value);
        ///
        /// static_assert(one_typed.inner() == 1_value);
        /// @endcode
        ///
        /// @return The inner `MetaValue` of `this` `Type`
        template<typename TDelay = type>
            requires std::same_as<TDelay, type> && std::is_trivially_default_constructible_v<TDelay>
                     && MetaValue<TDelay>
        [[nodiscard]] constexpr auto inner() const noexcept -> TDelay {
            return {};
        }

        /// @brief Returns whether the `type` of this `Type` is also a metaprogramming type
        /// (i.e. a `MetaValue`), in which case you may call `this->inner()` to obtain that an
        /// instance of that type.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto has_inner = decltype_(1_value);
        /// constexpr auto no_inner = decltype_<int>();
        ///
        /// static_assert(has_inner.has_inner());
        /// static_assert(not no_inner.has_inner());
        /// @endcode
        ///
        /// @return Whether the `type` of this `Type` is also a metaprogramming type
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto has_inner() const noexcept -> bool {
            return MetaValue<TDelay>;
        }

        /// @brief Applies the specified template metafunction to this specialization of `Type`.
        ///
        /// Applies `TMetaFunction` to this specialization of `Type` and returns the calculated
        /// value as a `Value` specialization.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `TypeMetaFunction`:
        ///     - It must be a template taking a single type parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `TMetaFunction<type>` must be a `MetaValue`
        ///
        /// @code {.cpp}
        /// template<typename TType>
        /// struct is_const {
        ///     using type = Value<std::is_const<std::remove_reference_t<TType>>, bool>;
        /// };
        ///
        /// // Type<Value<true, bool>>
        /// constexpr auto is_const = decltype_<const int>().apply<is_const>();
        ///
        /// static_assert(is_const.inner());
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply to this `Type`
        /// @return The result of applying `TMetaFunction`, as a `Value` specialization
        template<template<typename> typename TMetaFunction>
        [[nodiscard]] constexpr auto apply() const noexcept -> std::enable_if_t<
            TypeMetaFunction<TMetaFunction> && MetaValue<TMetaFunction<type>>,
            Value<TMetaFunction<type>::value, decltype(TMetaFunction<type>::value)>>;

        /// @brief Applies the specified template metafunction to this specialization of `Type`.
        ///
        /// Applies `TMetaFunction` to this specialization of `Type` and returns the calculated
        /// result as a (potentially different) `Type` specialization.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `TypeMetaFunction`:
        ///     - It must be a template taking a single type parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `TMetaFunction<type>` must be a `MetaType`
        /// - `typename TMetaFunction<type>::type` must not be a `MetaValue` or a `MetaType`
        ///
        /// @code {.cpp}
        /// constexpr auto const_int = decltype_<int>().apply<std::add_const>(); // Type<const int>
        ///
        /// static_assert(const_int == decltype_<const int>());
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply to this `Type`
        /// @return The result of applying `TMetaFunction`, as a `Type` specialization
        template<template<typename> typename TMetaFunction>
            requires TypeMetaFunction<TMetaFunction> && MetaType<TMetaFunction<type>>
                     && (!MetaValue<typename TMetaFunction<type>::type>)
                     && (!MetaType<typename TMetaFunction<type>::type>)
        [[nodiscard]] constexpr auto apply()
            // NOLINTNEXTLINE(modernize-type-traits)
            const noexcept -> Type<typename TMetaFunction<type>::type> {
            return {};
        }

        /// @brief Applies the specified template metafunction to this specialization of `Type`.
        ///
        /// Applies `TMetaFunction` to this specialization of `Type` and returns the calculated
        /// result as a `Value` or (potentially different) `Type` specialization.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `TypeMetaFunction`:
        ///     - It must be a template taking a single type parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `TMetaFunction<type>` must be a `MetaType`
        /// - `typename TMetaFunction<type>::type` must be a `MetaValue` or a `MetaType`
        ///
        /// @code {.cpp}
        /// template<typename TType>
        /// struct add_const {
        ///     using type = decltype(decltype_<TType>().add_const());
        /// };
        ///
        /// constexpr auto const_int = decltype_<int>().apply<std::add_const>(); // Type<const int>
        ///
        /// static_assert(const_int == decltype_<const int>());
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply to this `Type`
        /// @return The result of applying `TMetaFunction`, as a `Value` or `Type` specialization
        template<template<typename> typename TMetaFunction>
            requires TypeMetaFunction<TMetaFunction> && MetaType<TMetaFunction<type>>
                     && (MetaValue<typename TMetaFunction<type>::type>
                         || MetaType<typename TMetaFunction<type>::type>)
        [[nodiscard]] constexpr auto apply()
            // NOLINTNEXTLINE(modernize-type-traits)
            const noexcept {
            if constexpr(MetaValue<typename TMetaFunction<type>::type>) {
                return Value<TMetaFunction<type>::type::value,
                             std::remove_cvref_t<decltype(TMetaFunction<type>::type::value)>>{};
            }
            else {
                return Type<typename TMetaFunction<type>::type::type>{};
            }
        }

        /// @brief Applies the given metafunction to this specialization of `Type`.
        ///
        /// Applies `func` to this specialization of `Type` and returns the calculated result as a
        /// `Type` specialization.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<Type>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `func` with a `Type` must be a `MetaType`
        /// - The type member alias `type` of the invoke result of `func` must not be a `MetaType`
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto add_const = [](MetaType auto value)
        ///     -> decltype(decltype_(value).add_const())
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto const_int = decltype_<int>().apply(add_const); // Type<const int>
        ///
        /// static_assert(const_int == decltype_<const int>());
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Type` specialization, as a `Type`
        /// specialization
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Type<type>>
                     && MetaType<meta_result_t<TFunction, Type<type>>>
                     && (!MetaType<typename meta_result_t<TFunction, Type<type>>::type>)
        [[nodiscard]] constexpr auto apply(
            [[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
                                               // NOLINTNEXTLINE(modernize-type-traits)
            const noexcept -> Type<typename meta_result_t<TFunction, Type<type>>::type> {
            return {};
        }

        /// @brief Applies the given metafunction to this specialization of `Type`.
        ///
        /// Applies `func` to this specialization of `Type` and returns the calculated result as a
        /// `Value` specialization.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<Type>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `func` with a `Type` must be a `MetaValue`
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto get_size = [](MetaType auto value)
        ///     -> decltype(decltype_(value).sizeof_())
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto sizeof_int = decltype_<int>().apply(get_size); // Value<4, usize>
        ///
        /// static_assert(sizeof_int == 4_usize);
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Type` specialization, as a `Value`
        /// specialization
        template<typename TFunction>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> std::enable_if_t<
                MetaFunctionOf<TFunction, Type<type>>
                    && MetaValue<meta_result_t<TFunction, Type<type>>>,
                Value<meta_result_t<TFunction, Type<type>>::value,
                      std::remove_cvref_t<decltype(meta_result_t<TFunction, Type<type>>::value)>>>;

        /// @brief Applies the given metafunction to the `type` of this specialization of `Type`.
        ///
        /// Given that `type` is a `MetaValue`, applies `func` to the `MetaValue` `type` of this
        /// specialization of `Type`, as if by `type{}.apply(std::forward<TFunction>(func))`.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<type>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `func` with a `Type` must be a `MetaType`
        /// - `typename meta_result_t<TFunction, type>::type` must _not_ be a `MetaType`
        /// - `type` must be a `MetaValue`
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto is_two = [](MetaValue auto value)
        ///     -> Type<Value<decltype(value)::value == 2, bool>>
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto is_two = decltype_(2_value).apply(is_two); // Value<true, bool>
        ///
        /// static_assert(is_two);
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Type` specialization
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, type> && MetaType<meta_result_t<TFunction, type>>
                     && (!MetaType<typename meta_result_t<TFunction, type>::type>)
                     && MetaValue<type>
        [[nodiscard]] constexpr auto apply(
            [[maybe_unused]] TFunction&& func) const noexcept { // NOLINT(*-missing-std-forward)
            return type{}.apply(TFunction{});
        }

        /// @brief Applies the given metafunction to the `type` of this specialization of `Type`.
        ///
        /// Given that `type` is a `MetaValue`, applies `func` to the `MetaValue` `type` of this
        /// specialization of `Type`, as if by `type{}.apply(std::forward<TFunction>(func))`.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<type>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `func` with a `Type` must be a `MetaValue`
        /// - `type` must be a `MetaValue`
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto is_two = [](MetaValue auto value)
        ///     -> Value<decltype(value)::value == 2, bool>
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto is_two = decltype_(2_value).apply(is_two); // Value<true, bool>
        ///
        /// static_assert(is_two);
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Type` specialization
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, type> && MetaValue<meta_result_t<TFunction, type>>
                     && MetaValue<type>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) const noexcept { // NOLINT(*-missing-std-forward)
            return type{}.apply(TFunction{});
        }

        /// @brief Checks to see if this `Type` specialization satisfies the given template
        /// metafunction predicate, `TPredicate`
        ///
        /// # Requirements
        /// - `TPredicate` must be a `TypeMetaFunction`
        ///     - It must be a template taking a single value parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `TPredicate<type>` must be a `MetaValue`
        /// - The type of the value of `TPredicate`, `TPredicate<type>::value`, must be
        /// (possibly cv-ref qualified) `bool`)
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename TType>
        /// struct is_const {
        ///     static inline constexpr auto value
        ///         = std::is_const_v<std::remove_reference_t<TType>> == 2;
        /// };
        ///
        /// // `not_const` is `Value<false, bool>`
        /// constexpr auto not_const = decltype_<int>().satisfies<is_const>();
        /// // `was_const` is `Value<true, bool>`
        /// constexpr auto was_const = decltype_<const int>().satisfies<is_const>();
        ///
        /// static_assert(was_const);
        /// static_assert(not not_const);
        /// @endcode
        ///
        /// @tparam TPredicate The metafunction predicate to check with
        /// @return The result of checking this `Type` specialization against `TPredicate`, as a
        /// `Value` specialization
        template<template<typename> typename TPredicate>
        [[nodiscard]] constexpr auto satisfies() const noexcept -> std::enable_if_t<
            TypeMetaFunction<TPredicate> && MetaValue<TPredicate<type>>
                && std::same_as<std::remove_const_t<decltype(TPredicate<type>::value)>, bool>,
            Value<TPredicate<type>::value, bool>>;

        /// @brief Checks to see if this `Type` specialization satisfies the given metafunction
        /// predicate.
        ///
        /// # Requirements
        /// - `TPredicate` must be a `MetaFunctionOf<Type<type>>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TPredicate` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `predicate` with `Type` must be a `MetaValue`
        /// - The type of the value of the invoke result of `predicate` must be
        /// (possibly cv-ref qualified) `bool`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_const = [](MetaType auto type)
        ///     -> decltype(decltype(type){}.is_const())
        /// {
        ///     return {};
        /// };
        ///
        /// // `not_two` is `Value<false, bool>`
        /// constexpr auto not_const = decltype_<int>().satisfies(is_const);
        /// // `was_two` is `Value<true, bool>`
        /// constexpr auto was_const = decltype_<const int>().satisfies(is_const);
        ///
        /// static_assert(was_const);
        /// static_assert(not not_const);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metafunction predicate to check with
        /// @param predicate The metafunction predicate to check with
        /// `Value` specialization
        template<typename TPredicate>
            requires MetaFunctionOf<TPredicate, Type<type>>
                     && MetaValue<meta_result_t<TPredicate, Type<type>>>
                     && std::same_as<std::remove_cvref_t<
                                         decltype(meta_result_t<TPredicate, Type<type>>::value)>,
                                     bool>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept -> meta_result_t<TPredicate, Type<type>> {
            return {};
        }

        /// @brief Checks to see if this `Type` specialization satisfies the given metafunction
        /// predicate.
        ///
        /// # Requirements
        /// - `TPredicate` must be a `MetaFunctionOf<Type<type>>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TPredicate` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `predicate` with `Type` must be a `MetaType`
        /// - The `type` of the invoke result of `predicate`, ie,
        /// `typename meta_result_t<TPredicate, Type<type>>::type` must be a `MetaValue`
        /// - The type of the value of the `MetaValue` `type` of the invoke result of `predicate`
        /// must be (possibly cv-ref qualified) `bool`. That is,
        /// `decltype(typename meta_result_t<TPredicate, Type<type>>::type::value)` must be
        /// (possibly cv-ref qualified) `bool`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_const = [](MetaType auto type)
        ///     -> Type<decltype(decltype(type){}.is_const())>
        /// {
        ///     return {};
        /// };
        ///
        /// // `not_const` is `Value<false, bool>`
        /// constexpr auto not_const = decltype_<int>().satisfies(is_const);
        /// // `was_const` is `Value<true, bool>`
        /// constexpr auto was_const = decltype_<const int>().satisfies(is_const);
        ///
        /// static_assert(was_const);
        /// static_assert(not not_const);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metafunction predicate to check with
        /// @param predicate The metafunction predicate to check with
        /// @return The result of checking this `Type` specialization against `TPredicate`, as a
        /// `Value` specialization
        template<typename TPredicate>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
            const noexcept -> std::enable_if_t<
                MetaFunctionOf<TPredicate, Type<type>>
                    && MetaType<meta_result_t<TPredicate, Type<type>>>
                    && MetaValue<typename meta_result_t<TPredicate, Type<type>>::type>
                    && std::same_as<
                        std::remove_cvref_t<
                            decltype(meta_result_t<TPredicate, Type<type>>::type::value)>,
                        bool>,
                Value<meta_result_t<TPredicate, Type<type>>::type::value, bool>>;

        /// @brief Checks to see if this `Type` specialization satisfies the given metafunction
        /// predicate.
        ///
        /// Given that `type` is a `MetaValue`, checks that `predicate` is satisfied by the
        /// `MetaValue` `type` of this specialization of `Type`, as if by
        /// `type{}.satisfies(std::forward<TPredicate>(predicate))`.
        ///
        /// # Requirements
        /// - `type` must be a `MetaValue`
        /// - `TPredicate` must be a `MetaFunctionOf<type>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `predicate` with a `type` must be a `MetaValue`
        /// - The type of the value of the invoke result of `predicate` must be
        /// (possibly cv-ref qualified) `bool`.
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto is_two = [](MetaValue auto value)
        ///     -> Value<decltype(value)::value == 2, bool>
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto is_two = decltype_(2_value).satisfies(is_two); // Value<true, bool>
        /// constexpr auto not_two = decltype_(1_value).satisfies(is_two); // Value<false, bool>
        ///
        /// static_assert(is_two);
        /// static_assert(not not_two);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metafunction predicate to check with
        /// @param predicate The metafunction predicate to check with
        /// @return The result of checking this `Type` specialization against `predicate`, as a
        /// `Value` specialization
        template<typename TPredicate>
            requires MetaFunctionOf<TPredicate, type> && MetaValue<meta_result_t<TPredicate, type>>
                     && std::same_as<
                         std::remove_cvref_t<decltype(meta_result_t<TPredicate, type>::value)>,
                         bool>
                     && MetaValue<type>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-forward)
            const noexcept {
            return type{}.satisfies(TPredicate{});
        }

        /// @brief Checks to see if this `Type` specialization satisfies the given metafunction
        /// predicate.
        ///
        /// # Requirements
        /// - `type` must be a `MetaValue`
        /// - `TPredicate` must be a `MetaFunctionOf<type>` type
        ///     - It must be a callable with an overload taking a single `type` parameter
        ///     - The selected overload of `TPredicate` must return either a `MetaType` or a
        ///     `MetaValue`
        /// - The result of invoking `predicate` with `type` must be a `MetaType`
        /// - The `type` of the invoke result of `predicate`, ie,
        /// `typename meta_result_t<TPredicate, type>::type` must be a `MetaValue`
        /// - The type of the value of the `MetaValue` `type` of the invoke result of `predicate`
        /// must be (possibly cv-ref qualified) `bool`. That is,
        /// `decltype(typename meta_result_t<TPredicate, type>::type::value)` must be
        /// (possibly cv-ref qualified) `bool`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_two = [](MetaValue auto value)
        ///     -> Type<Value<decltype(value)::value == 2, bool>>
        /// {
        ///     return {};
        /// };
        ///
        /// constexpr auto is_two = decltype_(2_value).satisfies(is_two); // Value<true, bool>
        /// constexpr auto not_two = decltype_(1_value).satisfies(is_two); // Value<false, bool>
        ///
        /// static_assert(is_two);
        /// static_assert(not not_two);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metafunction predicate to check with
        /// @param predicate The metafunction predicate to check with
        /// @return The result of checking this `Type` specialization against `TPredicate`, as a
        /// `Value` specialization
        template<typename TPredicate>
        [[nodiscard]] constexpr auto
        satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-forward)
            const noexcept -> std::enable_if_t<
                MetaFunctionOf<TPredicate, type> && MetaType<meta_result_t<TPredicate, type>>
                    && MetaValue<typename meta_result_t<TPredicate, type>::type>
                    && std::same_as<
                        std::remove_cvref_t<decltype(meta_result_t<TPredicate, type>::type::value)>,
                        bool>
                    && MetaValue<type>,
                Value<meta_result_t<TPredicate, type>::type::value, bool>>;

        /// @brief Returns whether `this` `Type` specialization is the same as the given one.
        ///
        /// Checks if `this` is an instance of the same `Type` specialization as `rhs`, that is,
        /// whether `std::same_as<type, TRhs>`, and returns the result as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        ///
        /// constexpr auto is_int = int_t.is(decltype_<int>());
        /// constexpr auto is_const_int = int_t.is(decltype_<const int>());
        ///
        /// static_assert(is_int);
        /// static_assert(not is_const_int);
        /// @endcode
        ///
        /// @tparam TRhs The `type` of the `Type` specialization of `rhs`
        /// @param rhs The `Type` specialization to compare to
        /// @return Whether `this` and `rhs` are the same `Type` specialization
        template<typename TRhs>
        [[nodiscard]] constexpr auto is([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
            -> Value<std::same_as<type, TRhs>, bool>;

        /// @brief Returns whether the `type` of `this` `Type` specialization is the same as,
        /// or a cv-ref qualification of, the `type` of the given one.
        ///
        /// Checks if the `type` of `this` `Type` specialization is the (possibly cv-ref
        /// qualification of the) `type` of the `rhs` specialization, that is, whether
        /// `std::same_as<std::remove_cvref_t<type>, TRhs>`, and returns the result as a `Value`
        /// specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<const int&>();
        ///
        /// constexpr auto is_int = int_t.is_qualification_of(decltype_<int>());
        /// constexpr auto is_const_int = int_t.is_qualification_of(decltype_<const int>());
        /// constexpr auto is_constref_int = int_t.is_qualification_of(decltype_<const int&>());
        /// constexpr auto is_double = int_t.is_qualification_of(decltype_<double>());
        ///
        /// static_assert(is_int);
        /// static_assert(is_const_int);
        /// static_assert(is_constref_int);
        /// static_assert(not is_double);
        /// @endcode
        ///
        /// @tparam TRhs The `type` of the `Type` specialization of `rhs`
        /// @param rhs The `Type` specialization to compare to
        /// @return Whether the `type` of `this` is the (possibly cv-ref qualification of the)
        /// `type` of `rhs`
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_qualification_of([[maybe_unused]] const Type<TRhs>& rhs) const noexcept {
            return Type<TRhs>{}.is(Type<std::remove_cvref_t<type>>{});
        }

        /// @brief Returns whether the `type` of `this` `Type` specialization is `const`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto const_int_t = decltype_<const int>();
        ///
        /// static_assert(not int_t.is_const());
        /// static_assert(const_int_t.is_const());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is `const`
        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_const() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_const_v<std::remove_reference_t<TDelay>>, bool>>;

        /// @brief Returns whether the `type` of `this` `Type` specialization is an lvalue reference
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto intref_t = decltype_<int&>();
        ///
        /// static_assert(not int_t.is_lvalue_reference());
        /// static_assert(intref_t.is_lvalue_reference());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is an lvalue reference
        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_lvalue_reference() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_lvalue_reference_v<TDelay>, bool>>;

        /// @brief Returns whether the `type` of `this` `Type` specialization is an rvalue reference
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto intrefref_t = decltype_<int&&>();
        ///
        /// static_assert(not int_t.is_rvalue_reference());
        /// static_assert(intrefref_t.is_rvalue_reference());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is an rvalue reference
        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_rvalue_reference() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_rvalue_reference_v<TDelay>, bool>>;

        /// @brief Returns whether the `type` of `this` `Type` specialization is `volatile`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto volatile_int_t = decltype_<volatile int>();
        ///
        /// static_assert(not int_t.is_volatile());
        /// static_assert(volatile_int_t.is_volatile());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is `volatile`
        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_volatile() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_volatile_v<std::remove_reference_t<TDelay>>, bool>>;

        /// @brief Returns a `Type` specialization representing the `const`-qualified version of
        /// the type `this` specialization represents.
        ///
        /// The application of `const` _does not_ follow the same application as other mechanisms,
        /// like `std::add_const`, and instead follows the "intuitive" expectation that `const`
        /// would be applied through references. That is, whereas `std::add_const<int&>` would yield
        /// `int&`, `decltype_<int&>().as_const()` will yield `Type<const int&>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto const_int_t = int_t.as_const();
        ///
        /// static_assert(const_int_t.is_const());
        /// static_assert(const_int_t.is(decltype_<const int>()));
        /// @endcode
        ///
        /// @return the `Type` specialization representing `const type`
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_const() const noexcept {
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

        /// @brief Returns a `Type` specialization representing the lvalue reference-qualified
        /// version of the type `this` specialization represents.
        ///
        /// The application of lvalue reference follows the same application as other mechanisms,
        /// like `std::add_lvalue_reference`,  That is, where `std::add_lvalue_reference<int&&>`
        /// would yield `int&`, `decltype_<int&&>().as_lvalue_reference()` will also yield
        /// `Type<int&>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto intref_t = int_t.as_lvalue_reference();
        ///
        /// static_assert(intref_t.is_lvalue_reference());
        /// static_assert(intref_t.is(decltype_<int&>()));
        /// @endcode
        ///
        /// @return the `Type` specialization representing `type&`
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_lvalue_reference() const noexcept {
            using base_type = std::remove_reference_t<TDelay>;
            if constexpr(Type<TDelay>{}.is_lvalue_reference()) {
                return Type<TDelay>{};
            }
            else {
                return Type<std::add_lvalue_reference_t<base_type>>{};
            }
        }

        /// @brief Returns a `Type` specialization representing the rvalue reference-qualified
        /// version of the type `this` specialization represents.
        ///
        /// The application of rvalue reference _does not_ follow the same application as other
        /// mechanisms, like `std::add_rvalue_reference`,  That is, where
        /// `std::add_rvalue_reference<int&>` would yield `int&`,
        /// `decltype_<int&>().as_rvalue_reference()` will yield `Type<int&&>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto intrefref_t = int_t.as_rvalue_reference();
        ///
        /// static_assert(intref_t.is_rvalue_reference());
        /// static_assert(intrefref_t.is(decltype_<int&&>()));
        /// @endcode
        ///
        /// @return the `Type` specialization representing `type&&`
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_rvalue_reference() const noexcept {
            using base_type = std::remove_reference_t<TDelay>;
            if constexpr(Type<TDelay>{}.is_rvalue_reference()) {
                return Type<TDelay>{};
            }
            else {
                return Type<std::add_rvalue_reference_t<base_type>>{};
            }
        }

        /// @brief Returns a `Type` specialization representing the `volatile`-qualified version of
        /// the type `this` specialization represents.
        ///
        /// The application of `volatile` _does not_ follow the same application as other
        /// mechanisms, like `std::add_volatile`, and instead follows the "intuitive" expectation
        /// that `volatile` would be applied through references. That is, whereas
        /// `std::add_volatile<int&>` would yield `int&`,
        /// `decltype_<int&>().as_volatile()` will yield `Type<volatile int&>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto volatile_int_t = int_t.as_volatile();
        ///
        /// static_assert(volatile_int_t.is_volatile());
        /// static_assert(volatile_int_t.is(decltype_<volatile int>()));
        /// @endcode
        ///
        /// @return the `Type` specialization representing `volatile type`
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_volatile() const noexcept {
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

        /// @brief Returns whether the type `this` `Type` specialization represents is convertible
        /// to the type `rhs` represents, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct not_convertible {};
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto double_t = decltype_<double>();
        /// constexpr auto not_convertible_t = decltype_<not_convertible>();
        ///
        /// static_assert(int_t.is_convertible_to(double_t));
        /// static_assert(double_t.is_convertible_to(int_t));
        /// static_assert(not int_t.is_convertible_to(not_convertible_t));
        /// static_assert(not not_convertible_t.is_convertible_to(int_t));
        /// @endcode
        ///
        /// @tparam TRhs The type that `rhs` represents
        /// @param rhs The instance of the `Type` specialization representing the type to check that
        /// `type` is convertible to
        /// @return whether the type `this` represents is convertible to the type `rhs` represents,
        /// as a `Value` specialization
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_convertible_to([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
            -> Value<std::convertible_to<type, TRhs>, bool>;

        /// @brief Returns whether the type `this` `Type` specialization represents is derived from
        /// the type `rhs` represents, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct base {};
        /// struct derived : base {};
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto base_t = decltype_<base>();
        /// constexpr auto derived_t = decltype_<base>();
        ///
        /// static_assert(derived_t.is_derived_from(base_t));
        /// static_assert(not base_t.is_derived_from(base_t));
        /// static_assert(not int_t.is_derived_from(int_t));
        /// static_assert(not int_t.is_derived_from(base_t));
        /// @endcode
        ///
        /// @note The application of this differs from `std::is_base_of` or `std::derived_from`,
        /// in that this is `false` when called against the same type. That is,
        /// `decltype_<base>().is_derived_from(decltype_<base>()) == false`.
        ///
        /// @tparam TRhs The type that `rhs` represents
        /// @param rhs The instance of the `Type` specialization representing the type to check that
        /// `type` is derived from
        /// @return whether the type `this` represents is derived from the type `rhs` represents,
        /// as a `Value` specialization
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_derived_from([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
            -> Value<std::derived_from<type, TRhs> && !std::same_as<type, TRhs>, bool>;

        /// @brief Returns whether the type `this` `Type` specialization represents is a base class
        /// of the type `rhs` represents, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct base {};
        /// struct derived : base {};
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto base_t = decltype_<base>();
        /// constexpr auto derived_t = decltype_<base>();
        ///
        /// static_assert(base_t.is_base_of(derived_t));
        /// static_assert(not base_t.is_base_of(base_t));
        /// static_assert(not int_t.is_base_of(int_t));
        /// static_assert(not int_t.is_base_of(base_t));
        /// @endcode
        ///
        /// @note The application of this differs from `std::is_base_of` or `std::derived_from`,
        /// in that this is `false` when called against the same type. That is,
        /// `decltype_<base>().is_base_of(decltype_<base>()) == false`.
        ///
        /// @tparam TRhs The type that `rhs` represents
        /// @param rhs The instance of the `Type` specialization representing the type to check that
        /// `type` is a base of
        /// @return whether the type `this` represents is a base of the type `rhs` represents,
        /// as a `Value` specialization
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_base_of([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
            -> Value<std::derived_from<TRhs, type> && !std::same_as<type, TRhs>, bool>;

        /// @brief Returns whether the type `this` `Type` specialization represents is constructible
        /// from arguments of types `TTypes...`, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename... TTypes>
        /// struct list {};
        /// struct tag {};
        /// struct constructible {
        ///     constructible(int, double, tag);
        /// }
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible.is_constructible_from(list<int, double, tag>{}));
        /// static_assert(not constructible.is_constructible_from(list<double>{}));
        /// static_assert(not int_t.is_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TList The type-list type template representing the argument types to check with
        /// @tparam TTypes The list of argument types to check with
        /// @param list The type-list representing the argument types to check with
        /// @return whether the type `this` represents is constructible from arguments of types
        /// `TTypes...`, as a `Value` specialization
        template<template<typename...> typename TList, typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const TList<TTypes...>& list) const noexcept
            -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                                Value<std::is_constructible_v<type, TTypes...>, bool>>;

        /// @brief Returns whether the type `this` `Type` specialization represents is constructible
        /// from arguments of types `TTypes...`, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename... TTypes>
        /// struct list {};
        /// struct tag {};
        /// struct constructible {
        ///     constructible(int, double, tag);
        /// }
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible.is_constructible_from(decltype_<int>(),
        ///                                                   decltype_<double>(),
        ///                                                   decltype_<tag>()));
        /// static_assert(not constructible.is_constructible_from(decltype_<double>{}));
        /// static_assert(not int_t.is_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TTypes The list of argument types to check with
        /// @param list The `Type`s representing the argument types to check with
        /// @return whether the type `this` represents is constructible from arguments of types
        /// `TTypes...`, as a `Value` specialization
        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const Type<TTypes>&... list) const noexcept
            -> Value<std::is_constructible_v<type, TTypes...>, bool>;

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// constructible from arguments of types `TTypes...`, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename... TTypes>
        /// struct list {};
        /// struct tag {};
        /// struct constructible {
        ///     constructible(int, double, tag);
        ///     constructible(int, tag, tag) noexcept;
        /// }
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible.is_noexcept_constructible_from(list<int, tag, tag>{}));
        /// static_assert(not constructible
        ///                     .is_noexcept_constructible_from(list<int, double, tag>{}));
        /// static_assert(not constructible.is_noexcept_constructible_from(list<double>{}));
        /// static_assert(not int_t.is_noexcept_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TList The type-list type template representing the argument types to check with
        /// @tparam TTypes The list of argument types to check with
        /// @param list The type-list representing the argument types to check with
        /// @return whether the type `this` represents is `noexcept` constructible from arguments of
        /// types `TTypes...`, as a `Value` specialization
        template<template<typename...> typename TList, typename... TTypes>
        [[nodiscard]] constexpr auto
        is_noexcept_constructible_from([[maybe_unused]] const TList<TTypes...>& list) const noexcept
            -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                                Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>>;

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// constructible from arguments of types `TTypes...`, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// template<typename... TTypes>
        /// struct list {};
        /// struct tag {};
        /// struct constructible {
        ///     constructible(int, double, tag);
        ///     constructible(int, tag, tag) noexcept;
        /// }
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible.is_noexcept_constructible_from(decltype_<int>(),
        ///                                                            decltype_<tag>(),
        ///                                                            decltype_<tag>()));
        /// static_assert(not constructible.is_noexcept_constructible_from(decltype_<int>(),
        ///                                                                decltype_<double>(),
        ///                                                                decltype_<tag>()));
        /// static_assert(not constructible.is_noexcept_constructible_from(decltype_<double>{}));
        /// static_assert(not int_t.is_noexcept_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TTypes The list of argument types to check with
        /// @param list The `Type`s representing the argument types to check with
        /// @return whether the type `this` represents is `noexcept` constructible from arguments of
        /// types `TTypes...`, as a `Value` specialization
        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_noexcept_constructible_from([[maybe_unused]] const Type<TTypes>&... list) const noexcept
            -> Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_default_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_default_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_default_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,

                                Value<std::is_nothrow_default_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_default_constructible() const noexcept

            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_default_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_copy_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_copy_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_copy_constructible() const noexcept

            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_copy_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_copy_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_copy_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_move_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_move_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_move_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_move_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_move_constructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_move_constructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_copy_assignable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_copy_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_copy_assignable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_copy_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_copy_assignable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_copy_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_move_assignable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_move_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_move_assignable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_move_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_move_assignable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_move_assignable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_destructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_destructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_destructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_destructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_trivially_destructible() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_trivially_destructible_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto
        is_swappable() const noexcept -> std::enable_if_t<std::same_as<TDelay, type>,
                                                          Value<std::is_swappable_v<TDelay>, bool>>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto is_noexcept_swappable() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>,
                                Value<std::is_nothrow_swappable_v<TDelay>, bool>>;

        template<typename TRhs = std::conditional_t<std::is_reference_v<type>,
                                                    type,
                                                    std::add_lvalue_reference_t<type>>>
        [[nodiscard]] constexpr auto
        is_swappable_with([[maybe_unused]] const Type<TRhs>& rhs = Type<TRhs>{}) const noexcept
            -> Value<std::is_swappable_with_v<std::conditional_t<std::is_reference_v<type>,
                                                                 type,
                                                                 std::add_lvalue_reference_t<type>>,
                                              TRhs>,
                     bool>;

        template<typename TRhs = std::conditional_t<std::is_reference_v<type>,
                                                    type,
                                                    std::add_lvalue_reference_t<type>>>
        [[nodiscard]] constexpr auto
        is_noexcept_swappable_with([[maybe_unused]] const Type<TRhs>& rhs
                                   = Type<TRhs>{}) const noexcept
            -> Value<std::is_nothrow_swappable_with_v<
                         std::conditional_t<std::is_reference_v<type>,
                                            type,
                                            std::add_lvalue_reference_t<type>>,
                         TRhs>,
                     bool>;

        template<typename TDelay = type>
        [[nodiscard]] constexpr auto sizeof_() const noexcept
            -> std::enable_if_t<std::same_as<TDelay, type>, Value<sizeof(TDelay), usize>>;
    };

    /// @brief Returns an `mpl::Type` representing the type of the given argument
    ///
    /// # Requirements
    /// - `TType` must be a reference-qualified type
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto value = true;
    /// constexpr auto value_type = decltype_(value);
    ///
    /// static_assert(value_type.is(decltype_<const bool&>()));
    /// static_assert(value_type == decltype_<const bool&>());
    /// @endcode
    ///
    /// @tparam TType The type of the argument to get the type of
    /// @param type The argument to get the type of
    /// @return an `mpl::Type` representing the type of `type`
    template<typename TType>
        requires std::is_reference_v<TType>
    [[nodiscard]] constexpr auto
    decltype_([[maybe_unused]] TType&& type) // NOLINT(*-missing-std-forward)
        noexcept -> std::conditional_t<MetaType<std::remove_cvref_t<TType>>,
                                       std::remove_cvref_t<TType>,
                                       Type<decltype(type)>> {
        return {};
    }

    /// @brief Returns an `mpl::Type` representing the type of the given argument
    ///
    /// # Requirements
    /// - `TType` must _not_ be a reference-qualified type
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto value_type = decltype_(1);
    ///
    /// static_assert(value_type.is(decltype_<int>()));
    /// static_assert(value_type == decltype_<int>());
    /// @endcode
    ///
    /// @tparam TType The type of the argument to get the type of
    /// @param type The argument to get the type of
    /// @return an `mpl::Type` representing the type of `type`
    template<typename TType>
        requires(!std::is_reference_v<TType>)
    [[nodiscard]] constexpr auto
    decltype_([[maybe_unused]] TType&& type) // NOLINT(*-missing-std-forward)
        noexcept -> std::conditional_t<MetaType<TType>, TType, Type<TType>> {
        return {};
    }

    /// @brief Returns an `mpl::Type` representing the type `TType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto value_type = decltype_(1);
    /// constexpr auto int_type = decltype_<int>();
    ///
    /// static_assert(value_type.is(int_type));
    /// static_assert(value_type == int_type);
    /// @endcode
    ///
    /// @tparam TType The type to represent as an `mpl::Type`
    /// @return an `mpl::Type` representing the type `TType`
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
    [[nodiscard]] constexpr auto Type<TType>::apply() const noexcept -> std::enable_if_t<
        TypeMetaFunction<TMetaFunction> && MetaValue<TMetaFunction<type>>,
        Value<TMetaFunction<type>::value, decltype(TMetaFunction<type>::value)>> {
        return {};
    }

    template<typename TType>
    template<typename TFunction>
    [[nodiscard]] constexpr auto
    Type<TType>::apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
        const noexcept -> std::enable_if_t<
            MetaFunctionOf<TFunction, Type<type>>
                && MetaValue<meta_result_t<TFunction, Type<type>>>,
            Value<meta_result_t<TFunction, Type<type>>::value,
                  std::remove_cvref_t<decltype(meta_result_t<TFunction, Type<type>>::value)>>> {
        return as_value(meta_result_t<TFunction, Type<type>>{});
    }

    template<typename TType>
    template<template<typename> typename TPredicate>
    [[nodiscard]] constexpr auto Type<TType>::satisfies() const noexcept -> std::enable_if_t<
        TypeMetaFunction<TPredicate> && MetaValue<TPredicate<type>>
            && std::same_as<std::remove_const_t<decltype(TPredicate<type>::value)>, bool>,
        Value<TPredicate<type>::value, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TPredicate>
    [[nodiscard]] constexpr auto
    Type<TType>::satisfies([[maybe_unused]] TPredicate&& predicate) // NOLINT(*-missing-std-forward)
        const noexcept -> std::enable_if_t<
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
        const noexcept -> std::enable_if_t<
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
    [[nodiscard]] constexpr auto
    Type<TType>::is([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
        -> Value<std::same_as<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_const() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_const_v<std::remove_reference_t<TDelay>>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_lvalue_reference() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_lvalue_reference_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_rvalue_reference() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_rvalue_reference_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_volatile() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_volatile_v<std::remove_reference_t<TDelay>>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_convertible_to([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
        -> Value<std::convertible_to<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_derived_from([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
        -> Value<std::derived_from<type, TRhs> && !std::same_as<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_base_of([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
        -> Value<std::derived_from<TRhs, type> && !std::same_as<type, TRhs>, bool> {
        return {};
    }

    template<typename TType>
    template<template<typename...> typename TList, typename... TTypes>
    [[nodiscard]] constexpr auto
    Type<TType>::is_constructible_from([[maybe_unused]] const TList<TTypes...>& list) const noexcept
        -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                            Value<std::is_constructible_v<type, TTypes...>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename... TTypes>
    [[nodiscard]] constexpr auto
    Type<TType>::is_constructible_from([[maybe_unused]] const Type<TTypes>&... list) const noexcept
        -> Value<std::is_constructible_v<type, TTypes...>, bool> {
        return {};
    }

    template<typename TType>
    template<template<typename...> typename TList, typename... TTypes>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_constructible_from(
        [[maybe_unused]] const TList<TTypes...>& list) const noexcept
        -> std::enable_if_t<!MetaType<TList<TTypes...>>,
                            Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename... TTypes>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_constructible_from(
        [[maybe_unused]] const Type<TTypes>&... list) const noexcept
        -> Value<std::is_nothrow_constructible_v<type, TTypes...>, bool> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_default_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_default_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_default_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_default_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_default_constructible() const noexcept

        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_default_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_copy_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_copy_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_copy_constructible() const noexcept

        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_copy_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_copy_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_copy_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_move_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_move_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_move_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_move_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_move_constructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_move_constructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_copy_assignable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_copy_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_copy_assignable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_copy_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_copy_assignable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_copy_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_move_assignable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_move_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_move_assignable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_move_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_move_assignable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_move_assignable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_destructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_destructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_destructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_destructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_trivially_destructible() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_trivially_destructible_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_swappable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>, Value<std::is_swappable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::is_noexcept_swappable() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>,
                            Value<std::is_nothrow_swappable_v<TDelay>, bool>> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_swappable_with([[maybe_unused]] const Type<TRhs>& rhs) const noexcept -> Value<
        std::is_swappable_with_v<
            std::conditional_t<std::is_reference_v<type>, type, std::add_lvalue_reference_t<type>>,
            TRhs>,
        bool> {
        return {};
    }

    template<typename TType>
    template<typename TRhs>
    [[nodiscard]] constexpr auto
    Type<TType>::is_noexcept_swappable_with([[maybe_unused]] const Type<TRhs>& rhs) const noexcept
        -> Value<
            std::is_nothrow_swappable_with_v<std::conditional_t<std::is_reference_v<type>,
                                                                type,
                                                                std::add_lvalue_reference_t<type>>,
                                             TRhs>,
            bool> {
        return {};
    }

    template<typename TType>
    template<typename TDelay>
    [[nodiscard]] constexpr auto Type<TType>::sizeof_() const noexcept
        -> std::enable_if_t<std::same_as<TDelay, type>, Value<sizeof(TDelay), usize>> {
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

        constexpr auto is_const_type_type =
            [](MetaType auto type) noexcept -> Type<Value<(decltype(type){}).is_const(), bool>> {
            return {};
        };

        constexpr auto is_const_type_value =
            [](MetaType auto type) noexcept -> Value<(decltype(type){}).is_const(), bool> {
            return {};
        };

        constexpr auto is_true_value_type
            = [](MetaValue auto value) noexcept -> Type<Value<decltype(value)::value, bool>> {
            return {};
        };

        constexpr auto is_true_value_value
            = [](MetaValue auto value) noexcept -> Value<decltype(value)::value, bool> {
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

        constexpr auto add1 = [](MetaValue auto value) -> Value<decltype(value)::value + 1> {
            return {};
        };

        constexpr auto times2 = [](MetaValue auto value) -> Value<decltype(value)::value * 2> {
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

        constexpr auto add1_semi_typely
            = [](MetaType auto type) -> Value<std::remove_cvref_t<decltype(type)>::type::value + 1>
            requires MetaValue<typename decltype(type)::type>
        {
            return {};
        };

        constexpr auto times2_semi_typely
            = [](MetaType auto type) -> Value<std::remove_cvref_t<decltype(type)>::type::value * 2>
            requires MetaValue<typename decltype(type)::type>
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

        constexpr auto add1_fully_typely
            = [](MetaType auto type) -> Type<Value<decltype(type)::type::value + 1>>
            requires MetaValue<typename decltype(type)::type>
        {
            return {};
        };

        constexpr auto times2_fully_typely
            = [](MetaType auto type) -> Type<Value<decltype(type)::type::value * 2>>
            requires MetaValue<typename decltype(type)::type>
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

        constexpr auto add_const
            = [](MetaType auto type) -> std::add_const<typename decltype(type)::type> {
            return {};
        };

        constexpr auto add_lvalue_reference
            = [](MetaType auto type) -> std::add_lvalue_reference<typename decltype(type)::type> {
            return {};
        };

        constexpr auto remove_reference
            = [](MetaType auto type) -> std::remove_reference<typename decltype(type)::type> {
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

        constexpr auto add_const_typed
            = [](MetaType auto type) -> Type<std::add_const_t<typename decltype(type)::type>> {
            return {};
        };

        constexpr auto add_lvalue_reference_typed = [](MetaType auto type)
            -> Type<std::add_lvalue_reference_t<typename decltype(type)::type>> {
            return {};
        };

        constexpr auto remove_reference_typed =
            [](MetaType auto type) -> Type<std::remove_reference_t<typename decltype(type)::type>> {
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

        static_assert(decltype_<int>().sizeof_() == 4_usize,
                      "hyperion::mpl::Type::sizeof_ test case 1 (failing)");
        static_assert(decltype_<double>().sizeof_() == 8_usize,
                      "hyperion::mpl::Type::sizeof_ test case 2 (failing)");
        static_assert(decltype_<char>().sizeof_() == 1_usize,
                      "hyperion::mpl::Type::sizeof_ test case 3 (failing)");
    } // namespace _test::type
} // namespace hyperion::mpl

#endif // HYPERION_MPL_TYPE_H
