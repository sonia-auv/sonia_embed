
#include "RS485_control.h"

namespace sonia_embed
{
    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    RS485Control<MSG_SIZE, MAX_IDS>::~RS485Control()
    {
        delete m_serial_handler;
    }

    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    RETURN_CODE RS485Control<MSG_SIZE, MAX_IDS>::receive(uint8_t* data)
    {
        if (!m_serial_handler->readable())
        {
            return RETURN_PORT_UNREADABLE;
        }

        uint8_t read_data[1];
        m_serial_handler->read(read_data, 1);
        for (size_t i = 0; i <= this.m_table_index; i++)
        {
            if (this.m_id_table[i] == read_data[0])
            {
                break;
            }
            else if (i == this.m_table_index)
            {
                return RETURN_NOT_FOR_ME;
            }
        }


        data[0] = read_data[0];
        m_serial_handler->read(read_data, 1);
        uint8_t size = read_data[0];
        data[1] = size;
        uint8_t tmp_data[size-2];
        m_serial_handler->read(tmp_data, size-2);
        memcpy(tmp_data, &data[2], size-2);
        return RETURN_OK;
    }

    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    RETURN_CODE RS485Control<MSG_SIZE, MAX_IDS>::transmit(uint8_t* data)
    {   
        if (!m_serial_handler->writable())
        {
            return RETURN_PORT_UNWRITABLE;
        }

        size_t size = data[1];
        if (m_serial_handler->write(data, size) == size)
        {
            return RETURN_OK;
        }
        return RETURN_BAD_MSG_COUNT;
    }

    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    RETURN_CODE RS485Control<MSG_SIZE, MAX_IDS>::setup_com()
    {
        if (this.m_is_host)
        {
            m_serial_handler = new UnbufferedSerial(this.m_hoci, this.m_hico, this.m_baud);
        }
        else
        {
            m_serial_handler = new UnbufferedSerial(this.m_hico, this.m_hoci, this.m_baud);
        }

        m_serial_handler->set_blocking(this.m_is_blocking);

        return RETURN_OK;
    }


}