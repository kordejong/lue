#include "literal.hpp"


void lue_destruct_literal(LUE_Literal* literal)
{
    if (literal != nullptr)
    {
        if (literal->instance != nullptr)
        {
            delete lue_as_cxx_literal_ptr(literal);
        }

        delete literal;
    }
}
