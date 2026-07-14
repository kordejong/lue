#include "lue/framework/api/cxx/miscellaneous_operation.hpp"
#include "lue/framework.hpp"
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
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


    void bind_miscellaneous_operations(pybind11::module& module)
    {
        module.def("array_like",
            [](Array const& array, Scalar const& fill_value) -> Array
            {
                return array_like(array, fill_value);
            },
            "array"_a,
            "fill_value"_a);

        module.def(
            "create_array",
            [](Shape<Count, 2> const& array_shape,
               Scalar const& fill_value,
               std::optional<Shape<Count, 2>> const& partition_shape) -> Array
            {
                return partition_shape ? create_array(array_shape, *partition_shape, fill_value)
                                       : create_array(array_shape, fill_value);
            },
            "array_shape"_a,
            "fill_value"_a,
            pybind11::kw_only(),
            "partition_shape"_a = std::optional<Shape<Count, 2>>{});

        module.def(
            "create_scalar",
            [](double const value, pybind11::object const& dtype_args) -> Scalar
            {
                pybind11::dtype const dtype{pybind11::dtype::from_args(dtype_args)};

                return create_scalar(value, dtype);
            },
            "value"_a,
            "dtype"_a);
    }

}  // namespace lue::api
