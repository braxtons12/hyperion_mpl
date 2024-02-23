/// @file mpl.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Catch-all header for the Hyperion meta-programming library
/// @version 0.1
/// @date 2024-02-15
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

#ifndef HYPERION_MPL_H
#define HYPERION_MPL_H

/// @defgroup mpl Metaprogramming Library
/// Hyperion's metaprogramming library provides a full-featured set of basic
/// metaprogramming operations, inspired by Boost.Hana. It uses `constexpr` and value
/// semantics to make metaprogramming functional, simple, and (mostly) read and write
/// like normal code for users, instead of needing to rely on complex template-based
/// techniques that are difficult to reason about.
/// @headerfile "hyperion/mpl.h"

#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/metapredicates.h>
#include <hyperion/mpl/concepts.h>
#include <hyperion/mpl/index.h>
#include <hyperion/mpl/list.h>
#include <hyperion/mpl/pair.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/type_traits.h>
#include <hyperion/mpl/value.h>

#endif // HYPERION_MPL_H
