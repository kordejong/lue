#pragma once
#include "lue/concept.hpp"
#include "lue/netcdf4/attribute.hpp"
#include "lue/netcdf4/dimension.hpp"
#include "lue/netcdf4/hyperslab.hpp"
#include <cassert>
#include <vector>


namespace lue::netcdf {
    namespace detail {

        template<typename T>
        struct VariableValue
        {
        };


        template<>
        struct VariableValue<std::int8_t>
        {
                using ValueType = std::int8_t;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_schar(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_schar(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_schar(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_schar(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_schar(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_schar(group_id, variable_id, values);
                }
        };


        template<>
        struct VariableValue<std::uint8_t>
        {
                using ValueType = std::uint8_t;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_ubyte(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_ubyte(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_ubyte(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_ubyte(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_ubyte(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_ubyte(group_id, variable_id, values);
                }
        };


        template<>
        struct VariableValue<std::int32_t>
        {
                using ValueType = std::int32_t;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_int(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_int(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_int(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_int(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_int(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_int(group_id, variable_id, values);
                }
        };


        template<>
        struct VariableValue<std::uint32_t>
        {
                using ValueType = std::uint32_t;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_uint(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_uint(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_uint(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_uint(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_uint(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_uint(group_id, variable_id, values);
                }
        };


        template<>
        struct VariableValue<std::int64_t>
        {
                using ValueType = std::int64_t;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_long(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_long(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_long(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_long(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_long(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_long(group_id, variable_id, values);
                }
        };


        template<>
        struct VariableValue<std::uint64_t>
        {
                using ValueType = std::uint64_t;

                // The NetCDF API does not provide an overload for uint64_t variable values. It seems the
                // ulonglong version can be used (but maybe not on all platforms?).
                static_assert(std::is_same_v<ValueType, unsigned long>);
                static_assert(sizeof(ValueType) == sizeof(unsigned long long));

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_ulonglong(
                        group_id, variable_id, idxs.data(), (unsigned long long*)value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_ulonglong(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        (unsigned long long*)values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_ulonglong(group_id, variable_id, (unsigned long long*)values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_ulonglong(
                        group_id, variable_id, idxs.data(), (unsigned long long const*)values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_ulonglong(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        (unsigned long long const*)values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_ulonglong(group_id, variable_id, (unsigned long long const*)values);
                }
        };


        template<>
        struct VariableValue<float>
        {
                using ValueType = float;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_float(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_float(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_float(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_float(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_float(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_float(group_id, variable_id, values);
                }
        };


        template<>
        struct VariableValue<double>
        {
                using ValueType = double;

                static auto get(
                    int const group_id, int const variable_id, Indices const& idxs, ValueType* value) -> int
                {
                    return nc_get_var1_double(group_id, variable_id, idxs.data(), value);
                }

                static auto get(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType* values) -> int
                {
                    return nc_get_vars_double(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto get(int const group_id, int const variable_id, ValueType* values) -> int
                {
                    return nc_get_var_double(group_id, variable_id, values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Indices const& idxs,
                    ValueType const* values) -> int
                {
                    return nc_put_var1_double(group_id, variable_id, idxs.data(), values);
                }

                static auto put(
                    int const group_id,
                    int const variable_id,
                    Hyperslab const& hyperslab,
                    ValueType const* values) -> int
                {
                    return nc_put_vars_double(
                        group_id,
                        variable_id,
                        hyperslab.starts().data(),
                        hyperslab.counts().data(),
                        hyperslab.strides().data(),
                        values);
                }

                static auto put(int const group_id, int const variable_id, ValueType const* values) -> int
                {
                    return nc_put_var_double(group_id, variable_id, values);
                }
        };

    }  // namespace detail


    /*!
        @brief      A variable holds a multi-dimensional array of data

        A variable is part of a group. A NetCDF file itself is the root group. Attributes may be associated
        with a variable.
    */
    class LUE_NETCDF4_EXPORT Variable
    {

        public:

            Variable(int group_id, int variable_id);

            Variable(Variable const& other) = default;

            Variable(Variable&& other) noexcept = default;

            virtual ~Variable() = default;

            auto operator=(Variable const& other) -> Variable& = default;

            auto operator=(Variable&& other) noexcept -> Variable& = default;

            [[nodiscard]] auto type() const -> nc_type;

            [[nodiscard]] auto nr_dimensions() const -> int;

            [[nodiscard]] auto dimensions() const -> std::vector<Dimension>;

            [[nodiscard]] auto name() const -> std::string;

            /*!
                @brief      Set the fill value to @a value
                @exception  std::runtime_error In case the fill value cannot be set
            */
            template<typename T>
            void set_fill_value(T const& value)
            {
                if (auto const status = nc_def_var_fill(_group_id, _variable_id, NC_FILL, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot set fill value: {}", error_message(status)));
                }
            }

            void unset_fill_value() const;

            [[nodiscard]] auto has_fill_value() const -> bool;

            /*!
                @brief      Retrieve the fill value
                @exception  std::runtime_error In case the fill value cannot be obtained

                It is required that a fill value has been set.
            */
            template<typename T>
            auto fill_value() const -> T
            {
                int fill_mode{};
                T value{};

                if (auto const status = nc_inq_var_fill(_group_id, _variable_id, &fill_mode, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot get fill value: {}", error_message(status)));
                }

                assert(fill_mode == NC_FILL);

                return value;
            }


            /*!
                @brief      Write an attribute to the variable
                @param      name Name of attribute
                @param      values Array values of attribute
                @return     Written attribute
                @exception  std::runtime_error In case the attribute cannot be added
            */
            template<Arithmetic T>
            auto add_attribute(std::string name, std::size_t const nr_values, T const* values) -> Attribute
            {
                return Attribute::add_attribute(_group_id, _variable_id, std::move(name), nr_values, values);
            }


            /*!
                @brief      Write an attribute to the variable
                @param      name Name of attribute
                @param      value Scalar value of attribute
                @return     Written attribute
                @exception  std::runtime_error In case the attribute cannot be added
            */
            template<Arithmetic T>
            auto add_attribute(std::string name, T const& value) -> Attribute
            {
                return Attribute::add_attribute(_group_id, _variable_id, std::move(name), 1, &value);
            }


            [[nodiscard]] auto has_attribute(std::string const& name) const -> bool;

            [[nodiscard]] auto attribute(std::string name) const -> Attribute;

            [[nodiscard]] auto attributes() const -> std::vector<Attribute>;


            /*!
                @brief      Write a scalar value
                @exception  std::runtime_error In case the value cannot be written
            */
            template<Arithmetic T>
            void write(T const& value)
            {
                assert(nr_dimensions() == 0);

                if (auto const status = detail::VariableValue<T>::put(_group_id, _variable_id, {}, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot write value: {}", error_message(status)));
                }
            }


            /*!
                @brief      Write an array element
                @exception  std::runtime_error In case the element cannot be written
            */
            template<Arithmetic T>
            void write(Indices const& idxs, T const& value)
            {
                if (auto const status = detail::VariableValue<T>::put(_group_id, _variable_id, idxs, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot write value: {}", error_message(status)));
                }
            }


            /*!
                @brief      Write a strided array
                @exception  std::runtime_error In case the array cannot be written
            */
            template<Arithmetic T>
            void write(Hyperslab const& hyperslab, T const* values)
            {
                assert(hyperslab.nr_dimensions() == nr_dimensions());

                if (auto const status =
                        detail::VariableValue<T>::put(_group_id, _variable_id, hyperslab, values);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot write values: {}", error_message(status)));
                }
            }


            /*!
                @brief      Write an array
                @exception  std::runtime_error In case the array cannot be written
            */
            template<Arithmetic T>
            void write(T const* value)
            {
                if (auto const status = detail::VariableValue<T>::put(_group_id, _variable_id, value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot write values: {}", error_message(status)));
                }
            }


            /*!
                @brief      Read a scalar value
                @exception  std::runtime_error In case the value cannot be read
            */
            template<Arithmetic T>
            void read(T& value) const
            {
                assert(nr_dimensions() == 0);

                if (auto const status = detail::VariableValue<T>::get(_group_id, _variable_id, {}, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot read value: {}", error_message(status)));
                }
            }


            /*!
                @brief      Read an array element
                @exception  std::runtime_error In case the element cannot be read
            */
            template<Arithmetic T>
            void read(Indices const& idxs, T& value) const
            {
                if (auto const status = detail::VariableValue<T>::get(_group_id, _variable_id, idxs, &value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot read value: {}", error_message(status)));
                }
            }


            /*!
                @brief      Write a strided array
                @exception  std::runtime_error In case the array cannot be read
            */
            template<Arithmetic T>
            void read(Hyperslab const& hyperslab, T* values) const
            {
                assert(hyperslab.nr_dimensions() == nr_dimensions());

                if (auto const status =
                        detail::VariableValue<T>::get(_group_id, _variable_id, hyperslab, values);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot read values: {}", error_message(status)));
                }
            }


            /*!
                @brief      Read an array
                @exception  std::runtime_error In case the array cannot be read
            */
            template<Arithmetic T>
            void read(T* value) const
            {
                if (auto const status = detail::VariableValue<T>::get(_group_id, _variable_id, value);
                    status != NC_NOERR)
                {
                    throw std::runtime_error(std::format("Cannot read values: {}", error_message(status)));
                }
            }

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _variable_id;
    };

}  // namespace lue::netcdf
