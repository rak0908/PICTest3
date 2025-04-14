/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.0.0
*/

/*
? [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RC1 aliases
#define SCL_TRIS                 TRISCbits.TRISC1
#define SCL_LAT                  LATCbits.LATC1
#define SCL_PORT                 PORTCbits.RC1
#define SCL_WPU                  WPUCbits.WPUC1
#define SCL_OD                   ODCONCbits.ODCC1
#define SCL_ANS                  ANSELCbits.ANSC1
#define SCL_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define SCL_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define SCL_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define SCL_GetValue()           PORTCbits.RC1
#define SCL_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define SCL_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define SCL_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define SCL_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define SCL_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define SCL_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define SCL_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define SCL_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)
// get/set IO_RC2 aliases
#define SDA_TRIS                 TRISCbits.TRISC2
#define SDA_LAT                  LATCbits.LATC2
#define SDA_PORT                 PORTCbits.RC2
#define SDA_WPU                  WPUCbits.WPUC2
#define SDA_OD                   ODCONCbits.ODCC2
#define SDA_ANS                  ANSELCbits.ANSC2
#define SDA_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define SDA_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define SDA_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define SDA_GetValue()           PORTCbits.RC2
#define SDA_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define SDA_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define SDA_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define SDA_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define SDA_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define SDA_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define SDA_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define SDA_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/