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

#include <type_traits>

namespace hyperion::mpl::type_traits {

    template<typename TLhs>
    struct is_unary_plusable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs>
        requires requires(const TLhs& lhs) { +lhs; }
    struct is_unary_plusable<TLhs> : std::true_type {
        using result_type = decltype(+std::declval<TLhs>());
    };

    template<typename TLhs>
    static inline constexpr auto is_unary_plusable_v = is_unary_plusable<TLhs>::value;

    template<typename TLhs>
    using unary_plus_result_t = typename is_unary_plusable<TLhs>::result_type;

    template<typename TLhs>
    struct is_unary_minusable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs>
        requires requires(const TLhs& lhs) { -lhs; }
    struct is_unary_minusable<TLhs> : std::true_type {
        using result_type = decltype(-std::declval<TLhs>());
    };

    template<typename TLhs>
    static inline constexpr auto is_unary_minusable_v = is_unary_minusable<TLhs>::value;

    template<typename TLhs>
    using unary_minus_result_t = typename is_unary_minusable<TLhs>::result_type;

    template<typename TLhs>
    struct is_binary_notable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs>
        requires requires(const TLhs& lhs) { ~lhs; }
    struct is_binary_notable<TLhs> : std::true_type {
        using result_type = decltype(~std::declval<TLhs>());
    };

    template<typename TLhs>
    static inline constexpr auto is_binary_notable_v = is_binary_notable<TLhs>::value;

    template<typename TLhs>
    using binary_not_result_t = typename is_binary_notable<TLhs>::result_type;

    template<typename TLhs>
    struct is_boolean_notable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs>
        requires requires(const TLhs& lhs) { !lhs; }
    struct is_boolean_notable<TLhs> : std::true_type {
        using result_type = decltype(!std::declval<TLhs>());
    };

    template<typename TLhs>
    static inline constexpr auto is_boolean_notable_v = is_boolean_notable<TLhs>::value;

    template<typename TLhs>
    using boolean_not_result_t = typename is_boolean_notable<TLhs>::result_type;

    template<typename TLhs>
    struct is_addressable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs>
        requires requires(const TLhs& lhs) { &lhs; }
    struct is_addressable<TLhs> : std::true_type {
        using result_type = decltype(&std::declval<TLhs>());
    };

    template<typename TLhs>
    static inline constexpr auto is_addressable_v = is_addressable<TLhs>::value;

    template<typename TLhs>
    using address_result_t = typename is_addressable<TLhs>::result_type;

    template<typename TLhs>
    struct is_arrowable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs>
        requires std::is_pointer_v<TLhs> || requires(const TLhs& lhs) { lhs.operator->(); }
    struct is_arrowable<TLhs> : std::true_type {
        using result_type = std::conditional_t<std::is_pointer_v<TLhs>,
                                               std::remove_pointer_t<TLhs>,
                                               decltype(std::declval<TLhs>().operator->())>;
    };

    template<typename TLhs>
    static inline constexpr auto is_arrowable_v = is_arrowable<TLhs>::value;

    template<typename TLhs>
    using arrow_result_t = typename is_arrowable<TLhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_addable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs + rhs;
            rhs + lhs;
        }
    struct is_addable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() + std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_addable_v = is_addable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using add_result_t = typename is_addable<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_subtractable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs - rhs;
            rhs - lhs;
        }
    struct is_subtractable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() - std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_subtractable_v = is_subtractable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using subtract_result_t = typename is_subtractable<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_multipliable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs* rhs;
            rhs* lhs;
        }
    struct is_multipliable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() * std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_multipliable_v = is_multipliable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using multiply_result_t = typename is_multipliable<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_dividible : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs / rhs;
            rhs / lhs;
        }
    struct is_dividible<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() / std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_dividible_v = is_dividible<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using divide_result_t = typename is_dividible<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_binary_andable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs & rhs;
            rhs & lhs;
        }
    struct is_binary_andable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() & std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_binary_andable_v = is_binary_andable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using binary_and_result_t = typename is_binary_andable<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_binary_orable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs | rhs;
            rhs | lhs;
        }
    struct is_binary_orable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() | std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_binary_orable_v = is_binary_orable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using binary_or_result_t = typename is_binary_orable<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_boolean_andable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs&& rhs;
            rhs&& lhs;
        }
    struct is_boolean_andable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() && std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_boolean_andable_v = is_boolean_andable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using boolean_and_result_t = typename is_boolean_andable<TLhs, TRhs>::result_type;

    template<typename TLhs, typename TRhs = TLhs>
    struct is_boolean_orable : std::false_type {
        using result_type = void;
    };

    template<typename TLhs, typename TRhs>
        requires requires(const TLhs& lhs, const TRhs& rhs) {
            lhs || rhs;
            rhs || lhs;
        }
    struct is_boolean_orable<TLhs, TRhs> : std::true_type {
        using result_type = decltype(std::declval<TLhs>() || std::declval<TRhs>());
    };

    template<typename TLhs, typename TRhs = TLhs>
    static inline constexpr auto is_boolean_orable_v = is_boolean_orable<TLhs, TRhs>::value;

    template<typename TLhs, typename TRhs = TLhs>
    using boolean_or_result_t = typename is_boolean_orable<TLhs, TRhs>::result_type;

} // namespace hyperion::mpl::type_traits

#endif // HYPERION_MPL_TYPE_TRAITS_IS_OPERATOR_ABLE_H
