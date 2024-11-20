#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Dimension
    {

        public:

            Dimension(int id);

            Dimension(Dimension const& other) = delete;

            Dimension(Dimension&& other) noexcept;

            ~Dimension() = default;

            auto operator=(Dimension const& other) -> Dimension& = delete;

            auto operator=(Dimension&& other) noexcept -> Dimension&;

        private:

            int _id;
    };

}  // namespace lue::netcdf
