#pragma once

#include "mbed.h"
#include "serial_control.h"
#include "return_codes.h"

namespace sonia_embed
{
    struct RS485_data
    {
        size_t id;
        size_t size;
        uint8_t msg[112];
    };
    
    class RS485Control : public SerialControl
    {
    public:
        RS485Control(PinName hoci, PinName hico, int baud,
                     PinName receiver_enable, PinName transmiter_enable,
                     PinName termination_enable, bool is_host = false);
        ~RS485Control();

        std::pair<size_t, size_t> receive(uint8_t *data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t *data, const size_t size) override;

    private:
        void consume_data();
        RawSerial *m_serial_handler;
        DigitalOut m_receiver_enable;
        DigitalOut m_transmiter_enable;
        DigitalOut m_termination_enable;
        CircularBuffer<uint8_t, 255> m_cb;
    };
}