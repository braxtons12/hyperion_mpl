/// @file metapredicates.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Callable definitions for common metaprogramming predicates
/// @version 0.1.1
/// @date 2025-01-18
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

#include <hyperion/platform/def.h>
//
#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>

#include <concepts>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup metapredicates Common Metaprogramming Predicate definitions
/// Hyperion provides an assortment of common metaprogramming predicates
/// to make operations based on them clear and convenient.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/list.h>
/// #include <hyperion/mpl/metapredicates.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto example1 = List<int, double, float>{};
/// static_assert(example1.none_of(equal_to(decltype_<usize>())));
///
/// constexpr auto example2 = List<Value<1>, Value<2>, Value<3>>{};
/// static_assert(example2.all_of(less_than(4_value)));
///
/// constexpr auto example3 = List<std::string, std::string_view>{};
/// static_assert(example3.all_of(constructible_from(List<const char*, std::size_t>{})));
///
/// @endcode
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

/// @headerfile hyperion/mpl/metatypes.h
/// @}

#ifndef HYPERION_MPL_METAPREDICATES_H
    #define HYPERION_MPL_METAPREDICATES_H

namespace hyperion::mpl {

    /// @brief Returns a metaprogramming predicate object used to query whether an
    /// argument is equal to `value`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(auto arg) noexcept`, that when invoked, returns whether
    /// `arg` is equal to `value`. Equality is determined, using the exposition-only
    /// template metafunction `as_meta` (see the corresponding section in the
    /// @ref list module-level documentation), as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl = typename as_meta<decltype(arg)>::type{};
    /// constexpr auto value_as_mpl = typename as_meta<decltype(value)::type{};
    /// constexpr auto result = arg_as_mpl == value_as_mpl;
    /// @endcode
    /// If `arg` and `value` do not fulfill the same metaprogramming type concept
    /// (e.g. if `arg` is `MetaValue` but `value` is `MetaType`), always returns
    /// `Value<false>`.
    ///
    /// # Requirements
    /// - `value` must be an instance of a `MetaValue`, `MetaType`, or `MetaPair`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<const int&>{};
    ///
    /// static_assert(example.satisfies(equal_to(decltype_<const int&>())));
    /// static_assert(not example.satisfies(equal_to(decltype_<float>())));
    /// static_assert(not example.satisfies(equal_to(1_value)));
    /// @endcode
    ///
    /// @param value The value to check for equality with
    /// @return A metaprogramming predicate object to check that an argument is equal to
    /// `value`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto equal_to([[maybe_unused]] auto value) noexcept
        requires MetaValue<decltype(value)> || MetaType<decltype(value)>
                 || MetaPair<decltype(value)>
    {
        return [](auto element) noexcept
            requires MetaValue<decltype(element)> || MetaType<decltype(element)>
                     || MetaPair<decltype(element)>
        {
            if constexpr((MetaValue<decltype(element)> && MetaValue<decltype(value)>)
                         || (MetaType<decltype(element)> && MetaType<decltype(value)>)
                         || (MetaPair<decltype(element)> && MetaPair<decltype(value)>))
            {
                return Value < detail::convert_to_meta_t<decltype(element)>{}
                           == detail::convert_to_meta_t<decltype(value)>{},
                       bool > {};
            }
            else {
                return Value<false>{};
            }
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether an
    /// argument is _not_ equal to `value`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(auto arg) noexcept`, that when invoked, returns whether
    /// `arg` is _not_ equal to `value`. Inequality is determined, using the
    /// exposition-only template metafunction `as_meta` (see the corresponding section
    /// in the @ref list module-level documentation), as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl = typename as_meta<decltype(arg)>::type{};
    /// constexpr auto value_as_mpl = typename as_meta<decltype(value)::type{};
    /// constexpr auto result = arg_as_mpl != value_as_mpl;
    /// @endcode
    /// If `arg` and `value` do not fulfill the same metaprogramming type concept
    /// (e.g. if `arg` is `MetaValue` but `value` is `MetaType`), always returns
    /// `Value<true>`.
    ///
    /// # Requirements
    /// - `value` must be an instance of a `MetaValue`, `MetaType`, or `MetaPair`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<const int&>{};
    ///
    /// static_assert(not example.satisfies(not_equal_to(decltype_<const int&>())));
    /// static_assert(example.satisfies(not_equal_to(decltype_<float>())));
    /// static_assert(example.satisfies(not_equal_to(1_value)));
    /// @endcode
    ///
    /// @param value The value to check for inequality with
    /// @return A metaprogramming predicate object to check that an argument is _not_
    /// equal to `value`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto not_equal_to([[maybe_unused]] auto value) noexcept
        requires MetaValue<decltype(value)> || MetaType<decltype(value)>
                 || MetaPair<decltype(value)>
    {
        return [](auto element)
            requires MetaValue<decltype(element)> || MetaType<decltype(element)>
                     || MetaPair<decltype(element)>
        {
            return not equal_to(decltype(value){})(decltype(element){});
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether an
    /// argument is less than `value`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(auto arg) noexcept`, that when invoked, returns whether
    /// `arg` is less than `value`, determined as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl_value = Value<decltype(arg)::value>{};
    /// constexpr auto value_as_mpl_value = Value<decltype(value)::value>{};
    /// constexpr auto result = arg_as_mpl_value < value_as_mpl_value;
    /// @endcode
    ///
    /// # Requirements
    /// - `value` must be an instance of a `MetaValue`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = 3_value;
    ///
    /// static_assert(example.satisfies(less_than(1_value)));
    /// static_assert(not example.satisfies(less_than(4_value)));
    /// static_assert(not example.satisfies(less_than(5_value)));
    /// @endcode
    ///
    /// @param value The value to check that arguments are less than
    /// @return A metaprogramming predicate object to check that an argument is
    /// less than `value`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto less_than([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return detail::convert_to_meta_t<decltype(element)>{}
                   < detail::convert_to_meta_t<decltype(value)>{};
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether an
    /// argument is less than or equal to `value`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(auto arg) noexcept`, that when invoked, returns whether
    /// `arg` is less than or equal to `value`, determined as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl_value = Value<decltype(arg)::value>{};
    /// constexpr auto value_as_mpl_value = Value<decltype(value)::value>{};
    /// constexpr auto result = arg_as_mpl_value <= value_as_mpl_value;
    /// @endcode
    ///
    /// # Requirements
    /// - `value` must be an instance of a `MetaValue`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = 3_value;
    ///
    /// static_assert(example.satisfies(less_than_or_equal_to(1_value)));
    /// static_assert(example.satisfies(less_than_or_equal_to(3_value)));
    /// static_assert(not example.satisfies(less_than_or_equal_to(5_value)));
    /// @endcode
    ///
    /// @param value The value to check that arguments are less than or equal to
    /// @return A metaprogramming predicate object to check that an argument is
    /// less than or equal to `value`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto
    less_than_or_equal_to([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return detail::convert_to_meta_t<decltype(element)>{}
                   <= detail::convert_to_meta_t<decltype(value)>{};
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether an
    /// argument is greater than `value`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(auto arg) noexcept`, that when invoked, returns whether
    /// `arg` is greater than `value`, determined as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl_value = Value<decltype(arg)::value>{};
    /// constexpr auto value_as_mpl_value = Value<decltype(value)::value>{};
    /// constexpr auto result = arg_as_mpl_value > value_as_mpl_value;
    /// @endcode
    ///
    /// # Requirements
    /// - `value` must be an instance of a `MetaValue`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = 3_value;
    ///
    /// static_assert(example.satisfies(greater_than(1_value)));
    /// static_assert(not example.satisfies(greater_than(4_value)));
    /// static_assert(not example.satisfies(greater_than(5_value)));
    /// @endcode
    ///
    /// @param value The value to check that arguments are greater than
    /// @return A metaprogramming predicate object to check that an argument is
    /// greater than `value`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto greater_than([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return detail::convert_to_meta_t<decltype(element)>{}
                   > detail::convert_to_meta_t<decltype(value)>{};
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether an
    /// argument is greater than or equal to `value`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(auto arg) noexcept`, that when invoked, returns whether
    /// `arg` is greater than or equal to `value`, determined as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl_value = Value<decltype(arg)::value>{};
    /// constexpr auto value_as_mpl_value = Value<decltype(value)::value>{};
    /// constexpr auto result = arg_as_mpl_value >= value_as_mpl_value;
    /// @endcode
    ///
    /// # Requirements
    /// - `value` must be an instance of a `MetaValue`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = 3_value;
    ///
    /// static_assert(example.satisfies(greater_than_or_equal_to(1_value)));
    /// static_assert(example.satisfies(greater_than_or_equal_to(3_value)));
    /// static_assert(not example.satisfies(greater_than_or_equal_to(5_value)));
    /// @endcode
    ///
    /// @param value The value to check that arguments are greater than or equal to
    /// @return A metaprogramming predicate object to check that an argument is
    /// greater than or equal to `value`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto
    greater_than_or_equal_to([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return detail::convert_to_meta_t<decltype(element)>{}
                   >= detail::convert_to_meta_t<decltype(value)>{};
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents the same type as `type`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents the same type as `type`, determined as if by:
    /// @code {.cpp}
    /// constexpr auto arg_as_mpl_type = decltype_(arg);
    /// constexpr auto result = as_mpl_type.is(decltype_(type));
    /// @endcode
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<const int&>{};
    ///
    /// static_assert(example.satisfies(is(decltype_<const int&>())));
    /// static_assert(not example.satisfies(is(decltype_<int>())));
    /// static_assert(not example.satisfies(is(decltype_<float>())));
    /// @endcode
    ///
    /// @note This is very similar to `equal_to`, with the sole difference being that
    /// this and the returned predicate object are _only_ invocable with `MetaType`s,
    /// whereas `equal_to` is invocable with any type fulfilling a metaprogramming
    /// concept's requirements.
    ///
    /// @param type The `MetaType` representing the type to check that an argument is
    /// @return A metaprogramming predicate object to check that an argument represents
    /// the same type as `typ`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto is(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is(decltype_(decltype(type){}));
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is a qualification of the type
    /// represented by `type`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents the same type as, or a qualification of, the type
    /// represented by `type`, determined as if by:
    /// @code {.cpp}
    /// constexpr auto as_mpl_type = decltype_(arg);
    /// constexpr auto result = as_mpl_type.is_qualification_of(decltype_(type));
    /// @endcode
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<const int&>{};
    ///
    /// static_assert(example.satisfies(qualification_of(decltype_<int>())));
    /// static_assert(not example.satisfies(qualification_of(decltype_<const int&>())));
    /// static_assert(not example.satisfies(qualification_of(decltype_<float>())));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that an argument is a
    /// qualification of
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is (possibly) a qualification of the type represented by `type`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto qualification_of(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_qualification_of(decltype_(decltype(type){}));
        };
    }

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `const` qualified.
    ///
    /// Determines whether the represented type is `const` as if by
    /// `decltype_(type).is_const()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example1 = decltype_<const int&>{};
    /// constexpr auto example2 = decltype_<const int>{};
    /// constexpr auto example3 = decltype_<int>{};
    ///
    /// static_assert(example1.satisfies(is_const));
    /// static_assert(example2.satisfies(is_const));
    /// static_assert(not example3.satisfies(is_const));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is `const`
    /// @return whether the type represented by `type` is `const`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto is_const = [](MetaType auto type) noexcept {
        return decltype_(type).is_const();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is lvalue-reference qualified.
    ///
    /// Determines whether the represented type is lvalue-reference qualified as
    /// if by `decltype_(type).is_lvalue_reference()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example1 = decltype_<const int&>{};
    /// constexpr auto example2 = decltype_<const int>{};
    /// constexpr auto example3 = decltype_<int>{};
    ///
    /// static_assert(example1.satisfies(is_lvalue_reference));
    /// static_assert(not example2.satisfies(is_lvalue_reference));
    /// static_assert(not example3.satisfies(is_lvalue_reference));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// an lvalue-reference
    /// @return whether the type represented by `type` is an lvalue-reference
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto is_lvalue_reference = [](MetaType auto type) noexcept {
        return decltype_(type).is_lvalue_reference();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is rvalue-reference qualified.
    ///
    /// Determines whether the represented type is rvalue-reference qualified as
    /// if by `decltype_(type).is_rvalue_reference()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example1 = decltype_<const int&&>{};
    /// constexpr auto example2 = decltype_<const int>{};
    /// constexpr auto example3 = decltype_<int>{};
    ///
    /// static_assert(example1.satisfies(is_rvalue_reference));
    /// static_assert(not example2.satisfies(is_rvalue_reference));
    /// static_assert(not example3.satisfies(is_rvalue_reference));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// an rvalue-reference
    /// @return whether the type represented by `type` is an rvalue-reference
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto is_rvalue_reference = [](MetaType auto type) noexcept {
        return decltype_(type).is_rvalue_reference();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `volatile` qualified.
    ///
    /// Determines whether the represented type is `volatile` as if by
    /// `decltype_(type).is_volatile()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example1 = decltype_<volatile int&>{};
    /// constexpr auto example2 = decltype_<volatile int>{};
    /// constexpr auto example3 = decltype_<int>{};
    ///
    /// static_assert(example1.satisfies(is_volatile));
    /// static_assert(example2.satisfies(is_volatile));
    /// static_assert(not example3.satisfies(is_volatile));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is `volatile`
    /// @return whether the type represented by `type` is `volatile`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto is_volatile = [](MetaType auto type) noexcept {
        return decltype_(type).is_volatile();
    };

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is convertible to the type
    /// represented by `type`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type convertible to the type represented by `type`,
    /// determined as if by `decltype_(element).is_convertible_to(decltype_(type))`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<int>{};
    /// struct example2 {};
    ///
    /// static_assert(example.satisfies(convertible_to(decltype_<float>())));
    /// static_assert(not example.satisfies(convertible_to(decltype_<example2>())));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for convertibility to
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is convertible to the type represented by `type`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto convertible_to(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_convertible_to(decltype_(decltype(type){}));
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is derived from the type
    /// represented by `type`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type derived from the type represented by `type`,
    /// determined as if by `decltype_(element).is_derived_from(decltype_(type))`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<int>{};
    /// struct example2 {};
    ///
    /// static_assert(example.satisfies(derived_from(decltype_<float>())));
    /// static_assert(not example.satisfies(derived_from(decltype_<example2>())));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for convertibility to
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is derived from the type represented by `type`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto derived_from(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_derived_from(decltype_(decltype(type){}));
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is a base of the type
    /// represented by `type`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type that is a base of the type represented by `type`,
    /// determined as if by `decltype_(element).is_base_of(decltype_(type))`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// constexpr auto example = decltype_<int>{};
    /// struct example2 {};
    ///
    /// static_assert(example.satisfies(base_of(decltype_<float>())));
    /// static_assert(not example.satisfies(base_of(decltype_<example2>())));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for convertibility to
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is a base of the type represented by `type`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto base_of(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_base_of(decltype_(decltype(type){}));
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type constructible from a pack of
    /// parameters of the types represented by `types...`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type constructible from the types represented by
    /// `types...`, determined as if by
    /// `decltype_(element).is_constructible_from(decltype_(types)...)`.
    ///
    /// # Requirements
    /// - `types...` must be instances of a `MetaType`s
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t(int, float, double);
    ///
    /// };
    /// constexpr auto example = decltype_<example_t>{};
    ///
    /// static_assert(example.satisfies(constructible_from(decltype_<int>(),
    ///                                                    decltype_<float>(),
    ///                                                    decltype_<double>())));
    /// static_assert(not example.satisfies(constructible_from(decltype_<float>())));
    /// @endcode
    ///
    /// @param types The `MetaType` representing the types of the argument pack to check
    /// for constructibility from
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is constructible from an argument pack of the types represented by
    /// `types...`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto constructible_from(MetaType auto... types) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_constructible_from(decltype_(decltype(types){})...);
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type constructible from a pack of
    /// parameters of the types `TTypes...`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type constructible from the raw types represented by
    /// `TTypes...`. That is, if any type `type` of `TTypes...` is a `MetaType`,
    /// the check will use the wrapped type of `type`, `typename type::type`
    /// (this guarantee is provided by `mpl::Type::is_constructible_from`).
    /// The constructibility is determined as if by
    /// `decltype_(element).is_constructible_from(types)`.
    ///
    /// # Requirements
    /// - `types` _must not_ be a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t(int, float, double);
    ///
    /// };
    /// constexpr auto example = decltype_<example_t>{};
    ///
    /// static_assert(example.satisfies(constructible_from(List<int, float, double>{})));
    /// static_assert(not example.satisfies(constructible_from(List<float>{})));
    /// @endcode
    ///
    /// @tparam TList The metaprogramming list type representing the types of the arguments
    /// to check constructibility with
    /// @tparam TTypes The types of the arguments to check constructibility with
    /// @param types The metaprogramming list representing the types of the arguments to
    /// check constructibility with
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is constructible from an argument pack of the types represented by
    /// `types`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    template<template<typename...> typename TList, typename... TTypes>
        requires(!MetaType<TList<TTypes...>>)
    [[nodiscard]] constexpr auto constructible_from(TList<TTypes...> types) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_constructible_from(decltype(types){});
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type `noexcept` constructible from a
    /// pack of parameters of the types represented by `types...`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type `noexcept` constructible from the types
    /// represented by `types...`, determined as if by
    /// `decltype_(element).is_noexcept_constructible_from(decltype_(types)...)`.
    ///
    /// # Requirements
    /// - `types...` must be instances of a `MetaType`s
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t(int, float, double) noexcept;
    ///     example_t(float, int);
    ///
    /// };
    /// constexpr auto example = decltype_<example_t>{};
    ///
    /// static_assert(example.satisfies(noexcept_constructible_from(decltype_<int>(),
    ///                                                             decltype_<float>(),
    ///                                                             decltype_<double>())));
    /// static_assert(not example.satisfies(noexcept_constructible_from(decltype_<float>())));
    /// static_assert(not example.satisfies(noexcept_constructible_from(decltype_<float>(),
    ///                                                                 decltype_<int>()))));
    /// @endcode
    ///
    /// @param types The `MetaType` representing the types of the argument pack to check
    /// for `noexcept` constructibility from
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is `noexcept` constructible from an argument pack of the types
    /// represented by `types...`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto noexcept_constructible_from(MetaType auto... types) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_noexcept_constructible_from(
                decltype_(decltype(types){})...);
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type `noexcept` constructible from a pack of
    /// parameters of the types `TTypes...`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type `noexcept` constructible from the raw types
    /// represented by `TTypes...`. That is, if any type `type` of `TTypes...`
    /// is a `MetaType`, the check will use the wrapped type of `type`,
    /// `typename type::type` (this guarantee is provided by
    /// `mpl::Type::is_constructible_from`).
    /// The constructibility is determined as if by
    /// `decltype_(element).is_noexcept_constructible_from(types)`.
    ///
    /// # Requirements
    /// - `types` _must not_ be a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t(int, float, double) noexcept;
    ///     example_t(float, int);
    ///
    /// };
    /// constexpr auto example = decltype_<example_t>{};
    ///
    /// static_assert(example.satisfies(noexcept_constructible_from(List<int, float, double>{})));
    /// static_assert(not example.satisfies(noexcept_constructible_from(List<float>{})));
    /// static_assert(not example.satisfies(noexcept_constructible_from(List<float, int>{}))));
    /// @endcode
    ///
    /// @tparam TList The metaprogramming list type representing the types of the arguments
    /// to check `noexcept` constructibility with
    /// @tparam TTypes The types of the arguments to check `noexcept` constructibility with
    /// @param types The metaprogramming list representing the types of the arguments to
    /// check `noexcept` constructibility with
    /// @return A metaprogramming predicate object to check that an argument represents
    /// a type that is `noexcept` constructible from an argument pack of the types
    /// represented by `types`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    template<template<typename...> typename TList, typename... TTypes>
        requires(!MetaType<TList<TTypes...>>)
    [[nodiscard]] constexpr auto noexcept_constructible_from(TList<TTypes...> types) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_noexcept_constructible_from(decltype(types){});
        };
    }

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is default constructible.
    ///
    /// Determines whether the represented type is default constructible,
    /// as if by `decltype_(type).is_default_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {};
    /// struct example3_t {
    ///     example3_t() = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<int>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(default_constructible));
    /// static_assert(example2.satisfies(default_constructible));
    /// static_assert(not example3.satisfies(default_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// default constructible
    /// @return whether the type represented by `type` is default constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto default_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_default_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` default constructible.
    ///
    /// Determines whether the represented type is `noexcept` default constructible,
    /// as if by `decltype_(type).is_noexcept_default_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t() noexcept;
    /// };
    /// struct example2_t {
    ///     example2_t() noexcept(false);
    /// }
    /// struct example3_t {
    ///     example3_t() = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(noexcept_default_constructible));
    /// static_assert(not example2.satisfies(noexcept_default_constructible));
    /// static_assert(not example3.satisfies(noexcept_default_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// `noexcept` default constructible
    /// @return whether the type represented by `type` is `noexcept` default constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_default_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_default_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is trivially default constructible.
    ///
    /// Determines whether the represented type is trivially default constructible
    /// as if by `decltype_(type).is_trivially_default_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    /// };
    /// struct example2_t {
    ///     example2_t();
    /// };
    /// struct example3_t {
    ///     example3_t() = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(trivially_default_constructible));
    /// static_assert(not example2.satisfies(trivially_default_constructible));
    /// static_assert(not example3.satisfies(trivially_default_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// trivially default constructible
    /// @return whether the type represented by `type` is trivially default constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto trivially_default_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_trivially_default_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is copy constructible.
    ///
    /// Determines whether the represented type is copy constructible,
    /// as if by `decltype_(type).is_copy_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {};
    /// struct example3_t {
    ///     example3_t(const example3_t&) = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<int>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(copy_constructible));
    /// static_assert(example2.satisfies(copy_constructible));
    /// static_assert(not example3.satisfies(copy_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// copy constructible
    /// @return whether the type represented by `type` is copy constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto copy_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_copy_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` copy constructible.
    ///
    /// Determines whether the represented type is `noexcept` copy constructible,
    /// as if by `decltype_(type).is_noexcept_copy_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t(const example_t&) noexcept;
    /// };
    /// struct example2_t {
    ///     example2_t(const example2_t&) noexcept(false);
    /// }
    /// struct example3_t {
    ///     example3_t(const example3_t&) = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(noexcept_copy_constructible));
    /// static_assert(not example2.satisfies(noexcept_copy_constructible));
    /// static_assert(not example3.satisfies(noexcept_copy_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// `noexcept` copy constructible
    /// @return whether the type represented by `type` is `noexcept` copy constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_copy_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_copy_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is trivially copy constructible.
    ///
    /// Determines whether the represented type is trivially copy constructible
    /// as if by `decltype_(type).is_trivially_copy_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    /// };
    /// struct example2_t {
    ///     example2_t(const example2_t&);
    /// };
    /// struct example3_t {
    ///     example3_t(const example3_t&) = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(trivially_copy_constructible));
    /// static_assert(not example2.satisfies(trivially_copy_constructible));
    /// static_assert(not example3.satisfies(trivially_copy_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// trivially copy constructible
    /// @return whether the type represented by `type` is trivially copy constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto trivially_copy_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_trivially_copy_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is move constructible.
    ///
    /// Determines whether the represented type is move constructible,
    /// as if by `decltype_(type).is_move_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {};
    /// struct example3_t {
    ///     example3_t(example3_t&&) = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<int>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(move_constructible));
    /// static_assert(example2.satisfies(move_constructible));
    /// static_assert(not example3.satisfies(move_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// move constructible
    /// @return whether the type represented by `type` is move constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto move_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_move_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` move constructible.
    ///
    /// Determines whether the represented type is `noexcept` move constructible,
    /// as if by `decltype_(type).is_noexcept_move_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     example_t(example_t&&) noexcept;
    /// };
    /// struct example2_t {
    ///     example2_t(example2_t&&) noexcept(false);
    /// }
    /// struct example3_t {
    ///     example3_t(example3_t&&) = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(noexcept_move_constructible));
    /// static_assert(not example2.satisfies(noexcept_move_constructible));
    /// static_assert(not example3.satisfies(noexcept_move_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// `noexcept` move constructible
    /// @return whether the type represented by `type` is `noexcept` move constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_move_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_move_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is trivially move constructible.
    ///
    /// Determines whether the represented type is trivially move constructible
    /// as if by `decltype_(type).is_trivially_move_constructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    /// };
    /// struct example2_t {
    ///     example2_t(example2_t&&);
    /// };
    /// struct example3_t {
    ///     example3_t(example3_t&&) = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(trivially_move_constructible));
    /// static_assert(not example2.satisfies(trivially_move_constructible));
    /// static_assert(not example3.satisfies(trivially_move_constructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// trivially move constructible
    /// @return whether the type represented by `type` is trivially move constructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto trivially_move_constructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_trivially_move_constructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is copy assignable.
    ///
    /// Determines whether the represented type is copy assignable,
    /// as if by `decltype_(type).is_copy_assignable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {};
    /// struct example3_t {
    ///     auto operator=(const example3_t&) -> example3_t& = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<int>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(copy_assignable));
    /// static_assert(example2.satisfies(copy_assignable));
    /// static_assert(not example3.satisfies(copy_assignable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// copy assignable
    /// @return whether the type represented by `type` is copy assignable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto copy_assignable = [](MetaType auto type) noexcept {
        return decltype_(type).is_copy_assignable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` copy assignable.
    ///
    /// Determines whether the represented type is `noexcept` copy assignable,
    /// as if by `decltype_(type).is_noexcept_copy_assignable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     auto operator=(const example_t&) noexcept -> example_t&;
    /// };
    /// struct example2_t {
    ///     auto operator=(const example2_t&) noexcept(false) -> example2_t&;
    /// }
    /// struct example3_t {
    ///     auto operator=(const example3_t&) -> example3_t& = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(noexcept_copy_assignable));
    /// static_assert(not example2.satisfies(noexcept_copy_assignable));
    /// static_assert(not example3.satisfies(noexcept_copy_assignable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// `noexcept` copy assignable
    /// @return whether the type represented by `type` is `noexcept` copy assignable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_copy_assignable = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_copy_assignable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is trivially copy assignable.
    ///
    /// Determines whether the represented type is trivially copy assignable
    /// as if by `decltype_(type).is_trivially_copy_assignable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    /// };
    /// struct example2_t {
    ///     auto operator=(const example2_t&) -> example2_t&;
    /// };
    /// struct example3_t {
    ///     auto operator=(const example3_t&) -> example3_t& = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(trivially_copy_assignable));
    /// static_assert(not example2.satisfies(trivially_copy_assignable));
    /// static_assert(not example3.satisfies(trivially_copy_assignable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// trivially copy assignable
    /// @return whether the type represented by `type` is trivially copy assignable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto trivially_copy_assignable = [](MetaType auto type) noexcept {
        return decltype_(type).is_trivially_copy_assignable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is move assignable.
    ///
    /// Determines whether the represented type is move assignable,
    /// as if by `decltype_(type).is_move_assignable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {};
    /// struct example3_t {
    ///     auto operator=(example3_t&&) -> example3_t& = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<int>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(move_assignable));
    /// static_assert(example2.satisfies(move_assignable));
    /// static_assert(not example3.satisfies(move_assignable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// move assignable
    /// @return whether the type represented by `type` is move assignable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto move_assignable = [](MetaType auto type) noexcept {
        return decltype_(type).is_move_assignable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` move assignable.
    ///
    /// Determines whether the represented type is `noexcept` move assignable,
    /// as if by `decltype_(type).is_noexcept_move_assignable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     auto operator=(example_t&&) noexcept -> example_t&;
    /// };
    /// struct example2_t {
    ///     auto operator=(example2_t&&) noexcept(false) -> example2_t&;
    /// }
    /// struct example3_t {
    ///     auto operator=(example3_t&&) -> example3_t& = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(noexcept_move_assignable));
    /// static_assert(not example2.satisfies(noexcept_move_assignable));
    /// static_assert(not example3.satisfies(noexcept_move_assignable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// `noexcept` move assignable
    /// @return whether the type represented by `type` is `noexcept` move assignable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_move_assignable = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_move_assignable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is trivially move assignable.
    ///
    /// Determines whether the represented type is trivially move assignable
    /// as if by `decltype_(type).is_trivially_move_assignable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    /// };
    /// struct example2_t {
    ///     auto operator=(example2_t&&) -> example2_t&;
    /// };
    /// struct example3_t {
    ///     auto operator=(example3_t&&) -> example3_t& = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(trivially_move_assignable));
    /// static_assert(not example2.satisfies(trivially_move_assignable));
    /// static_assert(not example3.satisfies(trivially_move_assignable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// trivially move assignable
    /// @return whether the type represented by `type` is trivially move assignable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto trivially_move_assignable = [](MetaType auto type) noexcept {
        return decltype_(type).is_trivially_move_assignable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is destructible.
    ///
    /// Determines whether the represented type is destructible,
    /// as if by `decltype_(type).is_destructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {};
    /// struct example3_t {
    ///     ~example3_t() = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<int>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(destructible));
    /// static_assert(example2.satisfies(destructible));
    /// static_assert(not example3.satisfies(destructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// destructible
    /// @return whether the type represented by `type` is destructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto destructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_destructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` destructible.
    ///
    /// Determines whether the represented type is `noexcept` destructible,
    /// as if by `decltype_(type).is_noexcept_destructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     ~example_t() noexcept;
    /// };
    /// struct example2_t {
    ///     ~example2_t() noexcept(false);
    /// }
    /// struct example3_t {
    ///     ~example3_t() = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(noexcept_destructible));
    /// static_assert(not example2.satisfies(noexcept_destructible));
    /// static_assert(not example3.satisfies(noexcept_destructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// `noexcept` destructible
    /// @return whether the type represented by `type` is `noexcept` destructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_destructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_destructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is trivially destructible.
    ///
    /// Determines whether the represented type is trivially destructible
    /// as if by `decltype_(type).is_trivially_destructible()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    /// };
    /// struct example2_t {
    ///     ~example2_t();
    /// };
    /// struct example3_t {
    ///     ~example3_t() = delete;
    /// };
    /// constexpr auto example1 = decltype_<example_t>{};
    /// constexpr auto example2 = decltype_<example2_t>{};
    /// constexpr auto example3 = decltype_<example3_t>{};
    ///
    /// static_assert(example1.satisfies(trivially_destructible));
    /// static_assert(not example2.satisfies(trivially_destructible));
    /// static_assert(not example3.satisfies(trivially_destructible));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check that is
    /// trivially destructible
    /// @return whether the type represented by `type` is trivially destructible
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto trivially_destructible = [](MetaType auto type) noexcept {
        return decltype_(type).is_trivially_destructible();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is swappable.
    ///
    /// A type, `type` is swappable if , given `lhs` and `rhs` that are
    /// both `type&`, `std::swap(lhs, rhs)` is well-formed.
    ///
    /// Determines whether the represented type is swappable
    /// as if by `decltype_(type).is_swappable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     friend void swap(example_t&, example_t&);
    /// };
    /// struct example2_t {
    ///     friend void swap(example2_t&, example2_t&) = delete;
    /// };
    ///
    /// static_assert(example_t.satisfies(swappable));
    /// static_assert(not example2_t.satisfies(swappable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for swappability
    /// @return whether the type represented by `type` is swappable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto swappable = [](MetaType auto type) noexcept {
        return decltype_(type).is_swappable();
    };

    /// @brief Metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` swappable.
    ///
    /// A type, `type` is `noexcept` swappable if , given `lhs` and `rhs`
    /// that are both `type&`, `std::swap(lhs, rhs)` is well-formed and `noexcept`.
    ///
    /// Determines whether the represented type is `noexcept` swappable
    /// as if by `decltype_(type).is_noexcept_swappable()`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     friend void swap(example_t&, example_t&) noexcept;
    /// };
    /// struct example2_t {
    ///     friend void swap(example2_t&, example2_t&) noexcept(false);
    /// };
    /// struct example3_t {
    ///     friend void swap(example3_t&, example3_t&) = delete;
    /// };
    ///
    /// static_assert(example_t.satisfies(swappable));
    /// static_assert(not example2_t.satisfies(swappable));
    /// static_assert(not example3_t.satisfies(swappable));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for
    /// `noexcept` swappability
    /// @return whether the type represented by `type` is `noexcept` swappable
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    constexpr auto noexcept_swappable = [](MetaType auto type) noexcept {
        return decltype_(type).is_noexcept_swappable();
    };

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is swappable with the type
    /// represented by `type`.
    ///
    /// A type, `type1`, is swappable with another type, `type2`, if ,
    /// given `lhs` and `rhs` that are `type1&` and `type2&`, respectively,
    /// `std::swap(lhs, rhs)` and `std::swap(rhs, lhs)` are both well-formed.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent to
    /// `constexpr operator()(MetaType auto arg) noexcept`, that when invoked, returns
    /// whether `arg` represents a type swappable with the type represented by `type`,
    /// determined as if by `decltype_(element).is_swappable_with(decltype_(type))`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     friend void swap(example_t&, example_t&);
    ///     friend void swap(example_t&, int&);
    ///     friend void swap(int&, example_t&);
    /// };
    /// struct example2_t {
    ///     friend void swap(example2_t&, int&) = delete;
    /// };
    ///
    /// static_assert(example_t.satisfies(swappable_with(decltype_<int>())));
    /// static_assert(example_t.satisfies(swappable_with(decltype_<example_t>())));
    /// static_assert(not example2_t.satisfies(swappable_with(decltype_<int>())));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for
    /// swappability with
    /// @return A metaprogramming predicate object used to check that an argument
    /// represents a type that is swappable with the type represented by `type`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto swappable_with(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_swappable_with(decltype_(decltype(type){}));
        };
    }

    /// @brief Returns a metaprogramming predicate object used to query whether a
    /// `MetaType` argument represents a type that is `noexcept` swappable
    /// with the type represented by `type`.
    ///
    /// A type, `type1`, is `noexcept` swappable with another type, `type2`, if ,
    /// given `lhs` and `rhs` that are `type1&` and `type2&`, respectively,
    /// `std::swap(lhs, rhs)` and `std::swap(rhs, lhs)` are both well-formed
    /// and `noexcept`.
    ///
    /// The returned metaprogramming predicate object has call operator equivalent
    /// to `constexpr operator()(MetaType auto arg) noexcept`, that when invoked,
    /// returns whether `arg` represents a type `noexcept` swappable with the type
    /// represented by `type`, determined as if by
    /// `decltype_(element).is_noexcept_swappable_with(decltype_(type))`.
    ///
    /// # Requirements
    /// - `type` must be an instance of a `MetaType`
    ///
    /// # Example
    /// @code {.cpp}
    /// struct example_t {
    ///     friend void swap(example_t&, example_t&) noexcept(false);
    ///     friend void swap(example_t&, int&) noexcept;
    ///     friend void swap(int&, example_t&) noexcept;
    /// };
    /// struct example2_t {
    ///     friend void swap(example2_t&, int&) = delete;
    /// };
    ///
    /// static_assert(example_t.satisfies(noexcept_swappable_with(decltype_<int>())));
    /// static_assert(not example_t.satisfies(noexcept_swappable_with(decltype_<example_t>())));
    /// static_assert(not example2_t.satisfies(noexcept_swappable_with(decltype_<int>())));
    /// @endcode
    ///
    /// @param type The `MetaType` representing the type to check for
    /// `noexcept` swappability with
    /// @return A metaprogramming predicate object used to check that an argument
    /// represents a type that is `noexcept` swappable with the type represented
    /// by `type`
    /// @ingroup metapredicates
    /// @headerfile hyperion/mpl/metapredicates.h
    [[nodiscard]] constexpr auto noexcept_swappable_with(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return decltype_(element).is_noexcept_swappable_with(decltype_(decltype(type){}));
        };
    }
} // namespace hyperion::mpl

    // NOLINTNEXTLINE(misc-header-include-cycle)
    #include <hyperion/mpl/pair.h>
  // NOLINTNEXTLINE(misc-header-include-cycle)
    #include <hyperion/mpl/list.h>

namespace hyperion::mpl::_test::metapredicates {

    static_assert(decltype_<int>().satisfies(equal_to(decltype_<int>())),
                  "hyperion::mpl::equal_to predicate test case 1 (failing)");
    static_assert(not decltype_<const int>().satisfies(equal_to(decltype_<int>())),
                  "hyperion::mpl::equal_to predicate test case 2 (failing)");
    static_assert(not decltype_<int>().satisfies(equal_to(1_value)),
                  "hyperion::mpl::equal_to predicate test case 3 (failing)");
    static_assert(not decltype_<int>().satisfies(equal_to(Pair<int, int>{})),
                  "hyperion::mpl::equal_to predicate test case 4 (failing)");

    static_assert(not decltype_<int>().satisfies(not_equal_to(decltype_<int>())),
                  "hyperion::mpl::not_equal_to predicate test case 1 (failing)");
    static_assert(decltype_<const int>().satisfies(not_equal_to(decltype_<int>())),
                  "hyperion::mpl::not_equal_to predicate test case 2 (failing)");
    static_assert(decltype_<int>().satisfies(not_equal_to(1_value)),
                  "hyperion::mpl::not_equal_to predicate test case 3 (failing)");
    static_assert(decltype_<int>().satisfies(not_equal_to(Pair<int, int>{})),
                  "hyperion::mpl::not_equal_to predicate test case 4 (failing)");

    static_assert((1_value).satisfies(less_than(2_value)),
                  "hyperion::mpl::less_than predicate test case 1 (failing)");
    static_assert(not(1_value).satisfies(less_than(0_value)),
                  "hyperion::mpl::less_than predicate test case 2 (failing)");

    static_assert((1_value).satisfies(less_than_or_equal_to(2_value)),
                  "hyperion::mpl::less_than predicate test case 1 (failing)");
    static_assert((2_value).satisfies(less_than_or_equal_to(2_value)),
                  "hyperion::mpl::less_than predicate test case 2 (failing)");
    static_assert(not(1_value).satisfies(less_than_or_equal_to(0_value)),
                  "hyperion::mpl::less_than predicate test case 3 (failing)");

    static_assert((2_value).satisfies(greater_than(1_value)),
                  "hyperion::mpl::greater_than predicate test case 1 (failing)");
    static_assert(not(0_value).satisfies(greater_than(1_value)),
                  "hyperion::mpl::greater_than predicate test case 2 (failing)");

    static_assert((2_value).satisfies(greater_than_or_equal_to(1_value)),
                  "hyperion::mpl::greater_than predicate test case 1 (failing)");
    static_assert((2_value).satisfies(greater_than_or_equal_to(2_value)),
                  "hyperion::mpl::greater_than predicate test case 2 (failing)");
    static_assert(not(0_value).satisfies(greater_than_or_equal_to(1_value)),
                  "hyperion::mpl::greater_than predicate test case 3 (failing)");

    static_assert(decltype_<int>().satisfies(is(decltype_<int>())),
                  "hyperion::mpl::is predicate test case 1 (failing)");
    static_assert(not decltype_<const int>().satisfies(is(decltype_<int>())),
                  "hyperion::mpl::is predicate test case 2 (failing)");

    static_assert(decltype_<int>().satisfies(qualification_of(decltype_<int>())),
                  "hyperion::mpl::qualification_of predicate test case 1 (failing)");
    static_assert(decltype_<const int>().satisfies(qualification_of(decltype_<int>())),
                  "hyperion::mpl::qualification_of predicate test case 2 (failing)");
    static_assert(decltype_<const int&>().satisfies(qualification_of(decltype_<int>())),
                  "hyperion::mpl::qualification_of predicate test case 3 (failing)");
    static_assert(not decltype_<float>().satisfies(qualification_of(decltype_<int>())),
                  "hyperion::mpl::qualification_of predicate test case 4 (failing)");

    static_assert(decltype_<const int>().satisfies(is_const),
                  "hyperion::mpl::is_const predicat test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_const),
                  "hyperion::mpl::is_const predicat test case 2 (failing)");

    static_assert(decltype_<int&>().satisfies(is_lvalue_reference),
                  "hyperion::mpl::is_lvalue_reference predicat test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_lvalue_reference),
                  "hyperion::mpl::is_lvalue_reference predicat test case 2 (failing)");

    static_assert(decltype_<int&&>().satisfies(is_rvalue_reference),
                  "hyperion::mpl::is_rvalue_reference predicat test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_rvalue_reference),
                  "hyperion::mpl::is_rvalue_reference predicat test case 2 (failing)");

    static_assert(decltype_<volatile int>().satisfies(is_volatile),
                  "hyperion::mpl::is_volatile predicat test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_volatile),
                  "hyperion::mpl::is_volatile predicat test case 2 (failing)");

    struct not_convertible { };

    static_assert(decltype_<int>().satisfies(convertible_to(decltype_<float>())),
                  "hyperion::mpl::convertible_to predicate test case 1 (failing)");
    static_assert(not decltype_<not_convertible>().satisfies(convertible_to(decltype_<float>())),
                  "hyperion::mpl::convertible_to predicate test case 2 (failing)");

    struct base { };
    struct derived : base { };

    static_assert(decltype_<derived>().satisfies(derived_from(decltype_<base>())),
                  "hyperion::mpl::derived_from predicate test case 1 (failing)");
    static_assert(not decltype_<base>().satisfies(derived_from(decltype_<base>())),
                  "hyperion::mpl::derived_from predicate test case 2 (failing)");
    static_assert(not decltype_<int>().satisfies(derived_from(decltype_<base>())),
                  "hyperion::mpl::derived_from predicate test case 3 (failing)");

    static_assert(decltype_<base>().satisfies(base_of(decltype_<derived>())),
                  "hyperion::mpl::base_of predicate test case 1 (failing)");
    static_assert(not decltype_<derived>().satisfies(base_of(decltype_<derived>())),
                  "hyperion::mpl::base_of predicate test case 2 (failing)");
    static_assert(not decltype_<int>().satisfies(base_of(decltype_<derived>())),
                  "hyperion::mpl::base_of predicate test case 3 (failing)");

    struct constructible {
        constructible(int, float, double);
        constructible(float, double, int) noexcept;
    };

    static_assert(decltype_<constructible>().satisfies(constructible_from(decltype_<int>(),
                                                                          decltype_<float>(),
                                                                          decltype_<double>())),
                  "hyperion::mpl::constructible_from predicate test case 1 (failing)");
    static_assert(decltype_<constructible>().satisfies(constructible_from(decltype_<float>(),
                                                                          decltype_<double>(),
                                                                          decltype_<int>())),
                  "hyperion::mpl::constructible_from predicate test case 2 (failing)");
    static_assert(not decltype_<constructible>().satisfies(constructible_from(decltype_<int>())),
                  "hyperion::mpl::constructible_from predicate test case 3 (failing)");
    static_assert(
        decltype_<constructible>().satisfies(constructible_from(List<int, float, double>{})),
        "hyperion::mpl::constructible_from predicate test case 4 (failing)");
    static_assert(
        decltype_<constructible>().satisfies(constructible_from(List<float, double, int>{})),
        "hyperion::mpl::constructible_from predicate test case 5 (failing)");
    static_assert(not decltype_<constructible>().satisfies(constructible_from(List<int>{})),
                  "hyperion::mpl::constructible_from predicate test case 6 (failing)");

    static_assert(
        decltype_<constructible>().satisfies(
            noexcept_constructible_from(decltype_<float>(), decltype_<double>(), decltype_<int>())),
        "hyperion::mpl::noexcept_constructible_from predicate test case 1 (failing)");
    static_assert(
        not decltype_<constructible>().satisfies(
            noexcept_constructible_from(decltype_<int>(), decltype_<float>(), decltype_<double>())),
        "hyperion::mpl::noexcept_constructible_from predicate test case 2 (failing)");
    static_assert(
        not decltype_<constructible>().satisfies(noexcept_constructible_from(decltype_<int>())),
        "hyperion::mpl::noexcept_constructible_from predicate test case 3 (failing)");
    static_assert(decltype_<constructible>().satisfies(
                      noexcept_constructible_from(List<float, double, int>{})),
                  "hyperion::mpl::noexcept_constructible_from predicate test case 4 (failing)");
    static_assert(not decltype_<constructible>().satisfies(
                      noexcept_constructible_from(List<int, float, double>{})),
                  "hyperion::mpl::noexcept_constructible_from predicate test case 5 (failing)");
    static_assert(
        not decltype_<constructible>().satisfies(noexcept_constructible_from(List<int>{})),
        "hyperion::mpl::noexcept_constructible_from predicate test case 6 (failing)");

    struct not_default_constructible {
        not_default_constructible() = delete;
    };

    struct default_constructible {
        default_constructible();
    };

    struct noexcept_default_constructible {
        noexcept_default_constructible() noexcept;
    };

    struct trivially_default_constructible {
        trivially_default_constructible() noexcept(false) = default;
    };

    static_assert(decltype_<int>().satisfies(mpl::default_constructible),
                  "hyperion::mpl::default_constructible predicate test case 1 (failing)");
    static_assert(decltype_<default_constructible>().satisfies(mpl::default_constructible),
                  "hyperion::mpl::default_constructible predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_default_constructible>().satisfies(mpl::default_constructible),
                  "hyperion::mpl::default_constructible predicate test case 3 (failing)");
    static_assert(
        decltype_<trivially_default_constructible>().satisfies(mpl::default_constructible),
        "hyperion::mpl::default_constructible predicate test case 4 (failing)");
    static_assert(not decltype_<not_default_constructible>().satisfies(mpl::default_constructible),
                  "hyperion::mpl::default_constructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_default_constructible),
                  "hyperion::mpl::noexcept_default_constructible predicate test case 1 (failing)");
    static_assert(
        not decltype_<default_constructible>().satisfies(mpl::noexcept_default_constructible),
        "hyperion::mpl::noexcept_default_constructible predicate test case 2 (failing)");
    static_assert(
        decltype_<noexcept_default_constructible>().satisfies(mpl::noexcept_default_constructible),
        "hyperion::mpl::noexcept_default_constructible predicate test case 3 (failing)");
    static_assert(
        // MSVC and GCC override our `noexcept(false) above`
        HYPERION_PLATFORM_COMPILER_IS_GCC ?
            static_cast<bool>(decltype_<trivially_default_constructible>().satisfies(
                mpl::noexcept_default_constructible)) :
            not decltype_<trivially_default_constructible>().satisfies(
                mpl::noexcept_default_constructible),
        "hyperion::mpl::noexcept_default_constructible predicate test case 4 (failing)");
    static_assert(
        not decltype_<not_default_constructible>().satisfies(mpl::noexcept_default_constructible),
        "hyperion::mpl::noexcept_default_constructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::trivially_default_constructible),
                  "hyperion::mpl::trivially_default_constructible predicate test case 1 (failing)");
    static_assert(
        not decltype_<default_constructible>().satisfies(mpl::trivially_default_constructible),
        "hyperion::mpl::trivially_default_constructible predicate test case 2 (failing)");
    static_assert(not decltype_<noexcept_default_constructible>().satisfies(
                      mpl::trivially_default_constructible),
                  "hyperion::mpl::trivially_default_constructible predicate test case 3 (failing)");
    static_assert(decltype_<trivially_default_constructible>().satisfies(
                      mpl::trivially_default_constructible),
                  "hyperion::mpl::trivially_default_constructible predicate test case 4 (failing)");
    static_assert(
        not decltype_<not_default_constructible>().satisfies(mpl::trivially_default_constructible),
        "hyperion::mpl::trivially_default_constructible predicate test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_copy_constructible {
        not_copy_constructible(const not_copy_constructible&) = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct copy_constructible {
        copy_constructible(const copy_constructible&);
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_copy_constructible {
        noexcept_copy_constructible(const noexcept_copy_constructible&) noexcept;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct trivially_copy_constructible {
        trivially_copy_constructible(const trivially_copy_constructible&) noexcept(false) = default;
    };

    static_assert(decltype_<int>().satisfies(mpl::copy_constructible),
                  "hyperion::mpl::copy_constructible predicate test case 1 (failing)");
    static_assert(decltype_<copy_constructible>().satisfies(mpl::copy_constructible),
                  "hyperion::mpl::copy_constructible predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_constructible>().satisfies(mpl::copy_constructible),
                  "hyperion::mpl::copy_constructible predicate test case 3 (failing)");
    static_assert(decltype_<trivially_copy_constructible>().satisfies(mpl::copy_constructible),
                  "hyperion::mpl::copy_constructible predicate test case 4 (failing)");
    static_assert(not decltype_<not_copy_constructible>().satisfies(mpl::copy_constructible),
                  "hyperion::mpl::copy_constructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_copy_constructible),
                  "hyperion::mpl::noexcept_copy_constructible predicate test case 1 (failing)");
    static_assert(not decltype_<copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
                  "hyperion::mpl::noexcept_copy_constructible predicate test case 2 (failing)");
    static_assert(
        decltype_<noexcept_copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
        "hyperion::mpl::noexcept_copy_constructible predicate test case 3 (failing)");
    static_assert(
        // MSVC and GCC override our `noexcept(false) above`
        HYPERION_PLATFORM_COMPILER_IS_GCC ?
            static_cast<bool>(decltype_<trivially_copy_constructible>().satisfies(
                mpl::noexcept_copy_constructible)) :
            not decltype_<trivially_copy_constructible>().satisfies(
                mpl::noexcept_copy_constructible),
        "hyperion::mpl::noexcept_copy_constructible predicate test case 4 (failing)");
    static_assert(
        not decltype_<not_copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
        "hyperion::mpl::noexcept_copy_constructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::trivially_copy_constructible),
                  "hyperion::mpl::trivially_copy_constructible predicate test case 1 (failing)");
    static_assert(not decltype_<copy_constructible>().satisfies(mpl::trivially_copy_constructible),
                  "hyperion::mpl::trivially_copy_constructible predicate test case 2 (failing)");
    static_assert(
        not decltype_<noexcept_copy_constructible>().satisfies(mpl::trivially_copy_constructible),
        "hyperion::mpl::trivially_copy_constructible predicate test case 3 (failing)");
    static_assert(
        decltype_<trivially_copy_constructible>().satisfies(mpl::trivially_copy_constructible),
        "hyperion::mpl::trivially_copy_constructible predicate test case 4 (failing)");
    static_assert(
        not decltype_<not_copy_constructible>().satisfies(mpl::trivially_copy_constructible),
        "hyperion::mpl::trivially_copy_constructible predicate test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_move_constructible {
        not_move_constructible(not_move_constructible&&) = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct move_constructible {
        // NOLINTNEXTLINE(*-noexcept-move*)
        move_constructible(move_constructible&&);
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_move_constructible {
        noexcept_move_constructible(noexcept_move_constructible&&) noexcept;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct trivially_move_constructible {
        // NOLINTNEXTLINE(*-noexcept-move*)
        trivially_move_constructible(trivially_move_constructible&&) noexcept(false) = default;
    };

    static_assert(decltype_<int>().satisfies(mpl::move_constructible),
                  "hyperion::mpl::move_constructible predicate test case 1 (failing)");
    static_assert(decltype_<move_constructible>().satisfies(mpl::move_constructible),
                  "hyperion::mpl::move_constructible predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_move_constructible>().satisfies(mpl::move_constructible),
                  "hyperion::mpl::move_constructible predicate test case 3 (failing)");
    static_assert(decltype_<trivially_move_constructible>().satisfies(mpl::move_constructible),
                  "hyperion::mpl::move_constructible predicate test case 4 (failing)");
    static_assert(not decltype_<not_move_constructible>().satisfies(mpl::move_constructible),
                  "hyperion::mpl::move_constructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_move_constructible),
                  "hyperion::mpl::noexcept_move_constructible predicate test case 1 (failing)");
    static_assert(not decltype_<move_constructible>().satisfies(mpl::noexcept_move_constructible),
                  "hyperion::mpl::noexcept_move_constructible predicate test case 2 (failing)");
    static_assert(
        decltype_<noexcept_move_constructible>().satisfies(mpl::noexcept_move_constructible),
        "hyperion::mpl::noexcept_move_constructible predicate test case 3 (failing)");
    static_assert(
        // MSVC and GCC override our `noexcept(false) above`
        HYPERION_PLATFORM_COMPILER_IS_GCC ?
            static_cast<bool>(decltype_<trivially_move_constructible>().satisfies(
                mpl::noexcept_move_constructible)) :
            not decltype_<trivially_move_constructible>().satisfies(
                mpl::noexcept_move_constructible),
        "hyperion::mpl::noexcept_move_constructible predicate test case 4 (failing)");
    static_assert(
        not decltype_<not_move_constructible>().satisfies(mpl::noexcept_move_constructible),
        "hyperion::mpl::noexcept_move_constructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::trivially_move_constructible),
                  "hyperion::mpl::trivially_move_constructible predicate test case 1 (failing)");
    static_assert(not decltype_<move_constructible>().satisfies(mpl::trivially_move_constructible),
                  "hyperion::mpl::trivially_move_constructible predicate test case 2 (failing)");
    static_assert(
        not decltype_<noexcept_move_constructible>().satisfies(mpl::trivially_move_constructible),
        "hyperion::mpl::trivially_move_constructible predicate test case 3 (failing)");
    static_assert(
        decltype_<trivially_move_constructible>().satisfies(mpl::trivially_move_constructible),
        "hyperion::mpl::trivially_move_constructible predicate test case 4 (failing)");
    static_assert(
        not decltype_<not_move_constructible>().satisfies(mpl::trivially_move_constructible),
        "hyperion::mpl::trivially_move_constructible predicate test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_copy_assignable {
        auto operator=(const not_copy_assignable&) -> not_copy_assignable& = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct copy_assignable {
        auto operator=(const copy_assignable&) -> copy_assignable&;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_copy_assignable {
        auto operator=(const noexcept_copy_assignable&) noexcept -> noexcept_copy_assignable&;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct trivially_copy_assignable {
        auto operator=(const trivially_copy_assignable&) noexcept(false)
            -> trivially_copy_assignable& = default;
    };

    static_assert(decltype_<int>().satisfies(mpl::copy_assignable),
                  "hyperion::mpl::copy_assignable predicate test case 1 (failing)");
    static_assert(decltype_<copy_assignable>().satisfies(mpl::copy_assignable),
                  "hyperion::mpl::copy_assignable predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_assignable>().satisfies(mpl::copy_assignable),
                  "hyperion::mpl::copy_assignable predicate test case 3 (failing)");
    static_assert(decltype_<trivially_copy_assignable>().satisfies(mpl::copy_assignable),
                  "hyperion::mpl::copy_assignable predicate test case 4 (failing)");
    static_assert(not decltype_<not_copy_assignable>().satisfies(mpl::copy_assignable),
                  "hyperion::mpl::copy_assignable predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_copy_assignable),
                  "hyperion::mpl::noexcept_copy_assignable predicate test case 1 (failing)");
    static_assert(not decltype_<copy_assignable>().satisfies(mpl::noexcept_copy_assignable),
                  "hyperion::mpl::noexcept_copy_assignable predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_copy_assignable>().satisfies(mpl::noexcept_copy_assignable),
                  "hyperion::mpl::noexcept_copy_assignable predicate test case 3 (failing)");
    static_assert(
        // GCC overrides our `noexcept(false) above`
        HYPERION_PLATFORM_COMPILER_IS_GCC ?
            decltype_<trivially_copy_assignable>().satisfies(mpl::noexcept_copy_assignable) :
            static_cast<bool>(not decltype_<trivially_copy_assignable>().satisfies(
                mpl::noexcept_copy_assignable)),
        "hyperion::mpl::noexcept_copy_assignable predicate test case 4 (failing)");
    static_assert(not decltype_<not_copy_assignable>().satisfies(mpl::noexcept_copy_assignable),
                  "hyperion::mpl::noexcept_copy_assignable predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::trivially_copy_assignable),
                  "hyperion::mpl::trivially_copy_assignable predicate test case 1 (failing)");
    static_assert(not decltype_<copy_assignable>().satisfies(mpl::trivially_copy_assignable),
                  "hyperion::mpl::trivially_copy_assignable predicate test case 2 (failing)");
    static_assert(
        not decltype_<noexcept_copy_assignable>().satisfies(mpl::trivially_copy_assignable),
        "hyperion::mpl::trivially_copy_assignable predicate test case 3 (failing)");
    static_assert(decltype_<trivially_copy_assignable>().satisfies(mpl::trivially_copy_assignable),
                  "hyperion::mpl::trivially_copy_assignable predicate test case 4 (failing)");
    static_assert(not decltype_<not_copy_assignable>().satisfies(mpl::trivially_copy_assignable),
                  "hyperion::mpl::trivially_copy_assignable predicate test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_move_assignable {
        auto operator=(not_move_assignable&&) -> not_move_assignable& = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct move_assignable {
        // NOLINTNEXTLINE(*-noexcept-move*)
        auto operator=(move_assignable&&) -> move_assignable&;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_move_assignable {
        auto operator=(noexcept_move_assignable&&) noexcept -> noexcept_move_assignable&;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct trivially_move_assignable {
        // NOLINTNEXTLINE(*-noexcept-move*)
        auto operator=(trivially_move_assignable&&) noexcept(false)
            -> trivially_move_assignable& = default;
    };

    static_assert(decltype_<int>().satisfies(mpl::move_assignable),
                  "hyperion::mpl::move_assignable predicate test case 1 (failing)");
    static_assert(decltype_<move_assignable>().satisfies(mpl::move_assignable),
                  "hyperion::mpl::move_assignable predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_move_assignable>().satisfies(mpl::move_assignable),
                  "hyperion::mpl::move_assignable predicate test case 3 (failing)");
    static_assert(decltype_<trivially_move_assignable>().satisfies(mpl::move_assignable),
                  "hyperion::mpl::move_assignable predicate test case 4 (failing)");
    static_assert(not decltype_<not_move_assignable>().satisfies(mpl::move_assignable),
                  "hyperion::mpl::move_assignable predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_move_assignable),
                  "hyperion::mpl::noexcept_move_assignable predicate test case 1 (failing)");
    static_assert(not decltype_<move_assignable>().satisfies(mpl::noexcept_move_assignable),
                  "hyperion::mpl::noexcept_move_assignable predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_move_assignable>().satisfies(mpl::noexcept_move_assignable),
                  "hyperion::mpl::noexcept_move_assignable predicate test case 3 (failing)");
    static_assert(
        // GCC overrides our `noexcept(false) above`
        HYPERION_PLATFORM_COMPILER_IS_GCC ?
            decltype_<trivially_move_assignable>().satisfies(mpl::noexcept_move_assignable) :
            static_cast<bool>(not decltype_<trivially_move_assignable>().satisfies(
                mpl::noexcept_move_assignable)),
        "hyperion::mpl::noexcept_move_assignable predicate test case 4 (failing)");
    static_assert(not decltype_<not_move_assignable>().satisfies(mpl::noexcept_move_assignable),
                  "hyperion::mpl::noexcept_move_assignable predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::trivially_move_assignable),
                  "hyperion::mpl::trivially_move_assignable predicate test case 1 (failing)");
    static_assert(not decltype_<move_assignable>().satisfies(mpl::trivially_move_assignable),
                  "hyperion::mpl::trivially_move_assignable predicate test case 2 (failing)");
    static_assert(
        not decltype_<noexcept_move_assignable>().satisfies(mpl::trivially_move_assignable),
        "hyperion::mpl::trivially_move_assignable predicate test case 3 (failing)");
    static_assert(decltype_<trivially_move_assignable>().satisfies(mpl::trivially_move_assignable),
                  "hyperion::mpl::trivially_move_assignable predicate test case 4 (failing)");
    static_assert(not decltype_<not_move_assignable>().satisfies(mpl::trivially_move_assignable),
                  "hyperion::mpl::trivially_move_assignable predicate test case 5 (failing)");

    // NOLINTNEXTLINE(*-special-member-functions)
    struct not_destructible {
        ~not_destructible() = delete;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct destructible {
        ~destructible() noexcept(false);
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct noexcept_destructible {
        ~noexcept_destructible() noexcept;
    };

    // NOLINTNEXTLINE(*-special-member-functions)
    struct trivially_destructible {
        ~trivially_destructible() noexcept(false) = default;
    };

    static_assert(decltype_<int>().satisfies(mpl::destructible),
                  "hyperion::mpl::destructible predicate test case 1 (failing)");
    static_assert(decltype_<destructible>().satisfies(mpl::destructible),
                  "hyperion::mpl::destructible predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_destructible>().satisfies(mpl::destructible),
                  "hyperion::mpl::destructible predicate test case 3 (failing)");
    static_assert(decltype_<trivially_destructible>().satisfies(mpl::destructible),
                  "hyperion::mpl::destructible predicate test case 4 (failing)");
    static_assert(not decltype_<not_destructible>().satisfies(mpl::destructible),
                  "hyperion::mpl::destructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_destructible),
                  "hyperion::mpl::noexcept_destructible predicate test case 1 (failing)");
    static_assert(not decltype_<destructible>().satisfies(mpl::noexcept_destructible),
                  "hyperion::mpl::noexcept_destructible predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_destructible>().satisfies(mpl::noexcept_destructible),
                  "hyperion::mpl::noexcept_destructible predicate test case 3 (failing)");
    static_assert(
        // MSVC and GCC override our `noexcept(false) above`
        HYPERION_PLATFORM_COMPILER_IS_GCC ?
            static_cast<bool>(
                decltype_<trivially_destructible>().satisfies(mpl::noexcept_destructible)) :
            not decltype_<trivially_destructible>().satisfies(mpl::noexcept_destructible),
        "hyperion::mpl::noexcept_destructible predicate test case 4 (failing)");
    static_assert(not decltype_<not_destructible>().satisfies(mpl::noexcept_destructible),
                  "hyperion::mpl::noexcept_destructible predicate test case 5 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::trivially_destructible),
                  "hyperion::mpl::trivially_destructible predicate test case 1 (failing)");
    static_assert(not decltype_<destructible>().satisfies(mpl::trivially_destructible),
                  "hyperion::mpl::trivially_destructible predicate test case 2 (failing)");
    static_assert(not decltype_<noexcept_destructible>().satisfies(mpl::trivially_destructible),
                  "hyperion::mpl::trivially_destructible predicate test case 3 (failing)");
    static_assert(decltype_<trivially_destructible>().satisfies(mpl::trivially_destructible),
                  "hyperion::mpl::trivially_destructible predicate test case 4 (failing)");
    static_assert(not decltype_<not_destructible>().satisfies(mpl::trivially_destructible),
                  "hyperion::mpl::trivially_destructible predicate test case 5 (failing)");

    struct swappable {
        friend void swap(swappable& lhs, swappable& rhs) noexcept(false);
    };

    struct not_swappable {
        friend void swap(not_swappable& lhs, not_swappable& rhs) = delete;
    };

    struct noexcept_swappable {
        friend void swap(noexcept_swappable& lhs, noexcept_swappable& rhs) noexcept;
    };

    static_assert(decltype_<int>().satisfies(mpl::swappable),
                  "hyperion::mpl::swappable predicate test case 1 (failing)");
    static_assert(decltype_<swappable>().satisfies(mpl::swappable),
                  "hyperion::mpl::swappable predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_swappable>().satisfies(mpl::swappable),
                  "hyperion::mpl::swappable predicate test case 3 (failing)");
    static_assert(not decltype_<not_swappable>().satisfies(mpl::swappable),
                  "hyperion::mpl::swappable predicate test case 4 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_swappable),
                  "hyperion::mpl::noexcept_swappable predicate test case 1 (failing)");
    static_assert(decltype_<noexcept_swappable>().satisfies(mpl::noexcept_swappable),
                  "hyperion::mpl::noexcept_swappable predicate test case 2 (failing)");
    static_assert(not decltype_<swappable>().satisfies(mpl::noexcept_swappable),
                  "hyperion::mpl::noexcept_swappable predicate test case 3 (failing)");
    static_assert(not decltype_<not_swappable>().satisfies(mpl::noexcept_swappable),
                  "hyperion::mpl::noexcept_swappable predicate test case 4 (failing)");

    struct swappable_with {
        friend void swap(swappable_with& lhs, int& rhs) noexcept(false);
        friend void swap(int& lhs, swappable_with& rhs) noexcept(false);
    };

    struct noexcept_swappable_with {
        friend void swap(noexcept_swappable_with& lhs, int& rhs) noexcept;
        friend void swap(int& lhs, noexcept_swappable_with& rhs) noexcept;
    };

    static_assert(decltype_<int>().satisfies(mpl::swappable_with(decltype_<int>())),
                  "hyperion::mpl::swappable_with predicate test case 1 (failing)");
    static_assert(decltype_<swappable>().satisfies(mpl::swappable_with(decltype_<swappable>())),
                  "hyperion::mpl::swappable_with predicate test case 2 (failing)");
    static_assert(decltype_<noexcept_swappable>().satisfies(
                      mpl::swappable_with(decltype_<noexcept_swappable>())),
                  "hyperion::mpl::swappable_with predicate test case 3 (failing)");
    static_assert(
        not decltype_<not_swappable>().satisfies(mpl::swappable_with(decltype_<not_swappable>())),
        "hyperion::mpl::swappable_with predicate test case 4 (failing)");
    static_assert(decltype_<swappable_with>().satisfies(mpl::swappable_with(decltype_<int>())),
                  "hyperion::mpl::swappable_with predicate test case 5 (failing)");
    static_assert(
        decltype_<noexcept_swappable_with>().satisfies(mpl::swappable_with(decltype_<int>())),
        "hyperion::mpl::swappable_with predicate test case 6 (failing)");
    static_assert(not decltype_<not_swappable>().satisfies(mpl::swappable_with(decltype_<int>())),
                  "hyperion::mpl::swappable_with predicate test case 7 (failing)");

    static_assert(decltype_<int>().satisfies(mpl::noexcept_swappable_with(decltype_<int>())),
                  "hyperion::mpl::noexcept_swappable_with predicate test case 1 (failing)");
    static_assert(decltype_<noexcept_swappable>().satisfies(
                      mpl::noexcept_swappable_with(decltype_<noexcept_swappable>())),
                  "hyperion::mpl::noexcept_swappable_with predicate test case 2 (failing)");
    static_assert(
        not decltype_<swappable>().satisfies(mpl::noexcept_swappable_with(decltype_<swappable>())),
        "hyperion::mpl::noexcept_swappable_with predicate test case 3 (failing)");
    static_assert(not decltype_<not_swappable>().satisfies(
                      mpl::noexcept_swappable_with(decltype_<not_swappable>())),
                  "hyperion::mpl::noexcept_swappable_with predicate test case 4 (failing)");
    static_assert(decltype_<noexcept_swappable_with>().satisfies(
                      mpl::noexcept_swappable_with(decltype_<int>())),
                  "hyperion::mpl::noexcept_swappable_with predicate test case 5 (failing)");
    static_assert(
        not decltype_<swappable_with>().satisfies(mpl::noexcept_swappable_with(decltype_<int>())),
        "hyperion::mpl::noexcept_swappable_with predicate test case 6 (failing)");
    static_assert(
        not decltype_<not_swappable>().satisfies(mpl::noexcept_swappable_with(decltype_<int>())),
        "hyperion::mpl::noexcept_swappable_with predicate test case 7 (failing)");
} // namespace hyperion::mpl::_test::metapredicates

#endif // HYPERION_MPL_METAPREDICATES_H
