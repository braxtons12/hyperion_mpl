/// @file operator_able.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief tests for operator_able.h
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

#ifndef HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_TEST_H
#define HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_TEST_H

#include <hyperion/mpl/concepts/operator_able.h>

namespace hyperion::mpl::concepts::_test {

    struct nothing_able {
        // NOLINTNEXTLINE(google-runtime-operator)
        auto operator&() = delete;
    };

    static_assert(UnaryPlusable<int>,
                  "hyperion::mpl::concepts::UnaryPlusable test case 1 (failing)");
    static_assert(UnaryPlusable<double>,
                  "hyperion::mpl::concepts::UnaryPlusable test case 2 (failing)");
    static_assert(!UnaryPlusable<nothing_able>,
                  "hyperion::mpl::concepts::UnaryPlusable test case 3 (failing)");

    static_assert(UnaryMinusable<int>,
                  "hyperion::mpl::concepts::UnaryMinusable test case 1 (failing)");
    static_assert(UnaryMinusable<double>,
                  "hyperion::mpl::concepts::UnaryMinusable test case 2 (failing)");
    static_assert(!UnaryMinusable<nothing_able>,
                  "hyperion::mpl::concepts::UnaryMinusable test case 3 (failing)");

    static_assert(BinaryNotable<int>,
                  "hyperion::mpl::concepts::BinaryNotable test case 1 (failing)");
    static_assert(!BinaryNotable<double>,
                  "hyperion::mpl::concepts::BinaryNotable test case 2 (failing)");
    static_assert(!BinaryNotable<nothing_able>,
                  "hyperion::mpl::concepts::BinaryNotable test case 3 (failing)");

    static_assert(BooleanNotable<int>,
                  "hyperion::mpl::concepts::BooleanNotable test case 1 (failing)");
    static_assert(BooleanNotable<double>,
                  "hyperion::mpl::concepts::BooleanNotable test case 2 (failing)");
    static_assert(!BooleanNotable<nothing_able>,
                  "hyperion::mpl::concepts::BooleanNotable test case 3 (failing)");

    static_assert(Addressable<int>, "hyperion::mpl::concepts::Addressable test case 1 (failing)");
    static_assert(Addressable<double>,
                  "hyperion::mpl::concepts::Addressable test case 2 (failing)");
    static_assert(!Addressable<nothing_able>,
                  "hyperion::mpl::concepts::Addressable test case 3 (failing)");

    struct arrow_able {
        auto operator->() -> int*;
        auto operator->() const -> const int*;
    };

    static_assert(Arrowable<arrow_able>,
                  "hyperion::mpl::concepts::Arrowable test case 1 (failing)");
    static_assert(!Arrowable<int>, "hyperion::mpl::concepts::Arrowable test case 2 (failing)");
    static_assert(!Arrowable<double>, "hyperion::mpl::concepts::Arrowable test case 3 (failing)");
    static_assert(!Arrowable<int*>, "hyperion::mpl::concepts::Arrowable test case 4 (failing)");
    static_assert(!Arrowable<double*>, "hyperion::mpl::concepts::Arrowable test case 5 (failing)");
    static_assert(!Arrowable<nothing_able>,
                  "hyperion::mpl::concepts::Arrowable test case 6 (failing)");

    static_assert(!Dereferencible<int>,
                  "hyperion::mpl::concepts::Dereferencible test case 2 (failing)");
    static_assert(!Dereferencible<double>,
                  "hyperion::mpl::concepts::Dereferencible test case 3 (failing)");
    static_assert(Dereferencible<int*>,
                  "hyperion::mpl::concepts::Dereferencible test case 4 (failing)");
    static_assert(Dereferencible<double*>,
                  "hyperion::mpl::concepts::Dereferencible test case 5 (failing)");
    static_assert(!Dereferencible<nothing_able>,
                  "hyperion::mpl::concepts::Dereferencible test case 6 (failing)");

    static_assert(Addable<int>, "hyperion::mpl::concepts::Addable test case 1 (failing)");
    static_assert(Addable<double>, "hyperion::mpl::concepts::Addable test case 2 (failing)");
    static_assert(Addable<int, double>, "hyperion::mpl::concepts::Addable test case 3 (failing)");
    static_assert(Addable<double, bool>, "hyperion::mpl::concepts::Addable test case 4 (failing)");
    static_assert(!Addable<nothing_able>, "hyperion::mpl::concepts::Addable test case 5 (failing)");

    static_assert(Subtractable<int>, "hyperion::mpl::concepts::Subtractable test case 1 (failing)");
    static_assert(Subtractable<double>,
                  "hyperion::mpl::concepts::Subtractable test case 2 (failing)");
    static_assert(Subtractable<int, double>,
                  "hyperion::mpl::concepts::Subtractable test case 3 (failing)");
    static_assert(Subtractable<double, bool>,
                  "hyperion::mpl::concepts::Subtractable test case 4 (failing)");
    static_assert(!Subtractable<nothing_able>,
                  "hyperion::mpl::concepts::Subtractable test case 5 (failing)");

    static_assert(Multipliable<int>, "hyperion::mpl::concepts::Multipliable test case 1 (failing)");
    static_assert(Multipliable<double>,
                  "hyperion::mpl::concepts::Multipliable test case 2 (failing)");
    static_assert(Multipliable<int, double>,
                  "hyperion::mpl::concepts::Multipliable test case 3 (failing)");
    static_assert(Multipliable<double, bool>,
                  "hyperion::mpl::concepts::Multipliable test case 4 (failing)");
    static_assert(!Multipliable<nothing_able>,
                  "hyperion::mpl::concepts::Multipliable test case 5 (failing)");

    static_assert(Dividible<int>, "hyperion::mpl::concepts::Dividible test case 1 (failing)");
    static_assert(Dividible<double>, "hyperion::mpl::concepts::Dividible test case 2 (failing)");
    static_assert(Dividible<int, double>,
                  "hyperion::mpl::concepts::Dividible test case 3 (failing)");
    static_assert(Dividible<double, bool>,
                  "hyperion::mpl::concepts::Dividible test case 4 (failing)");
    static_assert(!Dividible<nothing_able>,
                  "hyperion::mpl::concepts::Dividible test case 5 (failing)");

    static_assert(BinaryAndable<int>,
                  "hyperion::mpl::concepts::BinaryAndable test case 1 (failing)");
    static_assert(!BinaryAndable<double>,
                  "hyperion::mpl::concepts::BinaryAndable test case 2 (failing)");
    static_assert(!BinaryAndable<int, double>,
                  "hyperion::mpl::concepts::BinaryAndable test case 3 (failing)");
    static_assert(!BinaryAndable<double, bool>,
                  "hyperion::mpl::concepts::BinaryAndable test case 4 (failing)");
    static_assert(!BinaryAndable<nothing_able>,
                  "hyperion::mpl::concepts::BinaryAndable test case 5 (failing)");

    static_assert(BinaryOrable<int>, "hyperion::mpl::concepts::BinaryOrable test case 1 (failing)");
    static_assert(!BinaryOrable<double>,
                  "hyperion::mpl::concepts::BinaryOrable test case 2 (failing)");
    static_assert(!BinaryOrable<int, double>,
                  "hyperion::mpl::concepts::BinaryOrable test case 3 (failing)");
    static_assert(!BinaryOrable<double, bool>,
                  "hyperion::mpl::concepts::BinaryOrable test case 4 (failing)");
    static_assert(!BinaryOrable<nothing_able>,
                  "hyperion::mpl::concepts::BinaryOrable test case 5 (failing)");

    static_assert(BooleanAndable<int>,
                  "hyperion::mpl::concepts::BooleanAndable test case 1 (failing)");
    static_assert(BooleanAndable<double>,
                  "hyperion::mpl::concepts::BooleanAndable test case 2 (failing)");
    static_assert(BooleanAndable<int, double>,
                  "hyperion::mpl::concepts::BooleanAndable test case 3 (failing)");
    static_assert(BooleanAndable<double, bool>,
                  "hyperion::mpl::concepts::BooleanAndable test case 4 (failing)");
    static_assert(!BooleanAndable<nothing_able>,
                  "hyperion::mpl::concepts::BooleanAndable test case 5 (failing)");

    static_assert(BooleanOrable<int>,
                  "hyperion::mpl::concepts::BooleanOrable test case 1 (failing)");
    static_assert(BooleanOrable<double>,
                  "hyperion::mpl::concepts::BooleanOrable test case 2 (failing)");
    static_assert(BooleanOrable<int, double>,
                  "hyperion::mpl::concepts::BooleanOrable test case 3 (failing)");
    static_assert(BooleanOrable<double, bool>,
                  "hyperion::mpl::concepts::BooleanOrable test case 4 (failing)");
    static_assert(!BooleanOrable<nothing_able>,
                  "hyperion::mpl::concepts::BooleanOrable test case 5 (failing)");

} // namespace hyperion::mpl::concepts::_test

#endif // HYPERION_MPL_CONCEPTS_OPERATOR_ABLE_TEST_H
