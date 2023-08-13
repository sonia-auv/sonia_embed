#include "can_control.h"
#include "canbus_toolkit.h"

namespace sonia_embed
{
    CanControl::CanControl(PinName hoci, PinName hico, bool is_host) : ComControl(hoci, hico, is_host)
    {
        if (m_is_host)
        {
            m_can_handler = new CAN(m_hoci, m_hico);
        }
        else
        {
            m_can_handler = new CAN(m_hico, m_hoci);
        }
    }

    CanControl::~CanControl()
    {
        delete m_can_handler;
    }

    std::pair<size_t, size_t> CanControl::receive(uint8_t* data)
    {
        int msg_size = 0;
        
        CANMessage msg;
        if (0 == m_can_handler->read(msg))
        {
            return std::pair<size_t, size_t>(RETURN_NO_MSG, 0);
        }

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

    RETURN_CODE CanControl::transmit(const size_t id, const uint8_t* data, const size_t size)
    {

        uint8_t can_msg_array[sonia_embed_toolkit::CanBusToolkit::MAX_CAN_SIZE];
        size_t can_size = sonia_embed_toolkit::CanBusToolkit::convert_to_can(size, data, can_msg_array);
        size_t msg_byte_counter = 0;
        for (size_t i = 0; i < can_size/8; i++)
        {
            uint8_t tmp_data[8];
            memcpy(tmp_data, &can_msg_array[i*8], 8);
            CANMessage msg(id, tmp_data);
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
            CANMessage msg(id, tmp_data, can_size % 8);
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

    RETURN_CODE CanControl::set_filter(unsigned int filter_id, unsigned int mask, int handle)
    {
        if (handle < 14 || handle > 24)
        {
            return RETURN_BAD;
        }
        m_can_handler->filter(filter_id, mask, CANStandard, handle);
        return RETURN_OK;
    }

    size_t CanControl::array_to_can(const uint8_t* serial, size_t size, CANMessage* can_msgs)
    {
        return size_t();
    }

} // namespace sonia_embed
