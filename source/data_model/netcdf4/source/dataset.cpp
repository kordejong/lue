#include "lue/netcdf4/dataset.hpp"
#include "lue/netcdf4/error.hpp"
#include "lue/string.hpp"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <sstream>


namespace lue::netcdf4 {

    /*!
        @brief      Create dataset
        @param      name Name of dataset
        @param      create_mode One of NC_CLOBBER, NC_NOCLOBBER, NC_DISKLESS, NC_PERSIST. To force the use
                    of netCDF4 format, create mode NC_NETCDF4 is added unconditionally, so the caller doesn't
                    have to.
        @return     Created dataset
        @exception  std::runtime_error In case @a create_mode is not compatible with netCDF-4 format or in
                    case the dataset cannot be created
    */
    auto Dataset::create(std::string const& name, int create_mode) -> Dataset
    {
        create_mode |= NC_NETCDF4;

        // if(create_mode &
        //
        // NC_SHARE
        // NC_64BIT_OFFSET
        // NC_64BIT_DATA
        // NC_CLASSIC_MODEL
        //
        // TODO Throw exception, invalid format
        //
        // NC_MMAP
        //
        // TODO Throw exception, deprecated create_mode

        int dataset_id{};

        if (auto const status = nc_create(name.c_str(), create_mode, &dataset_id); status != NC_NOERR)
        {
            throw std::runtime_error(
                std::format("Cannot create dataset {}: {}", name, error_message(status)));
        }

        return dataset_id;
    }


    /*!
        @brief      Open dataset
        @param      open_mode NC_NOWRITE, NC_WRITE, NC_SHARE, NC_WRITE | NC_SHARE
        @param      name Name of dataset
        @return     Opened dataset
        @exception  std::runtime_error In case the dataset cannot be opened
    */
    auto Dataset::open(std::string const& name, int open_mode) -> Dataset
    {
        int dataset_id{};

        if (auto const status = nc_open(name.c_str(), open_mode, &dataset_id); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot open dataset {}: {}", name, error_message(status)));
        }

        return dataset_id;
    }


    /*!
        @brief      Return string representation of the @a format passed in
        @exception  std::runtime_error In case @a format is not known
    */
    auto Dataset::format_as_string(int format) -> std::string
    {
        std::string string{};

        switch (format)
        {
            case NC_FORMAT_CLASSIC:
            {
                string = "NC_FORMAT_CLASSIC";
                break;
            }
            case NC_FORMAT_64BIT_OFFSET:
            {
                string = "NC_FORMAT_64BIT_OFFSET";
                break;
            }
            case NC_FORMAT_CDF5:
            {
                string = "NC_FORMAT_CDF5";
                break;
            }
            case NC_FORMAT_NETCDF4:
            {
                string = "NC_FORMAT_NETCDF4";
                break;
            }
            case NC_FORMAT_NETCDF4_CLASSIC:
            {
                string = "NC_FORMAT_NETCDF4_CLASSIC";
                break;
            }
            default:
            {
                throw std::runtime_error(std::format("Unknown format with ID: {}", format));
            }
        }

        return string;
    }


    /*!
        @brief      Create an instance given @a dataset_id passed in
    */
    Dataset::Dataset(int const dataset_id):

        Group{dataset_id}

    {
    }


    /*!
        @brief      Move construct an instance
        @param      other Instance whose value is not needed anymore
    */
    Dataset::Dataset(Dataset&& other) noexcept:

        Group{std::move(other)}

    {
        assert(!other.id_is_valid());
    }


    /*!
        @brief      Destruct instance

        In the dataset is open, it will be closed.
    */
    Dataset::~Dataset() noexcept
    {
        if (Group::id_is_valid())
        {
            if (auto const status = nc_close(Group::id()); status != NC_NOERR)
            {
                // TODO Can't throw here. Log an error? Abort? Forget about it?
            }
        }
    }


    /*!
        @brief      Move assign an instance
        @param      other Instance whose value is not needed anymore
    */
    auto Dataset::operator=(Dataset&& other) noexcept -> Dataset&
    {
        Group::operator=(std::move(other));

        return *this;
    }


    /*!
        @brief      Release the ownership of the layered dataset ID
        @return     Dataset ID
    */
    auto Dataset::release() -> int
    {
        return reset_id();
    }


    /*!
        @brief      Return the path (or opendap URL) of the dataset
        @exception  std::runtime_error In case the path cannot be obtained
    */
    auto Dataset::path() const -> std::string
    {
        std::size_t nr_bytes{0};

        if (auto const status = nc_inq_path(id(), &nr_bytes, nullptr); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get path length: {}", error_message(status)));
        }

        std::string name(nr_bytes, 'x');

        if (auto const status = nc_inq_path(id(), nullptr, name.data()); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get path: {}", error_message(status)));
        }

        return name;
    }


    /*!
        @brief      Return the NetCDF format identifier
        @exception  std::runtime_error In case the format cannot be obtained
    */
    auto Dataset::format() const -> int
    {
        int format{0};

        if (auto const status = nc_inq_format(id(), &format); status != NC_NOERR)
        {
            throw std::runtime_error(std::format("Cannot get format: {}", error_message(status)));
        }

        return format;
    }


    /*!
        @brief      Write the @a conventions to the `Conventions` attribute

        - Each convention will be trimmed of leading and trailing whitespace
        - Empty strings will be disregarded
        - If any convention contains whitespace, conventions are separated by a comma. If not, they are
          separated by a space.
        - Existing content is overwritten
    */
    void Dataset::set_conventions(std::vector<std::string> conventions)
    {
        // Write "Conventions" attribute. If one of the conventions contains a space, the conventions will
        // be separated by a comma. Otherwise they will be with a space.

        // Get rid of strings that only contain whitespace
        for (auto& convention : conventions)
        {
            convention = trim(convention);
        }

        conventions.erase(
            std::remove(conventions.begin(), conventions.end(), std::string{}), conventions.end());

        if (!conventions.empty())
        {
            // Build a string containing all conventions passed in and write is as the attribute's value
            auto is_space = [](unsigned char const character) { return std::isspace(character); };
            auto contains_space = [is_space](std::string const& convention)
            { return std::ranges::find_if(std::string_view{convention}, is_space); };

            char const separator =
                std::ranges::find_if(conventions.begin(), conventions.end(), contains_space) !=
                        conventions.end()
                    ? ','
                    : ' ';

            // C++23: auto joined_conventions = conventions | std::views::join_with(separator);

            std::ostringstream stream{};

            stream << conventions[0];

            for (std::size_t idx = 1; idx < conventions.size(); ++idx)
            {
                stream << separator << conventions[idx];
            }

            add_attribute("Conventions", stream.str());
        }
    }


    /*!
        @brief      Return the convention names stored in the `Conventions` attribute or an empty collection
                    if no such attribute is present

    */
    auto Dataset::conventions() const -> std::vector<std::string>
    {
        std::vector<std::string> convention_names{};

        if (has_attribute("Conventions"))
        {
            auto const conventions{attribute("Conventions").value()};

            // If the string contains a comma, it is assumed to be a comma-separated list. If not, it is
            // assumed to be a space separated list.
            char const separator = conventions.find(',') != std::string::npos ? ',' : ' ';

            for (auto const sub_range : std::ranges::split_view(conventions, separator))
            {
                std::string_view convention_name{trim({sub_range.begin(), sub_range.end()})};

                if (!convention_name.empty())
                {
                    convention_names.emplace_back(convention_name);
                }
            }
        }

        return convention_names;
    }

}  // namespace lue::netcdf4
