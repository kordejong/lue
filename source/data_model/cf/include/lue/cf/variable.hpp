// #pragma once
// #include "lue/netcdf4/variable.hpp"
//
//
// namespace lue::cf {
//
//     /*!
//         @brief      .
//         @tparam     .
//         @param      .
//         @return     .
//         @exception  .
//
//         - Variable names are not standardized
//     */
//     class Variable: public netcdf4::Variable
//     {
//
//         public:
//
//             enum class Kind {
//                 // auxiliary_coordinate,
//                 // scalar_coordinate,
//                 // multidimensional_coordinate,
//                 // ancillary
//                 coordinate,
//                 regular,
//             };
//
//             // static auto is_coordinate(netcdf4::Variable const& variable) -> bool;
//
//             // static auto is_scalar(netcdf4::Variable const& variable) -> bool;
//
//             Variable(netcdf4::Variable const& variable);
//
//             [[nodiscard]] auto standard_name() const -> std::string;
//
//             [[nodiscard]] auto long_name() const -> std::string;
//
//             [[nodiscard]] auto units() const -> std::string;
//
//             [[nodiscard]] auto kind() const -> Kind;
//
//             [[nodiscard]] auto axis() const -> std::string;
//
//             [[nodiscard]] auto positive() const -> std::string;
//     };
//
// }  // namespace lue::cf
