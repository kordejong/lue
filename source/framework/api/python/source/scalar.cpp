#include "lue/framework/api/cxx/scalar.hpp"
#include "lue/framework/api/cxx/miscellaneous/create_scalar.hpp"
#include "lue/framework/api/cxx/operator.hpp"
#include "lue/framework.hpp"
#include <pybind11/numpy.h>
#include <format>


using namespace pybind11::literals;


namespace lue::api {

    namespace {

        template<typename Element>
        auto create_scalar(double const value) -> std::optional<Scalar>
        {
            std::optional<Scalar> result{};

            if constexpr (lue::arithmetic_element_supported<Element>)
            {
                result = lue::api::create_scalar(static_cast<Element>(value));
            }

            return result;
        }


        auto create_scalar(double const value, pybind11::dtype const& dtype) -> Scalar
        {
            // TODO Out of range values must result in no-data values. This logic must be in the API layer or
            // higher. All bindings need it.
            //
            // auto output_value = value_policies::cast<std::int32_t>(input_value);

            auto const kind = dtype.kind();
            auto const size = dtype.itemsize();  // bytes
            std::optional<Scalar> scalar{};

            // NOLINTBEGIN(bugprone-switch-missing-default-case)
            switch (kind)
            {
                case 'i':
                {
                    // Signed integer
                    switch (size)
                    {
                        case 1:
                        {
                            scalar = create_scalar<std::int8_t>(value);
                            break;
                        }
                        case 2:
                        {
                            scalar = create_scalar<std::int16_t>(value);
                            break;
                        }
                        case 4:
                        {
                            scalar = create_scalar<std::int32_t>(value);
                            break;
                        }
                        case 8:
                        {
                            scalar = create_scalar<std::int64_t>(value);
                            break;
                        }
                    }

                    break;
                }
                case 'u':
                {
                    // Unsigned integer
                    switch (size)
                    {
                        case 1:
                        {
                            scalar = create_scalar<std::uint8_t>(value);
                            break;
                        }
                        case 2:
                        {
                            scalar = create_scalar<std::uint16_t>(value);
                            break;
                        }
                        case 4:
                        {
                            scalar = create_scalar<std::uint32_t>(value);
                            break;
                        }
                        case 8:
                        {
                            scalar = create_scalar<std::uint64_t>(value);
                            break;
                        }
                    }

                    break;
                }
                case 'f':
                {
                    // Floating-point
                    switch (size)
                    {
                        case 4:
                        {
                            scalar = create_scalar<float>(value);
                            break;
                        }
                        case 8:
                        {
                            scalar = create_scalar<double>(value);
                            break;
                        }
                    }

                    break;
                }
            }
            // NOLINTEND(bugprone-switch-missing-default-case)

            if (!scalar)
            {
                throw std::runtime_error(std::format("Unsupported dtype (kind={}, itemsize={})", kind, size));
            }

            return std::move(*scalar);
        }

    }  // Anonymous namespace

    void bind_scalar(pybind11::module& module)
    {
        pybind11::class_<Scalar>(module, "Scalar")

            // // bool(a), not a, if a, while a, ...
            // .def(
            //     "__bool__",
            //     []([[maybe_unused]] Scalar const& scalar) -> void
            //     {
            //         // ValueError
            //         throw std::invalid_argument("The truth value of a scalar is ambiguous");
            //     })

            // a + b, a += b
            // .def(pybind11::self + pybind11::self)
            .def("__add__", add, pybind11::is_operator())
            .def("__radd__", add, pybind11::is_operator())


            // TODO
            // https://pybind11.readthedocs.io/en/stable/advanced/classes.html#operator-overloading

            ;

        module.def(
            "create_scalar",
            [](double const value, pybind11::object const& dtype_args) -> Scalar
            {
                pybind11::dtype const dtype{pybind11::dtype::from_args(dtype_args)};

                return create_scalar(value, dtype);
            },
            "value"_a,
            "dtype"_a,
            pybind11::return_value_policy::move);
    }

}  // namespace lue::api
