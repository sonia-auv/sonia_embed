#pragma once

#include <string>

#include "mbed.h"
#include "return_codes.h"

namespace sonia_embed
{
    class ComControl
    {
    public:
        /**
         * @brief Construct a new Com Control object.
         *
         * @param hoci      Host Out Client In pin.
         * @param hico      Host In Client Out pin.
         * @param is_host   Is this interface the Host.
         */
        ComControl(PinName hoci, PinName hico, bool is_host = false) : m_hoci(hoci), m_hico(hico), m_is_host(is_host){};

        /**
         * @brief Receive data from the communication protocol.
         *
         * @param data Array of received data.
         * 
         * @return std::pair<size_t, size_t> Pair where first is the id of the message and second is the size of the message.
         */
        virtual std::pair<size_t, size_t> receive(uint8_t *data) = 0;

        /**
         * @brief Write to destination using the protocol.
         * 
         * @param id    Id of the destination.
         * @param data  The array of data to send.
         * @param size  Size of the array of data to send.
         * @return RETURN_CODE Returns `RETURN_PORT_UNWRITABLE if unwriteable else returns RETURN_OK.
         */
        virtual RETURN_CODE transmit(const size_t id, const uint8_t *data, const size_t size) = 0;

    protected:
        PinName m_hoci;
        PinName m_hico;
        bool m_is_host;
    };
}