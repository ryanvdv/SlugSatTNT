/* 
 * File:   hal_spi.h
 * Author: Andrew
 *
 * Created on June 4, 2018, 8:09 PM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H

#define SPI_CS_N_PIN LATDbits.LATD4

#define SPI_BEGIN()       SPI_CS_N_PIN = 0
#define SPI_TX(x)         SPI2BUF = x
#define SPI_WAIT_READY()  while (!SPI2STATbits.SPIRBF);
#define SPI_RX()          SPI2BUF
#define SPI_END()         SPI_CS_N_PIN = 1


#endif	/* HAL_SPI_H */

