/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/

#include <adc.h>
#include <stdint.h>
#include <stdbool.h>
#include <xc.h>


#define PIN_ANALOG    1
#define PIN_DIGITAL   0

#define PIN_INPUT     1
#define PIN_OUTPUT    0

/*********************************************************************
* Function: ADC_ReadPercentage(ADC_CHANNEL channel);
*
* Overview: Reads the requested ADC channel and returns the percentage 
*            of that conversions result (0-100%).
*
* PreCondition: channel is configured via the ADCConfigure() function
*
* Input: ADC_CHANNEL channel - enumeration of the ADC channels
*        available in this demo.  They should be meaningful names and 
*        not the names of the ADC pins on the device (as the demo code 
*        may be ported to other boards).
*         i.e. ADC_ReadPercentage(ADC_CHANNEL_POTENTIOMETER);
*
* Output: uint8_t indicating the percentage of the result 0-100% or
*         0xFF for an error
*
********************************************************************/
uint8_t ADC_ReadPercentage
    (ADC_CHANNEL channel) {
    uint8_t percent;

    switch(channel)
    {
        case ADC_CHANNEL_0:
            break;
        default:
            return 0xFF;
    }
    
    //A very crude percentage calculation
    percent = (ADC_Read10bit(channel) / 10);

    if(percent > 100)
    {
        percent = 100;
    }
    return percent;
}

/*********************************************************************
* Function: ADC_Read10bit(ADC_CHANNEL channel);
*
* Overview: Reads the requested ADC channel and returns the 10-bit
*           representation of this data.
*
* PreCondition: channel is configured via the ADCConfigure() function
*
* Input: ADC_CHANNEL channel - enumeration of the ADC channels
*        available in this demo.  They should be meaningful names and
*        not the names of the ADC pins on the device (as the demo code
*        may be ported to other boards).
*         i.e. - ADCReadPercentage(ADC_CHANNEL_POTENTIOMETER);
*
* Output: uint16_t the right adjusted 10-bit representation of the ADC
*         channel conversion or 0xFFFF for an error.
*
********************************************************************/
uint16_t ADC_Read10bit(ADC_CHANNEL channel)
{
    uint16_t result;

    switch(channel)
    {
        case ADC_CHANNEL_0:
            break;
        default:
            return 0xFFFF;
    }

    ADCON0bits.CHS = channel;

    ADCON0bits.GO = 1;              // Start AD conversion

    sendSignal();                   // send signal while waiting for conversion

    while(ADCON0bits.NOT_DONE);     // Wait for conversion

    result = ADRESH;
    result <<=8;
    result |= ADRESL;

    return result;
}

/*********************************************************************
* Function: bool ADC_Enable(ADC_CHANNEL channel, ADC_CONFIGURATION configuration);
*
* Overview: Configures the ADC module to specified setting
*
* PreCondition: none
*
* Input: ADC_CHANNEL channel - the channel to enable
*        ADC_CONFIGURATION configuration - the mode in which to run the ADC
*
* Output: bool - true if successfully configured.  false otherwise.
*
********************************************************************/
bool ADC_Enable(ADC_CHANNEL channel)
{
    switch(channel)
    {
        case ADC_CHANNEL_0:
            TRISAbits.TRISA0 = PIN_INPUT;
            ANSELAbits.ANSA0 = PIN_ANALOG;
            return true;

        case ADC_CHANNEL_1:
             TRISCbits.TRISC0 = PIN_OUTPUT;
            return true;
        default:
            return false;
    }
}

/*********************************************************************
* Function: bool ADC_SetConfiguration(ADC_CONFIGURATION configuration)
*
* Overview: Configures the ADC module to specified setting
*
* PreCondition: none
*
* Input: ADC_CONFIGURATION configuration - the mode in which to run the ADC
*
* Output: bool - true if successfully configured.  false otherwise.
*
********************************************************************/
bool ADC_SetConfiguration(ADC_CONFIGURATION configuration)
{
    if(configuration == ADC_CONFIGURATION_DEFAULT)
    {
        ADCON1=0;
        ADCON2=0xBE;
        ADCON0=0x01;
        
        return true;
    }

    return false;
}

/*********************************************************************
* Function: sendSignal()
*
* Overview: Send a signal on a pin
*
* PreCondition: Sending pins have to be preconfigured
*
* Input: 
*
* Output: 
*
********************************************************************/
void sendSignal(){
    LATCbits.LATC0 = 1;
    LATCbits.LATC0 = 0;
}