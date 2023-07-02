#pragma once

#include "mbed.h"
#include "serial_control.h"
#include "return_codes.h"

namespace sonia_embed
{
    template<size_t MAX_IDS>
    class RS485Control : public SerialControl<MAX_IDS>
    {
        public:
        RS485Control<MAX_IDS>(uint8_t handle, PinName hoci, PinName hico, int baud, bool is_blocking = true, bool is_host = false) : SerialControl<MAX_IDS>(handle, hoci, hico, baud, is_blocking, is_host){};
        ~RS485Control();

        std::pair<size_t, size_t> receive(uint8_t* data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t* data, const size_t size) override;

        private:
        UnbufferedSerial* m_serial_handler;

    };
}