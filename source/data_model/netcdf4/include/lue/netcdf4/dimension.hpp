#pragma once
#include <netcdf.h>
#include <string>


namespace lue::netcdf {

    class Dimension
    {

        public:

            Dimension(int group_id, int dimension_id);

            Dimension(Dimension const& other) = delete;

            Dimension(Dimension&& other) noexcept;

            ~Dimension() = default;

            auto operator=(Dimension const& other) -> Dimension& = delete;

            auto operator=(Dimension&& other) noexcept -> Dimension&;

            [[nodiscard]] auto id() const -> int;

            [[nodiscard]] auto name() const -> std::string;

            [[nodiscard]] auto length() const -> std::size_t;

        private:

            //! ID of the group
            int _group_id;

            //! ID of the variable
            int _dimension_id;
    };

}  // namespace lue::netcdf
