#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Variable
    {

        public:

            Variable(int id);

            Variable(Variable const& other) = delete;

            Variable(Variable&& other) noexcept;

            ~Variable() = default;

            auto operator=(Variable const& other) -> Variable& = delete;

            auto operator=(Variable&& other) noexcept -> Variable&;

        private:

            int _id;
    };

}  // namespace lue::netcdf
