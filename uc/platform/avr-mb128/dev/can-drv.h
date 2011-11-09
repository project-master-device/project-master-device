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

#ifndef _CAN_DRV_H_
#define _CAN_DRV_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "contiki-conf.h"


#define Max(a, b)          ( (a)>(b) ? (a) : (b) )
#define Min(a, b)          ( (a)<(b) ? (a) : (b) )

#ifndef MCU_MHZ
#error  You must define MCU_MHZ in "contiki-conf.h" file
#endif

#ifndef CAN_BAUDRATE_VARIABLE
#ifndef CAN_BAUDRATE
#error  You must define (in "contiki-conf.h" file) either CAN_BAUDRATE
#endif
#endif

#ifdef CAN_BAUDRATE_VARIABLE

typedef  enum
     { CAN_S0 = 0,  /* 10 KBit */
	   CAN_S1 = 1,  /* 20 KBit */
	   CAN_S2 = 2,  /* 50 Kbit */
	   CAN_S3 = 3,  /* 100 Kbit */
	   CAN_S4 = 4,  /* 125 Kbit */
	   CAN_S5 = 5,  /* 250 Kbit */
	   CAN_S6 = 6,  /* 500 Kbit */
	   CAN_S7 = 7,  /* 800 Kbit */
	   CAN_S8 = 8   /* 1 Mbit */
} can_baudrate_t;
	// dynamic baudrate values

extern const uint8_t can_baudrate_table[9][3] ;


#define CAN_BIT_TIMING(mode)  (can_variable_baudrate(mode))
#else
#if MCU_MHZ == 16             //!< Fclkio = 16 MHz, Tclkio = 62.5 ns

	// static baudrate values
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x12       // Tscl  = 10x Tclkio = 625 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0E       // Tscl  = 8x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x08       // Tscl  = 5x Tclkio = 312.5 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl  = 4x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl = 4x Tclkio = 250 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 125 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == CAN_AUTOBAUD  // Treated later in the file
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   else
#       error This CAN_BAUDRATE value is not in "can-drv.h" file
#   endif

#elif MCU_MHZ == 12           //!< Fclkio = 12 MHz, Tclkio = 83.333 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0A       // Tscl  = 6x Tclkio = 500 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x0A       // Tscl  = 6x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x04       // Tscl  = 3x Tclkio = 250 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x04       // Tscl  = 3x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 12x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 166.666 ns
#       define CONF_CANBT2  0x08       // Tsync = 1x Tscl, Tprs = 5x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x25       // Tpsh1 = 3x Tscl, Tpsh2 = 3x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 12x Tscl, sampling at 75%
#       define CONF_CANBT1  0x00       // Tscl  = 1x Tclkio = 83.333 ns
#       define CONF_CANBT2  0x08       // Tsync = 1x Tscl, Tprs = 5x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x25       // Tpsh1 = 3x Tscl, Tpsh2 = 3x Tscl, 3 sample points
#   elif CAN_BAUDRATE == CAN_AUTOBAUD  // Treated later in the file
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   else
#       error This CAN_BAUDRATE value is not found in "can-drv.h" file
#   endif

#elif MCU_MHZ == 8              //!< Fclkio = 8 MHz, Tclkio = 125 ns
#   if   CAN_BAUDRATE == 100       //!< -- 100Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x08       // Tscl  = 5x Tclkio = 625 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 125       //!< -- 125Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x06       // Tscl  = 4x Tclkio = 500 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 200       //!< -- 200Kb/s, 20x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x0E       // Tsync = 1x Tscl, Tprs = 8x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x4B       // Tpsh1 = 6x Tscl, Tpsh2 = 5x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 250       //!< -- 250Kb/s, 16x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x0C       // Tsync = 1x Tscl, Tprs = 7x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x37       // Tpsh1 = 4x Tscl, Tpsh2 = 4x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 500       //!< -- 500Kb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x02       // Tscl  = 2x Tclkio = 250 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == 1000      //!< -- 1 Mb/s, 8x Tscl, sampling at 75%
#       define CONF_CANBT1  0x00       // Tscl  = 1x Tclkio = 125 ns
#       define CONF_CANBT2  0x04       // Tsync = 1x Tscl, Tprs = 3x Tscl, Tsjw = 1x Tscl
#       define CONF_CANBT3  0x13       // Tpsh1 = 2x Tscl, Tpsh2 = 2x Tscl, 3 sample points
#   elif CAN_BAUDRATE == CAN_AUTOBAUD  // Treated later in the file
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   else
#       error This CAN_BAUDRATE value is not found in "can-drv.h" file
#   endif
#else
#       define CONF_CANBT1  0x00       // Unused
#       define CONF_CANBT2  0x00       // Unused
#       define CONF_CANBT3  0x00       // Unused
#   warning This MCU_MHZ value is not in found in "can-drv.h" file. CONF_CANBT1/CONF_CANBT2/CONF_CANBT3 set to 0
#endif
#define CAN_BIT_TIMING(mode)  (can_fixed_baudrate(mode))
#endif /* CAN_BAUDRATE_VARIABLE */
    // ----------


    // ----------
#define CAN_PORT_IN     PIND
#define CAN_PORT_DIR    DDRD
#define CAN_PORT_OUT    PORTD
#define CAN_INPUT_PIN   6
#define CAN_OUTPUT_PIN  5
    // ----------
#define ERR_GEN_MSK ((1<<SERG)|(1<<CERG)|(1<<FERG)|(1<<AERG))            //! MaSK for GENeral ERRors INTerrupts
#define INT_GEN_MSK ((1<<BOFFIT)|(1<<BXOK)|(ERR_GEN_MSK))                //! MaSK for GENeral INTerrupts

#define BRP_MSK     ((1<<BRP5)|(1<<BRP4)|(1<<BRP3)|(1<<BRP2)|(1<<BRP1)|(1<<BRP0))  //! Mask for BRP in CANBT1
#define SJW_MSK     ((1<<SJW1)|(1<<SJW0))                                //! MaSK for SJW  in CANBT2
#define PRS_MSK     ((1<<PRS2)|(1<<PRS1)|(1<<PRS0))                      //! MaSK for PRS  in CANBT2
#define PHS2_MSK    ((1<<PHS22)|(1<<PHS21)|(1<<PHS20))                   //! MaSK for PHS2 in CANBT2
#define PHS1_MSK    ((1<<PHS12)|(1<<PHS11)|(1<<PHS10))                   //! MaSK for PHS1 in CANBT2
#define BRP         (BRP0)                                               //! BRP  in CANBT1
#define SJW         (SJW0)                                               //! SJW  in CANBT2
#define PRS         (PRS0)                                               //! PRS  in CANBT2
#define PHS2        (PHS20)                                              //! PHS2 in CANBT2
#define PHS1        (PHS10)                                              //! PHS1 in CANBT2

#define HPMOB_MSK   ((1<<HPMOB3)|(1<<HPMOB2)|(1<<HPMOB1)|(1<<HPMOB0))    //! MaSK for MOb in HPMOB
#define MOBNB_MSK   ((1<<MOBNB3)|(1<<MOBNB2)|(1<<MOBNB1)|(1<<MOBNB0))    //! MaSK for MOb in CANPAGE

#define ERR_MOB_MSK ((1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR))  //! MaSK for MOb ERRors
#define OK_MOB_MSK  ((1<<DLCW)|(1<<TXOK)|(1<<RXOK))  					// mask for Mob OK status
#define INT_MOB_MSK ((1<<TXOK)|(1<<RXOK)|(1<<BERR)|(1<<SERR)|(1<<CERR)|(1<<FERR)|(1<<AERR)) //! MaSK for MOb INTerrupts

#define CONMOB_MSK  ((1<<CONMOB1)|(1<<CONMOB0))                          //! MaSK for CONfiguration MOb
#define DLC_MSK     ((1<<DLC3)|(1<<DLC2)|(1<<DLC1)|(1<<DLC0))            //! MaSK for Data Length Coding
#define CONMOB      (CONMOB0)                                            //! CONfiguration MOb
#define DLC         (DLC0)                                               //! Data Length Coding
    // ----------
#define BRP_MIN     1       //! Prescaler of MCU_MHZ (TQ generation)
#define BRP_MAX     64
#define NTQ_MIN     8       //! Number of TQ in one CAN bit
#define NTQ_MAX     25
                            //! True segment values in TQ
#define PRS_MIN     1       //! Propagation segment
#define PRS_MAX     8
#define PHS1_MIN    2       //! Phase segment 1
#define PHS1_MAX    8
#define PHS2_MIN    2       //! Phase segment 2
#define PHS2_MAX    8
#define SJW_MIN     1       //! Synchro jump width
#define SJW_MAX     4
    // ----------
#define NB_MOB       15
#define NB_DATA_MAX  8
#define LAST_MOB_NB  (NB_MOB-1)
#define NO_MOB       0xFF
    // ----------
typedef enum {
        MOB_0,  MOB_1, MOB_2,  MOB_3,  MOB_4,  MOB_5,  MOB_6, MOB_7,
        MOB_8,  MOB_9, MOB_10, MOB_11, MOB_12, MOB_13, MOB_14        } can_mob_t;
    // ----------
#define STATUS_CLEARED            0x00
    // ----------
#define MOB_NOT_COMPLETED         0x00                                              // 0x00
#define MOB_TX_COMPLETED        (1<<TXOK)                                           // 0x40
#define MOB_RX_COMPLETED        (1<<RXOK)                                           // 0x20
#define MOB_RX_COMPLETED_DLCW  ((1<<RXOK)|(1<<DLCW))                                // 0xA0
#define MOB_ACK_ERROR           (1<<AERR)                                           // 0x01
#define MOB_FORM_ERROR          (1<<FERR)                                           // 0x02
#define MOB_CRC_ERROR           (1<<CERR)                                           // 0x04
#define MOB_STUFF_ERROR         (1<<SERR)                                           // 0x08
#define MOB_BIT_ERROR           (1<<BERR)                                           // 0x10
#define MOB_PENDING            ((1<<RXOK)|(1<<TXOK))                                // 0x60
#define MOB_NOT_REACHED        ((1<<AERR)|(1<<FERR)|(1<<CERR)|(1<<SERR)|(1<<BERR))  // 0x1F
#define MOB_DISABLE               0xFF                                              // 0xFF
    // ----------
#define MOB_Tx_ENA  1
#define MOB_Rx_ENA  2
#define MOB_Rx_BENA 3
    // ----------

//_____ M A C R O S ____________________________________________________________

    // ----------
#define CAN_RESET()       ( CANGCON  =  (1<<SWRES) )
#define CAN_ENABLE()      ( CANGCON |=  (1<<ENASTB))
#define CAN_DISABLE()     ( CANGCON &= ~(1<<ENASTB))
    // ----------
#define CAN_SET_TCON(prescaler)       { CANTCON = (prescaler);}
#define CAN_GET_TIMESTAMP_MOB()       ( CANSTML | (CANSTMH << 8) )
    // ----------
#define CAN_FULL_ABORT()  { CANGCON |=  (1<<ABRQ); CANGCON &= ~(1<<ABRQ); }
    // ----------

    // ----------
#define CAN_INT_MOB_ENABLE()  { CANIE2 = 0xff ;  CANIE1 = 0x7f ; }
#define CAN_INT_ENABLE()	{ CANGIE |= (1<<ENIT) | (1<<ENRX) | (1<<ENTX) | (1 <<ENOVRT) ; }
#define CAN_GET_INT_MOB()   (( CANSIT1 << 8) | CANSIT2 )
#define CAN_GET_INT()		( CANGIT )
#define CAN_CLEAR_INT()		{ CANGIT = 0xff ; }
#define CAN_GET_STATUS_MOB() ( CANSTMOB )
    //-----------
#define CAN_SET_MOB(mob)       { CANPAGE = ((mob) << 4);}
#define CAN_SET_MASK_MOB()     {  CANIDM4=0xFF; CANIDM3=0xFF; CANIDM2=0xFF; CANIDM1=0xFF; }
#define CAN_CLEAR_MASK_MOB()   {  CANIDM4=0x00; CANIDM3=0x00; CANIDM2=0x00; CANIDM1=0x00; }
#define CAN_CLEAR_STATUS_MOB() { CANSTMOB=0x00; }
#define CAN_CLEAR_MOB()        { uint8_t  volatile *__i_; for (__i_=&CANSTMOB; __i_<&CANSTML; __i_++) { *__i_=0x00 ;}}
    // ----------
#define CAN_MOB_ABORT()   ( DISABLE_MOB )
    // ----------
#define CAN_SET_DLC(dlc)  ( CANCDMOB |= (dlc)        )
#define CAN_SET_IDE()     ( CANCDMOB |= (1<<IDE)     )
#define CAN_SET_RTR()     ( CANIDT4  |= (1<<RTRTAG)  )
#define CAN_SET_RPLV()    ( CANCDMOB |= (1<<RPLV)    )
    // ----------
#define CAN_CLEAR_DLC()   ( CANCDMOB &= ~DLC_MSK     )
#define CAN_CLEAR_IDE()   ( CANCDMOB &= ~(1<<IDE)    )
#define CAN_CLEAR_RTR()   ( CANIDT4  &= ~(1<<RTRTAG) )
#define CAN_CLEAR_RPLV()  ( CANCDMOB &= ~(1<<RPLV)   )
    // ----------
#define DISABLE_MOB       ( CANCDMOB &= (~CONMOB_MSK) )
#define CAN_CONFIG_TX()        { DISABLE_MOB; CANCDMOB |= (MOB_Tx_ENA  << CONMOB); }
#define CAN_CONFIG_RX()        { DISABLE_MOB; CANCDMOB |= (MOB_Rx_ENA  << CONMOB); }
#define CAN_CONFIG_RX_BUFFER() {              CANCDMOB |= (MOB_Rx_BENA << CONMOB); }
    // ----------
#define CAN_GET_DLC()      ((CANCDMOB &  DLC_MSK)     >> DLC   )
#define CAN_GET_IDE()      ((CANCDMOB &  (1<<IDE))    >> IDE   )
#define CAN_GET_RTR()      ((CANIDT4  &  (1<<RTRTAG)) >> RTRTAG)
    // ----------
#define CAN_SET_RTRMSK()   ( CANIDM4 |= (1<<RTRMSK) )
#define CAN_SET_IDEMSK()   ( CANIDM4 |= (1<<IDEMSK) )
    // ----------
#define CAN_CLEAR_RTRMSK() ( CANIDM4 &= ~(1<<RTRMSK) )
#define CAN_CLEAR_IDEMSK() ( CANIDM4 &= ~(1<<IDEMSK) )
    // ----------
                //!< STD ID TAG Reading
#define CAN_GET_STD_ID(identifier)  { *((uint8_t *)(&(identifier))+1) =  CANIDT1>>5              ; \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT2>>5)+(CANIDT1<<3); }
    // ----------
                //!< EXT ID TAG Reading
#define CAN_GET_EXT_ID(identifier)  { *((uint8_t *)(&(identifier))+3) =  CANIDT1>>3              ; \
                                      *((uint8_t *)(&(identifier))+2) = (CANIDT2>>3)+(CANIDT1<<5); \
                                      *((uint8_t *)(&(identifier))+1) = (CANIDT3>>3)+(CANIDT2<<5); \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT4>>3)+(CANIDT3<<5); }
    // ----------
                //!< STD ID Construction
#define CAN_SET_STD_ID_10_4(identifier)  (((*((uint8_t *)(&(identifier))+1))<<5)+((* (uint8_t *)(&(identifier)))>>3))
#define CAN_SET_STD_ID_3_0( identifier)  (( * (uint8_t *)(&(identifier))   )<<5)
    // ----------
                //!< STD ID TAG writing
#define CAN_SET_STD_ID(identifier)  { CANIDT1   = CAN_SET_STD_ID_10_4(identifier); \
                                      CANIDT2   = CAN_SET_STD_ID_3_0( identifier); \
                                      CANCDMOB &= (~(1<<IDE))                    ; }
    // ----------
                //!< STD ID MASK writing
#define CAN_SET_STD_MSK(mask)       { CANIDM1   = CAN_SET_STD_ID_10_4(mask); \
                                      CANIDM2   = CAN_SET_STD_ID_3_0( mask); }
    // ----------
                //!< EXT ID Construction
#define CAN_SET_EXT_ID_28_21(identifier)  (((*((uint8_t *)(&(identifier))+3))<<3)+((*((uint8_t *)(&(identifier))+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier)  (((*((uint8_t *)(&(identifier))+2))<<3)+((*((uint8_t *)(&(identifier))+1))>>5))
#define CAN_SET_EXT_ID_12_5( identifier)  (((*((uint8_t *)(&(identifier))+1))<<3)+((* (uint8_t *)(&(identifier))   )>>5))
#define CAN_SET_EXT_ID_4_0(  identifier)   ((* (uint8_t *)(&(identifier))   )<<3)
    // ----------
                //!< EXT ID TAG writing
#define CAN_SET_EXT_ID(identifier)  { CANIDT1   = CAN_SET_EXT_ID_28_21(identifier); \
                                      CANIDT2   = CAN_SET_EXT_ID_20_13(identifier); \
                                      CANIDT3   = CAN_SET_EXT_ID_12_5( identifier); \
                                      CANIDT4   = CAN_SET_EXT_ID_4_0(  identifier); \
                                      CANCDMOB |= (1<<IDE);                         }
    // ----------
                //!< EXT ID MASK writing
#define CAN_SET_EXT_MSK(mask)       { CANIDM1   = CAN_SET_EXT_ID_28_21(mask); \
                                      CANIDM2   = CAN_SET_EXT_ID_20_13(mask); \
                                      CANIDM3   = CAN_SET_EXT_ID_12_5( mask); \
                                      CANIDM4   = CAN_SET_EXT_ID_4_0(  mask); }
    // ----------

//------------------------------------------------------------------------------
//  @fn can_clear_all_mob
//!
//! This function clears the Mailbox content.
//! It uses CAN_CLEAR_MOB() macro and clears data FIFO o MOb[0] upto
//! MOb[LAST_MOB_NB].
//!
//! @warning none.
//!
//! @param none.
//!
//! @return none.
//!
extern  void can_clear_all_mob(void);

//------------------------------------------------------------------------------
//  @fn can_get_mob_free
//!
//! This function returns the number of the first MOb available or 0xFF if
//! no MOb available.
//!
//! @warning none.
//!
//! @param none.
//!
//! @return Handle of MOb.
//!          - MOb[0] upto MOb[LAST_MOB_NB]
//!          - 0xFF if no MOb
//!
extern  uint8_t can_get_mob_free(void);

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
//!
extern  void can_get_data(uint8_t* p_can_message_data);


#ifdef CAN_BAUDRATE_VARIABLE
//------------------------------------------------------------------------------
//  @fn can_variable_baudrate
//!
//! This function programs the CANBTx registers with  values
//! selected from table
//!
//! @warning
//!
//! @param ( mode = 0-8 -> see can-drv.h for assigned speed values )
//!
//! @return Baudrate Status
//!         fixed = 1: baudrate performed
//------------------------------------------------------------------------------
extern uint8_t can_variable_baudrate(uint8_t mode);
#else
//------------------------------------------------------------------------------
//  @fn can_fixed_baudrate
//!
//! This function programs the CANBTx registers with the fixed/predefined values
//! CONF_CANBT1, CONF_CANBT2, CONF_CANBT3.
//!
//! @warning
//!
//! @param (unused!)
//!
//! @return Baudrate Status
//!         fixed = 1: baudrate performed
//!
extern uint8_t can_fixed_baudrate(uint8_t eval);
#endif

//______________________________________________________________________________

#endif // _CAN_DRV_H_


