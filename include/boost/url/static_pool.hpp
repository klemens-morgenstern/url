//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_STATIC_POOL_HPP
#define BOOST_URL_STATIC_POOL_HPP

#include <boost/url/detail/config.hpp>
#include <cstdlib>
#include <string>
#include <type_traits>

namespace boost {
namespace urls {

/** The type of allocator returned by static_pool
*/
#ifdef BOOST_URL_DOCS
template<class T>
using static_pool_allocator = __see_below__
#else
template<class T>
class static_pool_allocator;
#endif

/** Base class for fixed-storage pool
*/
class basic_static_pool
{
    char* begin_;
    char* end_;
    char* top_;
    std::size_t n_ = 0;

    template<class T>
    friend class static_pool_allocator;

    void** table() noexcept;
    void** find(void* p) noexcept;

    BOOST_URL_DECL
    void*
    allocate(
        std::size_t bytes,
        std::size_t align);

    BOOST_URL_DECL
    void
    deallocate(
        void* p,
        std::size_t bytes,
        std::size_t align) noexcept;

public:
    /** Constructor (deleted)
    */
    basic_static_pool(
        basic_static_pool const&) = delete;

    /** Assignment (deleted)
    */
    basic_static_pool&
        operator=(basic_static_pool const&) = delete;

    /** The type of strings using the pool's allocator.
    */
    using string_type = std::basic_string<
        char, std::char_traits<char>,
            static_pool_allocator<char>>;

    /** Destructor
    */
    BOOST_URL_DECL
    ~basic_static_pool();

    /** Constructor
    */
    basic_static_pool(
        char* buffer,
        std::size_t size)
        : begin_(buffer)
        , end_(buffer + size)
        , top_(end_)
    {
    }

    /** Return an allocator which uses the pool.
    */
    static_pool_allocator<char>
    allocator() noexcept;

    /** Construct a std::basic_string using the pool as its allocator.
    */
    template<class... Args>
    string_type
    make_string(Args&&... args);
};

//------------------------------------------------

/** A fixed-size storage pool for allocating memory
*/
template<std::size_t N>
class static_pool : public basic_static_pool
{
    char buf_[N];

public:
    /** Constructor
    */
    static_pool() noexcept
        : basic_static_pool(buf_, N)
    {
    }
};

} // urls
} // boost

#include <boost/url/impl/static_pool.hpp>

#endif
