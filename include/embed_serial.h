#pragma once

#include "serial_control.h"

namespace sonia_embed
{
    /**
     * @brief Max message size.
     */
    const uint8_t MAX_MSG_SIZE = 112;
    class EmbedSerial : public SerialControl
    {
    public:


        /**
         * @brief Construct a new Embed Serial object.
         * 
         * @param hoci Host Out Client In Pin.
         * @param hico Host In Client Out Pin.
         * @param baud Baudrate.
         */
        EmbedSerial(PinName hoci, PinName hico, int baud);
        ~EmbedSerial();

        std::pair<size_t, size_t> receive(uint8_t *data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t *data, const size_t size) override;

    private:
        void consume_data();
    
        RawSerial *m_serial_handler;
        CircularBuffer<uint8_t, 255> m_cb;
    };

}