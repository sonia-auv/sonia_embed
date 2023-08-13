#pragma once

#include "com_control.h"
#include "return_codes.h"



namespace sonia_embed
{
    const size_t MAX_IDS = 10;
    class SerialControl : public ComControl
    {
    public:
        /**
         * @brief Construct a new Serial Control object.
         * 
         * @param hoci      Host Out Client In pin / port.
         * @param hico      Host In Client Out pin / port.
         * @param baud      Baud Rate of the connection.
         * @param is_host   Is this interface the host device.
         */
        SerialControl(PinName hoci, PinName hico, int baud, bool is_host = false)
            : ComControl(hoci, hico, is_host), m_baud(baud){};

        /**
         * @brief Add a filter to the interface. No set filter accepts all.
         * 
         * Mimics the canbus filter setup.
         * 
         * @param id Id of the fitler to add.
         * @return RETURN_CODE Returns RETURN_MAX_IDS_REACHED if max ids are reached else returns RETURN_OK.
         */
        RETURN_CODE add_filter(size_t id);

        /**
         * @brief Check whether the id is incuded in this interface.
         * 
         * @param id Id to check.
         * 
         * @return true     If ID has been set with this interface.
         * @return false    If ID has not been set with this interface.
         */
        bool check_filter(size_t id);

    protected:
        int m_baud;
        bool m_is_blocking;

        size_t m_id_table[MAX_IDS];
        size_t m_table_index = 0;
    };

} // namespace sonia_embed
