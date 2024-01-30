/// @file type_traits.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief This file provides a small collection of type traits useful for meta-programming
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

#ifndef HYPERION_MPL_TYPE_TRAITS_H
#define HYPERION_MPL_TYPE_TRAITS_H

/// @ingroup mpl
/// @{
/// @defgroup type_traits Type Traits
/// Hyperion provides an assortment of metaprogramming type traits used to detect
/// various characteristics of given type(s). Some of these are purely novel,
/// such as detection of an operator overload, while others are supplemental to
/// those provided by the standard, doing something similar to existing traits,
/// but filling in gaps in the API surface that were overlooked or missing for
/// other reasons.
/// @headerfile hyperion/mpl/type_traits.h
/// @}

#include <hyperion/mpl/type_traits/is_comparable.h>
#include <hyperion/mpl/type_traits/is_operator_able.h>
#include <hyperion/mpl/type_traits/std_supplemental.h>

#endif // HYPERION_MPL_TYPE_TRAITS_H
