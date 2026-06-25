#pragma once
#include "lue/framework/core/assert.hpp"
#include "lue/framework/core/define.hpp"
#include "lue/framework/core/locality.hpp"
#include <concepts>
#include <map>


namespace lue {
    namespace root {

        /*!
            @brief      Type used for managing a resource use count per key
        */
        template<std::equality_comparable Key>
        using ResourceUseCountByKey = std::map<Key, Count>;


        namespace detail {

            // Return a default instance, only dependent on type type of the Key. Use this if the collection
            // is shared between all keys.
            template<std::equality_comparable Key>
            auto resource_use_count_by() -> ResourceUseCountByKey<Key>&
            {
                static ResourceUseCountByKey<Key> use_count_by{};

                return use_count_by;
            }

        }  // namespace detail


        /*!
            @brief      Return the positive, non-zero use count by @a key

            The first time this function is called for a certain key, 1 is returned,
        */
        template<std::equality_comparable Key>
        auto resource_use_count_by(ResourceUseCountByKey<Key>& use_count_by, Key const& key) -> Count
        {
            lue_hpx_assert(on_root_locality());

            // If this is the first time key is added, initialize the count to 0
            use_count_by.try_emplace(key, 0);

            // Increment the count and return it
            return ++use_count_by.at(key);
        }


        /*!
            @overload
        */
        template<std::equality_comparable Key>
        auto resource_use_count_by(Key const& key) -> Count
        {
            return resource_use_count_by(detail::resource_use_count_by<Key>(), key);
        }


        /*!
            @brief      Type used for managing future per key and count representing some resource usage
                        to have finished
        */
        template<std::equality_comparable Key>
        using ResourceUseFinished = std::map<Key, std::map<Count, hpx::shared_future<void>>>;


        namespace detail {

            // Return a default instance, only dependent on type type of the Key. Use this if the collection
            // is shared between all keys.
            template<std::equality_comparable Key>
            auto resource_use_finished() -> ResourceUseFinished<Key>&
            {
                static ResourceUseFinished<Key> use_finished{};

                return use_finished;
            }

        }  // namespace detail


        /*!
            @brief      Add the @a future representing the @a count duration that a resource at the @a key
                        in @a use_finished is used

            Once the future becomes ready, it can be assumed that the next in line, having use count equal to
            count + 1, can use it.
        */
        template<std::equality_comparable Key>
        void add_resource_use_finished(
            ResourceUseFinished<Key>& use_finished,
            Key const& key,
            Count const count,
            hpx::future<void> future)
        {
            lue_hpx_assert(on_root_locality());

            lue_hpx_assert((!use_finished.contains(key)) || (!use_finished.at(key).contains(count)));
            lue_hpx_assert(future.valid());

            use_finished[key][count] = future.share();  // std::move(future);

            lue_hpx_assert(use_finished.contains(key));
            lue_hpx_assert(use_finished[key].contains(count));
            lue_hpx_assert(use_finished.at(key).at(count).valid());
        }


        /*!
            @overload
        */
        template<std::equality_comparable Key>
        void add_resource_use_finished(Key const& key, Count const count, hpx::future<void>&& future)
        {
            return add_resource_use_finished(
                detail::resource_use_finished<Key>(), key, count, std::move(future));
        }


        /*!
            @brief      Return the future representing the @a count duration that a resource at the @a key
                        in @a use_finished is used

            Once the future becomes ready, it can be assumed that the next in line, having use count equal to
            count + 1, can use it.
        */
        template<std::equality_comparable Key>
        auto resource_use_finished(ResourceUseFinished<Key>& use_finished, Key const& key, Count const count)
            -> hpx::shared_future<void>
        {
#ifndef NDEBUG
            lue_hpx_assert(on_root_locality());

            if (count > 0)
            {
                lue_hpx_assert(use_finished.contains(key));
                lue_hpx_assert(use_finished[key].contains(count));
                lue_hpx_assert(use_finished[key][count].valid());
            }
#endif

            return count > 0 ? use_finished.at(key).at(count) : hpx::make_ready_future().share();
        }


        /*!
            @overload
        */
        template<std::equality_comparable Key>
        auto resource_use_finished(Key const& key, Count const count) -> hpx::shared_future<void>
        {
            return resource_use_finished(detail::resource_use_finished<Key>(), key, count);
        }


        /*!
            @brief      Signal that @a count use of resource at @a key in @a use_finished is handled

            Implementation resources for managing the @a count of the resource will be erased. It is not
            strictly necessary to call this function, but if @a count can become large, it is needed to
            prevent accumulation of memory usage.
        */
        template<std::equality_comparable Key>
        void resource_use_handled(ResourceUseFinished<Key>& use_finished, Key const& key, Count const count)
        {
#ifndef NDEBUG
            lue_hpx_assert(on_root_locality());

            lue_hpx_assert(use_finished.contains(key));

            if (count == 0)
            {
                lue_hpx_assert(!use_finished[key].contains(count));
            }
            else
            {
                lue_hpx_assert(use_finished[key].contains(count));
                lue_hpx_assert(use_finished[key][count].valid());
                lue_hpx_assert(use_finished[key][count].is_ready());
            }
#endif

            use_finished.at(key).erase(count);
        }


        /*!
            @overload
        */
        template<std::equality_comparable Key>
        void resource_use_handled(Key const& key, Count const count)
        {
            return resource_use_handled(detail::resource_use_finished<Key>(), key, count);
        }

    }  // namespace root
}  // namespace lue
