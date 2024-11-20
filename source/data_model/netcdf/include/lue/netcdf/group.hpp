#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Group
    {

        public:

            Group(int id);

            Group(Group const& other) = delete;

            Group(Group&& other) noexcept;

            ~Group() = default;

            auto operator=(Group const& other) -> Group& = delete;

            auto operator=(Group&& other) noexcept -> Group&;

        private:

            int _id;
    };

}  // namespace lue::netcdf
