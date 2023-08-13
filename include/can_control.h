#pragma once

#include "CAN.h"
#include "com_control.h"
#include "mbed.h"

namespace sonia_embed
{
    class CanControl : public ComControl
    {
    public:
        /**
         * @brief Construct a new Can Control object
         *
         * @param hoci      Host Out Client In Pin / Port.
         * @param hico      Host In Client Out Pin / Port.
         * @param is_host   Is this interface the host.
         */
        CanControl(PinName hoci, PinName hico, bool is_host);
        ~CanControl();

        std::pair<size_t, size_t> receive(uint8_t *data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t *data,
                             const size_t size) override;

        /**
         * @brief Set the filter object
         *
         * @param filter_id The id of the filter
         * @param mask      Mask of the filter to determine the range of the filter.
         * @param handle    Handler for the filter.
         * @return RETURN_CODE Returns RETURN_BAD if handle is less than 14 or more
         * than 24 else returns RETURN_OK.
         */
        RETURN_CODE set_filter(unsigned int filter_id, unsigned int mask = 0xFFF,
                               int handle = 14);

    private:
        CAN *m_can_handler;

        size_t can_to_array(const CANMessage *can_msgs, uint8_t *serial);
        size_t array_to_can(const uint8_t *serial, size_t size, CANMessage *can_msgs);
    };
} // namespace sonia_embed