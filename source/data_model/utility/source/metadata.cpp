#include "lue/utility/metadata.hpp"
#include "lue/core/aspect.hpp"
#include <filesystem>
#include <format>
#include <fstream>


namespace lue {
    namespace utility {
        namespace json {

            auto has_key(JSON const& object, std::string const& name) -> bool
            {
                return !object.is_null() && object.contains(name);
            }


            auto has_key(JSON const& object, JSONPointer const& pointer) -> bool
            {
                return !object.is_null() && !object.value(pointer, JSON()).is_null();
            }


            void assert_has_key(JSON const& object, std::string const& name)
            {
                if (!has_key(object, name))
                {
                    throw std::runtime_error(std::format("JSON object does not contain key {}", name));
                }
            }


            void assert_has_key(JSON const& object, JSONPointer const& pointer)
            {
                if (!has_key(object, pointer))
                {
                    throw std::runtime_error(
                        std::format("JSON object does not contain key {}", pointer.to_string()));
                }
            }


            auto object(JSON const& object, std::string const& name) -> JSON
            {
                assert_has_key(object, name);

                return object[name];
            }


            auto object(JSON const& object, JSONPointer const& pointer) -> JSON
            {
                assert_has_key(object, pointer);

                return object[pointer];
            }


            /*!
                @brief      Find object in list
                @param      object Object to query
                @param      pointer Pointer to list
                @param      key Name of element to query
                @param      value Value of element to compare
                @exception  std::runtime_error In case the object cannot be found
            */
            auto object(
                JSON const& object,
                JSONPointer const& pointer,
                std::string const& key,
                std::string const& value) -> JSON
            {
                assert_has_key(object, pointer);

                auto const& list_json = json::object(object, pointer);
                auto const object_json_it = json::find(list_json, key, value);

                if (object_json_it == list_json.end())
                {
                    throw std::runtime_error(
                        std::format(
                            "No object whose key {} equals {} exists at {}",
                            key,
                            value,
                            pointer.to_string()));
                }

                return *object_json_it;
            }


            /*!
                @brief      Find object in list
                @param      object List object to query
                @param      key Name of element to query
                @param      value Value of element to compare
                @exception  std::runtime_error In case the object cannot be found
            */
            auto object(JSON const& object, std::string const& key, std::string const& value) -> JSON
            {
                auto const object_json_it = json::find(object, key, value);

                if (object_json_it == object.end())
                {
                    throw std::runtime_error(
                        std::format("No object whose key {} equals {} exists", key, value));
                }

                return *object_json_it;
            }


            auto string(JSON const& object, std::string const& name) -> std::string
            {
                return json::object(object, name);
            }


            auto string(JSON const& object, JSONPointer const& pointer) -> std::string
            {
                return json::object(object, pointer);
            }


            auto boolean(JSON const& object, JSONPointer const& pointer) -> bool
            {
                return json::object(object, pointer);
            }


            auto pointer(JSON const& object, std::string const& name) -> JSONPointer
            {
                return JSONPointer(string(object, name));
            }


            /*!
                @brief      Find an object in a list of objects
                @param      object List object to search
                @param      key Name of element to check
                @param      value Value of element to find
            */
            auto find(JSON const& object, std::string const& key, std::string const& value) -> JSONCIterator
            {
                JSONCIterator it;

                for (it = object.begin(); it != object.end(); ++it)
                {
                    // The key must exist.
                    assert_has_key(*it, key);

                    if (json::string((*it), key) == value)
                    {
                        break;
                    }
                }

                return it;
            }


            auto clock(JSON const& object) -> data_model::Clock
            {
                return data_model::Clock{object.at("unit"), object.at("nr_units")};
            }

        }  // namespace json


        /*!
            @brief      Construct an instance given a pathname to a file containing
                        metadata items
            @exception  std::runtime_error In case the file does not exist
        */
        Metadata::Metadata(std::string const& pathname)

            :
            _json()

        {
            if (!std::filesystem::exists(pathname))
            {
                throw std::runtime_error(
                    std::format("File containing metadata ({}) does not exist", pathname));
            }

            std::ifstream(pathname) >> _json;
        }


        /*!
            @brief      Construct an instance given an input stream containing
                        metadata items
        */
        Metadata::Metadata(std::istream& stream)

            :
            _json()

        {
            stream >> _json;
        }


        auto Metadata::object() const -> JSON const&
        {
            return _json;
        }


        auto Metadata::string(JSONPointer const& pointer, std::string const& default_value) const
            -> std::string
        {
            return json::has_key(_json, pointer) ? json::string(_json, pointer) : default_value;
        }


        auto Metadata::boolean(JSONPointer const& pointer, bool const default_value) const -> bool
        {
            return json::has_key(_json, pointer) ? json::boolean(_json, pointer) : default_value;
        }


        auto Metadata::string(
            std::string const& list_name,
            std::string const& key,
            std::string const& value,
            JSONPointer const& pointer,
            std::string const& default_value) const -> std::string
        {
            std::string result = default_value;

            // Check whether a json element with the name of the list to search
            // exists.
            if (json::has_key(_json, list_name))
            {
                // Obtain the object that should be the list.
                auto const list_json = json::object(_json, list_name);

                // Find the object that contains an element with a (key, value).
                auto const object_json_it = json::find(list_json, key, value);

                if (object_json_it != list_json.end())
                {
                    auto const& object_json = *object_json_it;

                    // Find the string pointed to by the pointer.
                    if (json::has_key(object_json, pointer))
                    {
                        result = json::string(object_json, pointer);
                    }
                }
            }

            return result;
        }


        auto Metadata::string(
            std::string const& list_name,
            std::string const& key,
            std::string const& value,
            JSONPointer const& pointer,
            std::string const& value_key,
            std::string const& default_value) const -> std::string
        {
            std::string result = default_value;

            // Check whether a json element with the name of the list to search
            // exists.
            if (json::has_key(_json, list_name))
            {
                // Obtain the object that should be the list.
                auto const list_json = json::object(_json, list_name);

                // Find the object that contains an element with a (key, value).
                auto const object_json_it = json::find(list_json, key, value);

                if (object_json_it != list_json.end())
                {
                    auto const& object_json = *object_json_it;

                    // Find the string pointed to by the pointer.
                    if (json::has_key(object_json, pointer))
                    {

                        auto const& object2_json = json::object(object_json, pointer);

                        if (json::has_key(object2_json, value_key))
                        {
                            result = json::string(object2_json, value_key);
                        }
                    }
                }
            }

            return result;
        }

    }  // namespace utility


    namespace data_model::time {

        void from_json(utility::JSON const& object, Unit& unit)
        {
            unit = string_to_aspect<Unit>(object.get<std::string>());
            // unit = string_to_unit(object.get<std::string>());
        }

    }  // namespace data_model::time

}  // namespace lue
