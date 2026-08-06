#include "lue/array/same_shape/constant_shape/value.hpp"
#include <algorithm>


namespace lue::data_model::same_shape::constant_shape {

    /*!
        @brief      Open value @a name in @a parent
    */
    Value::Value(hdf5::Group const& parent, std::string const& name):

        Array{parent, name}

    {
    }


    /*!
        @brief      Open value @a name in @a parent
    */
    Value::Value(hdf5::Group const& parent, std::string const& name, hdf5::Datatype const& memory_datatype):

        Array{parent, name, memory_datatype}

    {
    }


    /*!
        @brief      Move in @a dataset
    */
    Value::Value(Array&& array):

        Array{std::move(array)}

    {
    }


    /*!
        @brief      Make space for an additional number of object arrays
        @param      nr_arrays Number of object arrays
    */
    void Value::expand(Count const nr_arrays)
    {
        // Get current shape of the underlying dataset, and update it for
        // the new size
        hdf5::Shape shape{this->shape()};
        shape[0] += nr_arrays;

        // Resize the dataset
        resize(shape);
    }


    /*!
        @brief      Return number of arrays for which space is reserved
    */
    auto Value::nr_arrays() const -> Count
    {
        return shape()[0];
    }


    /*!
        @brief      Return shape of each object array

        The shape returned is not the shape of the underlying HDF5 dataset. It
        is the shape of each of the individual object arrays.
    */
    auto Value::array_shape() const -> hdf5::Shape
    {
        hdf5::Shape const& shape{this->shape()};

        assert(!shape.empty());

        return hdf5::Shape{shape.begin() + 1, shape.end()};
    }


    auto Value::rank() const -> Rank
    {
        return static_cast<Rank>(array_shape().size());
    }


    auto Value::hyperslab(IndexRange const& range) const -> hdf5::Hyperslab
    {
        hdf5::Shape const& shape{this->shape()};

        hdf5::Offset offset(shape.size(), 0);
        offset[0] = range.begin();

        hdf5::Count count(shape.begin(), shape.end());
        count[0] = range.size();

        return hdf5::Hyperslab{offset, count};
    }


    auto Value::hyperslab(Index const idx) const -> hdf5::Hyperslab
    {
        return hyperslab({idx, idx + 1});
    }


    void Value::read(Index const idx, void* buffer) const
    {
        Array::read(hyperslab(idx), buffer);
    }


    void Value::read(IndexRange const& range, void* buffer) const
    {
        Array::read(hyperslab(range), buffer);
    }


    void Value::write(Index const idx, void const* buffer)
    {
        Array::write(hyperslab(idx), buffer);
    }


    void Value::write(IndexRange const& range, void const* buffer)
    {
        Array::write(hyperslab(range), buffer);
    }


    /*!
        @brief      Create value @a name in @a parent
    */
    auto create_value(
        hdf5::Group& parent,
        std::string const& name,
        hdf5::Datatype const& memory_datatype,
        void const* no_data_value) -> Value
    {
        std::size_t const nr_chunk_dimensions_to_skip = 0;
        std::optional<hdf5::Shape> const& chunk_shape{};

        return create_value(
            parent,
            name,
            file_datatype(memory_datatype),
            memory_datatype,
            hdf5::Shape{},
            no_data_value,
            nr_chunk_dimensions_to_skip,
            chunk_shape);
    }


    /*!
        @brief      Create value @a name in @a parent
    */
    auto create_value(
        hdf5::Group& parent,
        std::string const& name,
        hdf5::Datatype const& memory_datatype,
        hdf5::Shape const& array_shape,
        void const* no_data_value,
        std::size_t nr_chunk_dimensions_to_skip,
        std::optional<hdf5::Shape> const& chunk_shape) -> Value
    {
        return create_value(
            parent,
            name,
            file_datatype(memory_datatype),
            memory_datatype,
            array_shape,
            no_data_value,
            nr_chunk_dimensions_to_skip,
            chunk_shape);
    }


    /*!
        @brief      Create value @a name in @a parent
    */
    auto create_value(
        hdf5::Group& parent,
        std::string const& name,
        hdf5::Datatype const& file_datatype,
        hdf5::Datatype const& memory_datatype,
        void const* no_data_value) -> Value
    {
        std::size_t const nr_chunk_dimensions_to_skip = 0;
        std::optional<hdf5::Shape> const& chunk_shape{};

        return create_value(
            parent,
            name,
            file_datatype,
            memory_datatype,
            hdf5::Shape{},
            no_data_value,
            nr_chunk_dimensions_to_skip,
            chunk_shape);
    }


    /*!
        @brief      Create value @a name in @a parent

        The @a array_shape passed in is the shape of each of the individual object arrays.

        The underlying HDF5 dataset is chunked according to hdf5::chunk_shape().
    */
    auto create_value(
        hdf5::Group& parent,
        std::string const& name,
        hdf5::Datatype const& file_datatype,
        hdf5::Datatype const& memory_datatype,
        hdf5::Shape const& array_shape,
        void const* no_data_value,
        std::size_t const nr_chunk_dimensions_to_skip,
        std::optional<hdf5::Shape> chunk_shape) -> Value
    {
        // The rank of the underlying dataset is one larger than the rank of the
        // object arrays. Object arrays are stored one after the other.
        hdf5::Shape dimension_sizes{array_shape};
        dimension_sizes.insert(dimension_sizes.begin(), 0);

        hdf5::Shape max_dimension_sizes{array_shape};
        max_dimension_sizes.insert(max_dimension_sizes.begin(), H5S_UNLIMITED);

        hdf5::Dataspace const dataspace{hdf5::create_dataspace(dimension_sizes, max_dimension_sizes)};

        hdf5::Dataset::CreationPropertyList creation_property_list;

        if (!chunk_shape)
        {
            hdf5::Shape chunk_shape_{
                hdf5::chunk_shape(array_shape, nr_chunk_dimensions_to_skip, file_datatype.size())};
            chunk_shape = chunk_shape_;
        }
        else
        {
            // In the data model API, chunk shapes are in bytes, so the same shape can be used for different,
            // same shaped LUE arrays, whatever the number of bytes per element.
            // In HDF5, chunk shapes are in number of elements, so different shapes should be used for
            // different, same shaped HDF5 datasets, depending on the number of bytes per element.
            hdf5::Shape& chunk_shape_{*chunk_shape};

            for (auto& extent : chunk_shape_)
            {
                assert(extent >= file_datatype.size());
                extent /= file_datatype.size();
            }
        }

        hdf5::Shape& chunk_shape_{*chunk_shape};

        for (std::size_t idx = 0; idx < chunk_shape_.size(); ++idx)
        {
            chunk_shape_[idx] = std::clamp<hdf5::Shape::value_type>(chunk_shape_[idx], 1, array_shape[idx]);
        }

        chunk_shape_.insert(chunk_shape_.begin(), 1);  // For the first, objects, dimension

        assert(static_cast<int>(chunk_shape_.size()) == dataspace.nr_dimensions());

        creation_property_list.set_chunk(chunk_shape_);
        creation_property_list.set_alloc_time(H5D_ALLOC_TIME_LATE);

        if (no_data_value != nullptr)
        {
            creation_property_list.set_fill_value(memory_datatype, no_data_value);
        }

        hdf5::Dataset dataset{
            hdf5::create_dataset(parent.id(), name, file_datatype, dataspace, creation_property_list)};

        return Value{Array{std::move(dataset), memory_datatype}};
    }


    auto value_exists(hdf5::Group const& parent, std::string const& name) -> bool
    {
        return hdf5::dataset_exists(parent.id(), name);
    }

}  // namespace lue::data_model::same_shape::constant_shape
