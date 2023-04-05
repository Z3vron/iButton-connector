/**
 * \file lcdas.h
 */


#ifndef __Open1768_LCD_H
#define __Open1768_LCD_H


#include "LPC17xx.h"

/* 8bit to 16bit LCD Interface

   PINS:
   - EN      = P1.25
   - DIR     = P1.24
   - LE      = P1.23
   - WR      = P0.23
   - CS      = P0.22
   - RD      = P0.21
   - RS      = P0.20
   - DB[0.7] = P2.0...P2.7
   - DB[8.15]= P2.0...P2.7
*/

#define PIN_EN   (1 << 25)
#define PIN_DIR  (1 << 24)
#define PIN_LE   (1 << 23)

#define PIN_WR   (1 << 23)
#define PIN_CS   (1 << 22)
#define PIN_RD   (1 << 21)
#define PIN_RS   (1 << 20)

/* Pin EN setting to 0 or 1                                                   */
#define LCD_EN(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_EN) : (LPC_GPIO1->FIOCLR = PIN_EN));
/* Pin DIR setting to 0 or 1                                                   */
#define LCD_DIR(x)  ((x) ? (LPC_GPIO1->FIOSET = PIN_DIR) : (LPC_GPIO1->FIOCLR = PIN_DIR));
/* Pin LE setting to 0 or 1                                                   */
#define LCD_LE(x)   ((x) ? (LPC_GPIO1->FIOSET = PIN_LE) : (LPC_GPIO1->FIOCLR = PIN_LE));
/* Pin WR setting to 0 or 1                                                   */
#define LCD_WR(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_WR) : (LPC_GPIO0->FIOCLR = PIN_WR));
/* Pin CS setting to 0 or 1                                                   */
#define LCD_CS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_CS) : (LPC_GPIO0->FIOCLR = PIN_CS));
/* Pin RD setting to 0 or 1                                                   */
#define LCD_RD(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RD) : (LPC_GPIO0->FIOCLR = PIN_RD));
/* Pin RS setting to 0 or 1                                                   */
#define LCD_RS(x)   ((x) ? (LPC_GPIO0->FIOSET = PIN_RS) : (LPC_GPIO0->FIOCLR = PIN_RS));


/* Display define ------------------------------------------------------------*/
#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8346A    10 /* 0x0046 */
#define  HX8347D    11 /* 0x0047 */
#define  HX8347A    12 /* 0x0047 */
#define  LGDP4535   13 /* 0x4535 */
#define  SSD2119    14 /* 3.5 LCD 0x9919 */

/* Chipset common registers --*/
#define OSCIL_ON 0x00  //Oscillator


/*****************************
 *  LCD control lines config
 */
void lcdConfiguration(void);


/*****************************
 *   Low level procedures
 */
void lcdSend (uint16_t byte);
uint16_t lcdRead (void);


/*****************************
 *   Mid-level
 *   comunication procedures
 */
void lcdWriteIndex(uint16_t index);
void lcdWriteData(uint16_t index);
uint16_t lcdReadData(void);


/*****************************
 *   High level (compound)
 *   comunication procedures
 */
void lcdWriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);
uint16_t lcdReadReg(uint16_t LCD_Reg);
void lcdSetCursor(uint16_t Xpos, uint16_t Ypos);

#endif
