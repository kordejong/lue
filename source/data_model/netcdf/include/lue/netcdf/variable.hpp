#pragma once
#include <netcdf.h>


namespace lue::netcdf {

    class Variable
    {

        public:

            Variable(int group_id, int variable_id);

            Variable(Variable const& other) = delete;

            Variable(Variable&& other) noexcept;

            ~Variable() = default;

            auto operator=(Variable const& other) -> Variable& = delete;

            auto operator=(Variable&& other) noexcept -> Variable&;

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;
    };

}  // namespace lue::netcdf
