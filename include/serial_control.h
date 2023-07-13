#pragma once

#include "com_control.h"
#include "return_codes.h"

#define MAX_IDS 10

namespace sonia_embed
{

    class SerialControl : public ComControl
    {
    public:
        // static const size_t MAX_IDS;

        SerialControl(PinName hoci, PinName hico, int baud, bool is_host = false)
            : ComControl(hoci, hico, is_host), m_baud(baud){};

        RETURN_CODE add_filter(size_t id);

        bool check_filter(size_t id);

    protected:
        int m_baud;
        bool m_is_blocking;

        size_t m_id_table[MAX_IDS];
        size_t m_table_index = 0;
    };

} // namespace sonia_embed
