// #pragma once

// #include "mbed.h"
// #include "serial_control.h"

// namespace sonia_embed
// {
//     class EmbedSerial : public SerialControl
//     {
//         public:
//         EmbedSerial(PinName hoci, PinName hico, int baud, bool is_blocking = true) : SerialControl(hoci, hico, baud, is_blocking, true){};
//         ~EmbedSerial();

//         std::pair<size_t, size_t> receive(uint8_t* data) override;
//         RETURN_CODE transmit(const size_t id, const uint8_t* data, const size_t size) override;

//         protected:
//         RETURN_CODE setup_com() override;
        
//         private:
//         BufferedSerial* m_serial_handler;

//     };
// }