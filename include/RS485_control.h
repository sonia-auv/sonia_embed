#pragma once

#include "mbed.h"
#include "serial_control.h"
#include "return_codes.h"

namespace sonia_embed
{
    template<uint8_t MSG_SIZE, size_t MAX_IDS>
    class RS485Control : public SerialControl<MSG_SIZE, MAX_IDS>
    {
        public:
        RS485Control<MSG_SIZE, MAX_IDS>(uint8_t handle, PinName hoci, PinName hico, int baud, bool is_blocking = true, bool is_host = false) : SerialControl<MSG_SIZE, MAX_IDS>(handle, hoci, hico, baud, is_blocking, is_host){};
        ~RS485Control();

        RETURN_CODE receive(uint8_t* data) override;
        RETURN_CODE transmit(uint8_t* data) override;
        
        protected:
        RETURN_CODE setup_com() override;

        private:
        UnbufferedSerial* m_serial_handler;

    };
}