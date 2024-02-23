/// @file metapredicates.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Callable definitions for common metaprogramming predicates
/// @version 0.1
/// @date 2024-02-22
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

#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/pair.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>
#include <hyperion/platform/def.h>

#include <concepts>
#include <type_traits>

#ifndef HYPERION_MPL_METAPREDICATES_H
    #define HYPERION_MPL_METAPREDICATES_H

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
/// constexpr auto add_const = [](MetaType auto type) noexcept {
///     return type.add_const();
/// };
///
/// constexpr auto example1 = List<int, double, float>{};
/// static_assert(example1.none_of(equal_to(decltype_<usize>())));
///
/// constexpr auto example2 = List<Value<1>, Value<2>, Value<3>>{};
/// static_assert(examle2.all_of(less_than(4_value)));
///
/// @endcode
/// @headerfile hyperion/mpl/metatypes.h
/// @}

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

namespace hyperion::mpl {

    [[nodiscard]] constexpr auto equal_to([[maybe_unused]] auto value) noexcept
        requires MetaValue<decltype(value)> || MetaType<decltype(value)>
                 || MetaPair<decltype(value)>
    {
        return [](auto element)
            requires MetaValue<decltype(element)> || MetaType<decltype(element)>
                     || MetaPair<decltype(element)>
        {
            if constexpr((MetaValue<decltype(element)> && MetaValue<decltype(value)>)
                         || (MetaType<decltype(element)> && MetaType<decltype(value)>)
                         || (MetaPair<decltype(element)> && MetaPair<decltype(value)>))
            {
                return Value<(decltype(element){} == decltype(value){}), bool>{};
            }
            else {
                return Value<false>{};
            }
        };
    }

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

    [[nodiscard]] constexpr auto less_than([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return Value<(decltype(element){} < decltype(value){}), bool>{};
        };
    }

    [[nodiscard]] constexpr auto
    less_than_or_equal_to([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return Value<(decltype(element){} <= decltype(value){}), bool>{};
        };
    }

    [[nodiscard]] constexpr auto greater_than([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return Value<(decltype(element){} > decltype(value){}), bool>{};
        };
    }

    [[nodiscard]] constexpr auto
    greater_than_or_equal_to([[maybe_unused]] MetaValue auto value) noexcept {
        return [](MetaValue auto element) {
            return Value<decltype(element){} >= decltype(value){}, bool>{};
        };
    }

    [[nodiscard]] constexpr auto is(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is(decltype(type){});
        };
    }

    [[nodiscard]] constexpr auto qualification_of(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_qualification_of(decltype(type){});
        };
    }

    constexpr auto is_const = [](MetaType auto type) noexcept {
        return type.is_const();
    };

    constexpr auto is_lvalue_reference = [](MetaType auto type) noexcept {
        return type.is_lvalue_reference();
    };

    constexpr auto is_rvalue_reference = [](MetaType auto type) noexcept {
        return type.is_rvalue_reference();
    };

    constexpr auto is_volatile = [](MetaType auto type) noexcept {
        return type.is_volatile();
    };

    [[nodiscard]] constexpr auto convertible_to(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_convertible_to(decltype(type){});
        };
    }

    [[nodiscard]] constexpr auto derived_from(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_derived_from(decltype(type){});
        };
    }

    [[nodiscard]] constexpr auto base_of(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_base_of(decltype(type){});
        };
    }

    [[nodiscard]] constexpr auto constructible_from(MetaType auto... types) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_constructible_from(decltype(types){}...);
        };
    }

    [[nodiscard]] constexpr auto noexcept_constructible_from(MetaType auto... types) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_noexcept_constructible_from(decltype(types){}...);
        };
    }

    constexpr auto default_constructible = [](MetaType auto type) noexcept {
        return type.is_default_constructible();
    };

    constexpr auto noexcept_default_constructible = [](MetaType auto type) noexcept {
        return type.is_noexcept_default_constructible();
    };

    constexpr auto trivially_default_constructible = [](MetaType auto type) noexcept {
        return type.is_trivially_default_constructible();
    };

    constexpr auto copy_constructible = [](MetaType auto type) noexcept {
        return type.is_copy_constructible();
    };

    constexpr auto noexcept_copy_constructible = [](MetaType auto type) noexcept {
        return type.is_noexcept_copy_constructible();
    };

    constexpr auto trivially_copy_constructible = [](MetaType auto type) noexcept {
        return type.is_trivially_copy_constructible();
    };

    constexpr auto move_constructible = [](MetaType auto type) noexcept {
        return type.is_move_constructible();
    };

    constexpr auto noexcept_move_constructible = [](MetaType auto type) noexcept {
        return type.is_noexcept_move_constructible();
    };

    constexpr auto trivially_move_constructible = [](MetaType auto type) noexcept {
        return type.is_trivially_move_constructible();
    };

    constexpr auto copy_assignable = [](MetaType auto type) noexcept {
        return type.is_copy_assignable();
    };

    constexpr auto noexcept_copy_assignable = [](MetaType auto type) noexcept {
        return type.is_noexcept_copy_assignable();
    };

    constexpr auto trivially_copy_assignable = [](MetaType auto type) noexcept {
        return type.is_trivially_copy_assignable();
    };

    constexpr auto move_assignable = [](MetaType auto type) noexcept {
        return type.is_move_assignable();
    };

    constexpr auto noexcept_move_assignable = [](MetaType auto type) noexcept {
        return type.is_noexcept_move_assignable();
    };

    constexpr auto trivially_move_assignable = [](MetaType auto type) noexcept {
        return type.is_trivially_move_assignable();
    };

    constexpr auto destructible = [](MetaType auto type) noexcept {
        return type.is_destructible();
    };

    constexpr auto noexcept_destructible = [](MetaType auto type) noexcept {
        return type.is_noexcept_destructible();
    };

    constexpr auto trivially_destructible = [](MetaType auto type) noexcept {
        return type.is_trivially_destructible();
    };

    constexpr auto swappable = [](MetaType auto type) noexcept {
        return type.is_swappable();
    };

    constexpr auto noexcept_swappable = [](MetaType auto type) noexcept {
        return type.is_noexcept_swappable();
    };

    [[nodiscard]] constexpr auto swappable_with(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_swappable_with(decltype(type){});
        };
    }

    [[nodiscard]] constexpr auto noexcept_swappable_with(MetaType auto type) noexcept {
        return [](MetaType auto element) noexcept {
            return element.is_noexcept_swappable_with(decltype(type){});
        };
    }

    namespace _test::metapredicates {

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
        static_assert(
            not decltype_<not_convertible>().satisfies(convertible_to(decltype_<float>())),
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
        static_assert(
            not decltype_<constructible>().satisfies(constructible_from(decltype_<int>())),
            "hyperion::mpl::constructible_from predicate test case 3 (failing)");

        static_assert(
            decltype_<constructible>().satisfies(noexcept_constructible_from(decltype_<float>(),
                                                                             decltype_<double>(),
                                                                             decltype_<int>())),
            "hyperion::mpl::noexcept_constructible_from predicate test case 1 (failing)");
        static_assert(not decltype_<constructible>().satisfies(
                          noexcept_constructible_from(decltype_<int>(),
                                                      decltype_<float>(),
                                                      decltype_<double>())),
                      "hyperion::mpl::noexcept_constructible_from predicate test case 2 (failing)");
        static_assert(
            not decltype_<constructible>().satisfies(noexcept_constructible_from(decltype_<int>())),
            "hyperion::mpl::noexcept_constructible_from predicate test case 3 (failing)");

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
        static_assert(
            decltype_<noexcept_default_constructible>().satisfies(mpl::default_constructible),
            "hyperion::mpl::default_constructible predicate test case 3 (failing)");
        static_assert(
            decltype_<trivially_default_constructible>().satisfies(mpl::default_constructible),
            "hyperion::mpl::default_constructible predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_default_constructible>().satisfies(mpl::default_constructible),
            "hyperion::mpl::default_constructible predicate test case 5 (failing)");

        static_assert(
            decltype_<int>().satisfies(mpl::noexcept_default_constructible),
            "hyperion::mpl::noexcept_default_constructible predicate test case 1 (failing)");
        static_assert(
            not decltype_<default_constructible>().satisfies(mpl::noexcept_default_constructible),
            "hyperion::mpl::noexcept_default_constructible predicate test case 2 (failing)");
        static_assert(
            decltype_<noexcept_default_constructible>().satisfies(
                mpl::noexcept_default_constructible),
            "hyperion::mpl::noexcept_default_constructible predicate test case 3 (failing)");
        static_assert(
            // MSVC and GCC override our `noexcept(false) above`
            HYPERION_PLATFORM_COMPILER_IS_CLANG ?
                static_cast<bool>(not decltype_<trivially_default_constructible>().satisfies(
                    mpl::noexcept_default_constructible)) :
                decltype_<trivially_default_constructible>().satisfies(
                    mpl::noexcept_default_constructible),
            "hyperion::mpl::noexcept_default_constructible predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_default_constructible>().satisfies(
                mpl::noexcept_default_constructible),
            "hyperion::mpl::noexcept_default_constructible predicate test case 5 (failing)");

        static_assert(
            decltype_<int>().satisfies(mpl::trivially_default_constructible),
            "hyperion::mpl::trivially_default_constructible predicate test case 1 (failing)");
        static_assert(
            not decltype_<default_constructible>().satisfies(mpl::trivially_default_constructible),
            "hyperion::mpl::trivially_default_constructible predicate test case 2 (failing)");
        static_assert(
            not decltype_<noexcept_default_constructible>().satisfies(
                mpl::trivially_default_constructible),
            "hyperion::mpl::trivially_default_constructible predicate test case 3 (failing)");
        static_assert(
            decltype_<trivially_default_constructible>().satisfies(
                mpl::trivially_default_constructible),
            "hyperion::mpl::trivially_default_constructible predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_default_constructible>().satisfies(
                mpl::trivially_default_constructible),
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
            trivially_copy_constructible(const trivially_copy_constructible&) noexcept(false)
                = default;
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
        static_assert(
            not decltype_<copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
            "hyperion::mpl::noexcept_copy_constructible predicate test case 2 (failing)");
        static_assert(
            decltype_<noexcept_copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
            "hyperion::mpl::noexcept_copy_constructible predicate test case 3 (failing)");
        static_assert(
            // MSVC and GCC override our `noexcept(false) above`
            HYPERION_PLATFORM_COMPILER_IS_CLANG ?
                static_cast<bool>(not decltype_<trivially_copy_constructible>().satisfies(
                    mpl::noexcept_copy_constructible)) :
                decltype_<trivially_copy_constructible>().satisfies(
                    mpl::noexcept_copy_constructible),
            "hyperion::mpl::noexcept_copy_constructible predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_copy_constructible>().satisfies(mpl::noexcept_copy_constructible),
            "hyperion::mpl::noexcept_copy_constructible predicate test case 5 (failing)");

        static_assert(
            decltype_<int>().satisfies(mpl::trivially_copy_constructible),
            "hyperion::mpl::trivially_copy_constructible predicate test case 1 (failing)");
        static_assert(
            not decltype_<copy_constructible>().satisfies(mpl::trivially_copy_constructible),
            "hyperion::mpl::trivially_copy_constructible predicate test case 2 (failing)");
        static_assert(
            not decltype_<noexcept_copy_constructible>().satisfies(
                mpl::trivially_copy_constructible),
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
        static_assert(
            not decltype_<move_constructible>().satisfies(mpl::noexcept_move_constructible),
            "hyperion::mpl::noexcept_move_constructible predicate test case 2 (failing)");
        static_assert(
            decltype_<noexcept_move_constructible>().satisfies(mpl::noexcept_move_constructible),
            "hyperion::mpl::noexcept_move_constructible predicate test case 3 (failing)");
        static_assert(
            // MSVC and GCC override our `noexcept(false) above`
            HYPERION_PLATFORM_COMPILER_IS_CLANG ?
                static_cast<bool>(not decltype_<trivially_move_constructible>().satisfies(
                    mpl::noexcept_move_constructible)) :
                decltype_<trivially_move_constructible>().satisfies(
                    mpl::noexcept_move_constructible),
            "hyperion::mpl::noexcept_move_constructible predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_move_constructible>().satisfies(mpl::noexcept_move_constructible),
            "hyperion::mpl::noexcept_move_constructible predicate test case 5 (failing)");

        static_assert(
            decltype_<int>().satisfies(mpl::trivially_move_constructible),
            "hyperion::mpl::trivially_move_constructible predicate test case 1 (failing)");
        static_assert(
            not decltype_<move_constructible>().satisfies(mpl::trivially_move_constructible),
            "hyperion::mpl::trivially_move_constructible predicate test case 2 (failing)");
        static_assert(
            not decltype_<noexcept_move_constructible>().satisfies(
                mpl::trivially_move_constructible),
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
        static_assert(
            decltype_<noexcept_copy_assignable>().satisfies(mpl::noexcept_copy_assignable),
            "hyperion::mpl::noexcept_copy_assignable predicate test case 3 (failing)");
        static_assert(
            // MSVC and GCC override our `noexcept(false) above`
            HYPERION_PLATFORM_COMPILER_IS_CLANG ?
                static_cast<bool>(not decltype_<trivially_copy_assignable>().satisfies(
                    mpl::noexcept_copy_assignable)) :
                decltype_<trivially_copy_assignable>().satisfies(mpl::noexcept_copy_assignable),
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
        static_assert(
            decltype_<trivially_copy_assignable>().satisfies(mpl::trivially_copy_assignable),
            "hyperion::mpl::trivially_copy_assignable predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_copy_assignable>().satisfies(mpl::trivially_copy_assignable),
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
        static_assert(
            decltype_<noexcept_move_assignable>().satisfies(mpl::noexcept_move_assignable),
            "hyperion::mpl::noexcept_move_assignable predicate test case 3 (failing)");
        static_assert(
            // MSVC and GCC override our `noexcept(false) above`
            HYPERION_PLATFORM_COMPILER_IS_CLANG ?
                static_cast<bool>(not decltype_<trivially_move_assignable>().satisfies(
                    mpl::noexcept_move_assignable)) :
                decltype_<trivially_move_assignable>().satisfies(mpl::noexcept_move_assignable),
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
        static_assert(
            decltype_<trivially_move_assignable>().satisfies(mpl::trivially_move_assignable),
            "hyperion::mpl::trivially_move_assignable predicate test case 4 (failing)");
        static_assert(
            not decltype_<not_move_assignable>().satisfies(mpl::trivially_move_assignable),
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
            HYPERION_PLATFORM_COMPILER_IS_CLANG ?
                static_cast<bool>(
                    not decltype_<trivially_destructible>().satisfies(mpl::noexcept_destructible)) :
                decltype_<trivially_destructible>().satisfies(mpl::noexcept_destructible),
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
        static_assert(not decltype_<not_swappable>().satisfies(
                          mpl::swappable_with(decltype_<not_swappable>())),
                      "hyperion::mpl::swappable_with predicate test case 4 (failing)");
        static_assert(decltype_<swappable_with>().satisfies(mpl::swappable_with(decltype_<int>())),
                      "hyperion::mpl::swappable_with predicate test case 5 (failing)");
        static_assert(
            decltype_<noexcept_swappable_with>().satisfies(mpl::swappable_with(decltype_<int>())),
            "hyperion::mpl::swappable_with predicate test case 6 (failing)");
        static_assert(
            not decltype_<not_swappable>().satisfies(mpl::swappable_with(decltype_<int>())),
            "hyperion::mpl::swappable_with predicate test case 7 (failing)");

        static_assert(decltype_<int>().satisfies(mpl::noexcept_swappable_with(decltype_<int>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 1 (failing)");
        static_assert(decltype_<noexcept_swappable>().satisfies(
                          mpl::noexcept_swappable_with(decltype_<noexcept_swappable>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 2 (failing)");
        static_assert(not decltype_<swappable>().satisfies(
                          mpl::noexcept_swappable_with(decltype_<swappable>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 3 (failing)");
        static_assert(not decltype_<not_swappable>().satisfies(
                          mpl::noexcept_swappable_with(decltype_<not_swappable>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 4 (failing)");
        static_assert(decltype_<noexcept_swappable_with>().satisfies(
                          mpl::noexcept_swappable_with(decltype_<int>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 5 (failing)");
        static_assert(not decltype_<swappable_with>().satisfies(
                          mpl::noexcept_swappable_with(decltype_<int>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 6 (failing)");
        static_assert(not decltype_<not_swappable>().satisfies(
                          mpl::noexcept_swappable_with(decltype_<int>())),
                      "hyperion::mpl::noexcept_swappable_with predicate test case 7 (failing)");
    } // namespace _test::metapredicates
} // namespace hyperion::mpl

#endif // HYPERION_MPL_METAPREDICATES_H
