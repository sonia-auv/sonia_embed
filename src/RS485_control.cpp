
#include "RS485_control.h"
#include "rs485_toolkit.h"
namespace sonia_embed
{
    template<size_t MAX_IDS>
    RS485Control<MAX_IDS>::~RS485Control()
    {
        delete m_serial_handler;
    }

    template<size_t MAX_IDS>
    std::pair<size_t, size_t> RS485Control<MAX_IDS>::receive(uint8_t* data)
    {
        if (!m_serial_handler->readable())
        {
            return std::pair<size_t, size_t>(RETURN_PORT_UNREADABLE, 0);
        }

        uint8_t header[3];
        m_serial_handler->read(header, 3);

        uint8_t serial_msg[header[2] + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        memcpy(serial_msg, header, 3);

        m_serial_handler->read(&serial_msg[3], header[2]);
        
        if (header[0] != sonia_embed_toolkit::RS485Toolkit::START_BYTE)
        {
            return std::pair<size_t, size_t>(RETURN_NO_START_BYTE, 0);
        }

        if (!this.check_filter(header[1]))
        {
            return std::pair<size_t, size_t>(RETURN_NOT_FOR_ME, 0);
        }

        size_t serial_size = sonia_embed_toolkit::RS485Toolkit::convert_serial_to_message(data, serial_msg);

        return std::pair<size_t, size_t>(header[1], serial_size);
    }

    template<size_t MAX_IDS>
    RETURN_CODE RS485Control<MAX_IDS>::transmit(const size_t id, const uint8_t* data, const size_t size)
    {   
        if (!m_serial_handler->writable())
        {
            return RETURN_PORT_UNWRITABLE;
        }

        uint8_t serial_msg[size + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        size_t serial_size = sonia_embed_toolkit::RS485Toolkit::convert_message_to_serial(id, size, data, serial_msg);

        if (m_serial_handler->write(serial_msg, serial_size) == serial_size)
        {
            return RETURN_OK;
        }
        return RETURN_BAD_MSG_COUNT;
    }

    template<size_t MAX_IDS>
    RETURN_CODE RS485Control<MAX_IDS>::setup_com()
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