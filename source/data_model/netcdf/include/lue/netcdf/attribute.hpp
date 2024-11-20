#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Attribute
    {

        public:

            Attribute(int id);

            Attribute(Attribute const& other) = delete;

            Attribute(Attribute&& other) noexcept;

            ~Attribute() = default;

            auto operator=(Attribute const& other) -> Attribute& = delete;

            auto operator=(Attribute&& other) noexcept -> Attribute&;

        private:

            int _id;
    };

}  // namespace lue::netcdf
