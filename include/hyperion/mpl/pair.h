/// @file pair.h
/// @author Braxton Salyer <braxtonsalyer@gmail.com>
/// @brief Meta-programming type for operating on a pair of metaprogramming values and/or types
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

#ifndef HYPERION_MPL_PAIR_H
#define HYPERION_MPL_PAIR_H

#include <hyperion/mpl/metatypes.h>
#include <hyperion/platform/def.h>
#include <hyperion/platform/types.h>

//

#include <concepts>
#include <type_traits>

/// @ingroup mpl
/// @{
/// @defgroup pair Metaprogramming Pair Type
/// Hyperion provides `mpl::Pair` as a metaprogramming type for storing, communicating,
/// working with, and operating on pairs of types or values.
///
/// # Example
/// @code {.cpp}
/// #include <hyperion/mpl/pair.h>
///
/// using namespace hyperion::mpl;
///
/// constexpr auto add_const = [](MetaPair auto pair) noexcept {
///     return make_pair(pair.make_first().as_const(),
///                      pair.make_second().as_const());
/// };
///
/// constexpr auto pair = Pair<int, double>{};
/// constexpr auto constified = pair.apply(add_const);
///
/// static_assert(constified == Pair<const int, const double>{});
/// @endcode
/// @headerfile hyperion/mpl/pair.h
/// @}

namespace hyperion::mpl {

    template<typename TFirst, typename TSecond>
    struct Pair {
        using first = detail::convert_to_meta_t<TFirst>;
        using second = detail::convert_to_meta_t<TSecond>;

        template<typename TDelay = first>
            requires std::same_as<TDelay, first>
        [[nodiscard]] constexpr auto make_first() const noexcept -> TDelay {
            return {};
        }

        template<typename TDelay = second>
            requires std::same_as<TDelay, second>
        [[nodiscard]] constexpr auto make_second() const noexcept -> TDelay {
            return {};
        }

        template<typename TFunction>
            requires MetaFunctionOf<TFunction, Pair>
        [[nodiscard]] constexpr auto
        apply([[maybe_unused]] TFunction&& func) // NOLINT(*-missing-std-forward)
            const noexcept -> meta_result_t<TFunction, Pair> {
            return {};
        }
    };

    template<typename TLHSFirst, typename TLHSSecond, typename TRHSFirst, typename TRHSSecond>
    [[nodiscard]] constexpr auto
    operator==([[maybe_unused]] const Pair<TLHSFirst, TLHSSecond>& lhs,
               [[maybe_unused]] const Pair<TRHSFirst, TRHSSecond>& rhs) noexcept -> bool {
        constexpr auto is_equal = []<typename TLhs, typename TRhs>() {
            if constexpr((MetaType<TLhs> && MetaType<TRhs>) || (MetaValue<TLhs> && MetaValue<TRhs>)
                         || (MetaPair<TLhs> && MetaPair<TRhs>))
            {
                return TLhs{} == TRhs{};
            }
            else {
                return std::same_as<TLhs, TRhs>;
            }
        };

        return is_equal.template operator()<detail::convert_to_meta_t<TLHSFirst>,
                                            detail::convert_to_meta_t<TRHSFirst>>()
               && is_equal.template operator()<detail::convert_to_meta_t<TLHSSecond>,
                                               detail::convert_to_meta_t<TRHSSecond>>();
    }

    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && (!std::is_reference_v<TSecond>)
    [[nodiscard]] constexpr auto make_pair(
        [[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
        [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires(!std::is_reference_v<TFirst>) && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }

    template<typename TFirst, typename TSecond>
        requires std::is_reference_v<TFirst> && std::is_reference_v<TSecond>
    [[nodiscard]] constexpr auto
    make_pair([[maybe_unused]] TFirst&& first,   // NOLINT(*-missing-std-forward)
              [[maybe_unused]] TSecond&& second) // NOLINT(*-missing-std-forward)
        noexcept -> Pair<TFirst, TSecond> {
        return {};
    }
} // namespace hyperion::mpl

#include <hyperion/mpl/type.h>
#include <hyperion/mpl/value.h>

namespace hyperion::mpl {

    namespace _test::pair {
        constexpr auto add_const = [](MetaPair auto pair) noexcept {
            return make_pair(pair.make_first().as_const(), pair.make_second().as_const());
        };

        constexpr auto pair = Pair<int, double>{};
        constexpr auto constified = pair.apply(add_const);

        static_assert(constified == Pair<const int, const double>{});
    } // namespace _test::pair

} // namespace hyperion::mpl

#endif // HYPERION_MPL_PAIR_H
