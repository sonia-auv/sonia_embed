#pragma once

#include "mbed.h"
#include "com_control.h"
#include "CAN.h"

namespace sonia_embed
{
    template<uint8_t const MSG_SIZE>
    class CanControl : public ComControl<MSG_SIZE>
    {
    public:
        CanControl<MSG_SIZE>(PinName hoci, PinName hico, bool is_host) : ComControl<MSG_SIZE>(hoci, hico, is_host){};
        ~CanControl();

        RETURN_CODE receive(size_t id, uint8_t* data, size_t size) override;
        RETURN_CODE transmit(size_t id, uint8_t* data, size_t size) override;

        RETURN_CODE set_filter(uint8_t filter_id);
        
        protected:
        RETURN_CODE setup_com() override;

        private:
        CAN* m_can_handler;
    };
}