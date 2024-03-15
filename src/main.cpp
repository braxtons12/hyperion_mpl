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
