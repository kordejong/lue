#include "literal.hpp"


void lue_destruct_literal(Literal* literal)
{
    if (literal != nullptr)
    {
        if (literal->instance != nullptr)
        {
            delete as_cxx_literal_ptr(literal);
        }

        delete literal;
    }
}
