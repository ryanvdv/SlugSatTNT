/*
 * File:   main.c
 * Author: admatthe
 *
 * Created on May 7, 2018, 1:33 PM
 */

#include "xc.h"
#include <plib.h>

// Configuration settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz
// Primary Osc with PLL (XT+, HS+, EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1
#pragma config POSCMOD = HS, FNOSC = PRIPLL
#pragma config FWDTEN = OFF

int main(void) {
    //System coniguration
    SYSTEMConfigPerformance(80000000);
    CheKseg0CacheOn(); // enable the cache for the best performance
    mBMXDisableDRMWaitState(); // no wait states for RAM
    
    //Define and initialize the transmit buffer
    static const char txBuff[] = "String of characters to be sent over the SPI channel";
    
    //Define the receiver buffer
    static char rxBuff[sizeof(txBuff)];
    
    TRISFbits.TRISF0 = 0;
    LATFbits.LATF0 = 0;
    
    //General variables
    long int ix;
    int rdData;
    const char *pSrc;
    char *pDst;
    int txferSize;
    int fail = 0;
    
    // initialize SPI channel 1 as master, 8 bits/character, frame master divide FPB by 2
    SpiChnOpen(1, SPICON_MSTEN | SPICON_FRMEN | SPICON_SMP | SPICON_ON, 2);
    
    // initialize SPI channel 2 as slave, 8 bits/character, frame slave divide FPB by 2
    SpiChnOpen(2, SPICON_FRMEN | SPICON_FRMSYNC | SPICON_SMP | SPICON_ON, 2);
    
    //initialize transfer size, ix, source pointer, and destination pointer variables
    txferSize = sizeof(txBuff);
    ix = txferSize + 1;
    pSrc = txBuff;
    pDst = rxBuff;
    
    while(ix--) {
        SpiChnPutC(1, *pSrc++);  // send data on master channel
        rdData = SpiChnGetC(1); // get the received data
        
        // IF statement skips the first received character, it's garbage
        if (ix != txferSize) {
            *pDst = rdData; // store the received data
            pDst++; // increment the destination pointer
        }
        
        rdData = SpiChnGetC(2); // receive data on the slave channel
        SpiChnPutC(2, rdData);  // relay data back
    }
    
    // check to see that the data was received okay
    pSrc = txBuff;
    pDst = rxBuff;
    
    // check each element of the array
    for (ix = 0; ix < sizeof(txBuff); ix++)
    {
        // check if the element in the destination array equals the element
        // in the source array
        // if difference, flag fail marker and break from loop
        if(*pDst != *pSrc){
            fail = 1;
            break;
        // else increment pointers and compare next elements
        } else {
            pDst++;
            pSrc++;
        }
    }
    
    LATFbits.LATF0 ^= 1;
    
    // check if data transfer was successful
    if (fail == 0) {
        // success
    } else {
        // failure
    }
    
    while (1);
}
