#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Dimension
    {

        public:

            Dimension(int dimension_id);

            Dimension(Dimension const& other) = delete;

            Dimension(Dimension&& other) noexcept;

            ~Dimension() = default;

            auto operator=(Dimension const& other) -> Dimension& = delete;

            auto operator=(Dimension&& other) noexcept -> Dimension&;

            [[nodiscard]] auto id() const -> int;

        private:

            int _id;
    };

}  // namespace lue::netcdf
