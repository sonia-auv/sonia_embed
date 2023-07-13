
#include "RS485_control.h"
#include "rs485_toolkit.h"
namespace sonia_embed
{
    RS485Control::RS485Control(PinName hoci, PinName hico, int baud, PinName receiver_enable, PinName transmiter_enable, PinName termination_enable, bool is_host) 
        : SerialControl(hoci, hico, baud, false, is_host)
    {
        if (is_host)
        {
            m_serial_handler = new RawSerial(hoci, hico, baud);
        }
        else
        {
            m_serial_handler = new RawSerial(hico, hoci, baud);
        }

        DigitalOut(receiver_enable).write(0);
        DigitalOut(termination_enable).write(1);
        m_transmit_enable = DigitalOut(transmiter_enable);
        m_transmit_enable.write(0);
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
        header[0] = m_serial_handler->getc();
        if (header[0] != sonia_embed_toolkit::RS485Toolkit::START_BYTE)
        {
            return std::pair<size_t, size_t>(RETURN_NO_START_BYTE, 0);
        }
        header[1] = m_serial_handler->getc();
        if (!check_filter(header[1]))
        {
            return std::pair<size_t, size_t>(RETURN_NOT_FOR_ME, 0);
        }
        header[2] = m_serial_handler->getc();

        uint8_t serial_msg[header[2] + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        memcpy(serial_msg, header, 3);

        for (size_t i = 0; i < header[2]; i++)
        {
            serial_msg[i+sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE] = m_serial_handler->getc();
        }
        
        // m_serial_handler->read(&serial_msg[3], header[2]);

        return sonia_embed_toolkit::RS485Toolkit::convert_serial_to_message(serial_msg, data);
    }

    RETURN_CODE RS485Control::transmit(const size_t id, const uint8_t* data, const size_t size)
    {   
        if (!m_serial_handler->writeable())
        {
            return RETURN_PORT_UNWRITABLE;
        }
        //TODO Fix the dynamic sizing.
        uint8_t serial_msg[size + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        sonia_embed_toolkit::RS485Toolkit::convert_message_to_serial(id, size, data, serial_msg);

        for (size_t i = 0; i < size + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE; i++)
        {
            if (m_serial_handler->putc(serial_msg[i]) != serial_msg[i])
            {
                return RETURN_BAD;
            }
        }

        return RETURN_OK;
    }

}