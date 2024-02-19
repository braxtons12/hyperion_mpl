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
#include <hyperion/mpl/list.h>
#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>

#include <concepts>

using namespace hyperion;
using namespace hyperion::mpl;

constexpr auto add_const = [](MetaType auto type) noexcept {
    return type.add_const();
};

constexpr auto list = List<int, double, float>{};
constexpr auto zipped = list.zip(List<u32, usize, i32>{});
contexpr auto constified = zipped.apply(add_const);

static_assert(constified == List<Pair<const int, const u32>,
                                 Pair<const double, const usize>,
                                 Pair<const float, const i32>>{});

constexpr auto val1 = Value<4>{};
constexpr auto val2 = Value<2>{};

constexpr auto meaning_of_life = (val1 * 10_value) + val2;

static_assert(meaning_of_life == 42);

template<ValueType TValue>
struct add_one {
    static inline constexpr auto value = TValue::value + 1;
};

template<ValueType TValue>
struct times_two {
    static inline constexpr auto value = TValue::value * 2;
};

static_assert(2_value
              .apply<add_one>()
              .apply<times_two>()
              .apply<add_one>() == 7);

constexpr auto val3 = 10;
static_assert(decltype_(val3)
              .apply<std::remove_reference>()
              .apply<std::remove_const>()
              .apply<std::add_rvalue_reference>()
              == decltype_<int&&>());

constexpr auto add_const = [](MetaType auto type)
    -> std::add_const<typename decltype(type)::type>
{
    return {};
};

constexpr auto add_lvalue_reference = [](MetaType auto type)
    -> std::add_lvalue_reference<typename decltype(type)::type>
{
    return {};
};

constexpr auto remove_reference = [](MetaType auto type)
    -> std::remove_reference<typename decltype(type)::type>
{
    return {};
};

static_assert(decltype_<int&&>()
              .apply(remove_reference)
              .apply(add_const)
              .apply(add_lvalue_reference)
              == decltype_<const int&>());
```

### Contributing

Feel free to submit issues, pull requests, etc.!<br>
When contributing code, please follow the general style expectations of Hyperion projects:
- Follow the project `.clang-format` (except in judicious cases of templates or requires clauses
        ruining formatting),
- Use trailing returns types,
- Use assign-init (e.g. `auto var = value;`) instead of direct-init (e.g. `auto var{value}`),
- By default, prefer simplicity and correctness over performance
- We try to target Clang/LLVM 15, which doesn't yet support usage of concepts in function
definitions occurring after the declaration (for example, defining a function template outside of
the class it was declared in). In these cases, revert to using `enable_if` instead.

### License

hyperion::mpl uses the MIT license.

