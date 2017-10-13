#pragma once
#include "lue/hdf5/group.hpp"


namespace lue {

class TimeDomain:
    public hdf5::Group
{

public:

    class Configuration
    {

    public:

        enum class ItemType
        {

            box

        };

                   Configuration       (ItemType const item_type);

                   Configuration       (hdf5::Attributes const& attributes);

                   Configuration       (Configuration const& other)=default;

                   ~Configuration      ()=default;

        Configuration& operator=       (Configuration const& other)=default;

        ItemType   item_type           () const;

        void       save                (hdf5::Attributes& attributes) const;

    private:

        ItemType   _item_type;

        void       load                (hdf5::Attributes const& attributes);

    };


                   TimeDomain          (hdf5::Group const& group);

                   TimeDomain          (hdf5::Group&& group);

                   TimeDomain          (TimeDomain const& other)=delete;

                   TimeDomain          (TimeDomain&& other)=default;

                   ~TimeDomain         ()=default;

    TimeDomain&    operator=           (TimeDomain const& other)=delete;

    TimeDomain&    operator=           (TimeDomain&& other)=default;

    Configuration const&
                   configuration       () const;

private:

    Configuration  _configuration;

};


TimeDomain         create_time_domain  (hdf5::Group const& group,
                                        TimeDomain::Configuration const&
                                            configuration);

bool               time_domain_exists (hdf5::Group const& group);

}  // namespace lue
