/// @file operator_able.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming C++20 concept definitions to determine if a type
/// (or types) support an operator
/// @version 0.1.1
/// @date 2025-07-08
///
/// MIT License
/// @copyright Copyright (c) 2025 Braxton Salyer <braxtonsalyer@gmail.com>
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

#ifndef HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_H
#define HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_H

#include <type_traits>

/// @ingroup concepts
/// @{
/// @defgroup operatorability_concepts Operatorability Concepts
/// Assortment of `concept` definitions requiring type(s) to
/// support various operators.
///
/// # Example:
/// @code {.cpp}
/// #include <hyperion/mpl/concepts/comparable.h
///
/// using namespace hyperion::mpl::concepts;
/// struct not_addable {};
///
/// static_assert(Addable<int, double>);
/// static_assert(!Addable<not_addable, int>);
///
/// @endcode
/// @headerfile hyperion/mpl/concepts/operator_able.h
/// @}

namespace hyperion::mpl::concepts {

    /// @brief Concept definition requiring that a `TLhs`
    /// supports the unary plus operator (unary `operator+`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept UnaryPlusable = requires(TLhs lhs) { +lhs; };

    /// @brief Concept definition requiring that a `TLhs`
    /// supports the unary minus operator (unary `operator-`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept UnaryMinusable = requires(TLhs lhs) { -lhs; };

    /// @brief Concept definition requiring that a `TLhs`
    /// supports the binary not operator (`operator~`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept BinaryNotable = requires(TLhs lhs) { ~lhs; };

    /// @brief Concept definition requiring that a `TLhs`
    /// supports the boolean not operator (`operator!`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept BooleanNotable = requires(TLhs lhs) { !lhs; };

    /// @brief Concept definition requiring that a `TLhs`
    /// is addressable (i.e., it supports unary `operator&`)
    ///
    /// @note This _*should*_ be `true` for _*all*_ types.
    /// It is rarely (read: never) not an error to overload
    /// the address-of operator
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept Addressable = requires(TLhs lhs) { &lhs; };

    /// @brief Concept definition requiring that a `TLhs`
    /// is dereferencible (i.e., it supports unary `operator*`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept Dereferencible = std::is_pointer_v<TLhs> || requires(TLhs lhs) { *lhs; };

    // clang-format off

    /// @brief Concept definition requiring that a `TLhs` supports the
    /// pointer-to-memeber operator (i.e., it supports unary `operator->`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs>
    concept Arrowable = (std::is_class_v<std::remove_pointer_t<TLhs>>
                         || std::is_union_v<std::remove_pointer_t<TLhs>>)
                        && (std::is_pointer_v<TLhs>
                            || requires(TLhs lhs) {
                                   lhs.operator->();
                               });
    // clang-format on

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are addable (they support binary `operator+`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept Addable = requires(TLhs lhs, TRhs rhs) {
        lhs + rhs;
        rhs + lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are subtractable (they support binary `operator-`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept Subtractable = requires(TLhs lhs, TRhs rhs) {
        lhs - rhs;
        rhs - lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are multipliable (they support binary `operator*`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept Multipliable = requires(TLhs lhs, TRhs rhs) {
        lhs* rhs;
        rhs* lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are dividible (they support binary `operator/`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept Dividible = requires(TLhs lhs, TRhs rhs) {
        lhs / rhs;
        rhs / lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are binary andable (they support binary `operator&`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept BinaryAndable = requires(TLhs lhs, TRhs rhs) {
        lhs & rhs;
        rhs & lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are binary orable (they support binary `operator|`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept BinaryOrable = requires(TLhs lhs, TRhs rhs) {
        lhs | rhs;
        rhs | lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are boolean andable (they support binary `operator&&`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept BooleanAndable = requires(TLhs lhs, TRhs rhs) {
        lhs&& rhs;
        rhs&& lhs;
    };

    /// @brief Concept definition requiring that `TLhs` and `TRhs`
    /// are boolean orable (they support binary `operator||`)
    /// @ingroup operatorability_concepts
    /// @headerfile hyperion/mpl/concepts/operator_able.h
    template<typename TLhs, typename TRhs = TLhs>
    concept BooleanOrable = requires(TLhs lhs, TRhs rhs) {
        lhs || rhs;
        rhs || lhs;
    };
} // namespace hyperion::mpl::concepts

#endif // HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_H
