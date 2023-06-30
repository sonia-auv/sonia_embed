#include "can_control.h"
#include "canbus_toolkit.h"

namespace sonia_embed
{
    template<uint8_t MSG_SIZE>
    CanControl<MSG_SIZE>::~CanControl()
    {
        delete m_can_handler;
    }

    template<uint8_t MSG_SIZE>
    std::pair<size_t, size_t> CanControl<MSG_SIZE>::receive(uint8_t* data)
    {
        // assumtion: tram => [id, size, data]
        int msg_size = 0;
        
        CANMessage msg;
        if (0 == m_can_handler->read(msg))
        {
            return std::pair<size_t, size_t>(RETURN_NO_START_BYTE, 0);
        }

        // TODO Make constant and not magic number
        uint8_t can_msgs[sonia_embed_toolkit::CanBusToolkit::MAX_CAN_SIZE];

        uint8_t total = msg.data[0] & 0b1111;

        memcpy(can_msgs, msg.data, msg.len);
        msg_size += msg.len;
        

        for (size_t i = 0; i < total; i++)
        {
            if (0 == m_can_handler->read(msg))
            {
                return std::pair<size_t, size_t>(RETURN_BAD, 0);
            }
            memcpy(can_msgs, msg.data, msg.len);
            msg_size += msg.len;
        }

        size_t serial_size = sonia_embed_toolkit::CanBusToolkit::convert_from_can(msg_size, can_msgs, data);
        return std::pair<size_t, size_t>(msg.id, serial_size);
    }

    template<uint8_t MSG_SIZE>
    RETURN_CODE CanControl<MSG_SIZE>::transmit(const size_t id, const uint8_t* data, const size_t size)
    {
        // assumtion: tram => [id, size, data]

        uint8_t can_msg_array[sonia_embed_toolkit::CanBusToolkit::MAX_CAN_SIZE];
        size_t can_size = sonia_embed_toolkit::CanBusToolkit::convert_to_can(size, data, can_msg_array);
        size_t msg_byte_counter = 0;
        for (size_t i = 0; i < can_size/8; i++)
        {
            uint8_t tmp_data[8];
            memcpy(tmp_data, &can_msg_array[i*8], 8);
            CANMessage msg(id, *tmp_data);
            if (0 == m_can_handler->write(msg))
            {
                return RETURN_BAD;
            }
            msg_byte_counter += 8;
        }

        if (can_size % 8 != 0)
        {
            uint8_t tmp_data[8];
            memcpy(tmp_data, &can_msg_array[msg_byte_counter], can_size % 8);
            CANMessage msg(id, *tmp_data, can_size % 8);
            if (0 == m_can_handler->write(msg))
            {
                return RETURN_BAD;
            }
            msg_byte_counter += can_size % 8;
        }

        if (msg_byte_counter == can_size)
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

    template<uint8_t MSG_SIZE>
    size_t CanControl<MSG_SIZE>::array_to_can(const uint8_t* serial, size_t size, CANMessage* can_msgs)
    {
        return size_t();
    }

} // namespace sonia_embed
