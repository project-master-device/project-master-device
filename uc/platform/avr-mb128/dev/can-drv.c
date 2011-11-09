/*
 * Copyright (c) 2010 MikloBit
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3  Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * @(#)$
 */


#include "can-drv.h"
#include "dev/rs232.h"

#ifdef CAN_BAUDRATE_VARIABLE
#if MCU_MHZ == 16
const uint8_t can_baudrate_table[9][3] = {
		{ 0x00, 0x00, 0x00 }, /* 10 KBit not available */
		{ 0x3E, 0x0E, 0x7E }, /* 20 KBit */
		{ 0x1E, 0x0E, 0x4A }, /* 50 KBit */
		{ 0x12, 0x0C, 0x37 }, /* 100 KBit */
		{ 0x0E, 0x0C, 0x36 }, /* 125 KBit */
		{ 0x06, 0x0C, 0x36 }, /* 250 KBit */
		{ 0x02, 0x0C, 0x36 }, /* 500 KBit */
		{ 0x00, 0x0E, 0x4A }, /* 800 KBit */
		{ 0x00, 0x0C, 0x36 }  /* 1 MBit */
};
#else
#   error can_baudrate_table not defined for given MCU_MHZ value
#endif
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
void can_clear_all_mob(void)
{
uint8_t  mob_number;
/*
    uint8_t  data_index;
*/

    for (mob_number = 0; mob_number < NB_MOB; mob_number++)
    {
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
uint8_t can_get_mob_free(void)
{
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
void can_get_data(uint8_t* p_can_message_data)
{
    uint8_t data_index;

    for (data_index = 0; data_index < (CAN_GET_DLC()); data_index++)
    {
        *(p_can_message_data + data_index) = CANMSG;
    }
}

//------------------------------------------------------------------------------
//  @fn can_conf_bt
//!
//! This function setup CAN baudrate registers.
//!
//! @warning none.
//!
//! @param values for CAN baudrate registers CANBT1, CANBT2, CANBT3.
//!
//! @return none.
//------------------------------------------------------------------------------
void can_conf_bt(uint8_t bt1 , uint8_t bt2, uint8_t bt3) {
	CANBT1=bt1;
	CANBT2=bt2;
	CANBT3=bt3;
	// rs232_printf (CAN_PORT,"IN:%02x%02x%02x OUT:%02x%02x%02x\r",bt1,bt2,bt3,CANBT1,CANBT2,CANBT3 );
}


#ifdef CAN_BAUDRATE_VARIABLE
//------------------------------------------------------------------------------
//  @fn can_variable_baudrate
//!
//! This function programs the CANBTx registers with the predefined values
//! CONF_CANBT1, CONF_CANBT2, CONF_CANBT3.
//!
//! @warning
//!
//! @param ( mode = 0-8 -> see can-drv.h for assigned speed values )
//!
//! @return Baudrate Status
//!         fixed = 1: baudrate performed
//------------------------------------------------------------------------------
uint8_t can_variable_baudrate(uint8_t mode)
{
	if( ( can_baudrate_table[mode][0] == 0 ) &&
		( can_baudrate_table[mode][1] == 0 ) &&
		( can_baudrate_table[mode][2] == 0 ) ) {
		return 0;   /* no parameters for selected speed  */
	}
    CAN_RESET();
    can_conf_bt(can_baudrate_table[mode][0],can_baudrate_table[mode][1],can_baudrate_table[mode][2]);
    return 1;
}

#else
//------------------------------------------------------------------------------
//  @fn can_fixed_baudrate
//!
//! This function programs the CANBTx registers with the predefined values
//! CONF_CANBT1, CONF_CANBT2, CONF_CANBT3.
//!
//! @warning
//!
//! @param (unused!)
//!
//! @return Baudrate Status
//!         fixed = 1: baudrate performed
//------------------------------------------------------------------------------
uint8_t can_fixed_baudrate(uint8_t mode)
{
    CAN_RESET();
    can_conf_bt(CONF_CANBT1,CONF_CANBT2,CONF_CANBT3);
    return 1;
}
#endif





















