#include "can_control.h"


namespace sonia_embed
{
    template<uint8_t MSG_SIZE>
    CanControl<MSG_SIZE>::~CanControl()
    {
        delete m_can_handler;
    }

    template<uint8_t MSG_SIZE>
    RETURN_CODE CanControl<MSG_SIZE>::receive(uint8_t* data)
    {
        // assumtion: tram => [id, size, data]
        int msg_counter = 0;
        
        CANMessage msg;
        if (m_can_handler->read(msg) == 1)
        {
            msg_counter++;
        }

        uint8_t size = msg.data[1];

        memcpy(msg.data, data, (size > 8) ? 8 : size);

        for (size_t i = 0; i < (size-8) / 8; i++)
        {
            CANMessage msg;
            if (m_can_handler->read(msg) == 1)
            {
                msg_counter++;
            }
            memcpy(msg.data, &data[8*i+8], 8);
        }

        if (size % 8 != 0)
        {
            CANMessage msg;
            if (m_can_handler->read(msg) == 1)
            {
                msg_counter++;
            }
            memcpy(msg.data, &data[size - (size % 8)], size % 8);
        }

        if (msg_counter == (size % 8 == 0) ? (size / 8) : (size / 8) + 1)
        {
            return RETURN_OK;
        }
        return RETURN_BAD_MSG_COUNT;
    }

    template<uint8_t MSG_SIZE>
    RETURN_CODE CanControl<MSG_SIZE>::transmit(uint8_t* data)
    {
        // assumtion: tram => [id, size, data]
        
        int msg_counter = 0;
        unsigned int id = data[0];
        int size = data[1];
        
        for (size_t i = 0; i < size/8; i++)
        {
            uint8_t* tmp_data[8];
            memcpy(&data[i*8], tmp_data, 8);
            CANMessage msg(id, *tmp_data);
            if (m_can_handler->write(msg) == 1)
            {
                msg_counter++;
            }
        }

        if (size % 8 != 0)
        {
            uint8_t* tmp_data[size % 8];
            memcpy(&data[size-(size % 8)], tmp_data, size % 8);
            CANMessage msg(id, *tmp_data);
            if (m_can_handler->write(msg) == 1)
            {
                msg_counter++;
            }
        }

        if (msg_counter == (size % 8 == 0) ? (size / 8) : (size / 8) + 1)
        {
            return RETURN_OK;
        }

        return RETURN_BAD_MSG_COUNT;
    }

    template<uint8_t MSG_SIZE>
    RETURN_CODE CanControl<MSG_SIZE>::set_filter(uint8_t filter_id)
    {
        // TODO: Make add filter function.
        return RETURN_BAD;
    }

    template<uint8_t MSG_SIZE>
    RETURN_CODE CanControl<MSG_SIZE>::setup_com()
    {
        if (this.m_is_host)
        {
            m_can_handler = new CAN(this.m_hoci, this.m_hico);
        }
        else
        {
            m_can_handler = new CAN(this.m_hico, this.m_hoci);
        }
        return RETURN_OK;
    }

} // namespace sonia_embed
