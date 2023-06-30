#pragma once

#include <string>

#include "mbed.h"
#include "return_codes.h"

namespace sonia_embed
{
    template<uint8_t const MSG_SIZE>
    class ComControl
    {
    public:
        ComControl<MSG_SIZE>(PinName hoci, PinName hico, bool is_host = false) : m_hoci(hoci), m_hico(hico), m_is_host(is_host)
        {
            setup_com();
        };

        /**
         * @brief Read data from system.
         * 
         * TRAM => [id, size, data]
         * 
         * @param data Data buffer to read into. Must be size MSG_SIZE;
         * @return RETURN_CODE RETURN_OK for no error.
         */
        virtual std::pair<size_t, size_t> receive(uint8_t* data) = 0;
        
        /**
         * @brief Send data to a recipient
         * 
         * @param data Tram of data to send. TRAM => [id, size, data]
         * @return ssize_t 0 if successfully sent else 1.
         */
        virtual RETURN_CODE transmit(const size_t id, const uint8_t* data, const size_t size) = 0;

        // virtual ssize_t validate(uint8_t* data, int size) = 0;

    protected:
        virtual RETURN_CODE setup_com() = 0;

        PinName m_hoci;
        PinName m_hico;
        bool m_is_host;
    
    private:

    };
}