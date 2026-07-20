#include "lue/framework/api/cxx/focal_operation.hpp"
#include "lue/framework.hpp"
#include <pybind11/numpy.h>


using namespace pybind11::literals;


namespace lue::api {

    namespace {

        template<Arithmetic Weight, Rank rank, Arithmetic Element>
        auto array_to_kernel(pybind11::array_t<Element> const& array) -> lue::Kernel<Weight, rank>
        {
            using Kernel = lue::Kernel<Weight, rank>;
            using Shape = typename Kernel::Shape;

            // Verify rank of shape of array corresponds with the one requested
            if (array.ndim() != rank)
            {
                throw std::runtime_error(
                    std::format(
                        "Rank of array shape must be equal to rank of array ({} != {})", array.ndim(), 2));
            }

            Shape shape{};
            std::copy_n(array.shape(), rank, shape.begin());

            if (!is_hypercube(shape))
            {
                throw std::runtime_error(
                    std::format("Shape of array to use as kernel must be a hypercube (a square in 2D)"));
            }


            // TODO In the current case (uint8 to uint8) all is well. In general it might not be.
            static_assert(std::is_same_v<Element, Weight>);
            // if(!is_convertible<Element, Weight>)
            // {
            // }

            pybind11::buffer_info buffer{array.request()};

            Kernel kernel{shape};
            std::copy_n(static_cast<Element*>(buffer.ptr), buffer.size, kernel.begin());

            return kernel;
        }

    }  // Anonymous namespace


    void bind_focal_operations(pybind11::module& module)
    {
        module.def(
            "focal_diversity",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_diversity(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_high_pass",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_high_pass(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_majority",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_majority(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_maximum",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_maximum(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_mean",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_mean(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_minimum",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_minimum(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
        module.def(
            "focal_sum",
            [](Field const& field, pybind11::array_t<BooleanElement> const& kernel) -> Field
            { return focal_sum(field, array_to_kernel<BooleanElement, 2>(kernel)); },
            "array"_a,
            "kernel"_a.noconvert());
    }

}  // namespace lue::api
