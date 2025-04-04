#include "lue/cf/domain.hpp"


namespace lue::cf {

    auto Domain::axes() const& -> Axes
    {
        return _axes;
    }

}  // namespace lue::cf
