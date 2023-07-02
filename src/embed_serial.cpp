// #include "embed_serial.h"


// namespace sonia_embed
// {
//     /* #region PUBLIC */
//     EmbedSerial::~EmbedSerial()
//     {
//         delete m_serial_handler;
//     }

//     RETURN_CODE EmbedSerial::receive(uint8_t* data)
//     {
//         if (!m_serial_handler->readable())
//         {
//             return RETURN_PORT_UNREADABLE;
//         }

//         size_t id_size[2] = {0};

//         m_serial_handler->read(id_size, 2);

//         size_t size = id_size[1];

//         uint8_t tmp_data[size-2];
//         memcpy(id_size, &data, 2);
//         m_serial_handler->read(tmp_data, size-2);
//         memcpy(tmp_data, &data[2], size-2);

//         return RETURN_OK;
//     }

//     RETURN_CODE EmbedSerial::transmit(uint8_t* data)
//     {
//         if (m_serial_handler->writable())
//         {
//             return RETURN_PORT_UNWRITABLE;
//         }
//         size_t size = data[1];
//         m_serial_handler->write(data, size);
//         return RETURN_OK;
//     }

//     /* #endregion */

//     /* #region PROTECTED */

//     RETURN_CODE EmbedSerial::setup_com()
//     {
//         m_serial_handler = new BufferedSerial(this.m_hoci, this.m_hico, this.m_baud);
//         m_serial_handler->set_blocking(this.m_is_blocking);
        
//         return RETURN_OK;
//     }

//     /* #endregion */

// }