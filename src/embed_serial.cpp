#include "embed_serial.h"


namespace sonia_embed
{
    /* #region PUBLIC */

    EmbedSerial::EmbedSerial(PinName hoci, PinName hico, int baud) : SerialControl(hoci, hico, baud, true)
    {
        m_serial_handler = new RawSerial(m_hoci, m_hico, m_baud);
        m_serial_handler->attach(callback(this, &EmbedSerial::consume_data), Serial::RxIrq);
    };

    EmbedSerial::~EmbedSerial()
    {
        delete m_serial_handler;
    }

    pair<size_t, size_t> EmbedSerial::receive(uint8_t* data)
    {
        if (m_cb.empty())
        {
            return pair<size_t, size_t>(RETURN_NO_MSG, 0);
        }
        uint8_t header[2];
        m_cb.pop(header[0]);
        while(!m_cb.pop(header[1]));
        size_t id = header[0];
        size_t size = header[1];

        size = (size > MAX_MSG_SIZE) ? MAX_MSG_SIZE : size;
        for (size_t i = 0; i < size; i++)
        {
            while(!m_cb.pop(data[i]));
        }

        return pair<size_t, size_t>(id, size);
    }

    RETURN_CODE EmbedSerial::transmit(const size_t id, const uint8_t *data, const size_t size)
    {
        if (!m_serial_handler->writeable())
        {
            return RETURN_PORT_UNWRITABLE;
        }
        m_serial_handler->putc(id);
        m_serial_handler->putc(size);

        for (size_t i = 0; i < size; i++)
        {
            m_serial_handler->putc(data[i]);
        }
        
        return RETURN_OK;
    }

    void EmbedSerial::consume_data()
    {
        m_cb.push(m_serial_handler->getc());
    }
    /* #endregion */
}