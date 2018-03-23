//
// Copyright Jason Rice 2018
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FULL_DUPLEX_FWD_PROMISE_HPP
#define FULL_DUPLEX_FWD_PROMISE_HPP

namespace full_duplex
{
  struct promise_tag { };

  template <typename F>
  struct promise_t;

  struct promise_fn
  {
    template <typename F>
    auto operator()(F&& f) const;
  };

  constexpr promise_fn promise{};
}

#endif
