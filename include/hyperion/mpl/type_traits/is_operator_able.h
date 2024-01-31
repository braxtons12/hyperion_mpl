/// @file is_operator_able.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming type traits to determine if a type (or types)
/// support an operator
/// @version 0.1
/// @date 2024-01-27
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

#ifndef HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_H
#define HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_H

#include <concepts>
#include <hyperion/mpl/concepts/operator_able.h>
#include <hyperion/platform/def.h>
#include <type_traits>

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

/// @ingroup type_traits
/// @{
/// @defgroup general_operator_detection General Operator Detection
/// Assortment of type traits to determine if an overload of an operator
/// exists accepting one (or more) types.
///
/// # Example:
/// @code {.cpp}
/// #include <hyperion/mpl/type_traits/is_operator_able.h>
///
/// using namespace hyperion::mpl::type_traits;
/// struct not_addable {};
///
/// static_assert(is_addable_v<int, int>);
/// static_assert(!is_addable_v<not_addable, not_addable>);
///
/// @endcode
/// @headerfile hyperion/mpl/type_traits/is_operator_able.h
/// @}

namespace hyperion::mpl::type_traits {

    /// @brief Type Trait to determine if a type supports unary
    /// `operator+`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports unary `operator+`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator+` overload,
    /// i.e. `decltype(+std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_unary_plusable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator+` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_unary_plusable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_unary_plusable : std::false_type {
        using result_type = void;
    };

    template<concepts::UnaryPlusable TLhs>
    struct is_unary_plusable<TLhs> : std::true_type {
        using result_type = decltype(+std::declval<TLhs>());
    };

    /// @brief Value of the type trait `is_unary_plusable`.
    /// Used to determine if a type supports unary `operator+`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator+` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_unary_plusable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_unary_plusable_v = is_unary_plusable<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_unary_plusable`.
    /// Used to determine the type of the returned result of invoking unary
    /// `operator+` on a `TLhs`, i.e. `decltype(+std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_unary_plusable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator+` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_unary_plusable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using unary_plus_result_t = typename is_unary_plusable<TLhs>::result_type;

    /// @brief Type Trait to determine if a type supports unary
    /// `operator-`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports unary `operator-`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator-` overload,
    /// i.e. `decltype(-std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_unary_minusable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator-` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_unary_minusable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_unary_minusable : std::false_type {
        using result_type = void;
    };

    template<concepts::UnaryMinusable TLhs>
    struct is_unary_minusable<TLhs> : std::true_type {
        using result_type = decltype(-std::declval<TLhs>());
    };

    /// @brief Value of the type trait `is_unary_minusable`.
    /// Used to determine if a type supports unary `operator-`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator-` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_unary_minusable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_unary_minusable_v = is_unary_minusable<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_unary_minusable`.
    /// Used to determine the type of the returned result of invoking unary
    /// `operator-` on a `TLhs`, i.e. `decltype(-std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_unary_minusable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator-` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_unary_minusable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using unary_minus_result_t = typename is_unary_minusable<TLhs>::result_type;

    /// @brief Type Trait to determine if a type supports the binary
    /// not operator, i.e. `operator~`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports `operator~`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator~` overload,
    /// i.e. `decltype(~std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_binary_notable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator~` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_binary_notable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_binary_notable : std::false_type {
        using result_type = void;
    };

    template<concepts::BinaryNotable TLhs>
    struct is_binary_notable<TLhs> : std::true_type {
        // NOLINTNEXTLINE(hicpp-signed-bitwise)
        using result_type = decltype(~std::declval<TLhs>());
    };

    /// @brief Value of the type trait `is_binary_notable`.
    /// Used to determine if a type supports the binary not operator,
    /// i.e. `operator~`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator~` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_binary_notable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_binary_notable_v = is_binary_notable<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_binary_notable`.
    /// Used to determine the type of the returned result of invoking
    /// `operator~` on a `TLhs`, i.e. `decltype(~std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_binary_notable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator~` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_binary_notable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using binary_not_result_t = typename is_binary_notable<TLhs>::result_type;

    /// @brief Type Trait to determine if a type supports the boolean
    /// not operator, i.e. `operator!`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports `operator!`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator!` overload,
    /// i.e. `decltype(!std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_boolean_notable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator!` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_boolean_notable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_boolean_notable : std::false_type {
        using result_type = void;
    };

    template<concepts::BooleanNotable TLhs>
    struct is_boolean_notable<TLhs> : std::true_type {
        // NOLINTNEXTLINE(cppcoreguidelines-narrowing-conversions)
        using result_type = decltype(!std::declval<TLhs>());
    };

    /// @brief Value of the type trait `is_boolean_notable`.
    /// Used to determine if a type supports the boolean not operator,
    /// i.e. `operator!`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator!` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_boolean_notable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_boolean_notable_v = is_boolean_notable<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_boolean_notable`.
    /// Used to determine the type of the returned result of invoking
    /// `operator!` on a `TLhs`, i.e. `decltype(!std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_boolean_notable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator!` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_boolean_notable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using boolean_not_result_t = typename is_boolean_notable<TLhs>::result_type;

    /// @brief Type Trait to determine if a type supports the address of
    /// operator, i.e. unary `operator&`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports unary `operator&`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator&` overload,
    /// i.e. `decltype(&std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_addressable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator&` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_addressable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_addressable : std::false_type {
        using result_type = void;
    };

    template<concepts::Addressable TLhs>
    struct is_addressable<TLhs> : std::true_type {
      private:
        using check_t = std::
            conditional_t<std::is_reference_v<TLhs>, TLhs, std::add_lvalue_reference_t<TLhs>>;

      public:
        using result_type = decltype(&std::declval<check_t>());
    };

    /// @brief Value of the type trait `is_addressable`.
    /// Used to determine if a type supports the address of operator,
    /// i.e. unary `operator&`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator&` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_addressable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_addressable_v = is_addressable<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_addressable`.
    /// Used to determine the type of the returned result of invoking
    /// unary `operator&` on a `TLhs`, i.e. `decltype(&std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_addressable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator&` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_addressable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using address_result_t = typename is_addressable<TLhs>::result_type;

    /// @brief Type Trait to determine if a type supports the pointer to
    /// member operator, i.e. `operator->`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports `operator->`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator->` overload,
    /// i.e. `decltype(std::declval<TLhs>()->)`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_arrowable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// `operator->` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_arrowable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_arrowable : std::false_type {
        using result_type = void;
    };

    template<concepts::Arrowable TLhs>
    struct is_arrowable<TLhs> : std::true_type {
        using result_type = std::conditional_t<std::is_pointer_v<TLhs>,
                                               std::remove_pointer_t<TLhs>,
                                               decltype(std::declval<TLhs>().operator->())>;
    };

    /// @brief Value of the type trait `is_arrowable`.
    /// Used to determine if a type supports the pointer to member operator,
    /// i.e. `operator->`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// `operator->` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_arrowable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_arrowable_v = is_arrowable<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_arrowable`.
    /// Used to determine the type of the returned result of invoking
    /// `operator->` on a `TLhs`, i.e. `decltype(std::declval<TLhs>()->)`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_arrowable<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// `operator->` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_arrowable` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using arrow_result_t = typename is_arrowable<TLhs>::result_type;

    /// @brief Type Trait to determine if a type supports the dereference
    /// operator, i.e. unary `operator*`.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` supports unary `operator*`, also provides
    /// the member `typedef` type `result_type` equal to the type of the
    /// returned result of invoking the associated `operator*` overload,
    /// i.e. `decltype(*std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_dereferencible<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator*` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_dereferencible` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    struct is_dereferencible : std::false_type {
        using result_type = void;
    };

    template<concepts::Dereferencible TLhs>
    struct is_dereferencible<TLhs> : std::true_type {
        using result_type = decltype(*std::declval<TLhs>());
    };

    /// @brief Value of the type trait `is_dereferencible`.
    /// Used to determine if a type supports the dereference operator,
    /// i.e. unary `operator*`.
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator*` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_dereferencible` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    static inline constexpr auto is_dereferencible_v = is_dereferencible<TLhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_dereferencible`.
    /// Used to determine the type of the returned result of invoking
    /// unary `operator*` on a `TLhs`, i.e. `decltype(*std::declval<TLhs>())`
    ///
    /// @tparam TLhs The type to check
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_dereferencible<TLhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification of the unqualified type
    /// of `TLhs`. I.E., given a type `TType`, if you intend to invoke the
    /// unary `operator*` overload associated with `TType` with a `const TType&`,
    /// make sure that you instantiate and use `is_dereferencible` with
    /// `TLhs = const TType&`, not `TType`, `TType&` or any other qualification.
    template<typename TLhs>
    using dereference_result_t = typename is_dereferencible<TLhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are addable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are addable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator+` overload, i.e.
    /// `decltype(std::declval<TLhs>() + std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_addable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the addition operator (i.e. `operator+`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_addable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_addable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::Addable<TLhs, TRhs>
    struct is_addable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() + std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_addable` .
    /// Used to determine if `TLhs` and `TRhs` are addable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the addition operator (i.e. `operator+`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_addable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_addable_v = is_addable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_addable`.
    /// Used to determine the type of the returned result of invoking the addition
    /// operator (i.e. `operator+`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() + std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_addable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the addition operator (i.e. `operator+`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_addable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using add_result_t = typename is_addable<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are subtractable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are subtractable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator-` overload, i.e.
    /// `decltype(std::declval<TLhs>() - std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_subtractable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the subtraction operator (i.e. `operator-`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_subtractable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_subtractable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::Subtractable<TLhs, TRhs>
    struct is_subtractable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() - std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_subtractable` .
    /// Used to determine if `TLhs` and `TRhs` are subtractable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the subtraction operator (i.e. `operator-`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_subtractable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_subtractable_v = is_subtractable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_subtractable`.
    /// Used to determine the type of the returned result of invoking the subtraction
    /// operator (i.e. `operator-`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() - std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_subtractable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the subtraction operator (i.e. `operator-`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_subtractable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using subtract_result_t = typename is_subtractable<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are multipliable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are multipliable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator*` overload, i.e.
    /// `decltype(std::declval<TLhs>() * std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_multipliable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the multiplication operator (i.e. `operator*`) overload associated
    /// with `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_multipliable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_multipliable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::Multipliable<TLhs, TRhs>
    struct is_multipliable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() * std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_multipliable` .
    /// Used to determine if `TLhs` and `TRhs` are multipliable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the multiplication operator (i.e. `operator*`) overload associated
    /// with `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_multipliable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_multipliable_v = is_multipliable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_multipliable`.
    /// Used to determine the type of the returned result of invoking the multiplication
    /// operator (i.e. `operator*`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() * std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_multipliable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the multiplication operator (i.e. `operator*`) overload associated
    /// with `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_multipliable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using multiply_result_t = typename is_multipliable<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are dividible.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are dividible, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator/` overload, i.e.
    /// `decltype(std::declval<TLhs>() / std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_dividible<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the division operator (i.e. `operator/`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_dividible` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_dividible : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::Dividible<TLhs, TRhs>
    struct is_dividible<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() / std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_dividible` .
    /// Used to determine if `TLhs` and `TRhs` are dividible.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the division operator (i.e. `operator/`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_dividible` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_dividible_v = is_dividible<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_dividible`.
    /// Used to determine the type of the returned result of invoking the division
    /// operator (i.e. `operator/`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() / std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_dividible<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the division operator (i.e. `operator/`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_dividible` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using divide_result_t = typename is_dividible<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are binary andable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are binary andable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator&` overload, i.e.
    /// `decltype(std::declval<TLhs>() & std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_binary_andable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the binary and operator (i.e. `operator&`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_binary_andable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_binary_andable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::BinaryAndable<TLhs, TRhs>
    struct is_binary_andable<TLhs, TRhs> : std::true_type {
        // NOLINTNEXTLINE(hicpp-signed-bitwise)
        using result_type = decltype(std::declval<TLhs>() & std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_binary_andable` .
    /// Used to determine if `TLhs` and `TRhs` are binary andable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the binary and operator (i.e. `operator&`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_binary_andable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_binary_andable_v = is_binary_andable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_binary_andable`.
    /// Used to determine the type of the returned result of invoking the binary and
    /// operator (i.e. `operator&`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() & std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_binary_andable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the binary and operator (i.e. `operator&`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_binary_andable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using binary_and_result_t = typename is_binary_andable<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are binary orable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are binary orable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator|` overload, i.e.
    /// `decltype(std::declval<TLhs>() | std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_binary_orable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the binary or operator (i.e. `operator|`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_binary_orable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_binary_orable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::BinaryOrable<TLhs, TRhs>
    struct is_binary_orable<TLhs, TRhs> : std::true_type {
        // NOLINTNEXTLINE(hicpp-signed-bitwise)
        using result_type = decltype(std::declval<TLhs>() | std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_binary_orable` .
    /// Used to determine if `TLhs` and `TRhs` are binary orable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the binary or operator (i.e. `operator|`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_binary_orable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_binary_orable_v = is_binary_orable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_binary_orable`.
    /// Used to determine the type of the returned result of invoking the binary or
    /// operator (i.e. `operator|`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() | std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_binary_orable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the binary or operator (i.e. `operator|`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_binary_orable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using binary_or_result_t = typename is_binary_orable<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are boolean andable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are boolean andable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator&&` overload, i.e.
    /// `decltype(std::declval<TLhs>() && std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_boolean_andable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the boolean and operator (i.e. `operator&&`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_boolean_andable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_boolean_andable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::BooleanAndable<TLhs, TRhs>
    struct is_boolean_andable<TLhs, TRhs> : std::true_type {
        // NOLINTNEXTLINE(cppcoreguidelines-narrowing-conversions)
        using result_type = decltype(std::declval<TLhs>() && std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_boolean_andable` .
    /// Used to determine if `TLhs` and `TRhs` are boolean andable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the boolean and operator (i.e. `operator&&`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_boolean_andable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_boolean_andable_v = is_boolean_andable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_boolean_andable`.
    /// Used to determine the type of the returned result of invoking the boolean and
    /// operator (i.e. `operator&&`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() && std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_boolean_andable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the boolean and operator (i.e. `operator&&`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_boolean_andable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using boolean_and_result_t = typename is_boolean_andable<TLhs, TRhs>::result_type;

    /// @brief Type trait to determine if `TLhs` and `TRhs` are boolean orable.
    ///
    /// In addition to providing the static member `value`, indicating
    /// whether a `TLhs` and `TRhs` are boolean orable, also provides the member
    /// `typedef` type `result_type` equal to the type of the returned result
    /// of invoking the associated `operator||` overload, i.e.
    /// `decltype(std::declval<TLhs>() || std::declval<TRhs>())`.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_boolean_orable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the boolean or operator (i.e. `operator||`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_boolean_orable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    struct is_boolean_orable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires concepts::BooleanOrable<TLhs, TRhs>
    struct is_boolean_orable<TLhs, TRhs> : std::true_type {
        // NOLINTNEXTLINE(cppcoreguidelines-narrowing-conversions)
        using result_type = decltype(std::declval<TLhs>() || std::declval<TRhs>());
    };

    /// @brief Value of the type trait `is_boolean_orable` .
    /// Used to determine if `TLhs` and `TRhs` are boolean orable.
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the boolean or operator (i.e. `operator||`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_boolean_orable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_boolean_orable_v = is_boolean_orable<TLhs, TRhs>::value;

    /// @brief Alias to the `result_type` member `typedef` of `is_boolean_orable`.
    /// Used to determine the type of the returned result of invoking the boolean or
    /// operator (i.e. `operator||`) with a `TLhs` and `TRhs`,
    /// i.e. `decltype(std::declval<TLhs>() || std::declval<TRhs>())`
    ///
    /// @tparam TLhs The type of the left-hand parameter
    /// @tparam TRhs The type of the right-hand parameter. Defaults to `TLhs`
    /// @ingroup general_operator_detection
    /// @headerfile hyperion/mpl/type_traits/is_operator_able.h
    ///
    /// @note Presence of `result_type` should not be used  as a short-hand
    /// or alternaitve to checking `value` in any way
    /// (e.g. in a SFINAE-compatible context or C++20 `requires` clause):
    /// if `is_boolean_orable<TLhs, TRhs>::value == false`,
    /// then `result_type` will be `void`
    ///
    /// @note Due to overloadability by cv-ref qualification, care should be taken
    /// to ensure that use of `result_type` (and this trait in general) is
    /// associated with the correct cv-ref qualification(s) of the unqualified types
    /// of `TLhs` and `TRhs`. I.E., given types `TType1` and `TType2`, if you intend
    /// to invoke the boolean or operator (i.e. `operator||`) overload associated with
    /// `TType1` and `TType2` with `const TType1&` and `const TType2&`, make sure
    /// sure that you instantiate and use `is_boolean_orable` with `TLhs = const TType1&`
    /// and `TRhs = const TType2&`, not `TType1`, `TType2&` or any other
    /// qualification of either type thereof.
    template<typename TLhs, typename TRhs = TLhs>
    using boolean_or_result_t = typename is_boolean_orable<TLhs, TRhs>::result_type;

    namespace _test {

        struct nothing_able {
            // NOLINTNEXTLINE(google-runtime-operator)
            auto operator&() = delete;
        };

        static_assert(is_unary_plusable_v<int>,
                      "hyperion::mpl::type_traits::is_unary_plusable test case 1 (failing)");
        static_assert(is_unary_plusable_v<double>,
                      "hyperion::mpl::type_traits::is_unary_plusable test case 2 (failing)");
        static_assert(!is_unary_plusable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_unary_plusable test case 3 (failing)");
        static_assert(std::same_as<unary_plus_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_unary_plusable test case 4 (failing)");
        static_assert(std::same_as<unary_plus_result_t<double>, double>,
                      "hyperion::mpl::type_traits::is_unary_plusable test case 5 (failing)");
        static_assert(std::same_as<unary_plus_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_unary_plusable test case 6 (failing)");

        static_assert(is_unary_minusable_v<int>,
                      "hyperion::mpl::type_traits::is_unary_minusable test case 1 (failing)");
        static_assert(is_unary_minusable_v<double>,
                      "hyperion::mpl::type_traits::is_unary_minusable test case 2 (failing)");
        static_assert(!is_unary_minusable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_unary_minusable test case 3 (failing)");
        static_assert(std::same_as<unary_minus_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_unary_minusable test case 4 (failing)");
        static_assert(std::same_as<unary_minus_result_t<double>, double>,
                      "hyperion::mpl::type_traits::is_unary_minusable test case 5 (failing)");
        static_assert(std::same_as<unary_minus_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_unary_minusable test case 6 (failing)");

        static_assert(is_binary_notable_v<int>,
                      "hyperion::mpl::type_traits::is_binary_notable test case 1 (failing)");
        static_assert(!is_binary_notable_v<double>,
                      "hyperion::mpl::type_traits::is_binary_notable test case 2 (failing)");
        static_assert(!is_binary_notable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_binary_notable test case 3 (failing)");
        static_assert(std::same_as<binary_not_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_binary_notable test case 4 (failing)");
        static_assert(std::same_as<binary_not_result_t<double>, void>,
                      "hyperion::mpl::type_traits::is_binary_notable test case 5 (failing)");
        static_assert(std::same_as<binary_not_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_binary_notable test case 6 (failing)");

        static_assert(is_boolean_notable_v<int>,
                      "hyperion::mpl::type_traits::is_boolean_notable test case 1 (failing)");
        static_assert(is_boolean_notable_v<double>,
                      "hyperion::mpl::type_traits::is_boolean_notable test case 2 (failing)");
        static_assert(!is_boolean_notable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_boolean_notable test case 3 (failing)");
        static_assert(std::same_as<boolean_not_result_t<int>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_notable test case 4 (failing)");
        static_assert(std::same_as<boolean_not_result_t<double>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_notable test case 5 (failing)");
        static_assert(std::same_as<boolean_not_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_boolean_notable test case 6 (failing)");

        static_assert(is_addressable_v<int>,
                      "hyperion::mpl::type_traits::is_addressable test case 1 (failing)");
        static_assert(is_addressable_v<double>,
                      "hyperion::mpl::type_traits::is_addressable test case 2 (failing)");
        static_assert(!is_addressable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_addressable test case 3 (failing)");
        static_assert(std::same_as<address_result_t<int>, int*>,
                      "hyperion::mpl::type_traits::is_addressable test case 4 (failing)");
        static_assert(std::same_as<address_result_t<double>, double*>,
                      "hyperion::mpl::type_traits::is_addressable test case 5 (failing)");
        static_assert(std::same_as<address_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_addressable test case 6 (failing)");

        struct arrow_able {
            auto operator->() -> arrow_able&;
            auto operator->() const -> const arrow_able&;
        };

        static_assert(is_arrowable_v<arrow_able>,
                      "hyperion::mpl::type_traits::is_arrowable test case 1 (failing)");
        static_assert(!is_arrowable_v<int>,
                      "hyperion::mpl::type_traits::is_arrowable test case 2 (failing)");
        static_assert(!is_arrowable_v<double>,
                      "hyperion::mpl::type_traits::is_arrowable test case 3 (failing)");
        static_assert(!is_arrowable_v<int*>,
                      "hyperion::mpl::type_traits::is_arrowable test case 4 (failing)");
        static_assert(!is_arrowable_v<double*>,
                      "hyperion::mpl::type_traits::is_arrowable test case 5 (failing)");
        static_assert(!is_arrowable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_arrowable test case 6 (failing)");
        static_assert(std::same_as<arrow_result_t<arrow_able>, arrow_able&>,
                      "hyperion::mpl::type_traits::is_arrowable test case 7 (failing)");
        static_assert(std::same_as<arrow_result_t<int>, void>,
                      "hyperion::mpl::type_traits::is_arrowable test case 8 (failing)");
        static_assert(std::same_as<arrow_result_t<double>, void>,
                      "hyperion::mpl::type_traits::is_arrowable test case 9 (failing)");
        static_assert(std::same_as<arrow_result_t<int*>, void>,
                      "hyperion::mpl::type_traits::is_arrowable test case 10 (failing)");
        static_assert(std::same_as<arrow_result_t<double*>, void>,
                      "hyperion::mpl::type_traits::is_arrowable test case 11 (failing)");
        static_assert(std::same_as<arrow_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_arrowable test case 12 (failing)");

        static_assert(!is_dereferencible_v<int>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 2 (failing)");
        static_assert(!is_dereferencible_v<double>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 3 (failing)");
        static_assert(is_dereferencible_v<int*>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 4 (failing)");
        static_assert(is_dereferencible_v<double*>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 5 (failing)");
        static_assert(!is_dereferencible_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 6 (failing)");
        static_assert(std::same_as<dereference_result_t<int>, void>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 8 (failing)");
        static_assert(std::same_as<dereference_result_t<double>, void>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 9 (failing)");
        static_assert(std::same_as<dereference_result_t<int*>, int&>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 10 (failing)");
        static_assert(std::same_as<dereference_result_t<double*>, double&>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 11 (failing)");
        static_assert(std::same_as<dereference_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_dereferencible test case 12 (failing)");

        static_assert(is_addable_v<int>,
                      "hyperion::mpl::type_traits::is_addable test case 1 (failing)");
        static_assert(is_addable_v<double>,
                      "hyperion::mpl::type_traits::is_addable test case 2 (failing)");
        static_assert(is_addable_v<int, double>,
                      "hyperion::mpl::type_traits::is_addable test case 3 (failing)");
        static_assert(is_addable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_addable test case 4 (failing)");
        static_assert(!is_addable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_addable test case 5 (failing)");
        static_assert(std::same_as<add_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_addable test case 6 (failing)");
        static_assert(std::same_as<add_result_t<double>, double>,
                      "hyperion::mpl::type_traits::is_addable test case 7 (failing)");
        static_assert(std::same_as<add_result_t<int, double>, double>,
                      "hyperion::mpl::type_traits::is_addable test case 8 (failing)");
        static_assert(std::same_as<add_result_t<double, bool>, double>,
                      "hyperion::mpl::type_traits::is_addable test case 9 (failing)");
        static_assert(std::same_as<add_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_addable test case 10 (failing)");

        static_assert(is_subtractable_v<int>,
                      "hyperion::mpl::type_traits::is_subtractable test case 1 (failing)");
        static_assert(is_subtractable_v<double>,
                      "hyperion::mpl::type_traits::is_subtractable test case 2 (failing)");
        static_assert(is_subtractable_v<int, double>,
                      "hyperion::mpl::type_traits::is_subtractable test case 3 (failing)");
        static_assert(is_subtractable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_subtractable test case 4 (failing)");
        static_assert(!is_subtractable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_subtractable test case 5 (failing)");
        static_assert(std::same_as<subtract_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_subtractable test case 6 (failing)");
        static_assert(std::same_as<subtract_result_t<double>, double>,
                      "hyperion::mpl::type_traits::is_subtractable test case 7 (failing)");
        static_assert(std::same_as<subtract_result_t<int, double>, double>,
                      "hyperion::mpl::type_traits::is_subtractable test case 8 (failing)");
        static_assert(std::same_as<subtract_result_t<double, bool>, double>,
                      "hyperion::mpl::type_traits::is_subtractable test case 9 (failing)");
        static_assert(std::same_as<subtract_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_subtractable test case 10 (failing)");

        static_assert(is_multipliable_v<int>,
                      "hyperion::mpl::type_traits::is_multipliable test case 1 (failing)");
        static_assert(is_multipliable_v<double>,
                      "hyperion::mpl::type_traits::is_multipliable test case 2 (failing)");
        static_assert(is_multipliable_v<int, double>,
                      "hyperion::mpl::type_traits::is_multipliable test case 3 (failing)");
        static_assert(is_multipliable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_multipliable test case 4 (failing)");
        static_assert(!is_multipliable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_multipliable test case 5 (failing)");
        static_assert(std::same_as<multiply_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_multipliable test case 6 (failing)");
        static_assert(std::same_as<multiply_result_t<double>, double>,
                      "hyperion::mpl::type_traits::is_multipliable test case 7 (failing)");
        static_assert(std::same_as<multiply_result_t<int, double>, double>,
                      "hyperion::mpl::type_traits::is_multipliable test case 8 (failing)");
        static_assert(std::same_as<multiply_result_t<double, bool>, double>,
                      "hyperion::mpl::type_traits::is_multipliable test case 9 (failing)");
        static_assert(std::same_as<multiply_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_multipliable test case 10 (failing)");

        static_assert(is_dividible_v<int>,
                      "hyperion::mpl::type_traits::is_dividible test case 1 (failing)");
        static_assert(is_dividible_v<double>,
                      "hyperion::mpl::type_traits::is_dividible test case 2 (failing)");
        static_assert(is_dividible_v<int, double>,
                      "hyperion::mpl::type_traits::is_dividible test case 3 (failing)");
        static_assert(is_dividible_v<double, bool>,
                      "hyperion::mpl::type_traits::is_dividible test case 4 (failing)");
        static_assert(!is_dividible_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_dividible test case 5 (failing)");
        static_assert(std::same_as<divide_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_dividible test case 6 (failing)");
        static_assert(std::same_as<divide_result_t<double>, double>,
                      "hyperion::mpl::type_traits::is_dividible test case 7 (failing)");
        static_assert(std::same_as<divide_result_t<int, double>, double>,
                      "hyperion::mpl::type_traits::is_dividible test case 8 (failing)");
        static_assert(std::same_as<divide_result_t<double, bool>, double>,
                      "hyperion::mpl::type_traits::is_dividible test case 9 (failing)");
        static_assert(std::same_as<divide_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_dividible test case 10 (failing)");

        static_assert(is_binary_andable_v<int>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 1 (failing)");
        static_assert(!is_binary_andable_v<double>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 2 (failing)");
        static_assert(!is_binary_andable_v<int, double>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 3 (failing)");
        static_assert(!is_binary_andable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 4 (failing)");
        static_assert(!is_binary_andable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 5 (failing)");
        static_assert(std::same_as<binary_and_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 6 (failing)");
        static_assert(std::same_as<binary_and_result_t<double>, void>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 7 (failing)");
        static_assert(std::same_as<binary_and_result_t<int, double>, void>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 8 (failing)");
        static_assert(std::same_as<binary_and_result_t<double, bool>, void>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 9 (failing)");
        static_assert(std::same_as<binary_and_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_binary_andable test case 10 (failing)");

        static_assert(is_binary_orable_v<int>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 1 (failing)");
        static_assert(!is_binary_orable_v<double>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 2 (failing)");
        static_assert(!is_binary_orable_v<int, double>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 3 (failing)");
        static_assert(!is_binary_orable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 4 (failing)");
        static_assert(!is_binary_orable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 5 (failing)");
        static_assert(std::same_as<binary_or_result_t<int>, int>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 6 (failing)");
        static_assert(std::same_as<binary_or_result_t<double>, void>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 7 (failing)");
        static_assert(std::same_as<binary_or_result_t<int, double>, void>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 8 (failing)");
        static_assert(std::same_as<binary_or_result_t<double, bool>, void>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 9 (failing)");
        static_assert(std::same_as<binary_or_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_binary_orable test case 10 (failing)");

        static_assert(is_boolean_andable_v<int>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 1 (failing)");
        static_assert(is_boolean_andable_v<double>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 2 (failing)");
        static_assert(is_boolean_andable_v<int, double>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 3 (failing)");
        static_assert(is_boolean_andable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 4 (failing)");
        static_assert(!is_boolean_andable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 5 (failing)");
        static_assert(std::same_as<boolean_and_result_t<int>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 6 (failing)");
        static_assert(std::same_as<boolean_and_result_t<double>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 7 (failing)");
        static_assert(std::same_as<boolean_and_result_t<int, double>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 8 (failing)");
        static_assert(std::same_as<boolean_and_result_t<double, bool>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 9 (failing)");
        static_assert(std::same_as<boolean_and_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_boolean_andable test case 10 (failing)");

        static_assert(is_boolean_orable_v<int>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 1 (failing)");
        static_assert(is_boolean_orable_v<double>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 2 (failing)");
        static_assert(is_boolean_orable_v<int, double>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 3 (failing)");
        static_assert(is_boolean_orable_v<double, bool>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 4 (failing)");
        static_assert(!is_boolean_orable_v<nothing_able>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 5 (failing)");
        static_assert(std::same_as<boolean_or_result_t<int>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 6 (failing)");
        static_assert(std::same_as<boolean_or_result_t<double>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 7 (failing)");
        static_assert(std::same_as<boolean_or_result_t<int, double>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 8 (failing)");
        static_assert(std::same_as<boolean_or_result_t<double, bool>, bool>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 9 (failing)");
        static_assert(std::same_as<boolean_or_result_t<nothing_able>, void>,
                      "hyperion::mpl::type_traits::is_boolean_orable test case 10 (failing)");

    } // namespace _test
} // namespace hyperion::mpl::type_traits

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;

#endif // HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_H
