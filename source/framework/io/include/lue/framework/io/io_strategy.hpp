#pragma once


namespace lue {

    /*!
        @brief      The IOStrategy class encodes the various aspects that together determine the strategy for
                    doing I/O
    */
    class IOStrategy
    {

        public:

            /*!
                @brief      The grouping of partitions that a single task should handle

                Grouping determines:

                - The number of input dependencies to wait for before the task will do useful work
                - The number of times a file is opened and closed
                - The number of bytes written/read by a single task
            */
            enum class PartitionGrouping {

                /*!
                    @brief      Handle each partition individually

                    Reading, once partition is ready (allocated):

                    1. Open file
                    2. Read single partition
                    3. Close file

                    Writing, once partition is ready (computed):

                    1. Open file
                    2. Write single partition
                    3. Close file
                */
                None,

                /*!
                    @brief      Handle all partitions in a locality

                    Reading, once partitions are ready (allocated):

                    1. Open file
                    2. Read multiple partitions
                    3. Close file

                    Writing, once partitions are ready (computed):

                    1. Open file
                    2. Write multiple partitions
                    3. Close file
                */
                Locality,

            };


            IOStrategy(PartitionGrouping grouping);

            IOStrategy(IOStrategy const& other) = default;

            IOStrategy(IOStrategy&& other) = default;

            ~IOStrategy() = default;

            auto operator=(IOStrategy const& other) -> IOStrategy& = default;

            auto operator=(IOStrategy&& other) -> IOStrategy& = default;

            [[nodiscard]] auto grouping() const -> PartitionGrouping;

        private:

            PartitionGrouping _grouping;
    };

}  // namespace lue