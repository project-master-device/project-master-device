#include "can_lowlevel.h"

#if MCU_MHZ == 16
const uint8_t can_baudrate_table[9][3] = {
		{ 0x3E, 0x0E, 0x7E }, /* 20 KBit */
		{ 0x1E, 0x0E, 0x4A }, /* 50 KBit */
		{ 0x12, 0x0C, 0x37 }, /* 100 KBit */
		{ 0x0E, 0x0C, 0x37 }, /* 125 KBit */
		{ 0x06, 0x0C, 0x37 }, /* 250 KBit */
		{ 0x02, 0x0C, 0x37 }, /* 500 KBit */
		{ 0x00, 0x0E, 0x4A }, /* 800 KBit */
		{ 0x00, 0x0C, 0x36 }  /* 1 MBit */
};
#else
#   error can_baudrate_table not defined for given MCU_MHZ value
#endif


//------------------------------------------------------------------------------
//  @fn can_clear_all_mob
//!
//! This function clears the Mailbox content.
//! It reset CANSTMOB, CANCDMOB, CANIDTx & CANIDMx and clears data FIFO of
//! MOb[0] upto MOb[LAST_MOB_NB].
//!
//! @warning: This version doesn't clears the data FIFO
//!
//! @param none
//!
//! @return none
//------------------------------------------------------------------------------
void can_clear_all_mob(void) {
    uint8_t  mob_number;

    for (mob_number = 0; mob_number < NB_MOB; mob_number++) {
        CANPAGE = (mob_number << 4);    //! Page index
        CAN_CLEAR_MOB();                //! All MOb Registers=0
    }
}

//------------------------------------------------------------------------------
//  @fn can_get_mob_free
//!
//! This function returns the number of the first MOb available or 0xFF if
//! no MOb is available.
//!
//! @warning none.
//!
//! @param  none.
//!
//! @return Handle of MOb.
//!          - MOb[0] upto MOb[LAST_MOB_NB]
//!          - 0xFF if no MOb
//------------------------------------------------------------------------------
uint8_t can_get_mob_free(void) {
    uint8_t mob_number, page_saved;

    page_saved = CANPAGE;
    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
        CAN_SET_MOB(mob_number);
        if ((CANCDMOB & 0xC0) == 0x00) //! Disable configuration
        {
            CANPAGE = page_saved;
            return (mob_number);
        }
    }
    CANPAGE = page_saved;
    return (NO_MOB);
}


//------------------------------------------------------------------------------
//  @fn can_get_data
//!
//! This function copy the data from the selected MOb to the address
//! passed as parameter.
//!
//! @warning none.
//!
//! @param CAN message data address.
//!
//! @return none.
//------------------------------------------------------------------------------
inline void can_get_data(uint8_t * p_can_message_data) {
    uint8_t data_index;

    for (data_index = 0; data_index < (CAN_GET_DLC()); data_index++) {
        *(p_can_message_data + data_index) = CANMSG;
    }
}

int can_set_baudrate(uint8_t mode) {
	if( ( can_baudrate_table[mode][0] == 0 ) &&
		( can_baudrate_table[mode][1] == 0 ) &&
		( can_baudrate_table[mode][2] == 0 ) ) {
		return -1; // no parameters for selected speed
	}
    CAN_RESET();

    CANBT1 = can_baudrate_table[mode][0];
    CANBT2 = can_baudrate_table[mode][1];
    CANBT3 = can_baudrate_table[mode][2];

    return 0;
}
