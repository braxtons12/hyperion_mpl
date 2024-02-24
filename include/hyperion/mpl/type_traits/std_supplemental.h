/// @file std_supplemental.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Supplemental type traits to those provided in `#include <type_traits>`
/// to provide functionality missing from the standard
/// @version 0.1
/// @date 2024-01-31
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

#ifndef HYPERION_MPL_TYPE_TRAITS_STD_SUPPLEMENTAL_H
#define HYPERION_MPL_TYPE_TRAITS_STD_SUPPLEMENTAL_H

#include <hyperion/platform/def.h>
#include <type_traits>

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

/// @ingroup type_traits
/// @{
/// @defgroup std_supplemental_traits Standard Supplemental Type Traits
/// Assortment of type traits to supplement  those provided by the standard.
/// These are generally either similar to existing traits, but implemented more or less stringently
/// to account for use-cases in which the standard implementations are incompatible, or completely
/// novel traits, filling in gaps in the API surface that the standard doesn't cover.
///
/// # Example:
/// @code {.cpp}
/// #include <hyperion/mpl/type_traits/std_supplemental.h>
///
/// using namespace hyperion::mpl::type_traits;
/// struct trivially_move_but_not_copyable {
///     trivially_move_but_not_copyable(const trivially_move_but_not_copyable&) = delete;
///     trivially_move_but_not_copyable(trivially_move_but_not_copyable&&) = default;
///
///     auto operator=(const trivially_move_but_not_copyable&)
///         -> trivially_move_but_not_copyable& = delete;
///     auto operator=(trivially_move_but_not_copyable&&)
///         -> trivially_move_but_not_copyable& = default;
/// };
///
/// static_assert(is_trivially_movable_v<trivially_move_but_not_copyable>);
/// static_assert(!is_trivially_movable_v<std::vector<int>>);
///
/// @endcode
/// @headerfile hyperion/mpl/type_traits/std_supplemental.h
/// @}

namespace hyperion::mpl::type_traits {

    /// @brief Type trait requiring that the type `TType` is trivially movable.
    /// A type that is trivially movable is both trivially move constructible
    /// and trivially move assignable.
    ///
    /// For a type to be trivially move constructible, the type must be move
    /// constructible, and its move constructor must either _not_ be user provided
    /// in any way, or explicitly defaulted by declaring it as `= default`.
    /// Similarly, for a type to be trivially move assignable, the type must be move
    /// assignable, and its move assignment operator must either _not_ be user provided
    /// in any way, or explicitly defaulted by declaring it as `= default`.
    ///
    /// @tparam TType The type to check
    /// @ingroup std_supplemental_traits
    /// @headerfile hyperion/mpl/type_traits/std_supplemental.h
    template<typename TType>
    struct is_trivially_movable
        : public std::bool_constant<std::is_trivially_move_constructible_v<TType>
                                    && std::is_trivially_move_assignable_v<TType>> { };

    /// @brief Value of the type trait `is_trivially_movable`.
    /// Requires that the type `TType` is trivially movable.
    /// A type that is trivially movable is both trivially move constructible
    /// and trivially move assignable.
    ///
    /// For a type to be trivially move constructible, the type must be move
    /// constructible, and its move constructor must either _not_ be user provided
    /// in any way, or explicitly defaulted by declaring it as `= default`.
    /// Similarly, for a type to be trivially move assignable, the type must be move
    /// assignable, and its move assignment operator must either _not_ be user provided
    /// in any way, or explicitly defaulted by declaring it as `= default`.
    ///
    /// @tparam TType The type to check
    /// @ingroup std_supplemental_traits
    /// @headerfile hyperion/mpl/type_traits/std_supplemental.h
    template<typename TType>
    static inline constexpr auto is_trivially_movable_v = is_trivially_movable<TType>::value;

    namespace _test {
        struct trivially_move_but_not_copyable {
            trivially_move_but_not_copyable() = default;
            ~trivially_move_but_not_copyable() = default;
            trivially_move_but_not_copyable(const trivially_move_but_not_copyable&) = delete;
            trivially_move_but_not_copyable(trivially_move_but_not_copyable&&) = default;

            auto operator=(const trivially_move_but_not_copyable&)
                -> trivially_move_but_not_copyable& = delete;
            auto operator=(trivially_move_but_not_copyable&&)
                -> trivially_move_but_not_copyable& = default;
        };

        // NOLINTNEXTLINE(*-special-member-functions)
        struct not_trivially_movable {

            auto operator=([[maybe_unused]] not_trivially_movable&& self) noexcept
                -> not_trivially_movable& {
                return *this;
            }
        };

        static_assert(is_trivially_movable_v<trivially_move_but_not_copyable>,
                      "hyperion::mpl::type_traits::is_trivially_movable test case 1 (failing)");
        static_assert(!is_trivially_movable_v<not_trivially_movable>,
                      "hyperion::mpl::type_traits::is_trivially_movable test case 2 (failing)");
    } // namespace _test
} // namespace hyperion::mpl::type_traits

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;

#endif // HYPERION_MPL_TYPE_TRAITS_STD_SUPPLEMENTAL_H
