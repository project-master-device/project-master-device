#ifndef CAN_LOWLEVEL_H_
#define CAN_LOWLEVEL_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "contiki-conf.h"


#define Max(a, b)          ( (a)>(b) ? (a) : (b) )
#define Min(a, b)          ( (a)<(b) ? (a) : (b) )

#ifndef MCU_MHZ
#error  You must define MCU_MHZ in "contiki-conf.h" file
#endif

extern const uint8_t can_baudrate_table[9][3];

#define CAN_PORT_IN     PIND
#define CAN_PORT_DIR    DDRD
#define CAN_PORT_OUT    PORTD
#define CAN_INPUT_PIN   6
#define CAN_OUTPUT_PIN  5

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

#define NB_MOB       15
#define NB_DATA_MAX  8
#define LAST_MOB_NB  (NB_MOB-1)
#define NO_MOB       0xFF

typedef enum {
        MOB_0,  MOB_1, MOB_2,  MOB_3,  MOB_4,  MOB_5,  MOB_6, MOB_7,
        MOB_8,  MOB_9, MOB_10, MOB_11, MOB_12, MOB_13, MOB_14
} can_mob_t;

#define STATUS_CLEARED            0x00

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

#define MOB_Tx_ENA  1
#define MOB_Rx_ENA  2
#define MOB_Rx_BENA 3


#define CAN_RESET()       ( CANGCON  =  (1<<SWRES) )
#define CAN_ENABLE()      ( CANGCON |=  (1<<ENASTB))
#define CAN_DISABLE()     ( CANGCON &= ~(1<<ENASTB))

#define CAN_SET_TCON(prescaler)       { CANTCON = (prescaler);}
#define CAN_GET_TIMESTAMP_MOB()       ( CANSTML | (CANSTMH << 8) )

#define CAN_FULL_ABORT()  { CANGCON |=  (1<<ABRQ); CANGCON &= ~(1<<ABRQ); }


#define CAN_INT_MOB_ENABLE()  { CANIE2 = 0xff ;  CANIE1 = 0x7f ; }
//#define CAN_INT_ENABLE()	{ CANGIE |= (1 << ENIT) | (1 << ENRX) | (1 << ENTX) | (1 << ENOVRT); }
#define CAN_INT_ENABLE()	{ CANGIE |= (1 << ENIT) | (1 << ENRX) | (1 << ENTX); }
#define CAN_GET_INT_MOB()   (( CANSIT1 << 8) | CANSIT2 )
#define CAN_GET_INT()		( CANGIT )
#define CAN_CLEAR_INT()		{ CANGIT = 0xff ; }
#define CAN_GET_STATUS_MOB() ( CANSTMOB )

#define CAN_SET_MOB(mob)       { CANPAGE = ((mob) << 4);}
#define CAN_SET_MASK_MOB()     {  CANIDM4=0xFF; CANIDM3=0xFF; CANIDM2=0xFF; CANIDM1=0xFF; }
#define CAN_CLEAR_MASK_MOB()   {  CANIDM4=0x00; CANIDM3=0x00; CANIDM2=0x00; CANIDM1=0x00; }
#define CAN_CLEAR_STATUS_MOB() { CANSTMOB=0x00; }
#define CAN_CLEAR_MOB()        { uint8_t  volatile *__i_; for (__i_=&CANSTMOB; __i_<&CANSTML; __i_++) { *__i_=0x00 ;}}

#define CAN_MOB_ABORT()   ( DISABLE_MOB )

#define CAN_SET_DLC(dlc)  ( CANCDMOB |= (dlc)        )
#define CAN_SET_IDE()     ( CANCDMOB |= (1<<IDE)     )
#define CAN_SET_RTR()     ( CANIDT4  |= (1<<RTRTAG)  )
#define CAN_SET_RPLV()    ( CANCDMOB |= (1<<RPLV)    )

#define CAN_CLEAR_DLC()   ( CANCDMOB &= ~DLC_MSK     )
#define CAN_CLEAR_IDE()   ( CANCDMOB &= ~(1<<IDE)    )
#define CAN_CLEAR_RTR()   ( CANIDT4  &= ~(1<<RTRTAG) )
#define CAN_CLEAR_RPLV()  ( CANCDMOB &= ~(1<<RPLV)   )

#define DISABLE_MOB       ( CANCDMOB &= (~CONMOB_MSK) )
#define CAN_CONFIG_TX()        { DISABLE_MOB; CANCDMOB |= (MOB_Tx_ENA  << CONMOB); }
#define CAN_CONFIG_RX()        { DISABLE_MOB; CANCDMOB |= (MOB_Rx_ENA  << CONMOB); }
#define CAN_CONFIG_RX_BUFFER() {              CANCDMOB |= (MOB_Rx_BENA << CONMOB); }

#define CAN_GET_DLC()      ((CANCDMOB &  DLC_MSK)     >> DLC   )
#define CAN_GET_IDE()      ((CANCDMOB &  (1<<IDE))    >> IDE   )
#define CAN_GET_RTR()      ((CANIDT4  &  (1<<RTRTAG)) >> RTRTAG)

#define CAN_SET_RTRMSK()   ( CANIDM4 |= (1<<RTRMSK) )
#define CAN_SET_IDEMSK()   ( CANIDM4 |= (1<<IDEMSK) )

#define CAN_CLEAR_RTRMSK() ( CANIDM4 &= ~(1<<RTRMSK) )
#define CAN_CLEAR_IDEMSK() ( CANIDM4 &= ~(1<<IDEMSK) )

//!< STD ID TAG Reading
#define CAN_GET_STD_ID(identifier)  { *((uint8_t *)(&(identifier))+1) =  CANIDT1>>5              ; \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT2>>5)+(CANIDT1<<3); }

//!< EXT ID TAG Reading
#define CAN_GET_EXT_ID(identifier)  { *((uint8_t *)(&(identifier))+3) =  CANIDT1>>3              ; \
                                      *((uint8_t *)(&(identifier))+2) = (CANIDT2>>3)+(CANIDT1<<5); \
                                      *((uint8_t *)(&(identifier))+1) = (CANIDT3>>3)+(CANIDT2<<5); \
                                      *((uint8_t *)(&(identifier))  ) = (CANIDT4>>3)+(CANIDT3<<5); }

//!< STD ID Construction
#define CAN_SET_STD_ID_10_4(identifier)  (((*((uint8_t *)(&(identifier))+1))<<5)+((* (uint8_t *)(&(identifier)))>>3))
#define CAN_SET_STD_ID_3_0( identifier)  (( * (uint8_t *)(&(identifier))   )<<5)

//!< STD ID TAG writing
#define CAN_SET_STD_ID(identifier)  { CANIDT1   = CAN_SET_STD_ID_10_4(identifier); \
                                      CANIDT2   = CAN_SET_STD_ID_3_0( identifier); \
                                      CANCDMOB &= (~(1<<IDE))                    ; }

//!< STD ID MASK writing
#define CAN_SET_STD_MSK(mask)       { CANIDM1   = CAN_SET_STD_ID_10_4(mask); \
                                      CANIDM2   = CAN_SET_STD_ID_3_0( mask); }

//!< EXT ID Construction
#define CAN_SET_EXT_ID_28_21(identifier)  (((*((uint8_t *)(&(identifier))+3))<<3)+((*((uint8_t *)(&(identifier))+2))>>5))
#define CAN_SET_EXT_ID_20_13(identifier)  (((*((uint8_t *)(&(identifier))+2))<<3)+((*((uint8_t *)(&(identifier))+1))>>5))
#define CAN_SET_EXT_ID_12_5( identifier)  (((*((uint8_t *)(&(identifier))+1))<<3)+((* (uint8_t *)(&(identifier))   )>>5))
#define CAN_SET_EXT_ID_4_0(  identifier)   ((* (uint8_t *)(&(identifier))   )<<3)

//!< EXT ID TAG writing
#define CAN_SET_EXT_ID(identifier)  { CANIDT1   = CAN_SET_EXT_ID_28_21(identifier); \
                                      CANIDT2   = CAN_SET_EXT_ID_20_13(identifier); \
                                      CANIDT3   = CAN_SET_EXT_ID_12_5( identifier); \
                                      CANIDT4   = CAN_SET_EXT_ID_4_0(  identifier); \
                                      CANCDMOB |= (1<<IDE);                         }

//!< EXT ID MASK writing
#define CAN_SET_EXT_MSK(mask)       { CANIDM1   = CAN_SET_EXT_ID_28_21(mask); \
                                      CANIDM2   = CAN_SET_EXT_ID_20_13(mask); \
                                      CANIDM3   = CAN_SET_EXT_ID_12_5( mask); \
                                      CANIDM4   = CAN_SET_EXT_ID_4_0(  mask); }

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
void can_clear_all_mob(void);

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
uint8_t can_get_mob_free(void);

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
void can_get_data(uint8_t * p_can_message_data);

/**
 * Program the CANBTx registers with the predefined values.
 * @param mode = 0-8
 * @return Baudrate status. 0 - baudrate performed. -1 - not correct mode.
 */
int can_set_baudrate(uint8_t mode);

#endif /* CAN_DRV_H_ */
