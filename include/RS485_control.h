#pragma once

#include "mbed.h"
#include "serial_control.h"
#include "return_codes.h"
#include "rs485_toolkit.h"

namespace sonia_embed
{    
    class RS485Control : public SerialControl
    {
    public:

        /**
         * @brief Construct a new RS485Control object.
         * 
         * @param hoci                  Host Out Client In Pin / Port.
         * @param hico                  Host In Client Out Pin / Port.
         * @param baud                  Baud Rate.
         * @param receiver_enable       Pin that controls the receiver.
         * @param transmiter_enable     Pin that controls the transmitter.
         * @param termination_enable    Pin that controls the termination
         * @param is_host               Is this interface the host.
         */
        RS485Control(PinName hoci, PinName hico, int baud,
                     PinName receiver_enable, PinName transmiter_enable,
                     PinName termination_enable, bool is_host = false);
        ~RS485Control();

        std::pair<size_t, size_t> receive(uint8_t *data) override;
        RETURN_CODE transmit(const size_t id, const uint8_t *data, const size_t size) override;

    private:
        void consume_data();
        RawSerial *m_serial_handler;
        DigitalOut m_receiver_enable;
        DigitalOut m_transmiter_enable;
        DigitalOut m_termination_enable;
        CircularBuffer<uint8_t, 255> m_cb;
    };
}