
#include "RS485_control.h"
#include "rs485_toolkit.h"
namespace sonia_embed
{
    RS485Control::RS485Control(PinName hoci, PinName hico, int baud,
                               PinName receiver_enable, PinName transmiter_enable,
                               PinName termination_enable, bool is_host)
        : SerialControl(hoci, hico, baud, is_host),
          m_receiver_enable(DigitalOut(receiver_enable)),
          m_transmiter_enable(DigitalOut(transmiter_enable)),
          m_termination_enable(DigitalOut(termination_enable))
    {
        if (m_is_host)
        {
            m_serial_handler = new RawSerial(m_hoci, m_hico, m_baud);
        }
        else
        {
            m_serial_handler = new RawSerial(m_hico, m_hoci, m_baud);
        }

        m_serial_handler->attach(callback(this, &RS485Control::consume_data), RawSerial::RxIrq);

        m_receiver_enable.write(0);
        m_termination_enable.write(0);
        m_transmiter_enable.write(0);
    };

    RS485Control::~RS485Control()
    {
        delete m_serial_handler;
    }

    std::pair<size_t, size_t> RS485Control::receive(uint8_t *data)
    {
        if (m_cb.empty())
        {
            return std::pair<size_t, size_t>(RETURN_NO_MSG, 0);
        }

        uint8_t header[3];
        m_cb.pop(header[0]);
        if (header[0] != sonia_embed_toolkit::RS485Toolkit::START_BYTE)
        {
            return std::pair<size_t, size_t>(RETURN_NO_START_BYTE, 0);
        }

        while (!m_cb.pop(header[1]));
        if (!check_filter(header[1]))
        {
            return std::pair<size_t, size_t>(RETURN_NOT_FOR_ME, 0);
        }
        while (!m_cb.pop(header[2]));
        header[2] = (header[2] > sonia_embed_toolkit::RS485Toolkit::MAX_MSG_SIZE) ? sonia_embed_toolkit::RS485Toolkit::MAX_MSG_SIZE : header[2]; 

        uint8_t serial_msg[header[2]+ sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        memcpy(serial_msg, header, 3);
        
        for (size_t i = 0; i < header[2]; i++)
        {
            while (!m_cb.pop(serial_msg[i + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE]));
        }

        return sonia_embed_toolkit::RS485Toolkit::convert_serial_to_message(serial_msg, data);
    }

    RETURN_CODE RS485Control::transmit(const size_t id, const uint8_t *data, const size_t size)
    {
        if (!m_serial_handler->writeable())
        {
            return RETURN_PORT_UNWRITABLE;
        }
        m_transmiter_enable.write(1);
        ThisThread::sleep_for(1);
        
        // TODO Fix the dynamic sizing.
        uint8_t serial_msg[size + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        sonia_embed_toolkit::RS485Toolkit::convert_message_to_serial(id, size, data, serial_msg);

        for (size_t i = 0; i < size + sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE; i++)
        {
            m_serial_handler->putc(serial_msg[i]);
        }
        ThisThread::sleep_for(1);
        m_transmiter_enable.write(0);
        return RETURN_OK;
    }

    void RS485Control::consume_data()
    {
        m_cb.push(m_serial_handler->getc());
    }
}