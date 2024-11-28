#pragma once
#include "lue/concept.hpp"
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include <cassert>


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

            Variable(Variable const& other) = default;

            Variable(Variable&& other) noexcept = default;

            ~Variable() = default;

            auto operator=(Variable const& other) -> Variable& = default;

            auto operator=(Variable&& other) noexcept -> Variable& = default;

            [[nodiscard]] auto type() const -> nc_type;

            [[nodiscard]] auto dimensions() const -> std::vector<Dimension>;

            [[nodiscard]] auto name() const -> std::string;

            /*!
                @brief      Set the fill value to @a value
                @exception  .
            */
            template<typename T>
            void set_fill_value(T const& value)
            {
                if (int status = nc_def_var_fill(_group_id, _variable_id, NC_FILL, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(fmt::format("Cannot set fill value: {}", error_message(status)));
                }
            }

            void unset_fill_value() const;

            [[nodiscard]] auto has_fill_value() const -> bool;

            /*!
                @brief      Retrieve the fill value

                It is required that a fill value has been set.
            */
            template<typename T>
            auto fill_value() const -> T
            {
                int fill_mode{};
                T value{};

                if (int status = nc_inq_var_fill(_group_id, _variable_id, &fill_mode, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(fmt::format("Cannot get fill value: {}", error_message(status)));
                }

                assert(fill_mode == NC_FILL);

                return value;
            }

            /*!
                @brief      Write an attribute to the variable
                @param      name Name of attribute
                @param      values Values of attribute
                @return     Written attribute
                @exception  std::runtime_error In case the attribute cannot be written
            */
            template<typename T>
            [[nodiscard]] auto add_attribute(std::string name, std::vector<T> const& values) -> Attribute
            {
                return Attribute::add_attribute(_group_id, _variable_id, std::move(name), values);
            }


            template<typename T>
            [[nodiscard]] auto add_attribute(std::string name, T&& value) -> Attribute
            {
                return Attribute::add_attribute(
                    _group_id, _variable_id, std::move(name), std::forward<T>(value));
            }


            template<Arithmetic T>
            void write(T const& value)
            {
                write(&value);
            }


            template<Arithmetic T>
            void write(T const* value)
            {
                int status{-1};

                if constexpr (std::is_same_v<T, std::int32_t>)
                {
                    status = nc_put_var_int(_group_id, _variable_id, value);
                }
                else
                {
                    assert(false);
                }

                // TODO Expand for all T

                if (status != NC_NOERR)
                {
                    throw std::runtime_error(
                        fmt::format("Cannot get write value: {}", error_message(status)));
                }
            }


            template<Arithmetic T>
            void read(T& value)
            {
                read(&value);
            }


            template<Arithmetic T>
            void read(T* value)
            {
                int status{-1};

                if constexpr (std::is_same_v<T, std::int32_t>)
                {
                    status = nc_get_var_int(_group_id, _variable_id, value);
                }
                else
                {
                    assert(false);
                }

                // TODO Expand for all T

                if (status != NC_NOERR)
                {
                    throw std::runtime_error(fmt::format("Cannot get read value: {}", error_message(status)));
                }
            }

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;
    };

}  // namespace lue::netcdf
