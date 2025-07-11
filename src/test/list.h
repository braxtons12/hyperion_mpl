/// @file list.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Tests for list.h
/// @version 0.8.1
/// @date 2025-07-08
///
/// MIT License
/// @copyright Copyright (c) 2025 Braxton Salyer <braxtonsalyer@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.

#ifndef HYPERION_MPL_LIST_TEST_H
#define HYPERION_MPL_LIST_TEST_H

#include <hyperion/mpl/list.h>

#if __has_include(<ranges>)
    #include <ranges>
#endif // __has_include(<ranges>)

namespace hyperion::mpl::_test::list {

    static_assert(List<int, double>{}.size() == 2,
                  "hyperion::mpl::List::size test case 1 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>>{}.size() == 4,
                  "hyperion::mpl::List::size test case 2 (failing)");

    static_assert(List<>{}.is_empty(), "hyperion::mpl::List::is_empty test case 1 (failing)");
    static_assert(not List<int>{}.is_empty(),
                  "hyperion::mpl::List::is_empty test case 2 (failing)");

    static constexpr auto add_const = [](MetaType auto type) {
        return type.as_const();
    };

    static_assert(
        std::same_as<decltype(List<int, double>{}.apply(add_const)), List<const int, const double>>,
        "hyperion::mpl::List::apply test case 1 (failing)");

    static_assert(List<int, double>{}.at<0>() == decltype_<int>(),
                  "hyperion::mpl::List::at test case 1 (failing)");
    static_assert(List<int, double>{}.at<1>() == decltype_<double>(),
                  "hyperion::mpl::List::at test case 2 (failing)");

    static_assert(std::same_as<decltype(List<int, double>{}.zip(List<double, int>{})),
                               List<Pair<int, double>, Pair<double, int>>>,
                  "hyperion::mpl::List::zip test case 1 (failing)");
    static_assert(List<int, double>{}.zip(List<float, usize>{}).apply(add_const)
                      == List<Pair<const int, const float>, Pair<const double, const usize>>{},
                  "hyperion::mpl::List::zip test case 2 (failing)");
    static_assert(List<int, double>{}.zip(List<float, usize>{}).apply(add_const).all_of(is_const),
                  "hyperion::mpl::List::zip test case 3 (failing)");

    static_assert(List<int, double>{} == List<int, double>{},
                  "hyperion::mpl::List operator== test case 1 (failing)");
    static_assert(List<int, double>{}.apply(add_const) == List<const int, const double>{},
                  "hyperion::mpl::List operator== test case 2 (failing)");
    static_assert(List<int, double>{}.zip(List<double, int>{})
                      == List<Pair<int, double>, Pair<double, int>>{},
                  "hyperion::mpl::List operator== test case 3 (failing)");

    constexpr auto test_metavalue = 1_value;
    constexpr auto test_metatype = decltype_<int>();
    constexpr auto test_value = "a string";

    static_assert(make_list(1, 1.0, 2_usize) == List<int, double, usize>{},
                  "hyperion::mpl::make_list test case 1 (failing)");
    static_assert(make_list(test_metavalue, test_metatype, test_value)
                      == List<Value<1_usize>, int, const char* const&>{},
                  "hyperion::mpl::make_list test case 2 (failing)");
    static_assert(make_list<int, double, Value<1>>() == List<int, double, Value<1>>{},
                  "hyperion::mpl::make_list test case 3 (failing)");
    static_assert(make_list<int, double, Type<usize>>() == List<int, double, usize>{},
                  "hyperion::mpl::make_list test case 4 (failing)");

    static_assert(List<int, double>{}.front() == decltype_<int>(),
                  "hyperion::mpl::List::front test (failing)");

    static_assert(List<int, double>{}.back() == decltype_<double>(),
                  "hyperion::mpl::List::back test (failing)");

    static_assert(List<int, double>{}.push_front(decltype_<float>()) == List<float, int, double>{},
                  "hyperion::mpl::List::push_front test case 1 (failing)");
    static_assert(List<int, double>{}.push_front(1_value) == List<Value<1>, int, double>{},
                  "hyperion::mpl::List::push_front test case 2 (failing)");
    static_assert(List<int, double>{}.push_front(Pair<int, double>{})
                      == List<Pair<int, double>, int, double>{},
                  "hyperion::mpl::List::push_front test case 3 (failing)");
    static_assert(List<int, double>{}.push_front(List<float, usize>{})
                      == List<float, usize, int, double>{},
                  "hyperion::mpl::List::push_front test case 4 (failing)");

    static_assert(List<int, double>{}.push_back(decltype_<float>()) == List<int, double, float>{},
                  "hyperion::mpl::List::push_back test case 1 (failing)");
    static_assert(List<int, double>{}.push_back(1_value) == List<int, double, Value<1>>{},
                  "hyperion::mpl::List::push_back test case 2 (failing)");
    static_assert(List<int, double>{}.push_back(Pair<int, double>{})
                      == List<int, double, Pair<int, double>>{},
                  "hyperion::mpl::List::push_back test case 3 (failing)");
    static_assert(List<int, double>{}.push_back(List<float, usize>{})
                      == List<int, double, float, usize>{},
                  "hyperion::mpl::List::push_back test case 4 (failing)");

    static_assert(List<int, double, float>{}.pop_front() == List<double, float>{},
                  "hyperion::mpl::List::pop_front test case 1 (failing)");
    static_assert(List<Value<1>, int, double>{}.pop_front() == List<int, double>{},
                  "hyperion::mpl::List::pop_front test case 2 (failing)");

    static_assert(List<int, double, float>{}.pop_back() == List<int, double>{},
                  "hyperion::mpl::List::pop_back test case 1 (failing)");
    static_assert(List<int, double, Value<1>>{}.pop_back() == List<int, double>{},
                  "hyperion::mpl::List::pop_back test case 2 (failing)");

    [[nodiscard]] constexpr auto test_for_each() noexcept -> bool {
        constexpr auto list = List<int, double, float, int>{};

        auto num_ints = 0;
        list.for_each([&num_ints](auto type) {
            if constexpr(MetaType<decltype(type)>) {
                if(type == decltype_<int>()) {
                    num_ints++;
                }
            }
        });

        return num_ints == 2;
    }

    static_assert(test_for_each(), "hyperion::mpl::List::for_each test (failing)");

    [[nodiscard]] constexpr auto test_for_each_n1() noexcept -> bool {
        constexpr auto list = List<int, double, float, int>{};

        auto num_ints = 0;
        list.for_each_n(
            [&num_ints](auto type) {
                if constexpr(MetaType<decltype(type)>) {
                    if(type == decltype_<int>()) {
                        num_ints++;
                    }
                }
            },
            3_value);

        return num_ints == 1;
    }

    [[nodiscard]] constexpr auto test_for_each_n2() noexcept -> bool {
        constexpr auto list = List<int, double, float, int>{};

        auto num_ints = 0;
        list.for_each_n(
            [&num_ints](auto type) {
                if constexpr(MetaType<decltype(type)>) {
                    if(type == decltype_<int>()) {
                        num_ints++;
                    }
                }
            },
            0_value);

        return num_ints == 0;
    }

    static_assert(test_for_each_n1(), "hyperion::mpl::List::for_each_n test case 1 (failing)");
    static_assert(test_for_each_n2(), "hyperion::mpl::List::for_each_n test case 2 (failing)");

    static_assert(List<int, double, float>{}.find_if([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<double>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }) == decltype_<double>(),
                  "hyperion::mpl::List::find_if test case 1 (failing)");
    static_assert(List<int, double, float>{}.find_if([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<usize>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }) == decltype_<not_found_tag>(),
                  "hyperion::mpl::List::find_if test case 2 (failing)");
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.find_if(is_const)
                      == decltype_<not_found_tag>(),
                  "hyperion::mpl::List::find_if test case 3 (failing)");

    static_assert(List<int, double, float>{}.all_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<std::is_arithmetic_v<typename decltype(type)::type>, bool>{};
        }
        else {
            return Value<false, bool>{};
        }
    }),
                  "hyperion::mpl::List::all_of test case 1 (failing)");
    static_assert(not List<int, double, float, Value<1>>{}.all_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<std::is_arithmetic_v<typename decltype(type)::type>, bool>{};
        }
        else {
            return Value<false, bool>{};
        }
    }),
                  "hyperion::mpl::List::all_of test case 2 (failing)");
    static_assert(not List<Value<1>, Value<2>, Value<3>>{}.all_of(is_const),
                  "hyperion::mpl::List::all_of test case 3 (failing)");

    static constexpr auto are_integral = [](MetaList auto list) {
        return list.all_of(
            [](MetaType auto type) { return type.template satisfies<std::is_integral>(); });
    };

    static_assert(List<List<bool>, List<i32>, List<u16>>{}.all_of(are_integral),
                  "hyperion::mpl::List::all_of test case 4 (failing)");
    static_assert(not List<List<float>, List<double>, List<u16>>{}.all_of(are_integral),
                  "hyperion::mpl::List::all_of test case 5 (failing)");

    static_assert(List<int, double, float>{}.any_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<double>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::any_of test case 1 (failing)");
    static_assert(not List<int, double, float>{}.any_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<usize>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::any_of test case 2 (failing)");
    static_assert(not List<Value<1>, Value<2>, Value<3>>{}.any_of(is_const),
                  "hyperion::mpl::List::any_of test case 3 (failing)");

    static_assert(List<int, double, float>{}.none_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<usize>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::none_of test case 1 (failing)");
    static_assert(not List<int, double, float>{}.none_of([](auto type) {
        if constexpr(MetaType<decltype(type)>) {
            return Value<decltype(type){} == decltype_<double>(), bool>{};
        }
        else {
            return Value<false>{};
        }
    }),
                  "hyperion::mpl::List::none_of test case 2 (failing)");
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.none_of(is_const),
                  "hyperion::mpl::List::none_of test case 3 (failing)");

    static_assert(List<Value<1>, Value<2>, Value<3>>{}.accumulate(0_value) == 6_value,
                  "hyperion::mpl::List::accumulate test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.accumulate(0_value,
                                                                  [](auto state, auto val) {
                                                                      return state + val;
                                                                  })
                      == 8_value,
                  "hyperion::mpl::List::accumulate test case 2 (failing)");

    static_assert(List<Value<1>, Value<2>, Value<3>>{}.accumulate(0_value) == 6_value,
                  "hyperion::mpl::List::accumulate test case 3 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.accumulate(0_value,
                                                                  [](auto state, auto val) {
                                                                      return state + val;
                                                                  })
                      == 8_value,
                  "hyperion::mpl::List::accumulate test case 4 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count_if([](auto val) {
        return val == 3_value;
    }) == 2_value,
                  "hyperion::mpl::List::count_if test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count_if([](auto val) {
        return val == 4_value;
    }) == 0_value,
                  "hyperion::mpl::List::count_if test case 2 (failing)");
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.count_if(is_const) == 0_value,
                  "hyperion::mpl::List::count_if test case 3 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count(3_value) == 2_value,
                  "hyperion::mpl::List::count test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<3>>{}.count(4_value) == 0_value,
                  "hyperion::mpl::List::count test case 2 (failing)");

    static_assert(List<Value<3>, int, double, Value<2>>{}.contains(2_value),
                  "hyperion::mpl::List::contains test case 1 (failing)");
    static_assert(List<Value<3>, int, double, Value<2>>{}.contains(decltype_<int>()),
                  "hyperion::mpl::List::contains test case 2 (failing)");
    static_assert(not List<Value<3>, int, double, Value<2>>{}.contains(decltype_<float>()),
                  "hyperion::mpl::List::contains test case 3 (failing)");
    static_assert(not List<Value<3>, int, double, Value<2>>{}.contains(4_value),
                  "hyperion::mpl::List::contains test case 4 (failing)");

    static_assert(List<Value<3>, Value<2>, Value<4>>{}.index_if(greater_than(3_value)) == 2_value,
                  "hyperion::mpl::List::index_if test case 1 (failing)");
    static_assert(List<Value<3>, Value<2>, Value<4>>{}.index_if(less_than(3_value)) == 1_value,
                  "hyperion::mpl::List::index_if test case 2 (failing)");
    static_assert(List<Value<1>, Value<2>, Value<3>>{}.index_if(is_const) == 3_value,
                  "hyperion::mpl::List::count_if test case 3 (failing)");

    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(decltype_<double>()) == 2_value,
                  "hyperion::mpl::List::index_of test case 1 (failing)");
    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(decltype_<int>()) == 1_value,
                  "hyperion::mpl::List::index_of test case 2 (failing)");
    static_assert(List<Value<3>, int, double, Value<4>>{}.index_of(4_value) == 3_value,
                  "hyperion::mpl::List::index_of test case 3 (failing)");

    constexpr auto is_value = []([[maybe_unused]] MetaValue auto value) noexcept {
        return Value<true>{};
    };

    static_assert(List<int, const double, float>{}.filter(is_const) == List<const double>{},
                  "hyperion::mpl::List::filter test case 1 (failing)");
    static_assert(List<int, double, float>{}.filter(is_const) == List<>{},
                  "hyperion::mpl::List::filter test case 2 (failing)");
    static_assert(List<int, double, const float>{}.filter(is_const) == List<const float>{},
                  "hyperion::mpl::List::filter test case 3 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>, float>{}.filter(is_value)
                      == List<Value<1>, Value<2>>{},
                  "hyperion::mpl::List::filter test case 4 (failing)");

    static_assert(List<int, const double, float>{}.remove_if(is_const) == List<int, float>{},
                  "hyperion::mpl::List::remove_if test case 1 (failing)");
    static_assert(List<int, double, float>{}.remove_if(is_const) == List<int, double, float>{},
                  "hyperion::mpl::List::remove_if test case 2 (failing)");
    static_assert(List<int, double, const float>{}.remove_if(is_const) == List<int, double>{},
                  "hyperion::mpl::List::remove_if test case 3 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>, float>{}.remove_if(is_value)
                      == List<int, double, float>{},
                  "hyperion::mpl::List::remove_if test case 4 (failing)");

    static_assert(List<int, const double, float>{}.remove(decltype_<const double>())
                      == List<int, float>{},
                  "hyperion::mpl::List::remove test case 1 (failing)");
    static_assert(List<int, double, float>{}.remove(decltype_<u32>()) == List<int, double, float>{},
                  "hyperion::mpl::List::remove test case 2 (failing)");
    static_assert(List<int, double, const float>{}.remove(decltype_<const float>())
                      == List<int, double>{},
                  "hyperion::mpl::List::remove test case 3 (failing)");
    static_assert(List<int, Value<1>, double, Value<2>, float>{}.remove(1_value)
                      == List<int, double, Value<2>, float>{},
                  "hyperion::mpl::List::remove test case 4 (failing)");

    static_assert(List<int, const double, float>{}.sift(List<Value<1>, Value<2>>{})
                      == List<const double, float>{},
                  "hyperion::mpl::List::sift test case 1 (failing)");
    static_assert(List<int, double, float>{}.sift(List<Value<0>, Value<2>>{}) == List<int, float>{},
                  "hyperion::mpl::List::sift test case 2 (failing)");
    static_assert(
        List<int, Value<1>, double, Value<2>, float>{}.sift(List<Value<1>, Value<2>, Value<4>>{})
            == List<Value<1>, double, float>{},
        "hyperion::mpl::List::sift test case 3 (failing)");

    static constexpr auto sum = [](MetaValue auto... value) noexcept {
        return (value + ...);
    };

    static constexpr auto num_ints = [](auto... value) noexcept {
        constexpr auto to_value = [](auto val) {
            if constexpr(MetaType<decltype(val)>) {
                if constexpr(detail::convert_to_meta_t<decltype(val)>{} == decltype_<int>()) {
                    return 1_value;
                }
                else {
                    return 0_value;
                }
            }
            else {
                return 0_value;
            }
        };

        return (to_value(value) + ...);
    };

    static_assert(List<Value<1>, Value<2>, Value<3>, Value<4>>{}.unwrap(sum) == 10_value,
                  "hyperion::mpl::List::unwrap test case 1 (failing)");
    static_assert(List<Value<3>, Value<4>, Value<3>, Value<4>>{}.unwrap(sum) == 14_value,
                  "hyperion::mpl::List::unwrap test case 2 (failing)");
    static_assert(List<Value<3>, Value<4>, Value<3>, Value<4>>{}.unwrap(sum) == 14_value,
                  "hyperion::mpl::List::unwrap test case 3 (failing)");
    static_assert(List<int, Value<1>, int, Value<2>, int>{}.unwrap(num_ints) == 3_value,
                  "hyperion::mpl::List::unwrap test case 4 (failing)");

#if __cpp_lib_ranges >= 202110L

    static constexpr auto test_ranges1() noexcept {
        constexpr auto list = List<int, double, float>{};
        constexpr auto manipped
            = list
              | std::ranges::views::transform([](MetaType auto type) { return type.as_const(); })
              | std::ranges::views::drop(1_value) | std::ranges::views::reverse;
        return manipped == List<const float, const double>{};
    }

    static constexpr auto test_ranges2() noexcept {
        constexpr auto list = List<int, const double, float>{};
        constexpr auto manipped
            = list | std::ranges::views::filter([](auto type) { return not type.is_const(); })
              | std::ranges::views::reverse;
        return manipped == List<float, int>{};
    }

    static constexpr auto test_ranges3() noexcept {
        constexpr auto list = List<int, const double, float>{};
        constexpr auto manipped
            = list | std::ranges::views::filter([](auto type) { return not type.is_const(); })
              | std::ranges::views::transform(
                  [](auto type) { return type.as_lvalue_reference().as_volatile(); })
              | std::ranges::views::reverse | std::ranges::views::drop(1_value);

        return manipped == List<volatile int&>{};
    }

    static constexpr auto test_ranges4() noexcept {
        constexpr auto list = List<int, Value<1>, const double, Value<2>, float>{};
        constexpr auto manipped
            = list
              | std::ranges::views::filter([](auto elem) { return not MetaValue<decltype(elem)>; })
              | std::ranges::views::transform(
                  [](auto type) { return type.as_lvalue_reference().as_volatile(); })
              | std::ranges::views::reverse | std::ranges::views::drop(1_value);

        return manipped == List<const volatile double&, volatile int&>{};
    }

    static_assert(test_ranges1(), "hyperion::mpl::List ranges support test case 1 (failing)");
    static_assert(test_ranges2(), "hyperion::mpl::List ranges support test case 2 (failing)");
    static_assert(test_ranges3(), "hyperion::mpl::List ranges support test case 3 (failing)");
    static_assert(test_ranges4(), "hyperion::mpl::List ranges support test case 4 (failing)");

#endif // __cpp_lib_ranges >= 202110L
} // namespace hyperion::mpl::_test::list

#endif // HYPERION_MPL_LIST_TEST_H
