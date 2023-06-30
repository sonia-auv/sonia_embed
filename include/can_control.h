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

        std::pair<size_t, size_t> receive(uint8_t* data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t* data, const size_t size) override;

        RETURN_CODE set_filter(uint8_t filter_id);
        
        protected:
        RETURN_CODE setup_com() override;

        private:
        CAN* m_can_handler;

        size_t can_to_array(const CANMessage* can_msgs, uint8_t* serial);
        size_t array_to_can(const uint8_t* serial, size_t size, CANMessage* can_msgs);
    };
}