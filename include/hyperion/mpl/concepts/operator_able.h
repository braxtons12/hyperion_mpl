/// @file operator_able.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming C++20 concept definitions to determine if a type
/// (or types) support an operator
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

#ifndef HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_H
#define HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_H

#include <type_traits>

namespace hyperion::mpl::concepts {

    template<typename TLhs>
    concept UnaryPlusable = requires(const TLhs& lhs) { +lhs; };

    template<typename TLhs>
    concept UnaryMinusable = requires(const TLhs& lhs) { -lhs; };

    template<typename TLhs>
    concept BinaryNotable = requires(const TLhs& lhs) { ~lhs; };

    template<typename TLhs>
    concept BooleanNotable = requires(const TLhs& lhs) { !lhs; };

    template<typename TLhs>
    concept Addressable = requires(const TLhs& lhs) { &lhs; };

    template<typename TLhs>
    concept Dereferencible = std::is_pointer_v<TLhs> || requires(const TLhs& lhs) { *lhs; };

    template<typename TLhs>
    concept Arrowable = std::is_pointer_v<TLhs> || requires(const TLhs& lhs) { lhs.operator->(); };

    template<typename TLhs, typename TRhs = TLhs>
    concept Addable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs + rhs;
        rhs + lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept Subtractable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs - rhs;
        rhs - lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept Multipliable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs* rhs;
        rhs* lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept Dividible = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs / rhs;
        rhs / lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept BinaryAndable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs & rhs;
        rhs & lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept BinaryOrable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs | rhs;
        rhs | lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept BooleanAndable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs&& rhs;
        rhs&& lhs;
    };

    template<typename TLhs, typename TRhs = TLhs>
    concept BooleanOrable = requires(const TLhs& lhs, const TRhs& rhs) {
        lhs || rhs;
        rhs || lhs;
    };

} // namespace hyperion::mpl::concepts

#endif // HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_H
