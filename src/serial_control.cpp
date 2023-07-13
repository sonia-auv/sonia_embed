#include "serial_control.h"

namespace sonia_embed
{
    RETURN_CODE SerialControl::add_filter(size_t id)
    {
        if (m_table_index == MAX_IDS)
        {
            return RETURN_MAX_IDS_REACHED;
        }
        m_id_table[m_table_index] = id;
        m_table_index++;
        return RETURN_OK;
    }

    bool SerialControl::check_filter(size_t id)
    {
        if (m_table_index == 0)
        {
            return true;
        }
        
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
