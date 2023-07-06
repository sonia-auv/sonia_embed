#pragma once

#include "mbed.h"
#include "serial_control.h"
#include "return_codes.h"

namespace sonia_embed
{
    class RS485Control : public SerialControl
    {
        public:
        RS485Control(PinName hoci, PinName hico, int baud, bool is_blocking = true, bool is_host = false);
        ~RS485Control();

        std::pair<size_t, size_t> receive(uint8_t* data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t* data, const size_t size) override;

        private:
        RawSerial* m_serial_handler;

    };
}