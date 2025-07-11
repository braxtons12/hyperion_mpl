/// @file test_main.cpp
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief simple example / test file
/// @version 0.1
/// @date 2025-07-10
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

#include "concepts/comparable.h" // NOLINT(misc-include-cleaner)
#include "concepts/operator_able.h" // NOLINT(misc-include-cleaner)
#include "list.h" // NOLINT(misc-include-cleaner)
#include "metapredicates.h" // NOLINT(misc-include-cleaner)
#include "metatypes.h" // NOLINT(misc-include-cleaner)
#include "pair.h" // NOLINT(misc-include-cleaner)
#include "type.h" // NOLINT(misc-include-cleaner)
#include "type_traits/is_comparable.h" // NOLINT(misc-include-cleaner)
#include "type_traits/is_operator_able.h" // NOLINT(misc-include-cleaner)
#include "value.h" // NOLINT(misc-include-cleaner)

#include <hyperion/mpl/list.h>
#include <hyperion/mpl/metapredicates.h>
#include <hyperion/mpl/metatypes.h>
#include <hyperion/mpl/value.h>
#include <hyperion/platform/types.h>

using namespace hyperion;      // NOLINT(google-build-using-namespace)
using namespace hyperion::mpl; // NOLINT(google-build-using-namespace)

[[nodiscard]] auto
main([[maybe_unused]] i32 argc, [[maybe_unused]] const char* const* argv) -> i32 {
    constexpr auto add_const = [](MetaType auto type) noexcept {
        return type.as_const();
    };

    return List<i32, usize, f32>{}
        .zip(List<f64, i64, u32>{})
        .apply(add_const)
        .all_of(is_const)
        .apply([](MetaValue auto value) {
            if constexpr(decltype(value)::value) {
                return Value<0, i32>{};
            }
            else {
                return Value<-1, i32>{};
            }
        });
}
