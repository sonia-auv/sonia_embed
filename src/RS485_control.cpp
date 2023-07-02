
#include "RS485_control.h"
#include "rs485_toolkit.h"
namespace sonia_embed
{
    RS485Control::RS485Control(PinName hoci, PinName hico, int baud, bool is_blocking, bool is_host) 
        : SerialControl(hoci, hico, baud, is_blocking, is_host)
    {
        if (is_host)
        {
            m_serial_handler = new UnbufferedSerial(hoci, hico, baud);
        }
        else
        {
            m_serial_handler = new UnbufferedSerial(hico, hoci, baud);
        }

        m_serial_handler->set_blocking(is_blocking);
    };

    RS485Control::~RS485Control()
    {
        delete m_serial_handler;
    }

    std::pair<size_t, size_t> RS485Control::receive(uint8_t* data)
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

        if (!check_filter(header[1]))
        {
            return std::pair<size_t, size_t>(RETURN_NOT_FOR_ME, 0);
        }

        return sonia_embed_toolkit::RS485Toolkit::convert_serial_to_message(data, serial_msg);
    }

    RETURN_CODE RS485Control::transmit(const size_t id, const uint8_t* data, const size_t size)
    {   
        if (!m_serial_handler->writable())
        {
            return RETURN_PORT_UNWRITABLE;
        }
        //TODO Fix the dynamic sizing.
        uint8_t serial_msg[size + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        size_t serial_size = sonia_embed_toolkit::RS485Toolkit::convert_message_to_serial(id, size, data, serial_msg);

        if (m_serial_handler->write(serial_msg, serial_size) == serial_size)
        {
            return RETURN_OK;
        }
        return RETURN_BAD_MSG_COUNT;
    }

}