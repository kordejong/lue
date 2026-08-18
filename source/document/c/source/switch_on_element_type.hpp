#include "lue/framework/api/c/type_info.h"
#include <cinttypes>
#include <optional>
#include <stdexcept>


namespace lue::api {

    template<typename Result, typename CaseHandler, typename... Arguments>
    auto switch_on_element_type(LUE_ElementType const element_type, Arguments&&... arguments) -> Result
    {
        std::optional<Result> result{};

        // NOLINTBEGIN(bugprone-switch-missing-default-case)
        switch (element_type)
        {
            case LUE_ElementType_UInt8:
            {
                result =
                    CaseHandler::template operator()<std::uint8_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_UInt16:
            {
                result =
                    CaseHandler::template operator()<std::uint16_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_UInt32:
            {
                result =
                    CaseHandler::template operator()<std::uint32_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_UInt64:
            {
                result =
                    CaseHandler::template operator()<std::uint64_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_Int8:
            {
                result = CaseHandler::template operator()<std::int8_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_Int16:
            {
                result =
                    CaseHandler::template operator()<std::int16_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_Int32:
            {
                result =
                    CaseHandler::template operator()<std::int32_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_Int64:
            {
                result =
                    CaseHandler::template operator()<std::int64_t>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_Float32:
            {
                result = CaseHandler::template operator()<float>(std::forward<Arguments>(arguments)...);
                break;
            }
            case LUE_ElementType_Float64:
            {
                result = CaseHandler::template operator()<double>(std::forward<Arguments>(arguments)...);
                break;
            }
        }
        // NOLINTEND(bugprone-switch-missing-default-case)

        if (!result)
        {
            throw std::runtime_error("Unsupported element type");
        }

        return std::move(*result);
    }

}  // namespace lue::api
