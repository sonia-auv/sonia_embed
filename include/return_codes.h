#pragma once
#include "stdlib.h"

namespace sonia_embed
{
    enum RETURN_CODE
    {
        RETURN_OK = 0,
        RETURN_BAD,
        RETURN_BAD_MSG_COUNT,
        RETURN_MAX_IDS_REACHED,
        RETURN_PORT_UNREADABLE,
        RETURN_PORT_UNWRITABLE,
        RETURN_NOT_FOR_ME,
    };
} // namespace sonia_embed
