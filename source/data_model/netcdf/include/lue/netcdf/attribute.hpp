#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Attribute
    {

        public:

            Attribute(int group_id, std::string name);

            Attribute(Attribute const& other) = delete;

            Attribute(Attribute&& other) noexcept;

            ~Attribute() = default;

            auto operator=(Attribute const& other) -> Attribute& = delete;

            auto operator=(Attribute&& other) noexcept -> Attribute&;

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;

            //! Name of the attribute
            std::string _name;
    };

}  // namespace lue::netcdf
