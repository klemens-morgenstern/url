//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SEGMENTS_VIEW_IPP
#define BOOST_URL_IMPL_SEGMENTS_VIEW_IPP

#include <boost/url/segments_view.hpp>
#include <boost/url/error.hpp>
#include <boost/url/bnf/parse.hpp>
#include <boost/url/rfc/paths_bnf.hpp>
#include <boost/url/rfc/query_bnf.hpp>
#include <boost/url/detail/except.hpp>
#include <iostream>

namespace boost {
namespace urls {

segments_view::
segments_view() noexcept
    : s_("")
    , n_(0)
{
}

//------------------------------------------------

segments_view::
iterator::
iterator(
    string_view s)
    : begin_(s.data())
    , pos_(s.data())
    , next_(s.data())
    , end_(s.data() + s.size())
{
    using bnf::parse;
    using bnf_t =
        path_rootless_bnf;
    if(next_ == end_)
    {
        next_ = nullptr;
        return;
    }
    error_code ec;
    if(*next_ == '/')
    {
        // "/" segment
        bnf_t::increment(next_,
            end_, ec, v_.s_);
        BOOST_ASSERT(! ec);
    }
    else
    {
        // segment-nz
        bnf_t::begin(next_,
            end_, ec, v_.s_);
        BOOST_ASSERT(! ec);
    }
}

segments_view::
iterator::
iterator(
    std::size_t n,
    string_view s) noexcept
    : i_(n)
    , begin_(s.data())
    , pos_(s.data() + s.size())
    , end_(s.data() + s.size())
{
}

auto
segments_view::
iterator::
operator++() noexcept ->
    iterator&
{
    using bnf::parse;
    using bnf_t =
        path_rootless_bnf;
    BOOST_ASSERT(next_ != nullptr);
    ++i_;
    pos_ = next_;
    error_code ec;
    // "/" segment
    bnf_t::increment(
        next_, end_, ec, v_.s_);
    if(ec == error::end)
    {
        next_ = nullptr;
        return *this;
    }
    BOOST_ASSERT(! ec);
    return *this;
}

auto
segments_view::
iterator::
operator--() noexcept ->
    iterator&
{
    using bnf::parse;
    using bnf_t =
        path_rootless_bnf;
    BOOST_ASSERT(i_ != 0);
    BOOST_ASSERT(pos_ != begin_);
    --i_;
    error_code ec;
    while(--pos_ != begin_)
    {
        if(*pos_ != '/')
            continue;
        // "/" segment
        next_ = pos_;
        bnf_t::increment(next_,
            end_, ec, v_.s_);
        BOOST_ASSERT(! ec);
        return *this;
    }
    next_ = pos_;
    if(*next_ == '/')
    {
        // "/" segment
        bnf_t::increment(next_,
            end_, ec, v_.s_);
        BOOST_ASSERT(! ec);
    }
    else
    {
        // segment-nz
        bnf_t::begin(next_,
            end_, ec, v_.s_);
        BOOST_ASSERT(! ec);
    }
    return *this;
}

//------------------------------------------------

auto
segments_view::
begin() const noexcept ->
    iterator
{
    return iterator(s_);
}

auto
segments_view::
end() const noexcept ->
    iterator
{
    return iterator(n_, s_);
}

//------------------------------------------------

std::ostream&
operator<<(
    std::ostream& os,
    segments_view const& pv)
{
    os.write(
        pv.encoded_path().data(),
        pv.encoded_path().size());
    return os;
}

//------------------------------------------------

segments_view
parse_path_abempty(
    string_view s,
    error_code& ec) noexcept
{
    using bnf::parse_string;
    path_abempty_bnf t;
    if(! parse_string(s, ec, t))
        return {};
    return segments_view(
        t.str, t.count);
}

segments_view
parse_path_abempty(
    string_view s)
{
    error_code ec;
    auto p = parse_path_abempty(s, ec);
    detail::maybe_throw(ec,
        BOOST_CURRENT_LOCATION);
    return p;
}

segments_view
parse_path_absolute(
    string_view s,
    error_code& ec) noexcept
{
    using bnf::parse_string;
    path_absolute_bnf t;
    if(! parse_string(s, ec, t))
        return {};
    return segments_view(
        t.str, t.count);
}

segments_view
parse_path_absolute(
    string_view s)
{
    error_code ec;
    auto p = parse_path_absolute(s, ec);
    detail::maybe_throw(ec,
        BOOST_CURRENT_LOCATION);
    return p;
}

segments_view
parse_path_noscheme(
    string_view s,
    error_code& ec) noexcept
{
    using bnf::parse_string;
    path_noscheme_bnf t;
    if(! parse_string(s, ec, t))
        return {};
    return segments_view(
        t.str, t.count);
}

segments_view
parse_path_noscheme(
    string_view s)
{
    error_code ec;
    auto p = parse_path_noscheme(s, ec);
    detail::maybe_throw(ec,
        BOOST_CURRENT_LOCATION);
    return p;
}

segments_view
parse_path_rootless(
    string_view s,
    error_code& ec) noexcept
{
    using bnf::parse_string;
    path_rootless_bnf t;
    if(! parse_string(s, ec, t))
        return {};
    return segments_view(
        t.str, t.count);
}

segments_view
parse_path_rootless(
    string_view s)
{
    error_code ec;
    auto p = parse_path_rootless(s, ec);
    detail::maybe_throw(ec,
        BOOST_CURRENT_LOCATION);
    return p;
}

} // urls
} // boost

#endif