#pragma once

#include "serial_control.h"

namespace sonia_embed
{
    class EmbedSerial : public SerialControl
    {
    public:
        static const uint8_t MAX_MSG_SIZE;
        EmbedSerial(PinName hoci, PinName hico, int baud);
        ~EmbedSerial();

        std::pair<size_t, size_t> receive(uint8_t *data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t *data, const size_t size) override;

    private:
        void consume_data();
    
        RawSerial *m_serial_handler;
        CircularBuffer<uint8_t, 255> m_cb;
    };

}