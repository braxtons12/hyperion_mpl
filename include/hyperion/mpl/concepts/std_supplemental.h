/// @file std_supplemental.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Supplemental C++20 concept definitions to those provided in
/// `#include <concepts>` to provide functionality missing from the standard
/// @version 0.1.1
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

#ifndef HYPERION_MPL_CONCEPTS_STD_SUPPLEMENTAL_H
#define HYPERION_MPL_CONCEPTS_STD_SUPPLEMENTAL_H

#include <hyperion/platform/def.h>
//
#include <hyperion/mpl/type_traits/std_supplemental.h>

HYPERION_IGNORE_DOCUMENTATION_WARNING_START;

/// @ingroup concepts
/// @{
/// @defgroup std_supplemental_concepts Standard Supplemental Concepts
/// Assortment of concept definitions to supplement those provided by the standard.
/// These are generally either similar to existing concepts, but implemented more or less
/// stringently to account for use-cases in which the standard implementations are incompatible,
/// completely novel concepts, filling in gaps in the API surface that the standard doesn't cover,
/// or concept definitions corresponding to standard type traits that have not had corresponding
/// concept definitions defined for them.
///
/// # Example:
/// @code {.cpp}
/// #include <hyperion/mpl/concepts/std_supplemental.h>
///
/// using namespace hyperion::mpl::concepts;
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
/// static_assert(TriviallyMovable<trivially_move_but_not_copyable>);
/// static_assert(!TriviallyMovable<std::vector<int>>);
///
/// @endcode
/// @headerfile hyperion/mpl/concepts/std_supplemental.h
/// @}

namespace hyperion::mpl::concepts {

    /// @brief Concept defining what a trivially movable type is.
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
    /// # Requirements
    /// - `TType` must be trivially move constructible
    /// - `TType` must be trivially move assignable
    ///
    /// @tparam TType The type to check
    /// @ingroup std_supplemental_traits
    /// @headerfile hyperion/mpl/type_traits/std_supplemental.h
    template<typename TType>
    concept TriviallyMovable = type_traits::is_trivially_movable_v<TType>;

    namespace _test {

        static_assert(TriviallyMovable<type_traits::_test::trivially_move_but_not_copyable>,
                      "hyperion::mpl::concepts::TriviallyMovable test case 1 (failing)");
        static_assert(!TriviallyMovable<type_traits::_test::not_trivially_movable>,
                      "hyperion::mpl::concepts::TriviallyMovable test case 2 (failing)");
    } // namespace _test
} // namespace hyperion::mpl::concepts

HYPERION_IGNORE_DOCUMENTATION_WARNING_STOP;

#endif // HYPERION_MPL_CONCEPTS_STD_SUPPLEMENTAL_H
