/*
 * File:   main.c
 * Author: admatthe
 *
 * Created on May 7, 2018, 1:33 PM
 */

#include "uCConfig.h"
#include <stdio.h>
#include <plib.h>

int main(void) {
    int addr;
    int wrData;
    int stByte;
    int rdData;  
    int flag;
    
    BOARD_Init();
    
    // CC1200 Reset Pin set high
    TRISDbits.TRISD3 = 0;
    LATDbits.LATD3 = 1;
    
    // CC1200 Slave Select pin set high
    TRISDbits.TRISD4 = 0;
    LATDbits.LATD4 = 1;
    
    IEC1CLR = 0x03800000;
    SPI2CON = 0;
    rdData = SPI2BUF;
    SPI2BRG = 1;
    SPI2STATCLR = 0x40;
    SPI2CON = 0x8120;
    
    
    while (1) {
        flag = 0;
        
        // enter hex value to be written over SPI
        printf("Enter an address: 0x");
        scanf("%x", &addr);
        
        if ((addr < 0x30) && (addr >> 7 == 0))
        {
            printf("Enter data to write: 0x");
            scanf("%x", &wrData);
            printf("data entered: 0x%04x\n", wrData);
            flag = 1;
        }
        
        printf("address entered: 0x%04x\n", addr);
        
        // bring SS pin low
        LATDbits.LATD4 = 0;
        
        // get status byte
        // send data on master channel to cc1200
        SPI2BUF = addr;
        // wait for cc1200 data to arrive in master SPI rx buffer
        while (!SpiChnDataRdy(2));
        // store SPI rx buffer data in rdData
        stByte = SPI2BUF;
        
        // get data
        // send data on master channel to cc1200
        if (flag == 1) {
            SPI2BUF = wrData;
        } else {
            SPI2BUF = 0;
        }
        // wait for cc1200 data to arrive in master SPI rx buffer
        while (!SpiChnDataRdy(2));
        // store SPI rx buffer data in rdData
        rdData = SPI2BUF;
        
        // bring SS pin high
        LATDbits.LATD4 = 1;
        
        // print status byte received from cc1200
        printf("status byte received from cc1200: 0x%04x\n", stByte);
        // print data received from cc1200
        printf("data received from cc1200: 0x%04x\n", rdData);
    }

    while (1);
}
