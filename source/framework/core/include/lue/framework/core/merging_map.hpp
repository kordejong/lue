#pragma once
#include <cassert>
#include <map>
#include <ranges>
#include <stdexcept>
#include <vector>


namespace lue {

    /*!
        @brief      An associative map supporting that multiple keys map to the same value (n to 1 relations)
        @tparam     Key The type for keys
        @tparam     Value The type for values

        This data structure maps multiple (different) keys to the same value. With the same value, really the
        same value is meant. Not the same by equality, but the same by identity. Changing a single value will
        be noticeable / visible by accesses by all keys associated with that value.
    */
    template<typename Key, typename Value>
    class MergingMap
    {

        public:

            using Count = std::size_t;

            using CountedValue = std::tuple<Value, Count>;

            MergingMap() = default;

            MergingMap(MergingMap const&) = default;

            MergingMap(MergingMap&&) = default;

            ~MergingMap() = default;

            auto operator=(MergingMap const&) -> MergingMap& = default;

            auto operator=(MergingMap&&) -> MergingMap& = default;


            /*!
                @brief      Return the number of keys associated with values
            */
            [[nodiscard]] auto nr_keys() const -> Count
            {
                return std::size(_idx_by_key);
            }


            /*!
                @brief      Return the number of values associated with keys

                The returned value will always be smaller or equal to the @a nr_keys().
            */
            [[nodiscard]] auto nr_values() const -> Count
            {
                return std::size(_value_by_idx);
            }


            /*!
                @brief      Return whether @a key is in the map
            */
            auto contains(Key const& key) const -> bool
            {
                return _idx_by_key.contains(key);
            }


            /*!
                @brief      Return the value associated with @a key
                @exception  std::invalid_argument In case the key is not in the map
            */
            auto operator[](Key const& key) const -> CountedValue const&
            {
                return _value_by_idx[idx_idx(key)];
            }


            /*!
                @brief      Add a mapping between @a keys and a @a value
                @param      keys Keys to map to the @a value. Non of these should already be in the map.
                @param      value Value mapped by the @a keys. Its reference count will be initialized to the
                            number of @a keys
                @exception  std::invalid_argument In case one of the keys is already in the map
            */
            auto insert(std::ranges::range auto const& keys, Value value) -> void
            {
                Index const value_idx{std::size(_value_by_idx)};
                Index const idx_idx{std::size(_idx_by_idx)};

                _value_by_idx.push_back({std::move(value), std::size(keys)});
                _idx_by_idx.push_back(value_idx);

                for (auto const& key : keys)
                {
                    insert_idx_by_key(key, idx_idx);
                }

                // return {Iterator{value_idx}, true};
            }


            /*!
                @brief      Associate @a new_key to value already mapped to @a key
                @param      new_key Key to map to @a key 's value
                @exception  std::invalid_argument In case one of the keys is already in the map
            */
            void associate(Key const& new_key, Key const& key)
            {
                Index const idx_idx{this->idx_idx(key)};

                insert_idx_by_key(new_key, idx_idx);
                increment_reference_count(idx_idx);
            }


            /*!
                @brief      Disassociate @a key from its value
                @exception  std::invalid_argument In case the key is not in the map

                In case the reference-count of the value associated with the @a key drops to zero, it is
                removed from the map.
            */
            void disassociate(Key const& key)
            {
                Index const idx_idx{this->idx_idx(key)};

                decrement_reference_count(idx_idx);
                _idx_by_key.erase(key);

                if (reference_count(idx_idx) == 0)
                {
                    // Remove info about the mapped-to value. None of the keys is associated with this value
                    // anymore. We are assuming all values in _value_by_idx are pointed to by a single record
                    // in idx_by_idx.

                    Index const value_idx = _idx_by_idx[idx_idx];

                    _value_by_idx.erase(_value_by_idx.begin() + value_idx);
                    _idx_by_idx.erase(_idx_by_idx.begin() + idx_idx);
                }
            }


            /*!
                @brief      Update the @a value associated with a @a key
                @exception  std::invalid_argument In case the key is not in the map
            */
            void update_value(Key const& key, Value value)
            {
                Index const idx_idx{this->idx_idx(key)};
                Index const value_idx = _idx_by_idx[idx_idx];
                std::get<0>(_value_by_idx[value_idx]) = std::move(value);
            }


        private:

            using Index = std::size_t;


            auto idx_idx(Key const& key) const -> Index
            {
                auto pos = _idx_by_key.find(key);

                if (pos == _idx_by_key.end())
                {
                    throw std::invalid_argument{"key not found"};
                }

                return pos->second;
            }


            void insert_idx_by_key(Key const& key, Index const idx_idx)
            {
                auto [pos, inserted] = _idx_by_key.insert({key, idx_idx});

                if (!inserted)
                {
                    throw std::invalid_argument{"key already present"};
                }
            }


            [[nodiscard]] auto reference_count(Index const idx_idx) const -> Count
            {
                return std::get<1>(_value_by_idx[idx_idx]);
            }


            void increment_reference_count(Index const idx_idx)
            {
                // It should have been deleted already
                assert(std::get<1>(_value_by_idx[idx_idx]) != 0);

                ++std::get<1>(_value_by_idx[idx_idx]);
            }


            void decrement_reference_count(Index const idx_idx)
            {
                // It should have been deleted already
                assert(std::get<1>(_value_by_idx[idx_idx]) != 0);

                --std::get<1>(_value_by_idx[idx_idx]);
            }


            // Mapping from key to index into _idx_by_idx
            std::map<Key, Index> _idx_by_key;

            // Mapping from index to index into _value_by_idx
            std::vector<Index> _idx_by_idx;

            // Otherwise, update Index
            static_assert(std::is_same_v<std::vector<Index>::size_type, Index>);

            // Mapping from index to value
            std::vector<CountedValue> _value_by_idx;
    };

}  // namespace lue
