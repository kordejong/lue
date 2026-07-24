#pragma once
#include "lue/framework/core/define.hpp"
#include "lue/framework/io/export.hpp"
#include <hpx/future.hpp>
#include <filesystem>


namespace lue::detail::root {

    LUE_FRAMEWORK_IO_EXPORT auto normalize(std::string const& pathname) -> std::filesystem::path;


    LUE_FRAMEWORK_IO_EXPORT auto from_lue_order(std::filesystem::path const& dataset_path) -> Count;

    LUE_FRAMEWORK_IO_EXPORT auto to_lue_order(std::filesystem::path const& dataset_path) -> Count;


    LUE_FRAMEWORK_IO_EXPORT auto current_from_lue_order(std::filesystem::path const& dataset_path) -> Count;

    LUE_FRAMEWORK_IO_EXPORT auto current_to_lue_order(std::filesystem::path const& dataset_path) -> Count;


    LUE_FRAMEWORK_IO_EXPORT void add_to_lue_finished(
        std::filesystem::path const& path, Count count, hpx::future<void> future);

    LUE_FRAMEWORK_IO_EXPORT void add_from_lue_finished(
        std::filesystem::path const& path, Count count, hpx::future<void> future);


    LUE_FRAMEWORK_IO_EXPORT auto to_lue_finished(std::filesystem::path const& path, Count count)
        -> hpx::shared_future<void>;

    LUE_FRAMEWORK_IO_EXPORT auto from_lue_finished(std::filesystem::path const& path, Count count)
        -> hpx::shared_future<void>;

}  // namespace lue::detail::root
