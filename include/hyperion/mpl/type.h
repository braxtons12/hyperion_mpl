/// @file type.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Metaprogramming type wrapper for use as metafunction parameter and return type
/// @version 0.3.0
/// @date 2025-11-24
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

#ifndef HYPERION_MPL_TYPE_H
#define HYPERION_MPL_TYPE_H


#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>
//
#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/value.h>

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
        /// metaprogramming type.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `TypeMetaFunction`:
        ///     - It must be a template taking a single type parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        ///
        /// # Example
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
        /// @return The result of applying `TMetaFunction`
        template<template<typename> typename TMetaFunction>
            requires TypeMetaFunction<TMetaFunction>
        [[nodiscard]] constexpr auto
        apply() const noexcept -> detail::convert_to_meta_t<TMetaFunction<type>> {
            return {};
        }

        /// @brief Applies the specified template metafunction to this specialization of `Type`.
        ///
        /// Applies `TMetaFunction` to the `Value` specialization this `Type` specialization
        /// represents, and returns the calculated metaprogramming type.
        ///
        /// # Requirements
        /// - `TMetaFunction` must be a `ValueMetaFunction`:
        ///     - It must be a template taking a single value parameter,
        ///     - It must have a `static constexpr` member variable, `value`, or a using alias type,
        ///     `type`
        /// - `type` must be a `MetaValue`
        ///
        /// # Example
        /// @code {.cpp}
        /// template<auto TValue>
        /// struct is_two {
        ///     using type = Value<TValue == 2, bool>;
        /// };
        ///
        /// // `was_two` is `Type<Value<true, bool>>>`
        /// constexpr auto was_two = decltype_(2_value).apply<is_two>();
        ///
        /// static_assert(was_two.inner());
        /// @endcode
        ///
        /// @tparam TMetaFunction The template metafunction to apply to this `Type`
        /// @return The result of applying `TMetaFunction`
        template<template<auto> typename TMetaFunction, typename TDelay = type>
            requires ValueMetaFunction<TMetaFunction> && std::same_as<TDelay, type>
                     && MetaValue<TDelay>
        [[nodiscard]] constexpr auto
        apply() const noexcept -> detail::convert_to_meta_t<TMetaFunction<TDelay::value>> {
            return {};
        }

        /// @brief Applies the given metafunction to this specialization of `Type`.
        ///
        /// Applies `func` to this specialization of `Type` and returns the calculated result as a
        /// metaprogramming type.
        ///
        /// # Requirements
        /// - `TFunction` must be a `MetaFunctionOf<Type>` type
        ///     - It must be a callable with an overload taking a single `Type` parameter
        ///     - The selected overload of `TFunction` must return either a `MetaType` or a
        ///     `MetaValue`
        ///
        /// # Example
        ///
        /// @code {.cpp}
        /// constexpr auto add_const = [](MetaType auto type) {
        ///     return type.as_const();
        /// };
        ///
        /// constexpr auto const_int = decltype_<int>().apply(add_const); // Type<const int>
        ///
        /// static_assert(const_int == decltype_<const int>());
        ///
        /// constexpr auto get_size = [](MetaType auto value) {
        ///     return value.sizeof_();
        /// };
        ///
        /// constexpr auto sizeof_int = decltype_<int>().apply(get_size); // Value<4, usize>
        ///
        /// static_assert(sizeof_int == 4_usize);
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Type` specialization, as a `Type`
        /// specialization
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Type<type>>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
                                                 // NOLINTNEXTLINE(modernize-type-traits)
            const noexcept -> detail::convert_to_meta_t<meta_result_t<TFunction, Type<type>>> {
            return {};
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
        /// - The result of invoking `func` with a `Type` must be a `MetaType`
        /// - `typename meta_result_t<TFunction, type>::type` must _not_ be a `MetaType`
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
        /// constexpr auto was_two = decltype_(2_value).apply(is_two); // Value<true, bool>
        ///
        /// static_assert(was_two);
        ///
        /// constexpr auto is_two_typed = [](MetaValue auto value)
        ///     -> Type<Value<decltype(value)::value == 2, bool>>
        /// {
        ///     return {};
        /// };
        ///
        /// // Value<true, bool>
        /// constexpr auto was_two_typed = decltype_(2_value).apply(is_two_typed);
        ///
        /// static_assert(was_two_typed);
        /// @endcode
        ///
        /// @tparam TFunction The type of the metafunction to apply
        /// @param func The metafunction to apply
        /// @return The result of applying `func` to this `Type` specialization
        template<typename TFunction>
            requires MetaFunctionOf<TFunction, type> && MetaValue<type>
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
        [[nodiscard]] constexpr auto
        satisfies() const noexcept -> Value<TPredicate<type>::value, bool> {
            return {};
        }

        /// @brief Checks to see if this `Type` specialization satisfies the given metafunction
        /// predicate.
        ///
        /// # Requirements
        /// - If `type` is a `MetaValue`:
        ///     - If `predicate` is invocable with `type`:
        ///         - The value returned by invoking `predicate` with `type` must either be a
        ///         `MetaValue`, or a `Type` specialization representing a `MetaValue`.
        ///             - Using `TValue` to represent that `MetaValue` type, the type of the value
        ///             of `TValue`, that is `decltype(TValue::value)`, must be of type
        ///             (possibly cv-ref qualified) `bool`
        ///         - If the above conditions on the return value of `predicate` are not met,
        ///         the program is ill-formed
        ///     - If `predicate` is not invocable with `type`, see the case where `predicate` is
        ///     invocable with `Type<type>`
        /// - If `predicate` is invocable with `Type<type>`:
        ///     - The value returned by invoking `predicate` with `Type<type>` must either be a
        ///     `MetaValue`, or a `Type` specialization representing a `MetaValue`.
        ///         - Using `TValue` to represent that `MetaValue` type, the type of the value of
        ///         `TValue`, that is `decltype(TValue::value)`, must be of type
        ///         (possibly cv-ref qualified) `bool`
        ///     - If the above conditions on the return value of `predicate` are not met,
        ///     the program is ill-formed
        /// - If `predicate` is not invocable with `Type<type>` nor `type`, returns `Value<false>`
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto is_const = [](MetaType auto type) {
        ///     return type.is_const();
        /// };
        ///
        /// constexpr auto not_const = decltype_<int>().satisfies(is_const);
        /// constexpr auto was_const = decltype_<const int>().satisfies(is_const);
        /// static_assert(was_const);
        /// static_assert(not not_const);
        /// @endcode
        ///
        /// @tparam TPredicate The type of the metafunction predicate to check with
        /// @param predicate The metafunction predicate to check with, as a
        /// `Value` specialization
        template<typename TPredicate>
        [[nodiscard]] constexpr auto satisfies(TPredicate&& predicate) const noexcept;

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
            -> Value<std::same_as<type, TRhs>, bool> {
            return {};
        }

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
        [[nodiscard]] constexpr auto
        is_const() const noexcept -> Value<std::is_const_v<std::remove_reference_t<type>>, bool> {
            return {};
        }

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
        [[nodiscard]] constexpr auto
        is_lvalue_reference() const noexcept -> Value<std::is_lvalue_reference_v<type>, bool> {
            return {};
        }

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
        [[nodiscard]] constexpr auto
        is_rvalue_reference() const noexcept -> Value<std::is_rvalue_reference_v<type>, bool> {
            return {};
        }

        /// @brief Returns whether the `type` of `this` `Type` specialization is a reference
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto intref_t = decltype_<int&>();
        /// constexpr auto intrefref_t = decltype_<int&&>();
        ///
        /// static_assert(not int_t.is_reference());
        /// static_assert(intref_t.is_reference());
        /// static_assert(intrefref_t.is_reference());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is a reference
        [[nodiscard]] constexpr auto
        is_reference() const noexcept -> Value<std::is_reference_v<type>, bool> {
            return {};
        }

        /// @brief Returns whether the `type` of `this` `Type` specialization is a pointer
        ///
        /// # Example
        /// @code {.cpp}
        /// contexpr auto int_t = decltype_<int>();
        /// constexpr auto int_ptr_t = decltype_<int*>();
        ///
        /// static_assert(not int_t.is_pointer());
        /// static_asser(int_ptr_t.is_pointer());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is a pointer
        [[nodiscard]] constexpr auto
        is_pointer() const noexcept -> Value<std::is_pointer_v<type>, bool> {
            return {};
        }

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
        [[nodiscard]] constexpr auto is_volatile() const noexcept
            -> Value<std::is_volatile_v<std::remove_reference_t<type>>, bool> {
            return {};
        }

        /// @brief Returns whether the `type` of `this` `Type` specialization is empty
        ///
        /// # Example
        /// @code {.cpp}
        /// struct empty {};
        /// constexpr auto yes = decltype_<empty>();
        /// constexpr auto no = decltype_<int>();
        ///
        /// static_assert(yes.is_empty());
        /// static_assert(not no.is_empty());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is empty
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto
        is_empty() const noexcept -> Value<std::is_empty_v<TDelay>, bool> {
            return {};
        }

        /// @brief Returns whether the `type` of `this` `Type` specialization is trivial
        ///
        /// # Example
        /// @code {.cpp}
        /// struct trivial {};
        /// struct not_trivial {
        ///     not_trivial(const not_trivial&);
        /// }
        /// constexpr auto yes = decltype_<trivial>();
        /// constexpr auto no = decltype_<not_trivial>();
        ///
        /// static_assert(yes.is_trivial());
        /// static_assert(not no.is_trivial());
        /// @endcode
        ///
        /// @return Whether the `type` of `this` is trivial
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto
        is_trivial() const noexcept -> Value<std::is_trivial_v<TDelay>, bool> {
            return {};
        }

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
        /// mechanisms, like `std::add_rvalue_reference`.  That is, where
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

        /// @brief Returns a `Type` specialization representing a poitner to the type `this`
        /// specialization represents.
        ///
        /// The application of pointer follows the same application as other mechanisms,
        /// such as `std::add_pointer`. That is, when `type` is a reference type, this strips the
        /// the reference and returns a pointer to the referred-to type.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto int_ref_t = decltype_<int&>();
        ///
        /// static_assert(int_t.as_pointer() == decltype_<int*>());
        /// static_assert(int_ref_t.as_pointer() == decltype_<int*>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing `type*`
        template<typename TDelay = type>
            requires std::same_as<TDelay, type>
        [[nodiscard]] constexpr auto as_pointer() const noexcept {
            using base_type = std::remove_reference_t<TDelay>;
            return Type<std::add_pointer_t<base_type>>{};
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

        /// @brief Returns a `Type` specialization representing the non-lvalue reference qualified
        /// version of the type `this` specialization represents.
        ///
        /// That is, if the type `this` specialization represents is lvalue reference qualified,
        /// this returns a `Type` specialization representing the referred-to type. If the type
        /// `this` specialization represents is _not_ lvalue reference qualified, returns the same
        /// specialization as `this`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto int_ref_t = decltype_<int&>();
        /// constexpr auto int_refref_t = decltype_<int&&>();
        ///
        /// static_assert(int_t.remove_lvalue_reference() == decltype_<int>());
        /// static_assert(int_ref_t.remove_lvalue_reference() = decltype_<int>());
        /// static_assert(int_refref_t.remove_lvalue_reference() == decltype_<int&&>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing the non-lvalue reference qualified `type`
        [[nodiscard]] constexpr auto remove_lvalue_reference() const noexcept {
            if constexpr(Type<type>{}.is_lvalue_reference()) {
                return Type<std::remove_reference_t<type>>{};
            }
            else {
                return *this;
            }
        }

        /// @brief Returns a `Type` specialization representing the non-rvalue reference qualified
        /// version of the type `this` specialization represents.
        ///
        /// That is, if the type `this` specialization represents is rvalue reference qualified,
        /// this returns a `Type` specialization representing the referred-to type. If the type
        /// `this` specialization represents is _not_ rvalue reference qualified, returns the same
        /// specialization as `this`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto int_ref_t = decltype_<int&>();
        /// constexpr auto int_refref_t = decltype_<int&&>();
        ///
        /// static_assert(int_t.remove_rvalue_reference() == decltype_<int>());
        /// static_assert(int_ref_t.remove_rvalue_reference() = decltype_<int&>());
        /// static_assert(int_refref_t.remove_rvalue_reference() == decltype_<int>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing the non-rvalue reference qualified `type`
        [[nodiscard]] constexpr auto remove_rvalue_reference() const noexcept {
            if constexpr(Type<type>{}.is_rvalue_reference()) {
                return Type<std::remove_reference_t<type>>{};
            }
            else {
                return *this;
            }
        }

        /// @brief Returns a `Type` specialization representing the non-reference qualified
        /// version of the type `this` specialization represents.
        ///
        /// That is, if the type `this` specialization represents is reference qualified,
        /// this returns a `Type` specialization representing the referred-to type. If the type
        /// `this` specialization represents is _not_ reference qualified, returns the same
        /// specialization as `this`.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto int_ref_t = decltype_<int&>();
        /// constexpr auto int_refref_t = decltype_<int&&>();
        ///
        /// static_assert(int_t.remove_reference() == decltype_<int>());
        /// static_assert(int_ref_t.remove_reference() = decltype_<int>());
        /// static_assert(int_refref_t.remove_reference() == decltype_<int>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing the non-reference qualified `type`
        [[nodiscard]] constexpr auto remove_reference() const noexcept {
            if constexpr(Type<type>{}.is_reference()) {
                return Type<std::remove_reference_t<type>>{};
            }
            else {
                return *this;
            }
        }

        /// @brief Returns a `Type` specialization representing the non-const qualified
        /// version of the type `this` specialization represents.
        ///
        /// If the type `this` specialization represents is reference-qualified, removes const
        /// qualification from the referred-to type, if any. Otherwise, removes const qualifiication
        /// from the type `this` specialization represents.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto const_int_t = decltype_<const int>();
        /// constexpr auto const_int_ref_t = decltype_<const int&>();
        ///
        /// static_assert(int_t.remove_const() == decltype_<int>());
        /// static_assert(const_int_t.remove_const() = decltype_<int>());
        /// static_assert(const_int_ref_t.remove_const() == decltype_<int&>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing the non-const qualified `type`
        [[nodiscard]] constexpr auto remove_const() const noexcept {
            constexpr auto self = Type<type>{};
            if constexpr(self.is_const() && self.is_lvalue_reference()) {
                using unref_t = std::remove_reference_t<type>;
                using unconst_t = std::remove_const_t<unref_t>;
                return Type<std::add_lvalue_reference_t<unconst_t>>{};
            }
            else if constexpr(self.is_const() && self.is_rvalue_reference()) {
                using unref_t = std::remove_reference_t<type>;
                using unconst_t = std::remove_const_t<unref_t>;
                return Type<std::add_rvalue_reference_t<unconst_t>>{};
            }
            else {
                return Type<std::remove_const_t<type>>{};
            }
        }

        /// @brief Returns a `Type` specialization representing the non-volatile qualified
        /// version of the type `this` specialization represents.
        ///
        /// If the type `this` specialization represents is reference-qualified, removes volatile
        /// qualification from the referred-to type, if any. Otherwise, removes volatile
        /// qualifiication from the type `this` specialization represents.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto volatile_int_t = decltype_<volatile int>();
        /// constexpr auto volatile_int_ref_t = decltype_<volatile int&>();
        ///
        /// static_assert(int_t.remove_volatile() == decltype_<int>());
        /// static_assert(volatile_int_t.remove_volatile() = decltype_<int>());
        /// static_assert(volatile_int_ref_t.remove_volatile() == decltype_<int&>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing the non-volatile qualified `type`
        [[nodiscard]] constexpr auto remove_volatile() const noexcept {
            constexpr auto self = Type<type>{};
            if constexpr(self.is_volatile() && self.is_lvalue_reference()) {
                using unref_t = std::remove_reference_t<type>;
                using unvolatile_t = std::remove_volatile_t<unref_t>;
                return Type<std::add_lvalue_reference_t<unvolatile_t>>{};
            }
            else if constexpr(self.is_volatile() && self.is_rvalue_reference()) {
                using unref_t = std::remove_reference_t<type>;
                using unvolatile_t = std::remove_volatile_t<unref_t>;
                return Type<std::add_rvalue_reference_t<unvolatile_t>>{};
            }
            else {
                return Type<std::remove_volatile_t<type>>{};
            }
        }

        /// @brief Returns a `Type` specialization representing the unqualified
        /// version of the type `this` specialization represents.
        ///
        /// # Example
        /// @code {.cpp}
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto const_int_t = decltype_<const int>();
        /// constexpr auto const_int_ref_t = decltype_<volatile int&>();
        /// constexpr auto volatile_int_t = decltype_<volatile int>();
        /// constexpr auto volatile_int_ref_t = decltype_<volatile int&>();
        ///
        /// static_assert(int_t.unqualified() == decltype_<int>());
        /// static_assert(const_int_t.unqualified() == decltype_<int>());
        /// static_assert(const_int_ref_t.unqualified() == decltype_<int>());
        /// static_assert(volatile_int_t.unqualified() = decltype_<int>());
        /// static_assert(volatile_int_ref_t.unqualified() == decltype_<int>());
        /// @endcode
        ///
        /// @return the `Type` specialization representing the non-volatile qualified `type`
        [[nodiscard]] constexpr auto
        unqualified() const noexcept -> Type<std::remove_cvref_t<type>> {
            return Type<std::remove_cvref_t<type>>{};
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
            -> Value<std::convertible_to<type, TRhs>, bool> {
            return {};
        }

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
            -> Value<std::derived_from<type, TRhs> && !std::same_as<type, TRhs>, bool> {
            return {};
        }

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
            -> Value<std::derived_from<TRhs, type> && !std::same_as<type, TRhs>, bool> {
            return {};
        }

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
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible_t.is_constructible_from(list<int, double, tag>{}));
        /// static_assert(not constructible_t.is_constructible_from(list<double>{}));
        /// static_assert(not int_t.is_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TList The type-list type template representing the argument types to check with
        /// @tparam TTypes The list of argument types to check with
        /// @param list The type-list representing the argument types to check with
        /// @return whether the type `this` represents is constructible from arguments of types
        /// `TTypes...`, as a `Value` specialization
        template<template<typename...> typename TList, typename... TTypes>
            requires(!MetaType<TList<TTypes...>>)
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const TList<TTypes...>& list) const noexcept {
            return Value<std::is_constructible_v<type, detail::convert_to_raw_t<TTypes>...>,
                         bool>{};
        }

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
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible_t.is_constructible_from(decltype_<int>(),
        ///                                                     decltype_<double>(),
        ///                                                     decltype_<tag>()));
        /// static_assert(not constructible_t.is_constructible_from(decltype_<double>{}));
        /// static_assert(not int_t.is_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TTypes The list of argument types to check with
        /// @param list The `Type`s representing the argument types to check with
        /// @return whether the type `this` represents is constructible from arguments of types
        /// `TTypes...`, as a `Value` specialization
        template<typename... TTypes>
        [[nodiscard]] constexpr auto
        is_constructible_from([[maybe_unused]] const Type<TTypes>&... list) const noexcept {
            return Value<std::is_constructible_v<type, TTypes...>, bool>{};
        }

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
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible_t.is_noexcept_constructible_from(list<int, tag, tag>{}));
        /// static_assert(not constructible_t
        ///                     .is_noexcept_constructible_from(list<int, double, tag>{}));
        /// static_assert(not constructible_t.is_noexcept_constructible_from(list<double>{}));
        /// static_assert(not int_t.is_noexcept_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TList The type-list type template representing the argument types to check with
        /// @tparam TTypes The list of argument types to check with
        /// @param list The type-list representing the argument types to check with
        /// @return whether the type `this` represents is `noexcept` constructible from arguments of
        /// types `TTypes...`, as a `Value` specialization
        template<template<typename...> typename TList, typename... TTypes>
            requires(!MetaType<TList<TTypes...>>)
        [[nodiscard]] constexpr auto is_noexcept_constructible_from(
            [[maybe_unused]] const TList<TTypes...>& list) const noexcept //
        {
            return Value<std::is_nothrow_constructible_v<type, detail::convert_to_raw_t<TTypes>...>,
                         bool>{};
        }

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
        /// };
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        ///
        /// static_assert(constructible_t.is_noexcept_constructible_from(decltype_<int>(),
        ///                                                              decltype_<tag>(),
        ///                                                              decltype_<tag>()));
        /// static_assert(not constructible_t.is_noexcept_constructible_from(decltype_<int>(),
        ///                                                                  decltype_<double>(),
        ///                                                                  decltype_<tag>()));
        /// static_assert(not constructible_t.is_noexcept_constructible_from(decltype_<double>{}));
        /// static_assert(not int_t.is_noexcept_constructible_from(constructible));
        /// @endcode
        ///
        /// @tparam TTypes The list of argument types to check with
        /// @param list The `Type`s representing the argument types to check with
        /// @return whether the type `this` represents is `noexcept` constructible from arguments of
        /// types `TTypes...`, as a `Value` specialization
        template<typename... TTypes>
        [[nodiscard]] constexpr auto is_noexcept_constructible_from(
            [[maybe_unused]] const Type<TTypes>&... list) const noexcept {
            return Value<std::is_nothrow_constructible_v<type, TTypes...>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is default
        /// constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible();
        /// };
        /// struct not_constructible {
        ///     not_constructible() = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(constructible_t.is_default_constructible());
        /// static_assert(int_t.is_default_constructible());
        /// static_assert(not not_constructible_.is_default_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is default constructible, as a `Value`
        /// specialization
        [[nodiscard]] constexpr auto is_default_constructible() const noexcept {
            return Value<std::is_default_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// default constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible();
        /// };
        /// struct noexcept_constructible {
        ///     noexcept_constructible() noexcept;
        /// };
        /// struct not_constructible {
        ///     not_constructible() = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto noexcept_constructible_t = decltype_<noexcept_constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(int_t.is_noexcept_default_constructible());
        /// static_assert(noexcept_constructible_t.is_noexcept_default_constructible());
        /// static_assert(not constructible_t.is_noexcept_default_constructible());
        /// static_assert(not not_constructible_.is_noexcept_default_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is `noexcept` default constructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_default_constructible() const noexcept {
            return Value<std::is_nothrow_default_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is trivially
        /// default constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible();
        /// };
        /// struct trivially_constructible {
        ///     trivially_constructible() = default;
        /// };
        /// struct not_constructible {
        ///     not_constructible() = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto trivially_constructible_t = decltype_<trivially_constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(int_t.is_trivially_default_constructible());
        /// static_assert(trivially_constructible_t.is_trivially_default_constructible());
        /// static_assert(not constructible_t.is_trivially_default_constructible());
        /// static_assert(not not_constructible_.is_trivially_default_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is trivially default constructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_trivially_default_constructible() const noexcept {
            return Value<std::is_trivially_default_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is copy
        /// constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible(const constructible&);
        /// };
        /// struct not_constructible {
        ///     not_constructible(const not_constructible&) = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(constructible_t.is_copy_constructible());
        /// static_assert(int_t.is_copy_constructible());
        /// static_assert(not not_constructible_.is_copy_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is copy constructible, as a `Value`
        /// specialization
        [[nodiscard]] constexpr auto is_copy_constructible() const noexcept {
            return Value<std::is_copy_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// copy constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible(const constructible&);
        /// };
        /// struct noexcept_constructible {
        ///     noexcept_constructible(const noexcept_constructible&) noexcept;
        /// };
        /// struct not_constructible {
        ///     not_constructible(const not_constructible&) = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto noexcept_constructible_t = decltype_<noexcept_constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(int_t.is_noexcept_copy_constructible());
        /// static_assert(noexcept_constructible_t.is_noexcept_copy_constructible());
        /// static_assert(not constructible_t.is_noexcept_copy_constructible());
        /// static_assert(not not_constructible_.is_noexcept_copy_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is `noexcept` copy constructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_copy_constructible() const noexcept {
            return Value<std::is_nothrow_copy_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is trivially
        /// copy constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible(const constructible&);
        /// };
        /// struct trivially_constructible {
        ///     trivially_constructible(const trivially_constructible&) = default;
        /// };
        /// struct not_constructible {
        ///     not_constructible(const not_constructible&) = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto trivially_constructible_t = decltype_<trivially_constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(int_t.is_trivially_copy_constructible());
        /// static_assert(trivially_constructible_t.is_trivially_copy_constructible());
        /// static_assert(not constructible_t.is_trivially_copy_constructible());
        /// static_assert(not not_constructible_.is_trivially_copy_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is trivially copy constructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_trivially_copy_constructible() const noexcept {
            return Value<std::is_trivially_copy_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is move
        /// constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible(constructible&&);
        /// };
        /// struct not_constructible {
        ///     not_constructible(not_constructible&&) = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(constructible_t.is_move_constructible());
        /// static_assert(int_t.is_move_constructible());
        /// static_assert(not not_constructible_.is_move_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is move constructible, as a `Value`
        /// specialization
        [[nodiscard]] constexpr auto is_move_constructible() const noexcept {
            return Value<std::is_move_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// move constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible(constructible&&);
        /// };
        /// struct noexcept_constructible {
        ///     noexcept_constructible(noexcept_constructible&&) noexcept;
        /// };
        /// struct not_constructible {
        ///     not_constructible(not_constructible&&) = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto noexcept_constructible_t = decltype_<noexcept_constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(int_t.is_noexcept_move_constructible());
        /// static_assert(noexcept_constructible_t.is_noexcept_move_constructible());
        /// static_assert(not constructible_t.is_noexcept_move_constructible());
        /// static_assert(not not_constructible_.is_noexcept_move_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is `noexcept` move constructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_move_constructible() const noexcept {
            return Value<std::is_nothrow_move_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is trivially
        /// move constructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct constructible {
        ///     constructible(constructible&&);
        /// };
        /// struct trivially_constructible {
        ///     trivially_constructible(trivially_constructible&&) = default;
        /// };
        /// struct not_constructible {
        ///     not_constructible(not_constructible&&) = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto constructible_t = decltype_<constructible>();
        /// constexpr auto trivially_constructible_t = decltype_<trivially_constructible>();
        /// constexpr auto not_constructible_t = decltype_<not_constructible>();
        ///
        /// static_assert(int_t.is_trivially_move_constructible());
        /// static_assert(trivially_constructible_t.is_trivially_move_constructible());
        /// static_assert(not constructible_t.is_trivially_move_constructible());
        /// static_assert(not not_constructible_.is_trivially_move_constructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is trivially move constructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_trivially_move_constructible() const noexcept {
            return Value<std::is_trivially_move_constructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is copy
        /// assignable, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(const assignable&) -> assignable&;
        /// };
        /// struct not_assignable {
        ///     auto operator=(const not_assignable&) -> not_assignable& = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(assignable_t.is_copy_assignable());
        /// static_assert(int_t.is_copy_assignable());
        /// static_assert(not not_assignable_.is_copy_assignable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is copy assignable, as a `Value`
        /// specialization
        [[nodiscard]] constexpr auto is_copy_assignable() const noexcept {
            return Value<std::is_copy_assignable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// copy assignable, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(const assignable&) -> assignable&;
        /// };
        /// struct noexcept_assignable {
        ///     auto operator=(const noexcept_assignable&) noexcept -> noexcept_assignable&;
        /// };
        /// struct not_assignable {
        ///     auto operator=(const not_assignable&) -> not_assignable& = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto noexcept_assignable_t = decltype_<noexcept_assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(int_t.is_noexcept_copy_assignable());
        /// static_assert(noexcept_assignable_t.is_noexcept_copy_assignable());
        /// static_assert(not assignable_t.is_noexcept_copy_assignable());
        /// static_assert(not not_assignable_.is_noexcept_copy_assignable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is `noexcept` copy assignable, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_copy_assignable() const noexcept {
            return Value<std::is_nothrow_copy_assignable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is trivially
        /// copy assignable, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(const assignable&) -> assignable&;
        /// };
        /// struct trivially_assignable {
        ///     auto operator=(const trivially_assignable&) -> trivially_assignable& = default;
        /// };
        /// struct not_assignable {
        ///     auto operator=(const not_assignable&) -> not_assignable& = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto trivially_assignable_t = decltype_<trivially_assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(int_t.is_trivially_copy_assignable());
        /// static_assert(trivially_assignable_t.is_trivially_copy_assignable());
        /// static_assert(not assignable_t.is_trivially_copy_assignable());
        /// static_assert(not not_assignable_.is_trivially_copy_assignable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is trivially copy assignable, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_trivially_copy_assignable() const noexcept {
            return Value<std::is_trivially_copy_assignable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is move
        /// assignable, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(assignable&&) -> assignable&;
        /// };
        /// struct not_assignable {
        ///     auto operator=(not_assignable&&) -> not_assignable& = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(assignable_t.is_move_assignable());
        /// static_assert(int_t.is_move_assignable());
        /// static_assert(not not_assignable_.is_move_assignable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is move assignable, as a `Value`
        /// specialization
        [[nodiscard]] constexpr auto is_move_assignable() const noexcept {
            return Value<std::is_move_assignable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// move assignable, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(assignable&&) -> assignable&;
        /// };
        /// struct noexcept_assignable {
        ///     auto operator=(noexcept_assignable&&) noexcept -> noexcept_assignable&;
        /// };
        /// struct not_assignable {
        ///     auto operator=(not_assignable&&) -> not_assignable& = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto noexcept_assignable_t = decltype_<noexcept_assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(int_t.is_noexcept_move_assignable());
        /// static_assert(noexcept_assignable_t.is_noexcept_move_assignable());
        /// static_assert(not assignable_t.is_noexcept_move_assignable());
        /// static_assert(not not_assignable_.is_noexcept_move_assignable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is `noexcept` move assignable, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_move_assignable() const noexcept {
            return Value<std::is_nothrow_move_assignable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is trivially
        /// move assignable, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(assignable&&) -> assignable&;
        /// };
        /// struct trivially_assignable {
        ///     auto operator=(trivially_assignable&&) -> trivially_assignable& = default;
        /// };
        /// struct not_assignable {
        ///     auto operator=(not_assignable&&) -> not_assignable& = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto trivially_assignable_t = decltype_<trivially_assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(int_t.is_trivially_move_assignable());
        /// static_assert(trivially_assignable_t.is_trivially_move_assignable());
        /// static_assert(not assignable_t.is_trivially_move_assignable());
        /// static_assert(not not_assignable_.is_trivially_move_assignable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is trivially move assignable, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_trivially_move_assignable() const noexcept {
            return Value<std::is_trivially_move_assignable_v<type>, bool>{};
        }

        /// @brief Returns whether a variable of the type `type` of `this` `Type` specialization
        /// is assignable from a value of the `type` of the given one, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(int) -> assignable&;
        /// };
        /// struct trivially_assignable {
        ///     auto operator=(int) -> trivially_assignable& = default;
        /// };
        /// struct not_assignable {
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto trivially_assignable_t = decltype_<trivially_assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(int_t.is_assignable_from(decltype_<int>()));
        /// static_assert(assignable_t.is_assignable_from(decltype_<int>()));
        /// static_assert(trivially_assignable_t.is_assignable_from(decltype_<int>()));
        /// static_assert(not not_assignable_.is_assignable_from(decltype_<int>()));
        /// @endcode
        ///
        /// @tparam TRhs The `type` of the `Type` specialization of `rhs`
        /// @param rhs The `Type` specialization to compare to
        /// @return Whether a variable of the `type` of `this` is assignable from the `type` of rhs
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_assignable_from([[maybe_unused]] const Type<TRhs>& rhs) const noexcept {
            // clang-format off
            return Value<
                std::is_assignable_v<
                    std::conditional_t<
                        std::is_reference_v<type>,
                        type,
                        std::add_lvalue_reference_t<type>
                    >,
                    TRhs
                >,
                bool
            >{};
            // clang-format on
        }

        /// @brief Returns whether a variable of the type `type` of `this` `Type` specialization
        /// is noexcept assignable from a value of the `type` of the given one,
        /// as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(int) -> assignable&;
        /// };
        /// struct noexcept_assignable {
        ///     auto operator=(int) noexcept -> noexcept_assignable&;
        /// };
        /// struct not_assignable {
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto noexcept_assignable_t = decltype_<noexcept_assignable>();
        /// constexpr auto not_assignable_t = decltype_<not_assignable>();
        ///
        /// static_assert(int_t.is_noexcept_assignable_from(decltype_<int>()));
        /// static_assert(not assignable_t.is_noexcept_assignable_from(decltype_<int>()));
        /// static_assert(noexcept_assignable_t.is_noexcept_assignable_from(decltype_<int>()));
        /// static_assert(not not_assignable_.is_noexcept_assignable_from(decltype_<int>()));
        /// @endcode
        ///
        /// @tparam TRhs The `type` of the `Type` specialization of `rhs`
        /// @param rhs The `Type` specialization to compare to
        /// @return Whether a variable of the `type` of `this` is noexcept assignable
        /// from the `type` of rhs
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_noexcept_assignable_from([[maybe_unused]] const Type<TRhs>& rhs) const noexcept {
            // clang-format off
            return Value<
                std::is_nothrow_assignable_v<
                    std::conditional_t<
                        std::is_reference_v<type>,
                        type,
                        std::add_lvalue_reference_t<type>
                    >,
                    TRhs
                >,
                bool
            >{};
            // clang-format on
        }

        /// @brief Returns whether a variable of the type `type` of `this` `Type` specialization
        /// is trivially assignable from a value of the `type` of the given one,
        /// as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct assignable {
        ///     auto operator=(int) -> assignable&;
        /// };
        /// struct trivially_assignable {
        ///     auto operator=(const trivially_assignable&) noexcept
        ///         -> trivially_assignable& = default;
        /// };
        /// struct trivial {
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto assignable_t = decltype_<assignable>();
        /// constexpr auto trivially_assignable_t = decltype_<trivially_assignable>();
        /// constexpr auto trivial_t = decltype_<trivial>();
        ///
        /// static_assert(int_t.is_trivially_assignable_from(decltype_<int>()));
        /// static_assert(not assignable_t.is_trivially_assignable_from(decltype_<int>()));
        /// static_assert(trivially_assignable_t.is_trivially_assignable_from(decltype_<trivially_assignable>()));
        /// static_assert(trivial_t.is_trivially_assignable_from(decltype_<trivial>()));
        /// @endcode
        ///
        /// @tparam TRhs The `type` of the `Type` specialization of `rhs`
        /// @param rhs The `Type` specialization to compare to
        /// @return Whether a variable of the `type` of `this` is trivially assignable
        /// from the `type` of rhs
        template<typename TRhs>
        [[nodiscard]] constexpr auto
        is_trivially_assignable_from([[maybe_unused]] const Type<TRhs>& rhs) const noexcept {
            // clang-format off
            return Value<
                std::is_trivially_assignable_v<
                    std::conditional_t<
                        std::is_reference_v<type>,
                        type,
                        std::add_lvalue_reference_t<type>
                    >,
                    TRhs
                >,
                bool
            >{};
            // clang-format on
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is
        /// destructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct destructible {
        ///     ~destructible();
        /// };
        /// struct not_destructible {
        ///     ~not_destructible() = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto destructible_t = decltype_<destructible>();
        /// constexpr auto not_destructible_t = decltype_<not_destructible>();
        ///
        /// static_assert(destructible_t.is_destructible());
        /// static_assert(int_t.is_destructible());
        /// static_assert(not not_destructible_.is_destructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is destructible, as a `Value`
        /// specialization
        [[nodiscard]] constexpr auto is_destructible() const noexcept {
            return Value<std::is_destructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// destructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct destructible {
        ///     ~destructible();
        /// };
        /// struct noexcept_destructible {
        ///     ~noexcept_destructible() noexcept;
        /// };
        /// struct not_destructible {
        ///     ~not_destructible() = delete;
        /// };
        ///
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto destructible_t = decltype_<destructible>();
        /// constexpr auto noexcept_destructible_t = decltype_<noexcept_destructible>();
        /// constexpr auto not_destructible_t = decltype_<not_destructible>();
        ///
        /// static_assert(int_t.is_noexcept_destructible());
        /// static_assert(noexcept_destructible_t.is_noexcept_destructible());
        /// static_assert(not destructible_t.is_noexcept_destructible());
        /// static_assert(not not_destructible_.is_noexcept_destructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is `noexcept` destructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_destructible() const noexcept {
            return Value<std::is_nothrow_destructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is trivially
        /// destructible, as a `Value` specialization.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct destructible {
        ///     ~destructible();
        /// }
        /// struct trivially_destructible {
        ///     ~trivially_destructible() = default;
        /// }
        /// struct not_destructible {
        ///     ~not_destructible() = delete;
        /// }
        /// constexpr auto int_t = decltype_<int>();
        /// constexpr auto destructible_t = decltype_<destructible>();
        /// constexpr auto trivially_destructible_t = decltype_<trivially_destructible>();
        /// constexpr auto not_destructible_t = decltype_<not_destructible>();
        ///
        /// static_assert(int_t.is_trivially_destructible());
        /// static_assert(trivially_destructible_t.is_trivially_destructible());
        /// static_assert(not destructible_t.is_trivially_destructible());
        /// static_assert(not not_destructible_.is_trivially_destructible());
        /// @endcode
        ///
        /// @return whether the type `this` represents is trivially destructible, as a
        /// `Value` specialization
        [[nodiscard]] constexpr auto is_trivially_destructible() const noexcept {
            return Value<std::is_trivially_destructible_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is swappable,
        /// as a `Value` specialization.
        ///
        /// If `type` is swappable, i.e., given `lhs` and `rhs` that are both `type&`,
        /// `std::swap(lhs, rhs)` is well-formed, returns `Value<true>`. Otherwise, returns
        /// `Value<false>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct not_swappable {
        ///     friend void swap(not_swappable&, not_swappable&) = delete;
        /// };
        /// struct swappable {
        ///     friend void swap(swappable&, swappable&);
        /// };
        ///
        /// static constexpr auto swappable_t = decltype_<swappable>();
        /// static constexpr auto not_swappable_t = decltype_<not_swappable>();
        ///
        /// static_assert(swappable_t.is_swappable());
        /// static_assert(not not_swappable_t.is_swappable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is swappable, as a `Value` specialization
        [[nodiscard]] constexpr auto is_swappable() const noexcept {
            return Value<std::is_swappable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// swappable, as a `Value` specialization.
        ///
        /// If `type` is `noexcept` swappable, i.e., given `lhs` and `rhs` that are both `type&`,
        /// `std::swap(lhs, rhs)` is both well-formed and `noexcept`, returns `Value<true>`.
        /// Otherwise, returns `Value<false>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct not_swappable {
        ///     friend void swap(not_swappable&, not_swappable&) = delete;
        /// };
        /// struct swappable {
        ///     friend void swap(swappable&, swappable&);
        /// };
        /// struct noexcept_swappable {
        ///     friend void swap(noexcept_swappable&, noexcept_swappable&) noexcept;
        /// };
        ///
        /// static constexpr auto swappable_t = decltype_<swappable>();
        /// static constexpr auto noexcept_swappable_t = decltype_<noexcept_swappable>();
        /// static constexpr auto not_swappable_t = decltype_<not_swappable>();
        ///
        /// static_assert(noexcept_swappable_t.is_noexcept_swappable());
        /// static_assert(not swappable_t.is_noexcept_swappable());
        /// static_assert(not not_swappable_t.is_noexcept_swappable());
        /// @endcode
        ///
        /// @return whether the type `this` represents is swappable, as a `Value` specialization
        [[nodiscard]] constexpr auto is_noexcept_swappable() const noexcept {
            return Value<std::is_nothrow_swappable_v<type>, bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is swappable
        /// with the type `rhs` represents, as a `Value` specialization.
        ///
        /// If the type `this` represents is swappable with the type `rhs` represents,
        /// i.e., given `val1` and `val2` that are `type&` and `TRhs&`, respectively,
        /// `std::swap(lhs, rhs)` is well-formed, returns `Value<true>`.
        /// Otherwise, returns `Value<false>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct not_swappable {
        ///     friend void swap(not_swappable&, int&) = delete;
        /// };
        /// struct swappable {
        ///     friend void swap(swappable&, int&);
        ///     friend void swap(int&, swappable&);
        /// };
        ///
        /// static constexpr auto swappable_t = decltype_<swappable>();
        /// static constexpr auto not_swappable_t = decltype_<not_swappable>();
        ///
        /// static_assert(swappable_t.is_swappable_with(decltype_<int>()));
        /// static_assert(not not_swappable_t.is_swappable_with(decltype_<int>()));
        /// @endcode
        ///
        /// @tparam TRhs The type to check that `type` is swappable with
        /// @param rhs The `Type` specialization representing the type to check that `type` is
        /// swappable with
        /// @return whether the type `this` represents is swappable with the type `rhs` represents,
        /// as a `Value` specialization
        template<typename TRhs = type>
        [[nodiscard]] constexpr auto
        is_swappable_with([[maybe_unused]] const Type<TRhs>& rhs = Type<TRhs>{}) const noexcept {
            return Value<
                std::is_swappable_with_v<std::conditional_t<std::is_reference_v<type>,
                                                            type,
                                                            std::add_lvalue_reference_t<type>>,
                                         std::conditional_t<std::is_reference_v<TRhs>,
                                                            TRhs,
                                                            std::add_lvalue_reference_t<TRhs>>>,
                bool>{};
        }

        /// @brief Returns whether the type `this` `Type` specialization represents is `noexcept`
        /// swappable with the type `rhs` represents, as a `Value` specialization.
        ///
        /// If the type `this` represents is `noexcept` swappable with the type `rhs` represents,
        /// i.e., given `val1` and `val2` that are `type&` and `TRhs&`, respectively,
        /// `std::swap(lhs, rhs)` is both well-formed and `noexcept`, returns `Value<true>`.
        /// Otherwise, returns `Value<false>`.
        ///
        /// # Example
        /// @code {.cpp}
        /// struct not_swappable {
        ///     friend void swap(not_swappable&, int&) = delete;
        /// };
        /// struct swappable {
        ///     friend void swap(swappable&, int&);
        ///     friend void swap(int&, swappable&);
        /// };
        /// struct noexcept_swappable {
        ///     friend void swap(swappable&, int&) noexcept;
        ///     friend void swap(int&, swappable&) noexcept;
        /// };
        ///
        /// static constexpr auto swappable_t = decltype_<swappable>();
        /// static constexpr auto noexcept_swappable_t = decltype_<swappable>();
        /// static constexpr auto not_swappable_t = decltype_<not_swappable>();
        ///
        /// static_assert(noexcept_swappable_t.is_noexcept_swappable_with(decltype_<int>()));
        /// static_assert(not swappable_t.is_noexcept_swappable_with(decltype_<int>()));
        /// static_assert(not not_swappable_t.is_noexcept_swappable_with(decltype_<int>()));
        /// @endcode
        ///
        /// @tparam TRhs The type to check that `type` is `noexcept` swappable with
        /// @param rhs The `Type` specialization representing the type to check that `type` is
        /// `noexcept` swappable with
        /// @return whether the type `this` represents is `noexcept` swappable with the type `rhs`
        /// represents, as a `Value` specialization
        template<typename TRhs = type>
        [[nodiscard]] constexpr auto is_noexcept_swappable_with(   //
            [[maybe_unused]] const Type<TRhs>& rhs = Type<TRhs>{}) //
            const noexcept                                         //
        {
            return Value<std::is_nothrow_swappable_with_v<
                             std::conditional_t<std::is_reference_v<type>,
                                                type,
                                                std::add_lvalue_reference_t<type>>,
                             std::conditional_t<std::is_reference_v<TRhs>,
                                                TRhs,
                                                std::add_lvalue_reference_t<TRhs>>>,
                         bool>{};
        }

        /// @brief Returns the `sizeof` the type `this` `Type` specialization represents,
        /// as a `Value` specialization.
        /// @return the `sizeof` the type `this` represents, as a `Value` specialization
        [[nodiscard]] constexpr auto sizeof_() const noexcept {
            return Value<sizeof(type), usize>{};
        }
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
        noexcept {
        if constexpr(MetaType<std::remove_cvref_t<TType>>) {
            return Type<typename std::remove_cvref_t<TType>::type>{};
        }
        else {
            return Type<decltype(type)>{};
        }
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
        noexcept {
        if constexpr(MetaType<TType>) {
            return Type<typename TType::type>{};
        }
        else {
            return Type<TType>{};
        }
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
    [[nodiscard]] constexpr auto decltype_() noexcept {
        if constexpr(MetaType<TType>) {
            return Type<typename TType::type>{};
        }
        else {
            return Type<TType>{};
        }
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

#ifndef HYPERION_MPL_VALUE_H
  // NOLINTNEXTLINE(misc-header-include-cycle)
    #include <hyperion/mpl/value.h>
#endif

namespace hyperion::mpl {
    template<typename TType>
    template<typename TPredicate>
    [[nodiscard]] constexpr auto Type<TType>::satisfies(TPredicate&& predicate) const noexcept {
        constexpr auto invoke = []<typename TPred>(TPred&& pred) noexcept {
            using invoke_type = std::
                conditional_t<MetaValue<type> && std::invocable<TPred, type>, type, Type<type>>;
            if constexpr(std::invocable<TPred, invoke_type>) {
                return std::forward<TPred>(pred)(invoke_type{});
            }
            else {
                return Value<false, bool>{};
            }
        };

        const auto result = invoke(std::forward<TPredicate>(predicate));
        if constexpr(MetaType<decltype(result)>) {
            using inner = typename decltype(result)::type;
            static_assert(MetaValue<inner>,
                          "Predicates passed to mpl::Type::satisfies must return a `MetaValue`"
                          "or an `mpl::Type<MetaValue>`");
            static_assert(std::is_same_v<std::remove_cvref_t<decltype(inner::value)>, bool>,
                          "`MetaValue`s returned by predicates passed to mpl::Type::satisfies"
                          "must have `value` of type (possibly cv-ref qualified) `bool`");
            return inner{};
        }
        else {
            static_assert(MetaValue<decltype(result)>,
                          "Predicates passed to mpl::Type::satisfies must return a `MetaValue`"
                          "or an `mpl::Type<MetaValue>`");
            static_assert(
                std::is_same_v<std::remove_cvref_t<decltype(decltype(result)::value)>, bool>,
                "`MetaValue`s returned by predicates passed to mpl::Type::satisfies"
                "must have `value` of type (possibly cv-ref qualified) `bool`");
            return decltype(result){};
        }
    }
} // namespace hyperion::mpl

#endif // HYPERION_MPL_TYPE_H
