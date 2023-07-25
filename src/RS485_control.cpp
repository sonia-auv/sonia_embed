
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

        uint8_t enc_header[sonia_embed_toolkit::RS485Toolkit::PACK_HEADER_SIZE];
        for (size_t i = 0; i < sonia_embed_toolkit::RS485Toolkit::PACK_HEADER_SIZE; i++)
        {
            while (!m_cb.pop(enc_header[i]));
        }

        uint8_t header[sonia_embed_toolkit::RS485Toolkit::HEADER_SIZE];
        sonia_embed_toolkit::RS485Toolkit::convert_serial_to_header(enc_header, header);

        if (header[0] != sonia_embed_toolkit::RS485Toolkit::START_BYTE)
        {
            return std::pair<size_t, size_t>(RETURN_NO_START_BYTE, 0);
        }

        if (!check_filter(header[1]))
        {
            return std::pair<size_t, size_t>(RETURN_NOT_FOR_ME, 0);
        }

        header[2] = (header[2] > sonia_embed_toolkit::RS485Toolkit::MAX_MSG_SIZE) ? sonia_embed_toolkit::RS485Toolkit::MAX_MSG_SIZE : header[2];
        size_t pack_msg_size = (size_t)ceil((double)(header[2]*2*7)/8);

        uint8_t enc_serial_msg[pack_msg_size + sonia_embed_toolkit::RS485Toolkit::PACK_HEADER_SIZE];
        memcpy(enc_serial_msg, enc_header, sonia_embed_toolkit::RS485Toolkit::PACK_HEADER_SIZE);
        
        for (size_t i = 0; i < pack_msg_size; i++)
        {
            while (!m_cb.pop(enc_serial_msg[i + sonia_embed_toolkit::RS485Toolkit::PACK_HEADER_SIZE]));
        }

        return sonia_embed_toolkit::RS485Toolkit::convert_serial_to_message(enc_serial_msg, data);
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
        uint8_t serial_msg[sonia_embed_toolkit::RS485Toolkit::PACK_MSG_SIZE + sonia_embed_toolkit::RS485Toolkit::PACK_HEADER_SIZE];
        size_t pack_size = sonia_embed_toolkit::RS485Toolkit::convert_message_to_serial(id, size, data, serial_msg);

        for (size_t i = 0; i < pack_size; i++)
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