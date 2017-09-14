#include "lue/hdf5/dataset.hpp"
#include "lue/hdf5/datatype.hpp"
#include "lue/hdf5/link.hpp"
#include <cassert>
#include <cstring>
#include <iostream>


namespace lue {
namespace hdf5 {

bool dataset_exists(
    Identifier const& identifier,
    std::string const& name)
{
    return link_exists(identifier, name) && link_is_dataset(identifier, name);
}


Dataset::CreationPropertyList::CreationPropertyList()

    : PropertyList(H5P_DATASET_CREATE)

{
}


void Dataset::CreationPropertyList::set_chunk(
    Shape const& chunk)
{
    auto status = ::H5Pset_chunk(id(), chunk.size(), chunk.data());

    if(status < 0) {
        throw std::runtime_error("Cannot set chunk size");
    }
}


/*!
    @brief      Construct a dataset instance based on an existing
                HDF5 dataset
    @exception  std::runtime_error In case dataset named @a name and
                located at @a location cannot be opened
*/
Dataset::Dataset(
    Identifier const& location,
    std::string const& name)

    : _id(::H5Dopen(location, name.c_str(), H5P_DEFAULT), ::H5Dclose)

{
    if(!_id.is_valid()) {
        throw std::runtime_error("Cannot open dataset " + name + " at " +
            location.pathname());
    }

    assert(_id.is_valid());
}


Dataset::Dataset(
    Identifier&& id)

    : _id{std::forward<Identifier>(id)}

{
    assert(_id.is_valid());
}


Identifier const& Dataset::id() const
{
    return _id;
}


/*!
    @brief      Return the datatype of the dataset

    The datatype returned is the in-file datatype of the dataset
*/
Datatype Dataset::datatype() const
{
    return Datatype(Identifier(::H5Dget_type(_id), ::H5Tclose));
}


void Dataset::resize(
    Shape const& new_dimension_sizes)
{
    assert(static_cast<int>(new_dimension_sizes.size()) ==
        dataspace().nr_dimensions());

    auto status = ::H5Dset_extent(_id, new_dimension_sizes.data());

    if(status < 0) {
        throw std::runtime_error("Cannot resize dataset");
    }
}


Dataspace Dataset::dataspace() const
{
    assert(_id.is_valid());

    return Dataspace(::H5Dget_space(_id));
}


Shape Dataset::shape() const
{
    assert(_id.is_valid());

    return dataspace().dimension_extents();
}


void Dataset::read(
    Datatype const& datatype,
    void* buffer) const
{
    read(datatype, Hyperslab(shape()), buffer);
}


void Dataset::read(
    Datatype const& datatype,
    Hyperslab const& hyperslab,
    void* buffer) const
{
    assert(datatype.is_native());

    // Select elements: create hyperslab
    auto const file_dataspace = this->dataspace();
    hsize_t const* block = nullptr;
    auto status = ::H5Sselect_hyperslab(file_dataspace.id(), H5S_SELECT_SET,
        hyperslab.start().data(), hyperslab.stride().data(),
        hyperslab.count().data(), block);

    if(status < 0) {
        throw std::runtime_error("Cannot create hyperslab");
    }

    assert(_id.is_valid());
    assert(file_dataspace.id().is_valid());


    auto const memory_dataspace = create_dataspace(
        Shape(hyperslab.count().begin(), hyperslab.count().end()));

    status = ::H5Dread(
        _id, datatype.id(),
        memory_dataspace.id(), file_dataspace.id(),
        H5P_DEFAULT, buffer);

    if(status < 0) {
        throw std::runtime_error("Cannot read from dataset");
    }
}


/*!
    @brief      Write buffer to underlying HDF5 dataset
    @param      datatype Data type of elements in @a buffer
    @param      buffer Buffer with elements for whole dataset

    In case the dataset contains values for multiple items, it is assumed
    that it is large enough for all elements.
*/
void Dataset::write(
    Datatype const& datatype,
    void const* buffer) const
{
    write(datatype, Hyperslab(shape()), buffer);
}


void Dataset::write(
    Datatype const& datatype,
    Hyperslab const& hyperslab,
    void const* buffer) const
{
    assert(datatype.is_native());

    // Select elements: create hyperslab
    auto const file_dataspace = this->dataspace();
    hsize_t const* block = nullptr;
    auto status = ::H5Sselect_hyperslab(file_dataspace.id(), H5S_SELECT_SET,
        hyperslab.start().data(), hyperslab.stride().data(),
        hyperslab.count().data(), block);

    if(status < 0) {
        throw std::runtime_error("Cannot create hyperslab");
    }

    auto const memory_dataspace = create_dataspace(
        Shape(hyperslab.count().begin(), hyperslab.count().end()));

    status = H5Dwrite(
        _id, datatype.id(),
        memory_dataspace.id(), file_dataspace.id(),
        H5P_DEFAULT, buffer);

    if(status < 0) {
        throw std::runtime_error("Cannot write to dataset");
    }
}


void Dataset::write(
    Datatype const& datatype,
    Dataspace const& memory_dataspace,
    Hyperslab const& hyperslab,
    void const* buffer) const
{
    assert(datatype.is_native());

    // Select elements: create hyperslab
    auto file_dataspace = this->dataspace();
    hsize_t const* block = nullptr;
    auto status = ::H5Sselect_hyperslab(file_dataspace.id(), H5S_SELECT_SET,
        hyperslab.start().data(), hyperslab.stride().data(),
        hyperslab.count().data(), block);

    if(status < 0) {
        throw std::runtime_error("Cannot create hyperslab");
    }

    status = ::H5Dwrite(_id, datatype.id(), memory_dataspace.id(),
        file_dataspace.id(), H5P_DEFAULT, buffer);

    if(status < 0) {
        throw std::runtime_error("Cannot write to dataset");
    }
}


/*!
    @brief      Fill the whole dataset with a single value
    @param      datatype In-memory datatype of fill value
    @param      hyperslab Selection of dataset to fill
    @param      buffer Pointer to fill value
    @exception  std::runtime_error In case the dataset cannot be filled
*/
void Dataset::fill(
    Datatype const& datatype,
    Hyperslab const& hyperslab,
    void const* buffer) const
{
    assert(datatype.is_native());

    // We have to fill the dataset with a single value. But to do this
    // it seems we have to have a memory buffer with the same number of
    // values that we want to write to. There is no fill functionality
    // in the HDF5 API.

    // Create a buffer to hold the fill values.
    using byte = unsigned char;
    static_assert(sizeof(byte) == 1, "");

    std::size_t const nr_elements = hyperslab.nr_elements();
    std::size_t const nr_bytes_per_element = datatype.size();
    std::size_t const nr_bytes = nr_elements * nr_bytes_per_element;
    auto memory_buffer = std::make_unique<byte[]>(nr_bytes);

    // Fill the buffer with the fill value.
    {
        auto dst = memory_buffer.get();

        for(std::size_t i = 0; i < nr_elements; ++i,
                dst += nr_bytes_per_element) {
            std::memcpy(dst, buffer, datatype.size());
        }
    }

    auto const memory_dataspace = create_dataspace(Shape{nr_elements});

    write(datatype, memory_dataspace, hyperslab, memory_buffer.get());
}


Dataset open_dataset(
    Identifier const& location,
    std::string const& name)
{
    Identifier dataset_location(::H5Dopen(location, name.c_str(),
        H5P_DEFAULT), ::H5Dclose);

    if(!dataset_location.is_valid()) {
        throw std::runtime_error("Cannot open dataset " + name);
    }

    return Dataset(std::move(dataset_location));
}


/*!
    @brief      Add an HDF5 dataset and return the corresponding Dataset
                instance
    @param      identifier
    @param      name
    @param      datatype
    @param      dataspace
    @param      creation_property_list
    @return     .
    @exception  .
*/
Dataset create_dataset(
    Identifier const& identifier,
    std::string const& name,
    Datatype const& datatype,
    Dataspace const& dataspace,
    Dataset::CreationPropertyList const& creation_property_list)
{
    assert(datatype.is_standard());

    if(dataset_exists(identifier, name)) {
        throw std::runtime_error("Dataset " + name + " already exists");
    }

    Identifier dataset_location(::H5Dcreate(identifier, name.c_str(),
        datatype.id(), dataspace.id(), H5P_DEFAULT,
        creation_property_list.id(), H5P_DEFAULT), ::H5Dclose);

    if(!dataset_location.is_valid()) {
        throw std::runtime_error("Cannot create dataset " + name);
    }

    return Dataset(std::move(dataset_location));
}

} // namespace hdf5
} // namespace lue