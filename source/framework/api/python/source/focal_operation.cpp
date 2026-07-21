#include "lue/framework/api/cxx/focal_operation.hpp"
#include "switch_on_dtype.hpp"
#include "lue/framework.hpp"
#include <pybind11/numpy.h>


using namespace pybind11::literals;


namespace lue::api {

    namespace {

        struct NumPyArrayToKernelConverter
        {
                template<Arithmetic Weight>
                static auto operator()(Shape<Count, 2> const& shape, pybind11::buffer_info const& buffer)
                    -> std::optional<lue::api::Kernel>
                {
                    std::optional<lue::api::Kernel> result{};

                    if constexpr (lue::arithmetic_element_supported<Weight>)
                    {
                        lue::Kernel<Weight, 2> kernel{shape};

                        std::copy_n(static_cast<Weight*>(buffer.ptr), buffer.size, kernel.begin());

                        return kernel;
                    }

                    return result;
                }
        };


        auto array_to_kernel(pybind11::array const& array) -> lue::api::Kernel
        {
            Rank const rank = array.ndim();

            if (rank != 2)
            {
                throw std::runtime_error(
                    std::format("Rank of array with kernel weights must be 2 ({} != {})", array.ndim(), 2));
            }

            Shape<Count, 2> shape{};

            std::copy_n(array.shape(), rank, shape.begin());

            if (!is_hypercube(shape))
            {
                throw std::runtime_error(
                    std::format("Shape of array to use as kernel must be a hypercube (a square in 2D)"));
            }

            pybind11::buffer_info buffer{array.request()};

            return switch_on_dtype<Kernel, NumPyArrayToKernelConverter>(array.dtype(), shape, buffer);
        }

    }  // Anonymous namespace


    void bind_focal_operations(pybind11::module& module)
    {
        module.def("aspect", aspect, "elevation"_a);
        module.def(
            "convolve",
            [](Field const& field, pybind11::array const& kernel) -> Field
            { return convolve(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_diversity",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_diversity(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_high_pass",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_high_pass(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_majority",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_majority(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_maximum",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_maximum(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_mean",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_mean(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_minimum",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_minimum(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_sum",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_sum(field, array_to_kernel(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def("slope", slope, "elevation"_a, "cell_size"_a);
    }

}  // namespace lue::api
