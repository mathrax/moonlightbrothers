/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "color_data.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
unsigned int count = -1;
unsigned int next_count = 0;
unsigned int prescale;

#ifdef MOON
float maxPrescale = 380;
#else
float maxPrescale = 400; 
#endif
int maxBright = 200;



int now_r[maxLED];
int now_g[maxLED];
int now_b[maxLED];

int next_r[maxLED];
int next_g[maxLED];
int next_b[maxLED];

int diff_r[maxLED];
int diff_g[maxLED];
int diff_b[maxLED];

int set_r[maxLED];
int set_g[maxLED];
int set_b[maxLED];

int sort_r[maxLED];
int sort_g[maxLED];
int sort_b[maxLED];

unsigned char red[maxLED];
unsigned char blu[maxLED];
unsigned char grn[maxLED];

unsigned int maxColorCount;



// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary local functions.
 */


void mathrax_loop() {
    unsigned int i;

    prescale++;
    if (prescale > maxPrescale) {

        prescale = 0;

        count++;

        if (count < maxColorCount - 1) {

            next_count = count + 1;

        } else if (count > maxColorCount - 1) {

            count = 0;
            next_count = 1;

        } else if (count == maxColorCount - 1) {

            next_count = 0;

        }
        for (i = 0; i < maxLED; i++) {
            now_r[i] = (color_data[i * maxColorCount + count] >> 16) & 0xFF;
            now_g[i] = (color_data[i * maxColorCount + count] >> 8) & 0xFF;
            now_b[i] = (color_data[i * maxColorCount + count]) & 0xFF;

            next_r[i] = (color_data[i * maxColorCount + next_count] >> 16) & 0xFF;
            next_g[i] = (color_data[i * maxColorCount + next_count] >> 8) & 0xFF;
            next_b[i] = (color_data[i * maxColorCount + next_count]) & 0xFF;

            if (now_r[i] >= maxBright)now_r[i] = maxBright;
            if (now_g[i] >= maxBright)now_g[i] = maxBright;
            if (now_b[i] >= maxBright)now_b[i] = maxBright;

            if (next_r[i] >= maxBright)next_r[i] = maxBright;
            if (next_g[i] >= maxBright)next_g[i] = maxBright;
            if (next_b[i] >= maxBright)next_b[i] = maxBright;
        }
    }


    for (i = 0; i < maxLED; i++) {
        diff_r[i] = next_r[i] - now_r[i];
        diff_g[i] = next_g[i] - now_g[i];
        diff_b[i] = next_b[i] - now_b[i];

        red[i] = (int) (now_r[i] + prescale * (diff_r[i] / maxPrescale));
        grn[i] = (int) (now_g[i] + prescale * (diff_g[i] / maxPrescale));
        blu[i] = (int) (now_b[i] + prescale * (diff_b[i] / maxPrescale));
        
    }

    show();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void) {
    unsigned int i;

    appData.state = APP_STATE_INIT;

    ANSELA = 0x0000; // all digital pins
    ANSELB = 0x0000;

    maxColorCount = sizeof (color_data) / sizeof (uint32_t) / maxLED; //Max Key Frame 

    for (i = 0; i < maxLED; i++) {
        now_r[i] = 0;
        now_g[i] = 0;
        now_b[i] = 0;

        next_r[i] = (color_data[i * maxColorCount] >> 16) & 0xFF;
        next_g[i] = (color_data[i * maxColorCount] >> 8) & 0xFF;
        next_b[i] = (color_data[i * maxColorCount]) & 0xFF;
    }

}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void) {
    unsigned int i;
    /* Check the application's current state. */
    switch (appData.state) {
            /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            if (appInitialized) {
//                deletePattern();
                for (i = 0; i < 50000; i++) {
                    Nop();
                    Nop();
                    Nop();
                }
                
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            //LOOP mathrax
            mathrax_loop();
            break;
        }

            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

void setPixelColor(unsigned int pixel, unsigned char r, unsigned char g, unsigned char b) {
    red[pixel] = r;
    grn[pixel] = g;
    blu[pixel] = b;
}

void show() {
    unsigned int i;

//    SYS_INT_Disable();

    neopixel_pin = 0;
    for (i = 0; i < 5000; i++) {
        Nop();
    }

    for (i = 0; i < maxLED; i++) {
        //Bit7
        if (grn[i] & 0b10000000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit6
        if (grn[i] & 0b01000000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit5
        if (grn[i] & 0b00100000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit4
        if (grn[i] & 0b00010000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit3
        if (grn[i] & 0b00001000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit2
        if (grn[i] & 0b00000100) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit1
        if (grn[i] & 0b00000010) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }
        //Bit0
        if (grn[i] & 0b00000001) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }


        //Bit7
        if (red[i] & 0b10000000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit6
        if (red[i] & 0b01000000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit5
        if (red[i] & 0b00100000) {
            neopixel_pin = 1;
            Neopixel700us();
            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit4
        if (red[i] & 0b00010000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit3
        if (red[i] & 0b00001000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit2
        if (red[i] & 0b00000100) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit1
        if (red[i] & 0b00000010) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }
        //Bit0
        if (red[i] & 0b00000001) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }






        //Bit7
        if (blu[i] & 0b10000000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit6
        if (blu[i] & 0b01000000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit5
        if (blu[i] & 0b00100000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit4
        if (blu[i] & 0b00010000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit3
        if (blu[i] & 0b00001000) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit2
        if (blu[i] & 0b00000100) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }

        //Bit1
        if (blu[i] & 0b00000010) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {
            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }
        //Bit0
        if (blu[i] & 0b00000001) {
            neopixel_pin = 1;
            Neopixel700us();

            neopixel_pin = 0;
            Neopixel600us();

        } else {

            neopixel_pin = 1;
            Neopixel350us();
            neopixel_pin = 0;
            Neopixel700us();

        }
    }

//    SYS_INT_Enable(); // Disable global interrupt

}

void deletePattern() {
    unsigned int i;
    for (i = 0; i < maxLED; i++) {
        red[i] = 0;
        grn[i] = 0;
        blu[i] = 0;
    }
}


/*******************************************************************************
 End of File
 */
