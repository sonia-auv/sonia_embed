#pragma once

#include "com_control.h"
#include "return_codes.h"

namespace sonia_embed
{
    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    class SerialControl : public ComControl<MSG_SIZE>
    {
    public:
        SerialControl<MSG_SIZE, MAX_IDS>(uint8_t handle, PinName hoci, PinName hico, int baud, int is_blocking = true, int is_host = false)
            : m_baud(baud), m_is_blocking(is_blocking), ComControl<MSG_SIZE>(handle, hoci, hico, is_host) {};

        RETURN_CODE add_filter(size_t id);

        bool check_filter(size_t id);
        

    protected:
        int m_baud;
        bool m_is_blocking;
        size_t m_id_table[MAX_IDS];
        size_t m_table_index = 0;
    };

    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    RETURN_CODE SerialControl<MSG_SIZE, MAX_IDS>::add_filter(size_t id)
    {
        if (m_table_index == MAX_IDS)
        {
            return RETURN_MAX_IDS_REACHED;
        }
        m_id_table[m_table_index] = id;
        m_table_index++;
        return RETURN_OK;
    }

    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    bool SerialControl<MSG_SIZE, MAX_IDS>::check_filter(size_t id)
    {
        for (size_t i = 0; i < m_table_index; i++)
        {
            if (id == m_id_table[i])
            {
                return true;
            }
        }
        return false;
    }

} // namespace sonia_embed
