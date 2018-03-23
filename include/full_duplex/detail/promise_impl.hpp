//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_DETAIL_PROMISE_IMPL_HPP
#define FULL_DUPLEX_DETAIL_PROMISE_IMPL_HPP

#include <boost/hana/core/is_a.hpp>
#include <utility>

namespace full_duplex::detail {
    namespace hana = boost::hana;

    //
    // async -  A promise operation with overloads for
    //          propagating errors

    struct async_tag { };

    template <typename Fn>
    struct async {
        using hana_tag = async_tag;

        Fn fn;
    };

    //
    // pmap - A partially applied mapping function
    //

    struct pmap_tag { };

    struct pmap_t {
        using hana_tag = pmap_tag;

        Fn fn;
    };
}
