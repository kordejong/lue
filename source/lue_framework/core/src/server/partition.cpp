#include "lue/framework/core/component/server/partition.hpp"
#include <type_traits>


namespace lue {
namespace server {

template<
    typename Index,
    typename Value,
    std::size_t rank>
Partition<Index, Value, rank>::Partition(
    Definition const& definition):

    _data{definition}

{
}


template<
    typename Index,
    typename Value,
    std::size_t rank>
Partition<Index, Value, rank>::Partition(
    Definition const& definition,
    Value value):

    _data{definition, value}

{
    static_assert(std::is_trivial_v<Value>);
}


template<
    typename Index,
    typename Value,
    std::size_t rank>
typename Partition<Index, Value, rank>::Data const&
    Partition<Index, Value, rank>::data() const
{
    return _data;
}


template class Partition<std::uint64_t, bool, 2>;
template class Partition<std::uint64_t, int32_t, 2>;
template class Partition<std::uint64_t, double, 2>;

}  // namespace server
}  // namespace lue


template<
    typename Index,
    typename Value,
    std::size_t rank>
using PartitionServer =
    hpx::components::component<lue::server::Partition<Index, Value, rank>>;


using PartitionServer1 = PartitionServer<std::uint64_t, bool, 2>;
using PartitionServer2 = PartitionServer<std::uint64_t, std::int32_t, 2>;
using PartitionServer3 = PartitionServer<std::uint64_t, double, 2>;

HPX_REGISTER_COMPONENT(PartitionServer1)
HPX_REGISTER_COMPONENT(PartitionServer2)
HPX_REGISTER_COMPONENT(PartitionServer3)