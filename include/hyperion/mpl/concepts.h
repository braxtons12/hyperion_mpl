/// @file concepts.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming C++20 concept definitions
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

#ifndef HYPERION_MPL_CONCEPTS_H
#define HYPERION_MPL_CONCEPTS_H

/// @ingroup mpl
/// @{
/// @defgroup concepts Concepts
/// Hyperion provides an assortment of metaprogramming `concept` definitions used to
/// define various conceptual models and characteristics of types.
/// Some of these are purely novel, such as comparability, while others are supplemental
/// to, or less stringent than, those provided by the standard, doing something similar
/// to existing `concept`s, but filling in gaps in the API surface that were overlooked
/// or missing for other reasons, or provided an alternative definition that is narrower
/// in scope.
/// @headerfile hyperion/mpl/concepts.h
/// @}

#include <hyperion/mpl/concepts/comparable.h>
#include <hyperion/mpl/concepts/operator_able.h>
#include <hyperion/mpl/concepts/std_supplemental.h>

#endif // HYPERION_MPL_CONCEPTS_H
