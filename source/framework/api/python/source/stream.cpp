#include "stream.hpp"
#include <cstddef>
#include <utility>


// This code is inspired by code found in the phylanx software. The specific module was:
//
//  Copyright (c) 2016-2018 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


namespace lue::api {

    PythonBuffer::PythonBuffer(pybind11::object pyostream)
    {
        {
            // Acquire GIL to avoid multi-threading problems
            pybind11::gil_scoped_acquire acquire;
            pybind11::object tmp = std::move(pyostream);
            _pywrite = tmp.attr("write");
            _pyflush = tmp.attr("flush");
        }

        setp(_buffer, _buffer + sizeof(_buffer) - 1);
    }


    PythonBuffer::~PythonBuffer()
    {
        // Sync before destroy
        {
            pybind11::gil_scoped_release release;
            sync();
        }

        _pywrite.release();
        _pyflush.release();
    }


    auto PythonBuffer::overflow(int chr) -> int
    {
        if (!traits_type::eq_int_type(chr, traits_type::eof()))
        {
            *pptr() = traits_type::to_char_type(chr);
            pbump(1);
        }

        return sync() != 0 ? traits_type::not_eof(chr) : traits_type::eof();
    }


    auto PythonBuffer::sync() -> int
    {
        if (pbase() != pptr())
        {
            {
                // Acquire GIL to avoid multi-threading problems
                pybind11::gil_scoped_acquire acquire;

                // This subtraction cannot be negative, so dropping the sign
                pybind11::str line(pbase(), static_cast<std::size_t>(pptr() - pbase()));

                _pywrite(line);
                _pyflush();
            }

            setp(pbase(), epptr());
        }

        return 0;
    }


    ScopedOstreamRedirect::ScopedOstreamRedirect(std::ostream& costream, pybind11::object pyostream):

        _costream(costream),
        _buffer(std::move(pyostream))

    {
        _old = _costream.rdbuf(&_buffer);
    }


    ScopedOstreamRedirect::~ScopedOstreamRedirect()
    {
        _costream.rdbuf(_old);
    }


    auto ScopedOstreamRedirect::import_stdout() -> pybind11::object
    {
        pybind11::gil_scoped_acquire acquire;
        return pybind11::module::import("sys").attr("stdout");
    }

}  // namespace lue::api
