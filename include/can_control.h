#pragma once

#include "mbed.h"
#include "com_control.h"
#include "CAN.h"

namespace sonia_embed
{
    class CanControl : public ComControl
    {
    public:
        CanControl(PinName hoci, PinName hico, bool is_host);
        ~CanControl();

        std::pair<size_t, size_t> receive(uint8_t* data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t* data, const size_t size) override;

        RETURN_CODE set_filter(unsigned int filter_id, unsigned int mask = 0xFFF, int handle = 14);

        private:
        CAN* m_can_handler;

        size_t can_to_array(const CANMessage* can_msgs, uint8_t* serial);
        size_t array_to_can(const uint8_t* serial, size_t size, CANMessage* can_msgs);
    };
}