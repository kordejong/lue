#pragma once
// #include "lue/cxx_api/time/omnipresent/space/omnipresent/property.h"
#include "lue/cxx_api/array.h"
#include "lue/cxx_api/value.h"
#include "lue/cxx_api/data_type/time/property.h"
// #include "lue/c_api/define.h"
#include <memory>


namespace lue {
namespace api {
namespace time {
namespace omnipresent {
namespace constant_shape {

// namespace omnipresent {
// namespace omnipresent {


/*!
    @ingroup    lue_cxx_api_group
*/
class Property:
    api::Property
{

public:

                   Property            (lue::Property& group,
                                        hid_t const type_id);
                                        // TypeId const type_id,
                                        // Shape const& shape,
                                        // Chunks const& chunks);

                   Property            (Property const& other)=delete;

                   Property            (Property&& other)=default;

                   ~Property           ()=default;

    Property&      operator=           (Property const& other)=delete;

    Property&      operator=           (Property&& other)=default;

    Array&         reserve_items       (hsize_t const nr_items);

    Array&         values              ();

private:

    std::unique_ptr<Value> _value;

    std::unique_ptr<Array> _items;

};


void               configure_property  (lue::Property const& location,
                                        hid_t const type_id,
                                        Shape const& shape,
                                        Chunks const& chunks);

}  // namespace constant_shape
}  // namespace omnipresent
}  // namespace time
}  // namespace api
}  // namespace lue
