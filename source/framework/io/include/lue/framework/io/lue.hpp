#pragma once
#include "lue/framework/core/define.hpp"
#include "lue/framework/io/export.hpp"
#include <hpx/future.hpp>
#include <filesystem>


namespace lue::detail {

    LUE_FRAMEWORK_IO_EXPORT auto normalize(std::string const& pathname) -> std::filesystem::path;


    namespace root {

        LUE_FRAMEWORK_IO_EXPORT auto from_lue_order(std::filesystem::path const& dataset_path) -> Count;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_order(std::filesystem::path const& dataset_path) -> Count;


        LUE_FRAMEWORK_IO_EXPORT auto current_from_lue_order(std::filesystem::path const& dataset_path)
            -> Count;

        LUE_FRAMEWORK_IO_EXPORT auto current_to_lue_order(std::filesystem::path const& dataset_path) -> Count;


        LUE_FRAMEWORK_IO_EXPORT void add_to_lue_finished(
            std::filesystem::path const& path, Count count, hpx::future<void> future);

        LUE_FRAMEWORK_IO_EXPORT void add_from_lue_finished(
            std::filesystem::path const& path, Count count, hpx::future<void> future);


        LUE_FRAMEWORK_IO_EXPORT auto to_lue_finished(std::filesystem::path const& path, Count count)
            -> hpx::shared_future<void>;

        LUE_FRAMEWORK_IO_EXPORT auto from_lue_finished(std::filesystem::path const& path, Count count)
            -> hpx::shared_future<void>;

    }  // namespace root


    namespace worker {

        LUE_FRAMEWORK_IO_EXPORT auto from_lue_open_dataset_promise_for(
            std::filesystem::path const& path, Count open_count) -> hpx::promise<void>;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_open_dataset_promise_for(
            std::filesystem::path const& path, Count open_count) -> hpx::promise<void>;


        LUE_FRAMEWORK_IO_EXPORT auto from_lue_close_dataset_promise_for(
            std::filesystem::path const& path, Count open_count) -> hpx::promise<void>;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_close_dataset_promise_for(
            std::filesystem::path const& path, Count open_count) -> hpx::promise<void>;


        LUE_FRAMEWORK_IO_EXPORT auto from_lue_close_dataset_when_predecessor_done(
            std::filesystem::path const& path, Count open_count) -> hpx::shared_future<void>;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_close_dataset_when_predecessor_done(
            std::filesystem::path const& path, Count open_count) -> hpx::shared_future<void>;


#ifndef NDEBUG

        LUE_FRAMEWORK_IO_EXPORT auto from_lue_open_dataset_done(
            std::filesystem::path const& path, Count count) -> hpx::shared_future<void>;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_open_dataset_done(std::filesystem::path const& path, Count count)
            -> hpx::shared_future<void>;


        LUE_FRAMEWORK_IO_EXPORT auto from_lue_close_dataset_done_available(
            std::filesystem::path const& path, Count count) -> bool;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_close_dataset_done_available(
            std::filesystem::path const& path, Count count) -> bool;


        LUE_FRAMEWORK_IO_EXPORT auto from_lue_close_dataset_done(
            std::filesystem::path const& path, Count count) -> hpx::shared_future<void>;

        LUE_FRAMEWORK_IO_EXPORT auto to_lue_close_dataset_done(std::filesystem::path const& path, Count count)
            -> hpx::shared_future<void>;
#endif

    }  // namespace worker

}  // namespace lue::detail
