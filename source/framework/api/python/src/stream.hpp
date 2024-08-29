#pragma once
#include <pybind11/pybind11.h>

#include <iostream>


// This code is inspired by code found in the phylanx software. The specific module was:
//
//  Copyright (c) 2016-2018 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


namespace lue::api {

    class PythonBuffer: public std::streambuf
    {

        public:

            PythonBuffer(pybind11::object pyostream);

            PythonBuffer(PythonBuffer const&) = delete;

            PythonBuffer(PythonBuffer&&) = delete;

            ~PythonBuffer() override;

            auto operator=(PythonBuffer const&) -> PythonBuffer& = delete;

            auto operator=(PythonBuffer&&) -> PythonBuffer& = delete;


        private:

            using traits_type = std::streambuf::traits_type;

            char _buffer[1024]{};

            pybind11::object _pywrite;

            pybind11::object _pyflush;

            auto overflow(int chr) -> int override;

            auto sync() -> int override;

    };


    class ScopedOstreamRedirect
    {

        public:

            ScopedOstreamRedirect(
                std::ostream& costream = std::cout,
                pybind11::object pyostream = import_stdout());

            ScopedOstreamRedirect(const ScopedOstreamRedirect&) = delete;

            ScopedOstreamRedirect(ScopedOstreamRedirect&&) = delete;

            ~ScopedOstreamRedirect();

            auto operator=(const ScopedOstreamRedirect&) -> ScopedOstreamRedirect& = delete;

            auto operator=(ScopedOstreamRedirect&&) -> ScopedOstreamRedirect& = delete;


        private:

            std::streambuf* _old{};

            std::ostream& _costream;

            PythonBuffer _buffer;

            static auto import_stdout() -> pybind11::object;

    };

}  // namespace lue::api
