/// @file metapredicates.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Tests for metapredicates.h
/// @version 0.1.3
/// @date 2025-07-08
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

#ifndef HYPERION_MPL_METAPREDICATES_TEST_H
#define HYPERION_MPL_METAPREDICATES_TEST_H

#include <hyperion/mpl/list.h>
#include <hyperion/mpl/metapredicates.h>
#include <hyperion/mpl/pair.h>

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
                  "hyperion::mpl::is_const predicate test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_const),
                  "hyperion::mpl::is_const predicate test case 2 (failing)");

    static_assert(decltype_<int&>().satisfies(is_lvalue_reference),
                  "hyperion::mpl::is_lvalue_reference predicate test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_lvalue_reference),
                  "hyperion::mpl::is_lvalue_reference predicate test case 2 (failing)");

    static_assert(decltype_<int&&>().satisfies(is_rvalue_reference),
                  "hyperion::mpl::is_rvalue_reference predicate test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_rvalue_reference),
                  "hyperion::mpl::is_rvalue_reference predicate test case 2 (failing)");

    static_assert(decltype_<int&>().satisfies(is_reference),
                  "hyperion::mpl::is_reference predicate test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_reference),
                  "hyperion::mpl::is_reference predicate test case 2 (failing)");
    static_assert(decltype_<int&&>().satisfies(is_reference),
                  "hyperion::mpl::is_reference predicate test case 3 (failing)");

    static_assert(decltype_<int*>().satisfies(is_pointer),
                  "hyperion::mpl::is_pointer predicate test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_pointer),
                  "hyperion::mpl::is_pointer predicate test case 2 (failing)");
    static_assert(decltype_<const int* const>().satisfies(is_pointer),
                  "hyperion::mpl::is_pointer predicate test case 3 (failing)");

    static_assert(decltype_<volatile int>().satisfies(is_volatile),
                  "hyperion::mpl::is_volatile predicate test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_volatile),
                  "hyperion::mpl::is_volatile predicate test case 2 (failing)");

    struct empty { };

    static_assert(decltype_<empty>().satisfies(is_empty),
                  "hyperion::mpl::is_empty test case 1 (failing)");
    static_assert(not decltype_<int>().satisfies(is_empty),
                  "hyperion::mpl::is_empty test case 2 (failing)");

    struct not_trivial {
        not_trivial(const not_trivial&);
    };
    static_assert(decltype_<empty>().satisfies(is_trivial),
                  "hyperion::mpl::is_trivial test case 1 (failing)");
    static_assert(decltype_<int>().satisfies(is_trivial),
                  "hyperion::mpl::is_trivial test case 2 (failing)");
    static_assert(not decltype_<not_trivial>().satisfies(is_trivial),
                  "hyperion::mpl::is_trivial test case 3 (failing)");

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
// GCC overrides our `noexcept(false) above`
#if HYPERION_PLATFORM_COMPILER_IS_GCC
    static_assert(static_cast<bool>(decltype_<trivially_default_constructible>().satisfies(
                      mpl::noexcept_default_constructible)),
                  "hyperion::mpl::noexcept_default_constructible predicate test case 4 (failing)");
#else
    static_assert(not decltype_<trivially_default_constructible>().satisfies(
                      mpl::noexcept_default_constructible),
                  "hyperion::mpl::noexcept_default_constructible predicate test case 4 (failing)");
#endif // HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
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
// GCC 13 and below overrides our `noexcept(false) above`
#if HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
    static_assert(static_cast<bool>(decltype_<trivially_copy_constructible>().satisfies(
                      mpl::noexcept_copy_constructible)),
                  "hyperion::mpl::noexcept_copy_constructible predicate test case 4 (failing)");
#else
    static_assert(
        not decltype_<trivially_copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
        "hyperion::mpl::noexcept_copy_constructible predicate test case 4 (failing)");
#endif // HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
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
// GCC 13 and below overrides our `noexcept(false) above`
#if HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
    static_assert(static_cast<bool>(decltype_<trivially_move_constructible>().satisfies(
                      mpl::noexcept_move_constructible)),
                  "hyperion::mpl::noexcept_move_constructible predicate test case 4 (failing)");
#else
    static_assert(
        not decltype_<trivially_move_constructible>().satisfies(mpl::noexcept_move_constructible),
        "hyperion::mpl::noexcept_move_constructible predicate test case 4 (failing)");
#endif // HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
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
// GCC 13 and below overrides our `noexcept(false) above`
#if HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
    static_assert(decltype_<trivially_copy_assignable>().satisfies(mpl::noexcept_copy_assignable),
                  "hyperion::mpl::noexcept_copy_assignable predicate test case 4 (failing)");
#else
    static_assert(static_cast<bool>(not decltype_<trivially_copy_assignable>().satisfies(
                      mpl::noexcept_copy_assignable)),
                  "hyperion::mpl::noexcept_copy_assignable predicate test case 4 (failing)");
#endif // HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
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
// GCC 13 and below overrides our `noexcept(false) above`
#if HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
    static_assert(decltype_<trivially_move_assignable>().satisfies(mpl::noexcept_move_assignable),
                  "hyperion::mpl::noexcept_move_assignable predicate test case 4 (failing)");
#else
    static_assert(static_cast<bool>(not decltype_<trivially_move_assignable>().satisfies(
                      mpl::noexcept_move_assignable)),
                  "hyperion::mpl::noexcept_move_assignable predicate test case 4 (failing)");
#endif // HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
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

    struct base_type { };

    template<typename T>
    struct assignable_from {
        auto operator=(T) -> assignable_from& {
            return *this;
        }
    };

    template<typename T>
    struct noexcept_assignable_from {
        auto operator=(T) -> noexcept_assignable_from& {
            return *this;
        }
    };

    static_assert(
        decltype_<assignable_from<int>>().satisfies(mpl::assignable_from(decltype_<int>())),
        "hyperion::mpl::assignable_from test case 1 (failing)");
    static_assert(decltype_<assignable_from<int>>().satisfies(
                      mpl::assignable_from(decltype_<assignable_from<int>>())),
                  "hyperion::mpl::assignable_from test case 2 (failing)");
    static_assert(not decltype_<assignable_from<int>>().satisfies(
                      mpl::assignable_from(decltype_<base_type>())),
                  "hyperion::mpl::assignable_from test case 3 (failing)");

    static_assert(decltype_<assignable_from<int>>().satisfies(
                      mpl::noexcept_assignable_from(decltype_<assignable_from<int>>())),
                  "hyperion::mpl::noexcept_assignable_from test case 1 (failing)");
    static_assert(not decltype_<assignable_from<int>>().satisfies(
                      mpl::noexcept_assignable_from(decltype_<int>())),
                  "hyperion::mpl::noexcept_assignable_from test case 2 (failing)");
    static_assert(not decltype_<assignable_from<int>>().satisfies(
                      mpl::noexcept_assignable_from(decltype_<base_type>())),
                  "hyperion::mpl::noexcept_assignable_from test case 3 (failing)");

    static_assert(decltype_<assignable_from<int>>().satisfies(
                      mpl::trivially_assignable_from(decltype_<assignable_from<int>>())),
                  "hyperion::mpl::trivially_assignable_from test case 1 (failing)");
    static_assert(not decltype_<assignable_from<int>>().satisfies(
                      mpl::trivially_assignable_from(decltype_<int>())),
                  "hyperion::mpl::trivially_assignable_from test case 2 (failing)");
    static_assert(not decltype_<assignable_from<int>>().satisfies(
                      mpl::trivially_assignable_from(decltype_<base_type>())),
                  "hyperion::mpl::trivially_assignable_from test case 3 (failing)");

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
// GCC 13 and below overrides our `noexcept(false) above`
#if HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
    static_assert(static_cast<bool>(
                      decltype_<trivially_destructible>().satisfies(mpl::noexcept_destructible)),
                  "hyperion::mpl::noexcept_destructible predicate test case 4 (failing)");
#else
    static_assert(not decltype_<trivially_destructible>().satisfies(mpl::noexcept_destructible),
                  "hyperion::mpl::noexcept_destructible predicate test case 4 (failing)");
#endif // HYPERION_PLATFORM_COMPILER_IS_GCC && __GNUC__ < 14
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

#endif // HYPERION_MPL_METAPREDICATES_TEST_H
