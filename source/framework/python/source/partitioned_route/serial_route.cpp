#include "lue/framework/serial_route.hpp"
#include "lue/framework.hpp"
#include "lue/py/bind.hpp"
#include "lue/py/configure.hpp"
#include "lue/py/framework/stream.hpp"
#include <pybind11/stl.h>


namespace lue::framework {

    namespace {

        template<typename RouteID, Rank rank>
        auto formal_string_representation(SerialRoute<RouteID, rank> const& route) -> std::string
        {
            return std::format(
                "SerialRoute<{}, {}>{{shape={}, shape_in_partitions={}}}",
                as_string<RouteID>(),
                rank,
                as_string(route.shape()),
                as_string(route.partitions().shape()));
        }


        template<typename RouteID, Rank rank>
        auto informal_string_representation(SerialRoute<RouteID, rank> const& route) -> std::string
        {
            return formal_string_representation(route);
        }


        template<typename RouteID, Rank rank>
        void bind_serial_route(pybind11::module& module)
        {
            namespace lfr = lue::framework;

            using Route = SerialRoute<RouteID, rank>;

            auto class_ =
                pybind11::class_<Route>(
                    module,
                    std::format("SerialRoute_{}_{}", as_string<RouteID>(), rank).c_str(),
                    std::format(
                        R"(
    Partitioned serial route type for routes of rank {}
)",
                        rank)
                        .c_str())

                    .def_property_readonly(
                        "shape",
                        [](Route const& self)
                        {
                            static_assert(rank == 2);

                            auto shape{self.shape()};

                            return std::make_tuple(shape[0], shape[1]);
                        },
                        "shape docstring...")

                    .def_property_readonly(
                        "shape_in_partitions",
                        [](Route const& self)
                        {
                            static_assert(rank == 2);

                            auto shape{self.partitions().shape()};

                            return std::make_tuple(shape[0], shape[1]);
                        },
                        "shape_in_partitions docstring...")

                    .def("__repr__", [](Route const& route) { return formal_string_representation(route); })

                    .def("__str__", [](Route const& route) { return informal_string_representation(route); })

                ;
        }


        class Binder
        {

            public:

                template<Arithmetic Element>
                static void bind(pybind11::module& module)
                {
                    Rank const rank{2};

                    bind_serial_route<Element, rank>(module);
                }
        };

    }  // Anonymous namespace


    void bind_serial_route(pybind11::module& module)
    {
        bind<Binder, ZoneElements>(module);
    }

}  // namespace lue::framework
