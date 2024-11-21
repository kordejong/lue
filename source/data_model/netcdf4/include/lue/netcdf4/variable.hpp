#pragma once
#include "lue/netcdf4/attribute.hpp"


namespace lue::netcdf {

    /*!
        @brief      A variable holds a multi-dimensional array of data
        @param      .
        @return     .
        @exception  .

        Attributes may be associated with a variable.
    */
    class Variable
    {

        public:

            Variable(int group_id, int variable_id);

            Variable(Variable const& other) = delete;

            Variable(Variable&& other) noexcept;

            ~Variable() = default;

            auto operator=(Variable const& other) -> Variable& = delete;

            auto operator=(Variable&& other) noexcept -> Variable&;

            /*!
                @brief      Write an attribute to the variable
                @param      name Name of attribute
                @param      values Values of attribute
                @return     Written attribute
                @exception  std::runtime_error In case the attribute cannot be written
            */
            template<typename T>
            [[nodiscard]] auto put_attribute(std::string name, std::vector<T> const& values) -> Attribute
            {
                return Attribute::put_attribute(_group_id, _variable_id, std::move(name), values);
            }


            template<typename T>
            [[nodiscard]] auto put_attribute(std::string name, T&& value) -> Attribute
            {
                return Attribute::put_attribute(
                    _group_id, _variable_id, std::move(name), std::forward<T>(value));
            }

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;
    };

}  // namespace lue::netcdf
