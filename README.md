# hyperion::mpl

hyperion::mpl is a C++20 meta programming library similar to Boost.Hana.

It's currently under active development and has not yet hit a stable release point.

### Quick Start

See the [Quick Start Guide](https://braxtons12.github.io/hyperion_mpl/quick_start.html)
for how to get started using hyperion::mpl.

### Documentation

You can also find the rest of the project documentation [here](https://braxtons12.github.io/hyperion_mpl)

### Example

After following the Quick Start Guide, you'll be able to use hyperion::mpl in your project.
A basic example of what you can do with hyperion::mpl is below:

```cpp
#include <hyperion/mpl/value.h>
#include <hyperion/mpl/type.h>

#include <concepts>

using namespace hyperion::mpl;

constexpr auto val1 = Value<4>{};
constexpr auto val2 = Value<2>{};

constexpr auto meaning_of_life = (val1 * 10_value) + val2;

static_assert(meaning_of_life == 42);

constexpr auto val3 = 10;
static_assert(std::same_as<typename decltype(decltype_(val3)
                                             .apply<std::remove_reference>()
                                             .apply<std::remove_const>()
                                             .apply<std::add_rvalue_reference>())::type,
                           int&&>);

template<ValueType TValue>
struct add_one {
    static inline constexpr auto value = TValue::value + 1;
};

template<ValueType TValue>
struct times_two {
    static inline constexpr auto value = TValue::value * 2;
};

static_assert(decltype_(2_value)
              .apply<add_one>()
              .apply<times_two>()
              .apply<add_one>() == 7);
```

### Contributing

Feel free to submit issues, pull requests, etc.!<br>
When contributing code, please follow the general style expectations of Hyperion projects:
- Follow the project `.clang-format` (except in judicious cases of templates or requires clauses
        ruining formatting),
- Use trailing returns types,
- Use assign-init (e.g. `auto var = value;`) instead of direct-init (e.g. `auto var{value}`),
- By default, prefer simplicity and correctness over performance

### License

hyperion::mpl uses the MIT license.

