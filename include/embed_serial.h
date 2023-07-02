#pragma once

#include "mbed.h"
#include "serial_control.h"

namespace sonia_embed
{
    template<size_t MAX_IDS>
    class EmbedSerial : public SerialControl<MAX_IDS>
    {
        public:
        EmbedSerial<MAX_IDS>(PinName hoci, PinName hico, int baud, bool is_blocking = true) : SerialControl<MAX_IDS>(hoci, hico, baud, is_blocking, true){};
        ~EmbedSerial();

        RETURN_CODE receive(uint8_t* data) override;
        RETURN_CODE transmit(uint8_t* data) override;

        protected:
        RETURN_CODE setup_com() override;
        
        private:
        BufferedSerial* m_serial_handler;

    };
}