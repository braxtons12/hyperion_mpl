/// @file std_supplemental.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Supplemental type traits to those provided in `#include <type_traits>`
/// to provide functionality missing from the standard
/// @version 0.1
/// @date 2024-01-27
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

#include <type_traits>

namespace hyperion::mpl::type_traits {

    template<typename TType>
    struct is_trivially_movable
        : public std::bool_constant<std::is_trivially_move_constructible_v<TType>
                                    && std::is_trivially_move_assignable_v<TType>> { };

    template<typename TType>
    static inline constexpr auto is_trivially_movable_v = is_trivially_movable<TType>::value;
} // namespace hyperion::mpl::type_traits

#endif // HYPERION_MPL_TYPE_TRAITS_STD_SUPPLEMENTAL_H
