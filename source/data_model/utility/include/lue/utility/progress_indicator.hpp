#pragma once
#include <ostream>


namespace lue::utility {

    class ProgressIndicator
    {

        public:

            ProgressIndicator(std::ostream& stream, std::string label, size_t nr_tasks);

            ProgressIndicator(ProgressIndicator const&) = delete;

            ProgressIndicator(ProgressIndicator&&) = delete;

            ~ProgressIndicator();

            auto operator=(ProgressIndicator const&) -> ProgressIndicator& = delete;

            auto operator=(ProgressIndicator&&) -> ProgressIndicator& = delete;

            void update_progress(size_t task);

        private:

            void init_stream();

            void update_stream(std::string const& string);

            void finish_stream();

            std::ostream& _stream;

            std::string const _label;

            size_t const _nr_tasks;
    };

}  // namespace lue::utility
