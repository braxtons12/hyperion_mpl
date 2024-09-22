/// @file main.cpp
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief simple example / test file
/// @version 0.1
/// @date 2024-09-21
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

#include <hyperion/mpl.h>
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
