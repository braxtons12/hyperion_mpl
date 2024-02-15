hyperion::mpl
*************

hyperion::mpl is a C++20 meta programming library similar to Boost.Hana.

It's currently under active development and has not yet hit a stable release point.

See the :doc:`Quick Start Guide <quick_start>` for how to get started using hyperion::mpl.

For an overview of each module, see the links in the left sidebar or below.

.. code-block:: cpp
    :caption: Example
    :linenos:

    #include <hyperion/mpl/value.h>
    #include <hyperion/mpl/type.h>

    #include <concepts>

    using namespace hyperion::mpl;

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


.. toctree::
    :caption: Getting Started

    quick_start

.. toctree::
    :caption: Quick Reference

    genindex

.. toctree::
    :caption: Metaprogramming Type Categories (Concept Definitions)
    
    metatypes

.. toctree::
    :caption: Metaprogramming Value Type
    
    value

.. toctree::
    :caption: Metaprogramming Type Manipulation Type
    
    type

.. toctree::
    :caption: Metaprogramming Pair Type
    
    pair

.. toctree::
    :caption: Type Traits
    
    type_traits

.. toctree::
    :caption: Concept Definitions
    
    concepts
